#include "message.h"

#include <iostream>
#include <memory>
#include <vector>

using std::cerr;
using std::copy;
using std::endl;
using std::make_shared;
using std::shared_ptr;
using std::vector;


uint64_t Message::getClientId() const
{
    return mClientId;
}

uint64_t Message::getMChainHash() const
{
    return mMChainHash;
}

uint64_t Message::getNodeId() const
{
    return mNodeId;
}

const vector<char> &Message::getData() const
{
    return mData;
}

void Message::setNodeId(uint64_t id)
{
    mNodeId = id;
}

size_t Message::getEncodedSize() const
{
    size_t header_size = sizeof(mClientId)
            + sizeof(mNonce)
            + sizeof(mMChainHash)
            + sizeof(mNodeId);
    return header_size + mData.size();
}

void Message::encodeChild(vector<char>::iterator begin, vector<char>::iterator end) const
{
//    write<typeof(mMChainHash)>(begin, mMChainHash);
    write<uint64_t>(begin, mMChainHash);
    write<typeof(mClientId)>(begin, mClientId);
    write<typeof(mNonce)>(begin, mNonce);
    write<typeof(mNodeId)>(begin, mNodeId);
    copy(mData.begin(), mData.end(), begin);
}

Message::Message(uint64_t clientId,
                 uint64_t nonce,
                 uint64_t mChainHash,
                 uint64_t nodeId,
                 vector<char> &&data):
    mClientId(clientId),
    mNonce(nonce),
    mMChainHash(mChainHash),
    mNodeId(nodeId),
    mData(move(data))
{

}

Message::Message(vector<char>::const_iterator begin,
                 vector<char>::const_iterator end):
    AbstractMessage(begin, end)
{
    begin += AbstractMessage::getOffset();

    //    uint64_t mClientId;
    //    uint64_t mNonce;
    //    uint64_t mMChainHash;
    //    uint64_t mNodeId;

    //    std::vector<char> mData;

    size_t header_size = sizeof(mClientId)
            + sizeof(mNonce)
            + sizeof(mMChainHash)
            + sizeof(mNodeId);

    if (begin + header_size > end) {
        cerr <<  "Received message [";
        bool first = true;
        for (auto p = begin; p != end; ++p)
        {
            if (first)
            {
                first = false;
            } else {
                cerr << ' ';
            }
            cerr << static_cast<int>(*p);
        }
        cerr << "]" << endl;
        throw std::logic_error("Buffer is too small");
    }

    size_t offset = 0;
    mMChainHash = parse<typeof(mMChainHash)>(begin, end, offset);
    mClientId = parse<typeof(mClientId)>(begin, end, offset);
    mNonce = parse<typeof(mNonce)>(begin, end, offset);
    mNodeId = parse<typeof(mNodeId)>(begin, end, offset);

    mData = vector<char>(begin + offset, end);
}

uint64_t Message::getNonce() const
{
    return mNonce;
}
