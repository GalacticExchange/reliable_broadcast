#ifndef SESSION_H
#define SESSION_H

#include "message.h"

#include <boost/uuid/sha1.hpp>

#include <atomic>
#include <memory>
#include <mutex>
#include <unordered_set>


class ReliableBroadcast;
class Message;

class Session
{
public:
    typedef __int128 Id;

private:
    const Id mId;
    const size_t n;
    const size_t t;
    const size_t mEchoMessageCountTarget;
    ReliableBroadcast &mOwner;
//    std::mutex mMessageMutex;
//    std::shared_ptr<const std::vector<char>> mMessage;
//    std::shared_ptr<const std::vector<char>> mMessageHash;
//    std::queue<std::shared_ptr<HashMessage>> mPendingHashMessages;
    std::mutex mEchoMessageCounterMutex;
    std::unordered_set<int> mEchoMessageCounter;
    std::mutex mReadyMessageCounterMutex;
    std::unordered_set<int> mReadyMessageCounter;
    std::atomic<bool> mReadyMessageWasSent;
    std::atomic<bool> mDelivered;    

public:
    Session(ReliableBroadcast &owner, Id id);
    Session(ReliableBroadcast &owner);

    void processMessage(std::shared_ptr<Message> message);
    Id getId() const;
    static Id getRandomId();

private:
    Session(Id mId, ReliableBroadcast &owner);

    static std::shared_ptr<std::vector<char>> calculateMessageHash(
            std::shared_ptr<const std::vector<char>> message);
    static size_t getT(size_t n);
    static size_t getEchoMessageCountTarget(size_t n, size_t t);
    void deliver();
};

#endif // SESSION_H
