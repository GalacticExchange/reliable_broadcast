#include "echomessage.h"

EchoMessage::EchoMessage(const SendMessage &sendMessage):
    InternalMessage(sendMessage.getSenderId(),
                    sendMessage.getSessionId(),
                    sendMessage.getMessageHashPointer())
{

}

Message::MessageType EchoMessage::getType() const
{
    return ECHO_MESSAGE;
}
