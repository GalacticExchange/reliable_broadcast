#include <memory>
#include <vector>

using std::copy;
using std::make_shared;
using std::shared_ptr;
using std::vector;

#include "echomessage.h"
#include "externalmessage.h"
#include "message.h"
#include "readymessage.h"
#include "sendmessage.h"


Message::Message(uint64_t clientId,
                 uint64_t nonce,
                 uint64_t mChainHash,
                 uint64_t nodeId,
                 Message::MessageType messageType,
                 vector<char> &&data):
    mClientId(clientId),
    mNonce(nonce),
    mMChainHash(mChainHash),
    mNodeId(nodeId),
    mType(messageType),
    mData(move(data))
{

}

std::vector<char> Message::encode() const
{
    size_t total_size = sizeof(mClientId)
            + sizeof(mNonce)
            + sizeof(mMChainHash)
            + sizeof(mNodeId)
            + 1
            + mData.size();
    vector<char> buffer(total_size);
    size_t offset = 0;
    write<typeof(mMChainHash)>(buffer.begin(), offset, mMChainHash);
    offset += sizeof(mMChainHash);
    write<typeof(mClientId)>(buffer.begin(), offset, mClientId);
    offset += sizeof(mClientId);
    write<typeof(mNonce)>(buffer.begin(), offset, mNonce);
    offset += sizeof(mNonce);
    write<typeof(mNodeId)>(buffer.begin(), offset, mNodeId);
    offset += sizeof(mNodeId);
    buffer[offset] = static_cast<char>(mType);
    offset += 1;
    copy(mData.begin(), mData.end(), buffer.begin() + offset);
    return buffer;
}

shared_ptr<Message> Message::parse(
        vector<char>::const_iterator begin,
        vector<char>::const_iterator end)
{
//    uint64_t mClientId;
//    uint64_t mNonce;
//    uint64_t mMChainHash;
//    uint64_t mNodeId;
//    MessageType mType;
//    std::vector<char> mData;

    size_t total_size = sizeof(mClientId)
            + sizeof(mNonce)
            + sizeof(mMChainHash)
            + sizeof(mNodeId)
            + 1;

    if (begin + total_size > end) {
        throw std::logic_error("Buffer is too small");
    }

    shared_ptr<Message> message = make_shared<Message>();
    size_t offset = 0;
    message->mMChainHash = parse<typeof(message->mMChainHash)>(begin, end, offset);
    offset += sizeof(mMChainHash);
    message->mClientId = parse<typeof(message->mClientId)>(begin, end, offset);
    offset += sizeof(mClientId);
    message->mNonce = parse<typeof(message->mNonce)>(begin, end, offset);
    offset += sizeof(mNonce);
    message->mNodeId = parse<typeof(message->mNodeId)>(begin, end, offset);
    offset += sizeof(mNodeId);
    char messageType = *(begin + offset);
    if (messageType >= 3)
    {
        throw std::logic_error("Wrong value for message type");
    }
    message->mType = static_cast<MessageType>(messageType);
    offset += 1;
    message->mData = vector<char>(begin + offset, end);

    return message;
}

uint64_t Message::getNonce() const
{
    return mNonce;
}

template<class T>
T Message::parse(vector<char>::const_iterator begin,
                          vector<char>::const_iterator end,
                          size_t offset)
{
    T value;
    copy(&*begin + offset, &*begin + offset + sizeof(T), reinterpret_cast<char*>(&value));
    return value;
}

template<class T>
void Message::write(vector<char>::iterator begin, size_t offset, const T &value)
{
    copy(reinterpret_cast<const char*>(&value),
         reinterpret_cast<const char*>(&value) + sizeof(T),
         &*begin + offset);
}

uint64_t Message::parseMChain(std::vector<char> encoded){
    return parse<typeof(Message::mMChainHash)>(encoded.begin(), encoded.end(), 0);

}