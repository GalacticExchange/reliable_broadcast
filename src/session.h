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
    typedef __int128 Id;

    const Id mId;
    const size_t n;
    const size_t t;
    const size_t mEchoMessageCountTarget;
    ReliableBroadcast &mOwner;


    std::mutex mEchoMessageCounterMutex;
    std::map<std::pair<uint64_t, std::vector<char>>, size_t> mEchoMessageCounter;
    std::mutex mReadyMessageCounterMutex;
    std::map<std::pair<uint64_t, std::vector<char>>, size_t> mReadyMessageCounter;
    std::atomic<bool> mReadyMessageWasSent;
    std::atomic<bool> mDelivered;

public:
    Session(ReliableBroadcast &owner, Id id);

    void processMessage(std::shared_ptr<Message> message);
    Id getId() const;
    static Id getRandomId();
    static Id getId(std::shared_ptr<Message> message);

private:
    Session(Id mId, ReliableBroadcast &owner);

    static std::shared_ptr<std::vector<char>> calculateMessageHash(
            std::shared_ptr<const std::vector<char>> message);
    static size_t getT(size_t n);
    static size_t getEchoMessageCountTarget(size_t n, size_t t);
    void deliver(std::shared_ptr<Message> message);
};

#endif // SESSION_H
