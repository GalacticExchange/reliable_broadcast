#ifndef RELIABLEBROADCAST_H
#define RELIABLEBROADCAST_H

#include <atomic>
#include <thread>
#include <unordered_map>
#include <vector>

#include <boost/asio.hpp>
#include <boost/thread/pthread/shared_mutex.hpp>

#include "internalmessage.h"
#include "message.h"
#include "node.h"
#include "messagelistener.h"
#include "threadsafequeue.h"


class ReliableBroadcast
{
public:
    class Session;

private:
    class SessionsPool
    {
        const size_t REMOVE_DELAY_SEC = 10;
        mutable boost::shared_mutex mSessionsMutex;
        std::unordered_map<uint64_t, std::shared_ptr<Session>> mSessions;
        ReliableBroadcast &mOwner;
        ThreadSafeQueue<std::pair<uint64_t, std::chrono::system_clock::time_point>> mRemoveQueue;
        std::thread mRemovingThread;

    public:
        SessionsPool(ReliableBroadcast &owner);
        std::shared_ptr<Session> getOrCreateSession(
                std::shared_ptr<Message> message);
        void removeSession(uint64_t sessionId);

    private:
        std::shared_ptr<Session> getSession(uint64_t id) const;
        std::shared_ptr<Session> addSession(std::shared_ptr<Message> message);
        void remove(uint64_t sessionId);
        void removeLoop();
    };

    int mId;
    uint64_t mMChainHash;
    std::unordered_map<int, Node> mNodes;
    MessageListener mMessageListener;
    boost::asio::io_service mIoService;
    SessionsPool mSessions;
    ThreadSafeQueue<std::shared_ptr<Message>> mMessageQueue;
    std::atomic<size_t> mCommitCounter;
    std::chrono::system_clock::time_point mStartTime;

public:
    ReliableBroadcast(int id, uint64_t mChainHash, const std::unordered_map<int, Node> &nodes);
    void start();
    void stop();
    void postMessage(std::shared_ptr<Message> message);

private:
    void asyncProcessMessage();
    void processMessage(std::shared_ptr<Message> message);
    void broadcast(std::shared_ptr<Message> message);
    std::string getPipeFileName() const;
};

#endif // RELIABLEBROADCAST_H
