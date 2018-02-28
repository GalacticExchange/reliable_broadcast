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
    uint64_t mId;
    std::shared_ptr<const std::vector<char>> mMessage;
    std::shared_ptr<const std::vector<char>> mMessageHash;
    ReliableBroadcast &mOwner;
    std::mutex mEchoMessageCounterMutex;
    std::unordered_set<int> mEchoMessageCounter;
    std::mutex mReadyMessageCounterMutex;
    std::unordered_set<int> mReadyMessageCounter;
    std::atomic<bool> mFirstReadyMessageWasSent;
    std::atomic<bool> mSecondReadyMessageWasSent;
    std::atomic<bool> mDelivered;
    static boost::uuids::detail::sha1 sHashFunction;

public:    
    Session(ReliableBroadcast &owner, std::shared_ptr<const std::vector<char>> message);
    Session(ReliableBroadcast &owner,
            std::shared_ptr<const std::vector<char>> message,
            uint64_t id);
    void start();
    void processMessage(std::shared_ptr<HashMessage> message);
    void processSendMessage();
    uint64_t getId() const;

private:
    static uint64_t getRandomId();
    static std::shared_ptr<std::vector<char>> calculateMessageHash(
            std::shared_ptr<const std::vector<char>> message);
    void deliver();
};

#endif // SESSION_H
