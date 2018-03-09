#ifndef READYMESSAGE_H
#define READYMESSAGE_H

#include "echomessage.h"
#include "hashmessage.h"

class ReadyMessage: public HashMessage
{
public:
    ReadyMessage(const EchoMessage &echoMessage);
    ReadyMessage(std::vector<char>::const_iterator begin, std::vector<char>::const_iterator end);
    MessageType getType() const override;
};

#endif // READYMESSAGE_H
