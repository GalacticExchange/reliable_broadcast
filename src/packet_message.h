#ifndef PACKET_MESSAGE_H
#define PACKET_MESSAGE_H

#include "abstract_message.h"


class PacketMessage : public AbstractMessage
{
    typedef uint16_t length_t;
    std::vector<std::shared_ptr<AbstractMessage>> mMessages;

public:
    PacketMessage();
    PacketMessage(std::vector<char>::const_iterator begin,
                  std::vector<char>::const_iterator end);
    PacketMessage(const std::vector<std::shared_ptr<AbstractMessage>> &messages);

    void addMessage(std::shared_ptr<AbstractMessage> message);
    const std::vector<std::shared_ptr<AbstractMessage>> &getMessages() const;
    void print(std::ostream &os, const std::string &prefix = "") const override;

protected:
    size_t getEncodedSize() const override;
    void encodeChild(std::vector<char>::iterator begin,
                     std::vector<char>::iterator end) const override;
};

#endif // PACKET_MESSAGE_H
