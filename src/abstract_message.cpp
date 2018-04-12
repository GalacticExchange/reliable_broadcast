#include "abstract_message.h"
#include "message.h"
#include "packet_message.h"

#include <boost/log/trivial.hpp>

using std::copy;
using std::endl;
using std::make_shared;
using std::ostream;
using std::vector;


AbstractMessage::AbstractMessage(AbstractMessage::MessageType messageType):
    mType(messageType)
{

}

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
    vector<char> buffer(getEncodedSize());
    encode(buffer.begin(), buffer.end());
    return buffer;
}

void AbstractMessage::encode(vector<char>::iterator begin,
                             vector<char>::iterator end) const
{
    if (begin + getEncodedSize() > end)
    {
        throw std::logic_error("Buffer is too small");
    }
    char typeByte = static_cast<char>(mType);
    write<typeof(typeByte)>(begin, typeByte);
    encodeChild(begin, end);
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
        } else if (messageType == PACKET) {
            return make_shared<PacketMessage>(begin, end);
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

void AbstractMessage::print(ostream &os, const std::string &prefix) const
{
    os << prefix << "Message" << endl;
    os << prefix << "Type: ";
    if (mType == SEND)
    {
        os << "Send";
    } else if (mType == ECHO_MESSAGE) {
        os << "Echo";
    } else if (mType == READY) {
        os << "Ready";
    } else if (mType == PACKET) {
        os << "Packet";
    }
    os << endl;
}

size_t AbstractMessage::getEncodedSize() const
{
    return 1;
}

std::ostream &operator <<(std::ostream &os, std::shared_ptr<AbstractMessage> message)
{
    message->print(os);
}
