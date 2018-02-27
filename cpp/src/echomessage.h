#ifndef ECHOMESSAGE_H
#define ECHOMESSAGE_H

#include <internalmessage.h>
#include <sendmessage.h>

class EchoMessage : public InternalMessage
{
public:
    EchoMessage(const SendMessage &sendMessage);
    MessageType getType() const override;
};

#endif // ECHOMESSAGE_H
