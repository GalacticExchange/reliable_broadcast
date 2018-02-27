#ifndef ECHOMESSAGE_H
#define ECHOMESSAGE_H

#include "hashmessage.h"
#include "sendmessage.h"

class EchoMessage : public HashMessage
{
public:
    EchoMessage(int sender,
                uint64_t sessionId,
                std::shared_ptr<const std::vector<char>> messageHash);
    EchoMessage(std::vector<char>::const_iterator begin, std::vector<char>::const_iterator end);
    MessageType getType() const override;
};

#endif // ECHOMESSAGE_H
