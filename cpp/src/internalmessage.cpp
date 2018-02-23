#include <vector>

using std::move;
using std::vector;

#include "internalmessage.h"


InternalMessage::InternalMessage(int sender, vector<char> &&messageHash):
    mSender(sender),
    mMessageHash(move(messageHash))
{

}
