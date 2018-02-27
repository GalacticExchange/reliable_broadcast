#include <vector>

using std::move;
using std::vector;

#include "sendmessage.h"


SendMessage::SendMessage(int sender, uint64_t sessionId, vector<char> &&messageHash):
    InternalMessage(sender, sessionId, move(messageHash))
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
