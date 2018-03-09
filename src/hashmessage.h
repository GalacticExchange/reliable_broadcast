#ifndef HASHMESSAGE_H
#define HASHMESSAGE_H

#include "internalmessage.h"

class HashMessage : public InternalMessage
{
    std::shared_ptr<const std::vector<char>> mMessageHash;
public:
    HashMessage(int sender,
                uint64_t sessionId,
                std::shared_ptr<const std::vector<char>> messageHash);
    HashMessage(std::vector<char>::const_iterator begin, std::vector<char>::const_iterator end);

    std::shared_ptr<const std::vector<char>> getMessageHashPtr() const;
    const std::vector<char> &getMessageHash() const;

protected:
    size_t getBytesNeeded() const override;
    void compile(std::vector<char>::iterator begin) const override;
};

#endif // HASHMESSAGE_H
