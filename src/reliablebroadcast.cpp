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
using std::stringstream;
using std::thread;
using std::unordered_map;
using std::vector;

#include <boost/thread/locks.hpp>
#include <boost/thread/pthread/shared_mutex.hpp>

using boost::shared_lock;
using boost::shared_mutex;
using boost::unique_lock;

#include "externalmessage.h"
#include "reliablebroadcast.h"
#include "session.h"

ReliableBroadcast::ReliableBroadcast(int id, uint64_t mChainHash, const unordered_map<int, Node> &nodes):
    mId(id),
    mMChainHash(mChainHash),
    mNodes(nodes),
    mMessageListener(getPipeFileName(), *this),
    mSessions(*this),
    mCommitCounter(0)
{
}

ReliableBroadcast::ReliableBroadcast(ChainConfig config) :
        mId(config.getId()),
        mMChainHash(config.getMChainHash()),
        mNodes(config.getNodes()),
        mMessageListener(config.getMChainPath(), *this),
        mSessions(*this),
        mCommitCounter(0)
{

}

void ReliableBroadcast::start()
{
    asyncProcessMessage();
    vector<thread> pool;
    for (size_t i = 0; i < thread::hardware_concurrency(); ++i)
    {
        pool.emplace_back([this]()
        {
            this->mIoService.run();
        });
    }
    mMessageListener.listen();
    for (auto &thread : pool)
    {
        thread.join();
    }
}

void ReliableBroadcast::postMessage(std::shared_ptr<Message> message)
{
    mMessageQueue.push(message);
}

void ReliableBroadcast::asyncProcessMessage()
{
    mIoService.post([this]()
    {
        shared_ptr<Message> message = mMessageQueue.pop();
        asyncProcessMessage();
        processMessage(message);
    });
}

void ReliableBroadcast::processMessage(shared_ptr<Message> message)
{
    throw std::logic_error("Unimplemented");
//    if (message->getType() == Message::MessageType::MESSAGE)
//    {
//        shared_ptr<ExternalMessage> externalMessage =
//                dynamic_pointer_cast<ExternalMessage>(message);

//        shared_ptr<SendMessage> sendMessage = make_shared<SendMessage>(mId, externalMessage);
//        broadcast(sendMessage);
//    } else {
//        shared_ptr<InternalMessage> internalMessage =
//                dynamic_pointer_cast<InternalMessage>(message);

//        shared_ptr<Session> session = mSessions.getOrCreateSession(internalMessage);
//        session->processMessage(internalMessage);
//    }
}

void ReliableBroadcast::broadcast(std::shared_ptr<Message> message)
{
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

    throw std::logic_error("Not implemented");

//    shared_ptr<vector<char>> rawMessagePtr =
//            make_shared<vector<char>>(move(message->compile(mId)));
////    cerr << "Going to brodcast message: [";
////    bool first = true;
////    for (char byte : *rawMessagePtr)
////    {
////        if (first)
////        {
////            first = false;
////        } else {
////            cerr <<  ", ";
////        }
////        cerr << static_cast<int>(byte);
////    }
////    cerr << "]" << endl;
//    for (auto node_ptr : mNodes)
//    {
//        if (node_ptr.first != mId)
//        {
//            const Node &node = node_ptr.second;
//            boost::asio::ip::udp::endpoint targetEndpoint(
//                        boost::asio::ip::address::from_string(node.getAddress()),
//                        node.getPort());
////            mMessageListener.send(targetEndpoint, rawMessagePtr);
//            throw std::logic_error("Not implemented");
//        }
//    }
//    processMessage(message);
}

std::string ReliableBroadcast::getPipeFileName() const
{
    stringstream ss;
    ss << "/tmp/m_chains/m_chain_" << mMChainHash;
    return ss.str();
}

ReliableBroadcast::SessionsPool::SessionsPool(ReliableBroadcast &owner):
    mOwner(owner),
    mRemovingThread([this]()
{
    this->removeLoop();
})
{

}

shared_ptr<ReliableBroadcast::Session>
ReliableBroadcast::SessionsPool::addSession(shared_ptr<Message> message)
{
    throw std::logic_error("Not implemented");
//    shared_ptr<ReliableBroadcast::Session> session = make_shared<Session>(mOwner, message);
//    {
//        unique_lock<shared_mutex> lock(mSessionsMutex);
//        auto session_ptr = mSessions.find(message->getSessionId());
//        if (session_ptr == mSessions.end())
//        {
//            mSessions[session->getId()] = session;
//        } else {
//            session = session_ptr->second;
//        }
//    }
//    return session;
}

void ReliableBroadcast::SessionsPool::remove(uint64_t sessionId)
{
//    cerr << "Remove session #" << sessionId << endl;
    unique_lock<shared_mutex> lock(mSessionsMutex);
    mSessions.erase(sessionId);
}

void ReliableBroadcast::SessionsPool::removeLoop()
{
    while (true)
    {
        pair<uint64_t, system_clock::time_point> task = mRemoveQueue.pop();
        std::this_thread::sleep_until(task.second);
        remove(task.first);
    }
}

shared_ptr<ReliableBroadcast::Session>
ReliableBroadcast::SessionsPool::getSession(uint64_t id) const
{
    shared_lock<shared_mutex> lock(mSessionsMutex);
    shared_ptr<ReliableBroadcast::Session> session = nullptr;
    auto sessionPtr = mSessions.find(id);
    if (sessionPtr != mSessions.end()) {
        session = sessionPtr->second;
    }
    return session;
}

shared_ptr<ReliableBroadcast::Session>
ReliableBroadcast::SessionsPool::getOrCreateSession(shared_ptr<Message> message)
{
    throw std::logic_error("Not implemented");
//    shared_ptr<ReliableBroadcast::Session> session = getSession(message->getSessionId());
//    if (!session)
//    {
//        session = addSession(message);
//    }
//    return session;
}

void ReliableBroadcast::SessionsPool::removeSession(uint64_t sessionId)
{
    mRemoveQueue.push(make_pair(
                          sessionId,
                          system_clock::now() + std::chrono::seconds(REMOVE_DELAY_SEC)));
}
