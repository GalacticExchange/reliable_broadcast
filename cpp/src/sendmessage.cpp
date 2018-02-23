#include <vector>

using std::move;
using std::vector;

#include "sendmessage.h"


SendMessage::SendMessage(int sender, vector<char> &&messageHash):
    InternalMessage(sender, move(messageHash))
{

}

Message::MessageType SendMessage::getType() const
{
    return MessageType::SEND;
}
