#ifndef SESSION_H
#define SESSION_H

#include <memory>

#include "externalmessage.h"
#include "internalmessage.h"
#include "reliablebroadcast.h"

class ReliableBroadcast::Session
{
    uint64_t mId;
    std::shared_ptr<ExternalMessage> mMessage;
    ReliableBroadcast &mOwner;
public:
    Session(ReliableBroadcast &owner, std::shared_ptr<ExternalMessage> message);
    void processMessage(std::shared_ptr<InternalMessage> message);

private:
    static uint64_t getRandomId();
};

#endif // SESSION_H
