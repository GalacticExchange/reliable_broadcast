#ifndef THREADSAFEQUEUE_H
#define THREADSAFEQUEUE_H

#include <condition_variable>
#include <mutex>
#include <queue>


template<class T>
class ThreadSafeQueue
{
    std::queue<T> mQueue;
    std::mutex mMutex;
    std::condition_variable mCondition;

public:
    T pop()
    {
        std::unique_lock<std::mutex> mlock(mMutex);
        while (mQueue.empty())
        {
            mCondition.wait(mlock);
        }
        auto item = mQueue.front();
        mQueue.pop();
        return item;
    }

    void push(const T& item)
    {
        std::unique_lock<std::mutex> mlock(mMutex);
        mQueue.push(item);
        mlock.unlock();
        mCondition.notify_one();
    }
};

#endif // THREADSAFEQUEUE_H
