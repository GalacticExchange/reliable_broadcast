#include <memory>
#include <vector>

using std::make_shared;
using std::move;
using std::shared_ptr;
using std::vector;

#include "internalmessage.h"


InternalMessage::InternalMessage(int sender, uint64_t sessionId, vector<char> &&messageHash):
    mSender(sender),
    mSessionId(sessionId),
    mMessageHash(make_shared<vector<char>>(move(messageHash)))
{

}

InternalMessage::InternalMessage(int sender,
                                 uint64_t sessionId,
                                 shared_ptr<const vector<char> > messageHash):
    mSender(sender),
    mSessionId(sessionId),
    mMessageHash(messageHash)
{

}

InternalMessage::InternalMessage(vector<char>::const_iterator begin,
                                 vector<char>::const_iterator end)
{
    for (size_t i = 0; i < sizeof(int); ++i)
    {
        reinterpret_cast<char*>(mSender)[i] = begin[i];
    }
    for (size_t i = 0; i < 8; ++i)
    {
        reinterpret_cast<char*>(mSessionId)[sizeof(int) + i] = begin[i];
    }
    shared_ptr<vector<char>> messageHash =
            make_shared<vector<char>>(end - begin - (sizeof(int) + 8));
    for (size_t i = 0; i < messageHash->size(); ++i)
    {
        (*messageHash)[i] = begin[sizeof(int) + 8 + i];
    }
    mMessageHash = messageHash;
}

int InternalMessage::getSenderId() const
{
    return mSender;
}

uint64_t InternalMessage::getSessionId() const
{
    return mSessionId;
}

const std::vector<char> &InternalMessage::getMessageHash() const
{
    return *mMessageHash;
}

std::shared_ptr<const std::vector<char> > InternalMessage::getMessageHashPointer() const
{
    return mMessageHash;
}

std::vector<char> InternalMessage::compile() const
{
    vector<char> rawMessage(1 + sizeof(int) + sizeof(mSessionId) + mMessageHash->size());
    rawMessage[0] = getType();
    for (size_t i = 0; i < sizeof(int); ++i)
    {
        rawMessage[1 + i] = reinterpret_cast<char*>(mSender)[i];
    }
    for (size_t i = 0; i < sizeof(mSessionId); ++i)
    {
        rawMessage[1 + sizeof(int) + i] = reinterpret_cast<char*>(mSessionId)[i];
    }
    for (size_t i = 0; i < mMessageHash->size(); ++i)
    {
        rawMessage[1 + sizeof(int) + sizeof(mSessionId) + i] = mMessageHash->operator [](i);
    }
    return rawMessage;
}
