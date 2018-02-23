#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>

using std::cerr;
using std::endl;
using std::make_shared;
using std::shared_ptr;
using std::vector;

#include "sendmessage.h"
#include "session.h"


ReliableBroadcast::Session::Session(ReliableBroadcast &owner, shared_ptr<ExternalMessage> message):
    mId(getRandomId()),
    mMessage(message),
    mOwner(owner)
{
    mOwner.broadcast(make_shared<SendMessage>(mOwner.mId, message->getMessageHash()));
}

template <int N>
struct BitLenght
{
    enum { value = 1 + BitLenght<N / 2>::value };
};

template <>
struct BitLenght<0>
{
    enum { value = 0 };
};

uint64_t ReliableBroadcast::Session::getRandomId()
{
    cerr << "RAND_MAX = " << RAND_MAX << endl;
    cerr << "bit length = " << BitLenght<RAND_MAX>::value << endl;
    uint64_t random = 0;
    for (size_t bits = 0; bits < 64; bits += BitLenght<RAND_MAX>::value)
    {
        random <<= BitLenght<RAND_MAX>::value;
        random ^= rand();
    }
    return random;
}
