#ifndef INTERNALMESSAGE_H
#define INTERNALMESSAGE_H

//#include <message.h>
#include <vector>
#include "message.h"


//class InternalMessage : public Message
//{
//    int mSender;
//    uint64_t mSessionId;

//public:
//    InternalMessage(int sender, uint64_t sessionId);
//    InternalMessage(std::vector<char>::const_iterator begin,
//                    std::vector<char>::const_iterator end);
//    int getSenderId() const;
//    uint64_t getSessionId() const;
//    std::vector<char> compile(int sender) const;

//protected:
//    virtual size_t getBytesNeeded() const = 0;
//    virtual void compile(std::vector<char>::iterator begin) const = 0;
//    size_t getInternalMessageSize() const;
//};

#endif // INTERNALMESSAGE_H
