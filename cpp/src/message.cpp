#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr;
using std::vector;

#include "externalmessage.h"
#include "message.h"


shared_ptr<Message> Message::parse(vector<char>::iterator begin, vector<char>::iterator end)
{
    if (begin >= end) {
        return nullptr;
    }
    if (*begin == MessageType::MESSAGE) {
        return make_shared<ExternalMessage>(begin + 1, end);
    } else {
        return nullptr;
    }
}
