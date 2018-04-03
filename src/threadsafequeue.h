#ifndef THREADSAFEQUEUE_H
#define THREADSAFEQUEUE_H

#include <condition_variable>
#include <mutex>
#include <queue>
#include "iostream" //todo

template<class T>
class ThreadSafeQueue {
    std::queue<T> mQueue;
    std::mutex mMutex;
    std::condition_variable mCondition;

public:
    T pop() {
        std::unique_lock<std::mutex> mlock(mMutex);
        while (mQueue.empty()) {
            mCondition.wait(mlock);
        }
        auto item = mQueue.front();
        mQueue.pop();
        return item;
    }

    void push(const T &item) {
        std::unique_lock<std::mutex> mlock(mMutex);
        mQueue.push(item);
        mlock.unlock();
        mCondition.notify_one();
    }

    bool isEmpty() {
        return mQueue.empty();
    }

    std::vector<T> drainTo(int n) {

        std::unique_lock<std::mutex> mlock(mMutex);
        std::vector<T> elements;

        if (mQueue.empty()) {
            return elements;
        }

        for (int i = n; i > 0; i--) {
            elements.push_back(mQueue.front());
            mQueue.pop();
            if (mQueue.size() < i) {
                break;
            }
        }

        mlock.unlock();

        return elements;

    };
};

#endif // THREADSAFEQUEUE_H
