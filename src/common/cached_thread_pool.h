// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/CachedThreadPool.java (lines 1-88)
// Ported from Java to C++

#pragma once

#include <thread>
#include <condition_variable>
#include <mutex>
#include <deque>
#include <functional>
#include <atomic>
#include <string>

namespace PinkReader {

// CachedThreadPool.java line 22: public class CachedThreadPool
class CachedThreadPool {
public:
    // line 34-37: constructor
    CachedThreadPool(int threads, std::string threadName)
        : mMaxThreads(threads)
        , mThreadName(std::move(threadName))
        , mThreadNameCount(0)
        , mRunningThreads(0)
        , mIdleThreads(0) {}

    ~CachedThreadPool();

    // line 39-50: public void add(final Runnable task)
    void add(std::function<void()> task);

private:
    // line 52-87: private final class Executor implements Runnable
    void executorThread();

    // line 24: private final ArrayDeque<Runnable> mTasks = new ArrayDeque<>(16);
    std::deque<std::function<void()>> mTasks;

    // line 27: private final int mMaxThreads;
    int mMaxThreads;

    // line 28: private final String mThreadName;
    std::string mThreadName;

    // line 29: private int mThreadNameCount = 0;
    std::atomic<int> mThreadNameCount;

    // line 31: private int mRunningThreads;
    std::atomic<int> mRunningThreads;

    // line 32: private int mIdleThreads;
    int mIdleThreads;

    // Synchronization (replaces synchronized(mTasks) and mTasks.wait/notifyAll)
    std::mutex mMutex;
    std::condition_variable mCondition;
};

} // namespace PinkReader
