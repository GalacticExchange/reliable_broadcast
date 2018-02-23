#include <vector>

using std::vector;

#include "externalmessage.h"

ExternalMessage::ExternalMessage(vector<char>::iterator begin, vector<char>::iterator end)
{
    mMessage = vector<char>(begin, end);
}

const std::vector<char> &ExternalMessage::getMessage() const
{
    return mMessage;
}

Message::MessageType ExternalMessage::getType() const
{
    return MESSAGE;
}

vector<char> ExternalMessage::getMessageHash() const
{
    sHashFunction.process_bytes(&*mMessage.begin(), mMessage.size());
    unsigned int digest[5];
    sHashFunction.get_digest(digest);
    vector<char> hash(20);
    for(size_t i = 0; i < 5; ++i)
    {
        const char* tmp = reinterpret_cast<char*>(digest);
        hash[i*4] = tmp[i*4 + 3];
        hash[i*4 + 1] = tmp[i*4 + 2];
        hash[i*4 + 2] = tmp[i*4 + 1];
        hash[i*4 + 3] = tmp[i*4];
    }
    return hash;
}

boost::uuids::detail::sha1 ExternalMessage::sHashFunction;
