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


ReliableBroadcast::Session::Session(ReliableBroadcast &owner, shared_ptr<ExternalMessage> message):
    mId(getRandomId()),
    mMessage(message),
    mOwner(owner)
{
    mEchoMessageCounter.insert(mOwner.mId);
    mReadyMessageCounter.insert(mOwner.mId);
    shared_ptr<SendMessage> sendMessage =
            make_shared<SendMessage>(mOwner.mId, mId, vector<char>(message->getMessageHash()));
    cerr << "Brodcast SEND message" << endl;
    mOwner.broadcast(sendMessage);
    cerr << "Brodcast ECHO message" << endl;
    mOwner.broadcast(make_shared<EchoMessage>(*sendMessage));
}

void ReliableBroadcast::Session::processMessage(std::shared_ptr<InternalMessage> message)
{
    if (message->getType() != Message::SEND)
    {
        if (message->getMessageHash() != mMessage->getMessageHash())
        {
            return;
        }
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

    if (message->getType() == Message::SEND) {
        cerr << "Received SEND message" << endl;
        shared_ptr<SendMessage> sendMessage = dynamic_pointer_cast<SendMessage>(message);
        cerr << "Brodcast ECHO message" << endl;
        mOwner.broadcast(make_shared<EchoMessage>(*sendMessage));
    } else if (message->getType() == Message::ECHO_MESSAGE) {
        cerr << "Received ECHO message" << endl;
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
                    cerr << "Brodcast READY message" << endl;
                    mOwner.broadcast(make_shared<ReadyMessage>(*echoMessage));
                }
            }
        }
    } else if (message->getType() == Message::READY) {
        cerr << "Received READY message" << endl;
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
                        mReadyMessageCounter.insert(mOwner.mId);
                        sendReadyMessage = true;
                    }
                }
            }
            if (sendReadyMessage)
            {
                cerr << "Brodcast READY message" << endl;
                mOwner.broadcast(make_shared<ReadyMessage>(*readyMessage));
            }
        } else {
            {
                lock_guard<mutex> lock(mReadyMessageCounterMutex);
                mReadyMessageCounter.insert(readyMessage->getSenderId());
                if (mReadyMessageCounter.size() >= 2 * t + 1)
                {
                    if (mDelivered.compare_exchange_strong(_false, true))
                    {
                        cerr << "Deliver message" << endl;
                        deliver();
                    }
                }
            }
        }
    }
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

void ReliableBroadcast::Session::deliver()
{
    mOwner.removeSession(mId);
    for (char character : mMessage->getMessage())
    {
        cout << character;
    }
    cout << endl;
}
