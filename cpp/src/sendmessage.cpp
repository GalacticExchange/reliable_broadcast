#include <memory>
#include <vector>

using std::copy;
using std::move;
using std::shared_ptr;
using std::vector;

#include "sendmessage.h"


SendMessage::SendMessage(int sender, uint64_t sessionId, shared_ptr<const vector<char>> message):
    InternalMessage(sender, sessionId),
    mMessage(message)
{

}

SendMessage::SendMessage(vector<char>::const_iterator begin, vector<char>::const_iterator end):
    InternalMessage(begin, end)
{

}

Message::MessageType SendMessage::getType() const
{
    return MessageType::SEND;
}

size_t SendMessage::getBytesNeeded() const
{
    return mMessage->size();
}

void SendMessage::compile(vector<char>::iterator begin) const
{
    copy(mMessage->begin(), mMessage->end(), begin);
}

std::shared_ptr<const std::vector<char> > SendMessage::getMessagePtr() const
{
    return mMessage;
}

const vector<char> &SendMessage::getMessage() const
{
    return *mMessage;
}
