#ifndef INTERNALMESSAGE_H
#define INTERNALMESSAGE_H

#include <message.h>
#include <vector>


class InternalMessage : public Message
{
    int mSender;
    uint64_t mSessionId;
    std::shared_ptr<const std::vector<char>> mMessageHash;

public:
    InternalMessage(int sender, uint64_t sessionId, std::vector<char> &&messageHash);
    InternalMessage(int sender,
                    uint64_t
                    sessionId,
                    std::shared_ptr<const std::vector<char>> messageHash);
    InternalMessage(std::vector<char>::const_iterator begin,
                    std::vector<char>::const_iterator end);
    int getSenderId() const;
    uint64_t getSessionId() const;
    const std::vector<char> &getMessageHash() const;
    std::shared_ptr<const std::vector<char>> getMessageHashPointer() const;
    virtual std::vector<char> compile() const;
};

#endif // INTERNALMESSAGE_H
