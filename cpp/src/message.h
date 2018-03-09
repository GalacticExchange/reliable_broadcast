#ifndef MESSAGE_H
#define MESSAGE_H

#include <memory>
#include <vector>


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

    std::vector<char> encode() const;
    virtual MessageType getType() const = 0;
    static std::shared_ptr<Message> parse(
            std::vector<char>::iterator begin,
            std::vector<char>::iterator end);
};

#endif // MESSAGE_H
