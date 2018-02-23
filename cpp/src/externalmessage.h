#ifndef EXTERNALMESSAGE_H
#define EXTERNALMESSAGE_H

#include <boost/uuid/sha1.hpp>

#include "message.h"


class ExternalMessage : public Message
{
    std::vector<char> mMessage;
    static boost::uuids::detail::sha1 sHashFunction;
public:
    ExternalMessage(std::vector<char>::iterator begin, std::vector<char>::iterator end);

    const std::vector<char> &getMessage() const;
    MessageType getType() const override;
    std::vector<char> getMessageHash() const;
};

#endif // EXTERNALMESSAGE_H
