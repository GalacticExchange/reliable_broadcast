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
                                    shared_ptr<InternalMessage> internalMessage):
    Session(internalMessage->getSessionId(), owner)
{
    if (internalMessage->getType() == Message::MessageType::SEND)
    {
        shared_ptr<SendMessage> sendMessage = dynamic_pointer_cast<SendMessage>(internalMessage);
        mMessage = sendMessage->getMessagePtr();
        mMessageHash = calculateMessageHash(mMessage);
    } else {
        shared_ptr<HashMessage> hashMessage = dynamic_pointer_cast<HashMessage>(internalMessage);
        mPendingHashMessages.push(hashMessage);
    }
}

void ReliableBroadcast::Session::processMessage(std::shared_ptr<InternalMessage> message)
{
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
        shared_ptr<SendMessage> sendMessage = dynamic_pointer_cast<SendMessage>(message);
        bool messageWasAdded = false;
        {
            lock_guard<mutex> lock(mMessageMutex);

            if(!mMessage)
            {
                mMessage = sendMessage->getMessagePtr();
                mMessageHash = calculateMessageHash(mMessage);
                messageWasAdded = true;
            }
        }
        if (messageWasAdded)
        {
            while (!mPendingHashMessages.empty())
            {
                processMessage(mPendingHashMessages.front());
                mPendingHashMessages.pop();
            }
        }
        mOwner.broadcast(make_shared<EchoMessage>(mOwner.mId, mId, mMessageHash));
    } else {
        shared_ptr<HashMessage> hashMessage = dynamic_pointer_cast<HashMessage>(message);
        bool process = false;
        {
            lock_guard<mutex> lock(mMessageMutex);
            if (mMessage)
            {
                process = true;
            } else {
                mPendingHashMessages.push(hashMessage);
                // cerr << "(wait)" << endl;
            }
        }
        if (process)
        {
            if (message->getType() == Message::ECHO_MESSAGE)
            {
                shared_ptr<EchoMessage> echoMessage = dynamic_pointer_cast<EchoMessage>(message);
                if (!mReadyMessageWasSent)
                {
                    size_t count;
                    {
                        lock_guard<mutex> lock(mEchoMessageCounterMutex);
                        mEchoMessageCounter.insert(echoMessage->getSenderId());
                        count = mEchoMessageCounter.size();
                    }
                    if (count >= mEchoMessageCountTarget)
                    {
                        bool _false = false;
                        if (mReadyMessageWasSent.compare_exchange_strong(_false, true))
                        {
                            mOwner.broadcast(make_shared<ReadyMessage>(*echoMessage));
                        }
                    }
                }
            } else if (message->getType() == Message::READY) {
                shared_ptr<ReadyMessage> readyMessage = dynamic_pointer_cast<ReadyMessage>(message);
                if (!mDelivered)
                {
                    size_t count;
                    {
                        lock_guard<mutex> lock(mReadyMessageCounterMutex);
                        mReadyMessageCounter.insert(readyMessage->getSenderId());
                        count = mReadyMessageCounter.size();
                    }
                    if (!mReadyMessageWasSent && count > t)
                    {
                        bool _false = false;
                        if (mReadyMessageWasSent.compare_exchange_strong(_false, true))
                        {
                            mOwner.broadcast(make_shared<ReadyMessage>(*readyMessage));
                        }
                    }
                    if (!mDelivered && count > 2 * t)
                    {
                        bool _false = false;
                        if (mDelivered.compare_exchange_strong(_false, true))
                        {
                            deliver();
                        }
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

ReliableBroadcast::Session::Session(uint64_t id,
                                    ReliableBroadcast &owner):
    mId(id),
    n(owner.mNodes.size()),
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

size_t ReliableBroadcast::Session::getT(size_t n)
{
    size_t t = n / 3;
    if (t * 3 == n)
    {
        --t;
    }
    return t;
}

size_t ReliableBroadcast::Session::getEchoMessageCountTarget(size_t n, size_t t)
{
    return (n + t + 1) / 2 + (n + t + 1) % 2;
}

boost::uuids::detail::sha1 ReliableBroadcast::Session::sHashFunction;

void ReliableBroadcast::Session::deliver()
{
    const size_t UPDATE_INTERVAL = 100;
    size_t commitCount = mOwner.mCommitCounter.fetch_add(1) + 1;
    if (commitCount == 1)
    {
        mOwner.mStartTime = std::chrono::system_clock::now();
    }
    if (commitCount % UPDATE_INTERVAL == 0)
    {
        size_t runningTimeSec = std::chrono::duration_cast<std::chrono::seconds>(
                    std::chrono::system_clock::now() - mOwner.mStartTime).count();
        if (runningTimeSec)
        {
            cout << commitCount / runningTimeSec << " commits per second" << endl;
        }
    }
//    cerr << "Deliver message" << endl;
//    for (char character : *mMessage)
//    {
//        cout << character;
//    }
//    cout << endl;
    mOwner.mSessions.removeSession(mId);
}
