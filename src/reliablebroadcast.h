#ifndef RELIABLEBROADCAST_H
#define RELIABLEBROADCAST_H

#include "message.h"
#include "node.h"
#include "messagelistener.h"
#include "session.h"
#include "threadsafequeue.h"
#include "chain_config.h"
#include "../router/node_config.h"

#include <boost/asio.hpp>
#include <boost/thread/pthread/shared_mutex.hpp>
#include <cpp_redis/cpp_redis>

#include <atomic>
#include <thread>
#include <unordered_map>
#include <vector>


class ReliableBroadcast {
    class SessionsPool {
        const size_t REMOVE_DELAY_SEC = 10;
        mutable boost::shared_mutex mSessionsMutex;
        std::unordered_map<Session::Id, std::shared_ptr<Session>> mSessions;
        ReliableBroadcast &mOwner;
        ThreadSafeQueue<std::pair<Session::Id, std::chrono::system_clock::time_point>> mRemoveQueue;
        std::thread mRemovingThread;

    public:
        SessionsPool(ReliableBroadcast &owner);

        std::shared_ptr<Session> getOrCreateSession(Session::Id id);

        void removeSession(Session::Id sessionId);

    private:
        std::shared_ptr<Session> getSession(Session::Id id) const;

        std::shared_ptr<Session> addSession(Session::Id id);

        void remove(Session::Id sessionId);

        void removeLoop();
    };

    const size_t BROADCAST_PORT = 1122;

    int mId;
    uint64_t mMChainHash;
    std::unordered_map<int, Node> mNodes;
    MessageListener mMessageListener;
    boost::asio::io_service mIoService;
    SessionsPool mSessions;
    ThreadSafeQueue<std::shared_ptr<std::vector<char>>> mMessageQueue;
    std::atomic<size_t> mCommitCounter;
    std::chrono::system_clock::time_point mStartTime;
    boost::asio::ip::udp::socket mBroadcastSocket;
    cpp_redis::client mRedisClient;    
    std::mutex mTimesMutex;
    std::queue<std::chrono::system_clock::time_point> mMessageDeliverTimes;

public:
    ReliableBroadcast(int id,
                      uint64_t mChainHash,
                      const std::string &path,
                      const std::unordered_map<int, Node> &nodes);

    ReliableBroadcast(const NodeConfig &nodeConfig, const ChainConfig &chainConfig);

    void start();

    void stop();

    void postMessage(std::shared_ptr<std::vector<char>> buffer);

    size_t getNodesCount() const;

    void broadcast(Message::MessageType messageType, std::shared_ptr<Message> message);

    void deliver(std::shared_ptr<Message> message);

    void asyncProcessMessage(std::shared_ptr<Message> message);

private:
    void asyncProcessMessage();

    void processMessage(std::shared_ptr<Message> message);

    std::string getPipeFileName(const std::string &path) const;

    void connectToRedis();
};

#endif // RELIABLEBROADCAST_H
