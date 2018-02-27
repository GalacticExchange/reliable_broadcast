#include "readymessage.h"


ReadyMessage::ReadyMessage(const EchoMessage &echoMessage):
    InternalMessage(
        echoMessage.getSenderId(),
        echoMessage.getSessionId(),
        echoMessage.getMessageHashPointer())
{

}

Message::MessageType ReadyMessage::getType() const
{
    return MessageType::READY;
}
