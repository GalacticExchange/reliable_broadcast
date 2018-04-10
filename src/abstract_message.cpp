#include "abstract_message.h"
#include "message.h"

#include <boost/log/trivial.hpp>

using std::copy;
using std::make_shared;
using std::vector;


AbstractMessage::AbstractMessage(vector<char>::const_iterator begin,
                                 vector<char>::const_iterator end)
{
    if (begin + 1 > end)
    {
        throw std::logic_error("Buffer is too small");
    }
    char buffer = *begin;
    if (buffer >= MessageType::SIZE)
    {
        throw std::logic_error("Message type is wrong");
    }
    mType = static_cast<MessageType>(buffer);
}

vector<char> AbstractMessage::encode() const
{
    vector<char> buffer(getOffset() + getEncodedSize());
    vector<char>::iterator begin = buffer.begin();
    char typeByte = static_cast<char>(mType);
    write<typeof(typeByte)>(begin, typeByte);
    encodeChild(begin, buffer.end());
}

std::shared_ptr<AbstractMessage> AbstractMessage::parse(std::vector<char>::const_iterator begin,
                                                        std::vector<char>::const_iterator end)
{
    if (end - begin < 1)
    {
        BOOST_LOG_TRIVIAL(debug) << "Message is too short";
        return nullptr;
    }
    char buffer = *begin;
    if (buffer > MessageType::SIZE)
    {
        BOOST_LOG_TRIVIAL(debug) << "Message type is wrong";
        return nullptr;
    }

    try
    {
        MessageType messageType = static_cast<MessageType>(buffer);
        if (messageType <= READY)
        {
            return make_shared<Message>(begin, end);
        } else {
            throw std::logic_error("Not implemented");
        }
    }
    catch (std::logic_error e)
    {
        BOOST_LOG_TRIVIAL(debug) << e.what();
        return nullptr;
    }
}

AbstractMessage::MessageType AbstractMessage::getType() const
{
    return mType;
}

void AbstractMessage::setMessageType(AbstractMessage::MessageType messageType)
{
    mType = messageType;
}

size_t AbstractMessage::getOffset()
{
    return 1;
}
