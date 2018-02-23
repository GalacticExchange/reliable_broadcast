#ifndef SENDMESSAGE_H
#define SENDMESSAGE_H

#include "internalmessage.h"

class SendMessage : public InternalMessage
{
public:
    SendMessage(int sender, std::vector<char> &&messageHash);
    MessageType getType() const override;
};

#endif // SENDMESSAGE_H
