#include <vector>

using std::vector;

#include "echomessage.h"

EchoMessage::EchoMessage(int sender,
                         uint64_t sessionId,
                         std::shared_ptr<const std::vector<char>> messageHash):
    HashMessage(sender,
                sessionId,
                messageHash)
{

}

EchoMessage::EchoMessage(vector<char>::const_iterator begin, vector<char>::const_iterator end):
    HashMessage(begin, end)
{

}

Message::MessageType EchoMessage::getType() const
{
    return ECHO_MESSAGE;
}
