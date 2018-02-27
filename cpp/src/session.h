#ifndef SESSION_H
#define SESSION_H

#include <atomic>
#include <memory>
#include <unordered_set>

#include "externalmessage.h"
#include "internalmessage.h"
#include "reliablebroadcast.h"

class ReliableBroadcast::Session
{
    uint64_t mId;
    std::shared_ptr<ExternalMessage> mMessage;
    ReliableBroadcast &mOwner;
    std::mutex mEchoMessageCounterMutex;
    std::unordered_set<int> mEchoMessageCounter;
    std::mutex mReadyMessageCounterMutex;
    std::unordered_set<int> mReadyMessageCounter;
    std::atomic<bool> mFirstReadyMessageWasSent;
    std::atomic<bool> mSecondReadyMessageWasSent;
    std::atomic<bool> mDelivered;

public:    
    Session(ReliableBroadcast &owner, std::shared_ptr<ExternalMessage> message);
    void processMessage(std::shared_ptr<InternalMessage> message);
    uint64_t getId() const;

private:
    static uint64_t getRandomId();
    void deliver();
};

#endif // SESSION_H
