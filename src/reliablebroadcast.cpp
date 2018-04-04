#include "packet_manager.h"
#include "reliablebroadcast.h"
#include "session.h"

#include <boost/log/trivial.hpp>
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


ReliableBroadcast::ReliableBroadcast(const NodeConfig &nodeConfig):
    mNodeConfig(nodeConfig),
    mSocketController(nodeConfig.getPort()),
    mPacketManager(mIoService, *this, mSocketController, nodeConfig),
    mSessions(*this)
{

}

void ReliableBroadcast::start() {
    boost::asio::io_service::work service(mIoService);
    vector<thread> pool;
    for (size_t i = 0; i < thread::hardware_concurrency(); ++i) {
        pool.emplace_back([this]() {
            this->mIoService.run();
        });
    }

    mSocketController.listen([this](std::vector<char>::const_iterator begin,
                                    std::vector<char>::const_iterator end)
    {
        this->mPacketManager.asyncProcess(begin, end);
    });

    for (auto &thread : pool) {
        thread.join();
    }
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

    message->setNodeId(mNodeConfig.getId());
    message->setMessageType(messageType);    

    BOOST_LOG_TRIVIAL(debug) << "Broadcast message of type "
                             << message->getType() << " with nonce "
                             << message->getNonce();

    mPacketManager.asyncBroadcast(message);

    processMessage(message);
}

void ReliableBroadcast::deliver(std::shared_ptr<Message> message) {
    BOOST_LOG_TRIVIAL(debug) << "Deliver message with nonce "
                             << message->getNonce() << ": ["
                             << string(message->getData().begin(), message->getData().end())
                             << "]";
//    cerr << "Deliver message with nonce " << message->getNonce() << endl;
    uint64_t mChainHash = message->getMChainHash();
    if (mRedisClient.is_connected())
    {
        mRedisClient.rpush(to_string(mChainHash),
                           vector<string>(1,
                                          string(message->getData().begin(),
                                                 message->getData().end())));
        mRedisClient.commit();
        BOOST_LOG_TRIVIAL(debug) << "Saved message to redis";

        const std::chrono::seconds WINDOW_LENGTH(5);
        double rps;
        auto current = std::chrono::system_clock::now();
        {
            std::lock_guard<std::mutex> lock(mTimesMutex);
            mMessageDeliverTimes.push(current);
            while (!mMessageDeliverTimes.empty()
                   && mMessageDeliverTimes.front() < current - WINDOW_LENGTH)
            {
                mMessageDeliverTimes.pop();
            }
            rps = static_cast<double>(mMessageDeliverTimes.size()) / WINDOW_LENGTH.count();
        }
        BOOST_LOG_TRIVIAL(debug) << "Process " << rps << " messages per second";
        cerr << endl;
    } else {
        BOOST_LOG_TRIVIAL(warning) << "Redis is disconnected. Retry to connect.";
        cerr << endl;
        connectToRedis();
        deliver(message);
    }
}

void ReliableBroadcast::asyncProcessMessage(std::shared_ptr<Message> message)
{
    mIoService.post([this, message]()
    {
        this->processMessage(message);
    });
}

void ReliableBroadcast::connectToRedis()
{
    mRedisClient.connect(mNodeConfig.getRedisHost(),
                         mNodeConfig.getRedisPort(),
                         [](const std::string& host,
                            std::size_t port,
                            cpp_redis::client::connect_state status)
    {
        BOOST_LOG_TRIVIAL(debug) << "Connection status to redis on "
                                 << host << ':' << port << " is " << static_cast<int>(status);
    },
                         1000,
                         10);
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
        pair<Session::Id, system_clock::time_point> task = mRemoveQueue.pop();
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

size_t ReliableBroadcast::getNodesCount(uint64_t mChainHash) const {
    return mNodeConfig.getChainConfig(mChainHash).getNodes().size();
}
