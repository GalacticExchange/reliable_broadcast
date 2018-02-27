#ifndef SENDMESSAGE_H
#define SENDMESSAGE_H

#include "internalmessage.h"

class SendMessage : public InternalMessage
{
public:
    SendMessage(int sender, uint64_t sessionId, std::vector<char> &&messageHash);
    SendMessage(std::vector<char>::const_iterator begin, std::vector<char>::const_iterator end);
    MessageType getType() const override;
};

#endif // SENDMESSAGE_H
