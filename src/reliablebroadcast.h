#ifndef RELIABLEBROADCAST_H
#define RELIABLEBROADCAST_H

#include "message.h"
#include "node.h"
#include "packet_manager.h"
#include "session.h"
#include "socket_controller.h"
#include "threadsafequeue.h"
#include "chain_config.h"
#include "node_config.h"

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

    const NodeConfig &mNodeConfig;
    boost::asio::io_service mIoService;

    SocketController mSocketController;
    PacketManager mPacketManager;
    SessionsPool mSessions;

    cpp_redis::client mRedisClient;
    std::mutex mTimesMutex;
    std::queue<std::chrono::system_clock::time_point> mMessageDeliverTimes;

public:
    ReliableBroadcast(const NodeConfig &nodeConfig);

    void start();
    void stop();
    size_t getNodesCount(uint64_t mChainHash) const;
    void broadcast(Message::MessageType messageType, std::shared_ptr<Message> message);
    void deliver(std::shared_ptr<Message> message);
    void asyncProcessMessage(std::shared_ptr<Message> message);

private:
    void processMessage(std::shared_ptr<Message> message);
    void connectToRedis();
};

#endif // RELIABLEBROADCAST_H
