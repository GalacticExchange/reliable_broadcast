#ifndef READYMESSAGE_H
#define READYMESSAGE_H

#include "echomessage.h"
#include "internalmessage.h"

class ReadyMessage: public InternalMessage
{
public:
    ReadyMessage(const EchoMessage &echoMessage);
    MessageType getType() const override;
};

#endif // READYMESSAGE_H
