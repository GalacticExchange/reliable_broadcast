#ifndef EXTERNALMESSAGE_H
#define EXTERNALMESSAGE_H

#include <boost/uuid/sha1.hpp>

#include "message.h"


class ExternalMessage : public Message
{
    std::shared_ptr<const std::vector<char>> mMessage;
    mutable std::vector<char> mMessageHash;    
public:
    ExternalMessage(std::vector<char>::iterator begin, std::vector<char>::iterator end);

    std::shared_ptr<const std::vector<char>> getMessagePtr() const;
    const std::vector<char> &getMessage() const;
    MessageType getType() const override;    
};

#endif // EXTERNALMESSAGE_H
