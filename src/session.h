#ifndef SESSION_H
#define SESSION_H

#include <boost/uuid/sha1.hpp>

#include <atomic>
#include <map>
#include <memory>
#include <mutex>
#include <unordered_set>


class ReliableBroadcast;

class Session
{
public:
    class Id
    {
        friend class std::hash<Id>;

        uint64_t mMChainHash;
        uint64_t mClientId;
        uint64_t mNonce;

    public:
        Id(uint64_t mChainHash, uint64_t clientId, uint64_t nonce);

        bool operator == (const Id &id) const;
        uint64_t getMChainHash() const;
    };

    const Id mId;
    const size_t n;
    const size_t t;
    const size_t mEchoMessageCountTarget;
    ReliableBroadcast &mOwner;


    std::mutex mEchoMessageCounterMutex;
    std::map<std::vector<char>, std::unordered_set<uint64_t>> mEchoMessageCounter;
    std::mutex mReadyMessageCounterMutex;
    std::map<std::vector<char>, std::unordered_set<uint64_t>> mReadyMessageCounter;
    std::atomic<bool> mReadyMessageWasSent;
    std::atomic<bool> mDelivered;

public:
    Session(ReliableBroadcast &owner, Id id);

    void processMessage(std::shared_ptr<Message> message);
    Id getId() const;    
    static Id getId(std::shared_ptr<Message> message);

private:
    Session(Id mId, ReliableBroadcast &owner);

    static std::shared_ptr<std::vector<char>> calculateMessageHash(
            std::shared_ptr<const std::vector<char>> message);
    static size_t getT(size_t n);
    static size_t getEchoMessageCountTarget(size_t n, size_t t);
    void deliver(std::shared_ptr<Message> message);
};

namespace std {

template<>
class hash<Session::Id>
{
public:
    size_t operator () (const Session::Id &id) const;
};

}

#endif // SESSION_H
