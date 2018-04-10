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
    static std::shared_ptr<AbstractMessage> parse(
            std::vector<char>::const_iterator begin,
            std::vector<char>::const_iterator end);

    MessageType getType() const;
    void setMessageType(MessageType messageType);

protected:
    template <class T>
    static T parse(std::vector<char>::const_iterator begin,
                   std::vector<char>::const_iterator end,
                   size_t &offset);
    template <class T>
    static void write(std::vector<char>::iterator &begin, const T &value);
    virtual size_t getEncodedSize() const = 0;
    static size_t getOffset();
    virtual void encodeChild(std::vector<char>::iterator begin,
                        std::vector<char>::iterator end) const = 0;
};

#include "abstract_message.tcc"

#endif // ABSTRACT_MESSAGE_H
