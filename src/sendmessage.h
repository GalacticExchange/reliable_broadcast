#ifndef SENDMESSAGE_H
#define SENDMESSAGE_H

#include "externalmessage.h"
#include "internalmessage.h"

//class SendMessage : public InternalMessage
//{
//    std::shared_ptr<const std::vector<char>> mMessage;
//public:
//    SendMessage(int sender, std::shared_ptr<ExternalMessage> externalMessage);
//    SendMessage(std::vector<char>::const_iterator begin, std::vector<char>::const_iterator end);
//    MessageType getType() const override;
//    std::shared_ptr<const std::vector<char>> getMessagePtr() const;
//    const vector<char> &getMessage() const;

//protected:
//    size_t getBytesNeeded() const override;
//    void compile(std::vector<char>::iterator begin) const override;

//private:
//    SendMessage(int sender, uint64_t sessionId, std::shared_ptr<const std::vector<char>> message);
//};

#endif // SENDMESSAGE_H
