#include "reliablebroadcast.h"
#include "session.h"

#include <boost/log/trivial.hpp>

#include <cstdlib>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <vector>

using std::cerr;
using std::cout;
using std::dynamic_pointer_cast;
using std::endl;
using std::lock_guard;
using std::make_shared;
using std::map;
using std::mutex;
using std::pair;
using std::shared_ptr;
using std::vector;


Session::Session(ReliableBroadcast &owner,
                 Id id):
    Session(id, owner)
{

}

void Session::processMessage(std::shared_ptr<Message> message)
{    
    BOOST_LOG_TRIVIAL(debug) << "Process message of type "
                             << message->getType() << " with nonce "
                             << message->getNonce();

//    cerr << "Process message ";
//    if (message->getType() == Message::MessageType::SEND)
//    {
//        cerr << "SEND";
//    } else if (message->getType() == Message::MessageType::ECHO_MESSAGE) {
//        cerr << "ECHO";
//    } else if (message->getType() == Message::MessageType::READY) {
//        cerr << "READY";
//    } else {
//        cerr << "UNKNOWN";
//    }
//    cerr << " from " << message->getSenderId()
//         << " in session #" << message->getSessionId() << endl;    

    if (message->getType() == Message::MessageType::SEND)
    {
        mOwner.broadcast(Message::MessageType::ECHO_MESSAGE, message);
    } else if (message->getType() == Message::MessageType::ECHO_MESSAGE) {
        if (!mReadyMessageWasSent)
        {
            size_t count;
            {
                lock_guard<mutex> lock(mEchoMessageCounterMutex);
                count = ++mEchoMessageCounter[make_pair(message->getNodeId(), message->getData())];
            }
            if (count >= mEchoMessageCountTarget)
            {
                bool _false = false;
                if (mReadyMessageWasSent.compare_exchange_strong(_false, true))
                {
                    mOwner.broadcast(Message::MessageType::READY, message);
                }
            }
        }
    } else if (message->getType() == Message::MessageType::READY) {
        if (!mDelivered)
        {
            size_t count;
            {
                lock_guard<mutex> lock(mReadyMessageCounterMutex);
                count = ++mReadyMessageCounter[make_pair(message->getNodeId(), message->getData())];
            }
            if (!mReadyMessageWasSent && count > t)
            {
                bool _false = false;
                if (mReadyMessageWasSent.compare_exchange_strong(_false, true))
                {
                    mOwner.broadcast(Message::MessageType::READY, message);
                }
            }
            if (!mDelivered && count > 2 * t)
            {
                bool _false = false;
                if (mDelivered.compare_exchange_strong(_false, true))
                {
                    deliver(message);
                }
            }
        }
    } else {
        throw std::logic_error("Unknown message type");
    }
}

Session::Id Session::getId() const
{
    return mId;
}

Session::Session(Id id,
                 ReliableBroadcast &owner):
    mId(id),
    n(owner.getNodesCount()),
    t(Session::getT(n)),
    mEchoMessageCountTarget(Session::getEchoMessageCountTarget(n, t)),
    mOwner(owner),
    mReadyMessageWasSent(false),
    mDelivered(false)
{

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

Session::Id Session::getRandomId()
{
    Id random = 0;
    for (size_t bits = 0; bits < sizeof(Id); bits += BitLenght<RAND_MAX>::value)
    {
        random <<= BitLenght<RAND_MAX>::value;
        random ^= rand();
    }
    return random;
}

Session::Id Session::getId(std::shared_ptr<Message> message)
{
    Id id = message->getClientId();
    auto nonce = message->getNonce();
    id = (id << sizeof(nonce)) ^ nonce;
    return id;
}

shared_ptr<vector<char> > Session::calculateMessageHash(
        shared_ptr<const vector<char> > message)
{
    unsigned int digest[5];
    boost::uuids::detail::sha1 hashFunction;
    hashFunction.process_bytes(&*message->begin(), message->size());
    hashFunction.get_digest(digest);

    shared_ptr<vector<char>> hash = make_shared<vector<char>>(20);
    for(size_t i = 0; i < 5; ++i)
    {
        const char* tmp = reinterpret_cast<char*>(digest);
        (*hash)[i*4] = tmp[i*4 + 3];
        (*hash)[i*4 + 1] = tmp[i*4 + 2];
        (*hash)[i*4 + 2] = tmp[i*4 + 1];
        (*hash)[i*4 + 3] = tmp[i*4];
    }

    return hash;
}

size_t Session::getT(size_t n)
{
    size_t t = n / 3;
    if (t * 3 == n)
    {
        --t;
    }
    return t;
}

size_t Session::getEchoMessageCountTarget(size_t n, size_t t)
{
    return (n + t + 1) / 2 + (n + t + 1) % 2;
}

void Session::deliver(std::shared_ptr<Message> message)
{
    mOwner.deliver(message);
}
