#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr;
using std::vector;

#include "echomessage.h"
#include "externalmessage.h"
#include "message.h"
#include "readymessage.h"
#include "sendmessage.h"


//shared_ptr<Message> Message::parse(vector<char>::iterator begin, vector<char>::iterator end)
//{
//    if (begin >= end) {
//        return nullptr;
//    }
//    if (*begin == MessageType::SEND) {
//        return make_shared<SendMessage>(begin + 1, end);
//    } if (*begin == MessageType::ECHO_MESSAGE) {
//        return make_shared<EchoMessage>(begin + 1, end);
//    } if (*begin == MessageType::READY) {
//        return make_shared<ReadyMessage>(begin + 1, end);
//    } else {
//        return make_shared<ExternalMessage>(begin, end);
//    }
//}

Session::Id Message::getSessionId() const
{
    auto client = mInternalMessageReader.getClientId();
    auto nonce = mInternalMessageReader.getNonce();
    Session::Id id = client;
    id = (id << (sizeof(nonce) * 8)) ^ nonce;
    return id;
}
