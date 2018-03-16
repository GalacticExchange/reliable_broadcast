#ifndef MESSAGE_H
#define MESSAGE_H

#include <memory>
#include <vector>

#include "../capnproto/internal_message.capnp.h"
#include "session.h"

class Message
{
public:
    enum MessageType
    {
        MESSAGE = 0,
        SEND,
        ECHO_MESSAGE,
        READY
    };

private:
    InternalMessage mInternalMessage;
    InternalMessage::Reader mInternalMessageReader;

public:
    Message(InternalMessage &&internalMessage);
    Message(const Message&) = delete;
    Message& operator = (const Message &) = delete;
    Session::Id getSessionId() const;

//    std::vector<char> encode() const;
//    virtual MessageType getType() const = 0;
//    static std::shared_ptr<Message> parse(
//            std::vector<char>::iterator begin,
//            std::vector<char>::iterator end);
};

#endif // MESSAGE_H
