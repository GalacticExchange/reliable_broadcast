#ifndef ABSTRACT_MESSAGE_H
#define ABSTRACT_MESSAGE_H

#include <memory>
#include <vector>


class AbstractMessage
{
public:
    enum MessageType
    {
        SEND,
        ECHO_MESSAGE,
        READY,
        PACKET,
        SIZE
    };

private:
    MessageType mType;

public:
    AbstractMessage() = default;
    AbstractMessage(MessageType messageType);
    AbstractMessage(std::vector<char>::const_iterator begin,
                    std::vector<char>::const_iterator end);
    AbstractMessage(const AbstractMessage &) = delete;
    AbstractMessage &operator = (const AbstractMessage &) = delete;

    std::vector<char> encode() const;
    void encode(std::vector<char>::iterator begin,
                std::vector<char>::iterator end) const;
    static std::shared_ptr<AbstractMessage> parse(
            std::vector<char>::const_iterator begin,
            std::vector<char>::const_iterator end);
    virtual size_t getEncodedSize() const;

    MessageType getType() const;
    void setMessageType(MessageType messageType);
    virtual void print(std::ostream &os, const std::string &prefix = "") const;

protected:
    template <class T>
    static T parse(std::vector<char>::const_iterator begin,
                   std::vector<char>::const_iterator end,
                   size_t &offset);
    template <class T>
    static void write(std::vector<char>::iterator &begin, const T &value);
    virtual void encodeChild(std::vector<char>::iterator begin,
                        std::vector<char>::iterator end) const = 0;
};

std::ostream &operator << (std::ostream &os, std::shared_ptr<AbstractMessage> message);

#include "abstract_message.tcc"

#endif // ABSTRACT_MESSAGE_H
