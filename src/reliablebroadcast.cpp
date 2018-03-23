#include "reliablebroadcast.h"
#include "session.h"

#include <boost/thread/locks.hpp>
#include <boost/thread/pthread/shared_mutex.hpp>

using boost::shared_lock;
using boost::shared_mutex;
using boost::unique_lock;

#include <chrono>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <thread>
#include <unordered_map>
#include <vector>

using std::cerr;
using std::chrono::system_clock;
using std::endl;
using std::dynamic_pointer_cast;
using std::make_pair;
using std::make_shared;
using std::mutex;
using std::pair;
using std::shared_ptr;
using std::string;
using std::stringstream;
using std::thread;
using std::to_string;
using std::unordered_map;
using std::vector;


ReliableBroadcast::ReliableBroadcast(int id,
                                     uint64_t mChainHash,
                                     const string &path,
                                     const unordered_map<int, Node> &nodes) :
        mId(id),
        mMChainHash(mChainHash),
        mNodes(nodes),
        mMessageListener(getPipeFileName(path), *this),
        mSessions(*this),
        mCommitCounter(0),
        mBroadcastSocket(mIoService) {
    mRedisClient.connect();
    mBroadcastSocket.open(boost::asio::ip::udp::v4());
}

ReliableBroadcast::ReliableBroadcast(const NodeConfig &nodeConfig,
                                     const ChainConfig &chainConfig) :
        ReliableBroadcast(nodeConfig.getId(),
                          chainConfig.getMChainHash(),
                          chainConfig.getMChainPath(),
                          chainConfig.getNodes()) {

}

void ReliableBroadcast::start() {
    asyncProcessMessage();
    vector<thread> pool;
    for (size_t i = 0; i < thread::hardware_concurrency(); ++i) {
        pool.emplace_back([this]() {
            this->mIoService.run();
        });
    }
    mMessageListener.listen();
    for (auto &thread : pool) {
        thread.join();
    }
}

void ReliableBroadcast::postMessage(std::shared_ptr<vector<char>> buffer) {
    mMessageQueue.push(buffer);
}

void ReliableBroadcast::asyncProcessMessage() {
    mIoService.post([this]() {
        shared_ptr<vector<char>> buffer = mMessageQueue.pop();
        asyncProcessMessage();
        shared_ptr<Message> message = Message::parse(buffer->begin(), buffer->end());
        processMessage(message);
    });
}

void ReliableBroadcast::processMessage(shared_ptr<Message> message) {
    shared_ptr<Session> session = mSessions.getOrCreateSession(Session::getId(message));
    session->processMessage(message);
}

void ReliableBroadcast::broadcast(Message::MessageType messageType, shared_ptr<Message> message) {
//    cerr << "\tBrodcast ";
//    if (message->getType() == Message::MessageType::SEND)
//    {
//        cerr << "SEND";
//    } else if (message->getType() == Message::MessageType::ECHO_MESSAGE) {
//        cerr << "ECHO";
//    } else if (message->getType() == Message::MessageType::READY) {
//        cerr << "READY";
//    } else {
//        cerr << "UNKNOWN";
//    }
//    cerr << " message in session #" << message->getSessionId() << endl;

    message->setNodeId(mId);
    message->setMessageType(messageType);
    shared_ptr<vector<char>> buffer = make_shared<vector<char>>(message->encode());
    if (!mBroadcastSocket.is_open()) {
        throw std::logic_error("Broadcast socket is closed");
    }
    mBroadcastSocket.async_send_to(boost::asio::buffer(*buffer),
                                   boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(),
                                                                  BROADCAST_PORT),
                                   [buffer](const boost::system::error_code &error,
                                            std::size_t) {
                                       if (error) {
                                           cerr << "Error on send: " << error.message() << endl;
                                       }
                                   });
    processMessage(message);
}

void ReliableBroadcast::deliver(std::shared_ptr<Message> message) {
    cerr << "Deliver message with nonce " << message->getNonce() << endl;
    uint64_t mChainHash = message->getMChainHash();
    mRedisClient.rpush(to_string(mChainHash),
                       vector<string>(1,
                                      string(message->getData().begin(),
                                             message->getData().end())));
    mRedisClient.commit();
}

std::string ReliableBroadcast::getPipeFileName(const std::string &path) const {
    stringstream ss;
    ss << path << "/" << mMChainHash;
    return ss.str();
}

ReliableBroadcast::SessionsPool::SessionsPool(ReliableBroadcast &owner) :
        mOwner(owner),
        mRemovingThread([this]() {
            this->removeLoop();
        }) {

}

shared_ptr<Session>
ReliableBroadcast::SessionsPool::addSession(Session::Id id) {
    shared_ptr<Session> session = make_shared<Session>(mOwner, id);
    {
        unique_lock<shared_mutex> lock(mSessionsMutex);
        auto session_ptr = mSessions.find(id);
        if (session_ptr == mSessions.end()) {
            mSessions[session->getId()] = session;
        } else {
            session = session_ptr->second;
        }
    }
    return session;
}

void ReliableBroadcast::SessionsPool::remove(Session::Id sessionId) {
//    cerr << "Remove session #" << sessionId << endl;
    unique_lock<shared_mutex> lock(mSessionsMutex);
    mSessions.erase(sessionId);
}

void ReliableBroadcast::SessionsPool::removeLoop() {
    while (true) {
        pair<uint64_t, system_clock::time_point> task = mRemoveQueue.pop();
        std::this_thread::sleep_until(task.second);
        remove(task.first);
    }
}

shared_ptr<Session>
ReliableBroadcast::SessionsPool::getSession(Session::Id id) const {
    shared_lock<shared_mutex> lock(mSessionsMutex);
    shared_ptr<Session> session = nullptr;
    auto sessionPtr = mSessions.find(id);
    if (sessionPtr != mSessions.end()) {
        session = sessionPtr->second;
    }
    return session;
}

shared_ptr<Session>
ReliableBroadcast::SessionsPool::getOrCreateSession(Session::Id id) {
    shared_ptr<Session> session = getSession(id);
    if (!session) {
        session = addSession(id);
    }
    return session;
}

void ReliableBroadcast::SessionsPool::removeSession(Session::Id sessionId) {
    mRemoveQueue.push(make_pair(
            sessionId,
            system_clock::now() + std::chrono::seconds(REMOVE_DELAY_SEC)));
}

size_t ReliableBroadcast::getNodesCount() const {
    return mNodes.size();
}
