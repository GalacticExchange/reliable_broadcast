#ifndef SESSION_H
#define SESSION_H

#include <atomic>
#include <memory>
#include <unordered_set>

#include <boost/uuid/sha1.hpp>

#include "externalmessage.h"
#include "hashmessage.h"
#include "internalmessage.h"
#include "reliablebroadcast.h"
#include "sendmessage.h"

class ReliableBroadcast::Session
{
    const uint64_t mId;
    const size_t n;
    const size_t t;
    const size_t mEchoMessageCountTarget;
    ReliableBroadcast &mOwner;
    std::mutex mMessageMutex;
    std::shared_ptr<const std::vector<char>> mMessage;
    std::shared_ptr<const std::vector<char>> mMessageHash;
    std::queue<std::shared_ptr<HashMessage>> mPendingHashMessages;
    std::mutex mEchoMessageCounterMutex;
    std::unordered_set<int> mEchoMessageCounter;
    std::mutex mReadyMessageCounterMutex;
    std::unordered_set<int> mReadyMessageCounter;
    std::atomic<bool> mReadyMessageWasSent;
    std::atomic<bool> mDelivered;
    static boost::uuids::detail::sha1 sHashFunction;

public:
    Session(ReliableBroadcast &owner, std::shared_ptr<InternalMessage> internalMessage);

    void processMessage(std::shared_ptr<InternalMessage> message);
    uint64_t getId() const;
    static uint64_t getRandomId();

private:
    Session(uint64_t mId, ReliableBroadcast &owner);

    static std::shared_ptr<std::vector<char>> calculateMessageHash(
            std::shared_ptr<const std::vector<char>> message);
    static size_t getT(size_t n);
    static size_t getEchoMessageCountTarget(size_t n, size_t t);
    void deliver();
};

#endif // SESSION_H
