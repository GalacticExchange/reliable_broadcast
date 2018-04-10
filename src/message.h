#ifndef MESSAGE_H
#define MESSAGE_H

#include "abstract_message.h"

#include <memory>
#include <vector>


class Message: public AbstractMessage
{
private:
    uint64_t mMChainHash;
    uint64_t mClientId;
    uint64_t mNonce;    
    uint64_t mNodeId;    
    std::vector<char> mData;

public:
    Message() = default;
    Message(uint64_t clientId,
            uint64_t nonce,
            uint64_t mChainHash,
            uint64_t nodeId,            
            std::vector<char> &&data);
    Message(std::vector<char>::const_iterator begin,
            std::vector<char>::const_iterator end);
    Message(const Message &) = delete;
    Message &operator = (const Message &) = delete;

    uint64_t getNonce() const;
    uint64_t getClientId() const;
    uint64_t getMChainHash() const;
    uint64_t getNodeId() const;
    const std::vector<char> &getData() const;

    void setNodeId(uint64_t id);

protected:
    size_t getEncodedSize() const override;
    void encodeChild(std::vector<char>::iterator begin, std::vector<char>::iterator end) const override;
};

#endif // MESSAGE_H
