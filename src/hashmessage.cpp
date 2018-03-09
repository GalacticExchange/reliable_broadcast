#include <memory>
#include <vector>

using std::copy;
using std::make_shared;
using std::shared_ptr;
using std::vector;

#include "hashmessage.h"

HashMessage::HashMessage(int sender,
                         uint64_t sessionId,
                         shared_ptr<const vector<char> > messageHash):
    InternalMessage(sender, sessionId),
    mMessageHash(messageHash)
{

}

HashMessage::HashMessage(vector<char>::const_iterator begin, vector<char>::const_iterator end):
    InternalMessage(begin, end),
    mMessageHash(make_shared<vector<char>>(begin + getInternalMessageSize(), end))
{

}

shared_ptr<const vector<char> > HashMessage::getMessageHashPtr() const
{
    return mMessageHash;
}

const std::vector<char> &HashMessage::getMessageHash() const
{
    return *mMessageHash;
}

size_t HashMessage::getBytesNeeded() const
{
    return mMessageHash->size();
}

void HashMessage::compile(vector<char>::iterator begin) const
{
    copy(mMessageHash->begin(), mMessageHash->end(), begin);
}
