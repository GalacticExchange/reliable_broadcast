#include "packet_message.h"

#include <iostream>

using std::endl;
using std::string;
using std::vector;


PacketMessage::PacketMessage():
    AbstractMessage(PACKET)
{

}

PacketMessage::PacketMessage(std::vector<char>::const_iterator begin,
                             std::vector<char>::const_iterator end):
    AbstractMessage(begin, end)
{
    begin += AbstractMessage::getEncodedSize();

    if (begin + sizeof(length_t) > end)
    {
        throw std::logic_error("Buffer is too small");
    }
    size_t offset = 0;
    length_t messagesCount = parse<length_t>(begin, end, offset);
    vector<length_t> lengths(messagesCount);
    if (begin + (messagesCount + 1) * sizeof(length_t) > end)
    {
        throw std::logic_error("Buffer is too small");
    }
    for (auto &length : lengths)
    {
        length = parse<length_t>(begin, end, offset);
    }
    begin += offset;
    for (const auto length : lengths)
    {
        if (begin >= end)
        {
            throw std::logic_error("Buffer is too small");
        }
        mMessages.emplace_back(AbstractMessage::parse(begin, begin + length));
        begin += length;
    }
}

PacketMessage::PacketMessage(const std::vector<std::shared_ptr<AbstractMessage> > &messages):
    AbstractMessage(PACKET),
    mMessages(messages)
{

}

void PacketMessage::addMessage(std::shared_ptr<AbstractMessage> message)
{
    mMessages.emplace_back(message);
}

const std::vector<std::shared_ptr<AbstractMessage> > &PacketMessage::getMessages() const
{
    return mMessages;
}

void PacketMessage::print(std::ostream &os, const string &prefix) const
{
    os << prefix << "Messages in packet: " << mMessages.size() << endl;
    for (auto message : mMessages)
    {
        message->print(os, prefix + string(4, ' '));
    }
}

size_t PacketMessage::getEncodedSize() const
{
    size_t size = sizeof(length_t) * (mMessages.size() + 1);
    for (const auto &message : mMessages)
    {
        size += message->getEncodedSize();
    }
    return AbstractMessage::getEncodedSize() + size;
}

void PacketMessage::encodeChild(vector<char>::iterator begin,
                                vector<char>::iterator end) const
{
    write<length_t>(begin, mMessages.size());
    vector<size_t> lengths;
    for (auto message : mMessages)
    {
        lengths.emplace_back(message->getEncodedSize());
    }
    for (auto length : lengths)
    {
        write<length_t>(begin, length);
    }
    for (size_t i = 0; i < mMessages.size(); ++i)
    {
        mMessages[i]->encode(begin, begin + lengths[i]);
        begin += lengths[i];
    }
}
