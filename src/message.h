#ifndef MESSAGE_H
#define MESSAGE_H

#include <memory>
#include <vector>


class Message
{
public:
    enum MessageType
    {
        SEND,
        ECHO_MESSAGE,
        READY
    };

private:
    uint64_t mClientId;
    uint64_t mNonce;
    uint64_t mMChainHash;
    uint64_t mNodeId;
    MessageType mType;
    std::vector<char> mData;

public:
    Message() = default;
    Message(uint64_t clientId,
            uint64_t nonce,
            uint64_t mChainHash,
            uint64_t nodeId,
            MessageType messageType,
            std::vector<char> &&data);
    Message(const Message &) = delete;
    Message &operator = (const Message &) = delete;

    std::vector<char> encode() const;    
    static std::shared_ptr<Message> parse(
            std::vector<char>::const_iterator begin,
            std::vector<char>::const_iterator end);

//    virtual MessageType getType() const = 0;
    uint64_t getNonce() const;

    static uint64_t parseMChain(std::vector<char> encoded);

private:
    template <class T>
    static T parse(std::vector<char>::const_iterator begin,
                   std::vector<char>::const_iterator end,
                   size_t offset);

    template <class T>
    static void write(std::vector<char>::iterator begin, size_t offset, const T &value);
};

#endif // MESSAGE_H
