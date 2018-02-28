#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

using std::cerr;
using std::endl;
using std::dynamic_pointer_cast;
using std::make_shared;
using std::mutex;
using std::shared_ptr;
using std::thread;
using std::unique_lock;
using std::vector;

#include "externalmessage.h"
#include "reliablebroadcast.h"
#include "session.h"

ReliableBroadcast::ReliableBroadcast(int id, const std::unordered_map<int, Node> &nodes):
    mId(id),
    mNodes(nodes),
    mSocketController(mNodes[id].getPort(), *this)
{
}

void ReliableBroadcast::start()
{
    asyncProcessMessage();
    vector<thread> pool;
    for (size_t i = 0; i < thread::hardware_concurrency(); ++i)
    {
        pool.emplace_back([this]
        {
            this->mIoService.run();
        });
    }
    mSocketController.listen();
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
    if (message->getType() == Message::MessageType::MESSAGE)
    {
        shared_ptr<ExternalMessage> externalMessage =
                dynamic_pointer_cast<ExternalMessage>(message);

        shared_ptr<Session> session = make_shared<Session>(
                    *this,
                    externalMessage->getMessagePtr());
        addSession(session);
        session->start();
    } else {
        shared_ptr<InternalMessage> internalMessage =
                dynamic_pointer_cast<InternalMessage>(message);

        if (internalMessage->getType() == Message::MessageType::SEND)
        {
            shared_ptr<SendMessage> sendMessage =
                    dynamic_pointer_cast<SendMessage>(internalMessage);
            cerr << "Received SEND message from " << sendMessage->getSenderId() << endl;

            shared_ptr<Session> session = getSession(sendMessage->getSessionId());
            if (session == nullptr)
            {
                session = make_shared<Session>(
                            *this,
                            sendMessage->getMessagePtr(),
                            sendMessage->getSessionId());
                addSession(session);
            }
            session->processSendMessage();
        } else {
            shared_ptr<HashMessage> hashMessage =
                    dynamic_pointer_cast<HashMessage>(internalMessage);

            shared_ptr<Session> session = getSession(hashMessage->getSessionId());
            if (session)
            {
                session->processMessage(hashMessage);
            }
        }
    }
}

void ReliableBroadcast::broadcast(std::shared_ptr<InternalMessage> message)
{
    shared_ptr<vector<char>> rawMessagePtr = make_shared<vector<char>>(move(message->compile()));
//    cerr << "Going to brodcast message: [";
//    bool first = true;
//    for (char byte : *rawMessagePtr)
//    {
//        if (first)
//        {
//            first = false;
//        } else {
//            cerr <<  ", ";
//        }
//        cerr << static_cast<int>(byte);
//    }
//    cerr << "]" << endl;
    for (auto node_ptr : mNodes)
    {
        if (node_ptr.first != mId)
        {
            const Node &node = node_ptr.second;
            boost::asio::ip::udp::endpoint targetEndpoint(
                        boost::asio::ip::address::from_string(node.getAddress()),
                        node.getPort());
            mSocketController.send(targetEndpoint, rawMessagePtr);
        }
    }
    processMessage(message);
}

void ReliableBroadcast::addSession(std::shared_ptr<ReliableBroadcast::Session> session)
{
    unique_lock<mutex> lock(mWriteMutex);
    while (mReadersCount)
    {
        mCanWriteConditionVariable.wait(lock);
    }
    mSessions[session->getId()] = session;
}

void ReliableBroadcast::removeSession(uint64_t id)
{
    {
        unique_lock<mutex> lock(mWriteMutex);
        while (!mReadersCount)
        {
            mCanWriteConditionVariable.wait(lock);
        }
        auto sessionPtr = mSessions.find(id);
        if (sessionPtr != mSessions.end())
        {
            mSessions.erase(sessionPtr);
        }
    }
    mCanWriteConditionVariable.notify_one();
}

std::shared_ptr<ReliableBroadcast::Session> ReliableBroadcast::getSession(uint64_t id) const
{
    ++mReadersCount;
    {
        unique_lock<mutex> lock(mWriteMutex);
    }
    auto sessionPtr = mSessions.find(id);
    shared_ptr<ReliableBroadcast::Session> session = nullptr;
    if (sessionPtr != mSessions.end()) {
        session = sessionPtr->second;
    }
    if (mReadersCount.fetch_sub(1) == 1) {
        mCanWriteConditionVariable.notify_one();
    }
    return session;
}
