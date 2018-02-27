#ifndef RELIABLEBROADCAST_H
#define RELIABLEBROADCAST_H

#include <atomic>
#include <unordered_map>
#include <vector>

#include <boost/asio.hpp>

#include "internalmessage.h"
#include "message.h"
#include "node.h"
#include "socketcontroller.h"
#include "threadsafequeue.h"


class ReliableBroadcast
{
    class Session;

    int mId;
    std::unordered_map<int, Node> mNodes;
    SocketController mSocketController;
    boost::asio::io_service mIoService;
    mutable std::atomic<size_t> mReadersCount;
    mutable std::condition_variable mCanWriteConditionVariable;
    mutable std::mutex mWriteMutex;
    std::unordered_map<uint64_t, std::shared_ptr<Session>> mSessions;
    ThreadSafeQueue<std::shared_ptr<Message>> mMessageQueue;
public:
    ReliableBroadcast(int id, const std::unordered_map<int, Node> &nodes);
    void start();
    void stop();
    void postMessage(std::shared_ptr<Message> message);

private:
    void asyncProcessMessage();
    void processMessage(std::shared_ptr<Message> message);
    void broadcast(std::shared_ptr<InternalMessage> message);
    void addSession(std::shared_ptr<Session> session);
    void removeSession(uint64_t id);
    std::shared_ptr<Session> getSession(uint64_t id) const;
};

#endif // RELIABLEBROADCAST_H
