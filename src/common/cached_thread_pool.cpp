// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/CachedThreadPool.java (lines 1-88)
// Implementation

#include "cached_thread_pool.h"

namespace PinkReader {

CachedThreadPool::~CachedThreadPool() {
    // Wake all threads so they can exit
    {
        std::lock_guard<std::mutex> lock(mMutex);
        mMaxThreads = 0; // Prevent new threads
    }
    mCondition.notify_all();
}

// line 39-50: public void add(final Runnable task)
void CachedThreadPool::add(std::function<void()> task) {
    // line 41: synchronized(mTasks) {
    std::unique_lock<std::mutex> lock(mMutex);

    // line 42: mTasks.addLast(task);
    mTasks.push_back(std::move(task));

    // line 43: mTasks.notifyAll();
    mCondition.notify_one();

    // line 45-48: if(mIdleThreads < 1 && mRunningThreads < mMaxThreads)
    if (mIdleThreads < 1 && mRunningThreads.load() < mMaxThreads) {
        // line 46: mRunningThreads++;
        mRunningThreads++;

        // line 47: new Thread(mExecutor, mThreadName + " " + (mThreadNameCount++)).start();
        std::string name = mThreadName + " " + std::to_string(mThreadNameCount++);
        std::thread worker(&CachedThreadPool::executorThread, this);
        worker.detach();
    }
    // line 49: }
}

// line 54-86: Executor.run()
void CachedThreadPool::executorThread() {
    // line 57: while(true) {
    while (true) {
        std::function<void()> taskToRun;

        // line 61: synchronized(mTasks) {
        {
            std::unique_lock<std::mutex> lock(mMutex);

            // line 63: if(mTasks.isEmpty()) {
            if (mTasks.empty()) {
                // line 65: mIdleThreads++;
                mIdleThreads++;

                // line 67-68: mTasks.wait(30_000)
                mCondition.wait_for(lock, std::chrono::seconds(30));

                // line 72: mIdleThreads--;
                mIdleThreads--;

                // line 75: if(mTasks.isEmpty()) {
                if (mTasks.empty()) {
                    // line 76: mRunningThreads--;
                    mRunningThreads--;
                    // line 77: return;
                    return;
                }
            }

            // line 81: taskToRun = mTasks.removeFirst();
            taskToRun = std::move(mTasks.front());
            mTasks.pop_front();
            // line 82: }
        }

        // line 84: taskToRun.run();
        taskToRun();
    }
}

} // namespace PinkReader
