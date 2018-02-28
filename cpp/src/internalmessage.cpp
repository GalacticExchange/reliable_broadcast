#include <iostream>
#include <memory>
#include <vector>

using std::cerr;
using std::endl;
using std::make_shared;
using std::move;
using std::shared_ptr;
using std::vector;

#include "internalmessage.h"


InternalMessage::InternalMessage(int sender, uint64_t sessionId):
    mSender(sender),
    mSessionId(sessionId)
{

}

InternalMessage::InternalMessage(vector<char>::const_iterator begin,
                                 vector<char>::const_iterator end)
{
    if (end - begin < static_cast<int>(sizeof(mSender) + sizeof(mSessionId)))
    {
        throw std::logic_error("Buffer is too small");
    }
    for (size_t i = 0; i < sizeof(mSender); ++i)
    {
        reinterpret_cast<char*>(&mSender)[i] = begin[i];
    }
    for (size_t i = 0; i < sizeof(mSessionId); ++i)
    {
        reinterpret_cast<char*>(&mSessionId)[i] = begin[sizeof(mSender) + i];
    }
}

int InternalMessage::getSenderId() const
{
    return mSender;
}

uint64_t InternalMessage::getSessionId() const
{
    return mSessionId;
}

std::vector<char> InternalMessage::compile() const
{
    vector<char> rawMessage(1 + sizeof(mSender) + sizeof(mSessionId) + getBytesNeeded());
    rawMessage[0] = getType();
    for (size_t i = 0; i < sizeof(mSender); ++i)
    {
        rawMessage[1 + i] = reinterpret_cast<const char*>(&mSender)[i];
    }
    for (size_t i = 0; i < sizeof(mSessionId); ++i)
    {
        rawMessage[1 + sizeof(mSender) + i] = reinterpret_cast<const char*>(&mSessionId)[i];
    }
    compile(rawMessage.begin() + 1 + sizeof(mSender) + sizeof(mSessionId));
    return rawMessage;
}

size_t InternalMessage::getInternalMessageSize() const
{
    return sizeof(mSender) + sizeof(mSessionId);
}
