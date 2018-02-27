#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr;
using std::vector;

#include "externalmessage.h"
#include "message.h"
#include "sendmessage.h"


shared_ptr<Message> Message::parse(vector<char>::iterator begin, vector<char>::iterator end)
{
    if (begin >= end) {
        return nullptr;
    }
    if (*begin == MessageType::SEND) {
        return make_shared<SendMessage>(begin + 1, end);
    } else {
        return make_shared<ExternalMessage>(begin, end);
    }
}
