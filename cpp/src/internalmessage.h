#ifndef INTERNALMESSAGE_H
#define INTERNALMESSAGE_H

#include <message.h>
#include <vector>


class InternalMessage : public Message
{
    int mSender;
    std::vector<char> mMessageHash;
public:
    InternalMessage(int sender, std::vector<char> &&messageHash);
};

#endif // INTERNALMESSAGE_H
