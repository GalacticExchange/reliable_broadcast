#include <cstdlib>
#include <iostream>
#include <memory>
#include <mutex>
#include <vector>

using std::cerr;
using std::cout;
using std::dynamic_pointer_cast;
using std::endl;
using std::lock_guard;
using std::make_shared;
using std::mutex;
using std::shared_ptr;
using std::vector;

#include "echomessage.h"
#include "readymessage.h"
#include "sendmessage.h"
#include "session.h"


ReliableBroadcast::Session::Session(ReliableBroadcast &owner,
                                    shared_ptr<const vector<char>> message):
    mId(getRandomId()),
    mMessage(message),
    mMessageHash(calculateMessageHash(message)),
    mOwner(owner)
{    

}

ReliableBroadcast::Session::Session(ReliableBroadcast &owner,
                                    shared_ptr<const vector<char> > message,
                                    uint64_t id):
    mId(id),
    mMessage(message),
    mMessageHash(calculateMessageHash(message)),
    mOwner(owner)
{

}

void ReliableBroadcast::Session::start()
{
    shared_ptr<SendMessage> sendMessage =
            make_shared<SendMessage>(mOwner.mId, mId, mMessage);
    cerr << "\tBrodcast SEND message" << endl;
    mOwner.broadcast(sendMessage);
}

void ReliableBroadcast::Session::processMessage(std::shared_ptr<HashMessage> message)
{
    cerr << "Process message "
         << ((message->getType() == Message::MessageType::ECHO_MESSAGE) ? "ECHO" : "READY")
         << " from " << message->getSenderId()
         << " in session #" << message->getSessionId() << endl;

    if (message->getMessageHash() != *mMessageHash)
    {
        return;
    }

    bool _false = false;
    const size_t n = mOwner.mNodes.size();
    size_t t = n / 3;
    if (3 * t >= n)
    {
        --t;
    }
    size_t echoMessageTarget = (n + t + 1) / 2;
    if (2 * echoMessageTarget < n + t + 1)
    {
        ++echoMessageTarget;
    }

    if (message->getType() == Message::ECHO_MESSAGE)
    {
        cerr << "Received ECHO message from " << message->getSenderId() << endl;
        shared_ptr<EchoMessage> echoMessage = dynamic_pointer_cast<EchoMessage>(message);
        if (!mFirstReadyMessageWasSent)
        {
            size_t count;
            {
                lock_guard<mutex> lock(mEchoMessageCounterMutex);
                mEchoMessageCounter.insert(echoMessage->getSenderId());
                count = mEchoMessageCounter.size();
            }
            if (count >= echoMessageTarget)
            {
                if (mFirstReadyMessageWasSent.compare_exchange_strong(_false, true))
                {
                    cerr << "\tBrodcast READY message" << endl;
                    mOwner.broadcast(make_shared<ReadyMessage>(*echoMessage));
                }
            }
        }
    } else if (message->getType() == Message::READY) {
        cerr << "Received READY message from " << message->getSenderId() << endl;
        shared_ptr<ReadyMessage> readyMessage = dynamic_pointer_cast<ReadyMessage>(message);
        if (!mSecondReadyMessageWasSent)
        {
            bool sendReadyMessage = false;
            {
                lock_guard<mutex> lock(mReadyMessageCounterMutex);
                mReadyMessageCounter.insert(readyMessage->getSenderId());
                if (mEchoMessageCounter.size() >= t + 1)
                {
                    if (mSecondReadyMessageWasSent.compare_exchange_strong(_false, true))
                    {
                        mReadyMessageCounter.clear();                        
                        sendReadyMessage = true;
                    }
                }
            }
            if (sendReadyMessage)
            {
                cerr << "\tBrodcast READY message" << endl;
                mOwner.broadcast(make_shared<ReadyMessage>(*readyMessage));
            }
        } else {
            bool deliverFlag = false;
            {
                lock_guard<mutex> lock(mReadyMessageCounterMutex);
                mReadyMessageCounter.insert(readyMessage->getSenderId());
                if (mReadyMessageCounter.size() >= 2 * t + 1)
                {
                    if (mDelivered.compare_exchange_strong(_false, true))
                    {
                        deliverFlag = true;
                    }
                }
            }
            if (deliverFlag)
            {
                cerr << "Deliver message" << endl;
                deliver();
            }
        }
    }
}

void ReliableBroadcast::Session::processSendMessage()
{
    cerr << "\tBrodcast ECHO message" << endl;
    mOwner.broadcast(make_shared<EchoMessage>(mOwner.mId, mId, mMessageHash));
}

uint64_t ReliableBroadcast::Session::getId() const
{
    return mId;
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
    uint64_t random = 0;
    for (size_t bits = 0; bits < 64; bits += BitLenght<RAND_MAX>::value)
    {
        random <<= BitLenght<RAND_MAX>::value;
        random ^= rand();
    }
    return random;
}

shared_ptr<vector<char> > ReliableBroadcast::Session::calculateMessageHash(
        shared_ptr<const vector<char> > message)
{
    sHashFunction.process_bytes(&*message->begin(), message->size());
    unsigned int digest[5];

    sHashFunction.get_digest(digest);

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

boost::uuids::detail::sha1 ReliableBroadcast::Session::sHashFunction;

void ReliableBroadcast::Session::deliver()
{
    for (char character : *mMessage)
    {
        cout << character;
    }
    cout << endl;
    mOwner.removeSession(mId);
}
