/*
 * PinkReader - GPLv3
 * File: prioritised_cached_thread_pool.cpp
 * Exact port of PrioritisedCachedThreadPool.java (107 lines)
 */

#include "prioritised_cached_thread_pool.h"

#include <QThread>
#include <stdexcept>

namespace PinkReader {

// ============================================================================
// PrioritisedCachedThreadPool — Constructor
// Port of: public PrioritisedCachedThreadPool(final int threads,
//                                              final String threadName)
// Java lines 36-39
// ============================================================================
PrioritisedCachedThreadPool::PrioritisedCachedThreadPool(
        int threads,
        const QString &threadName)
    : m_maxThreads(threads)
    , m_threadName(threadName)
{
    m_tasks.reserve(16);
}

// ============================================================================
// Destructor — C++ addition for graceful shutdown
// ============================================================================
PrioritisedCachedThreadPool::~PrioritisedCachedThreadPool()
{
    m_shuttingDown = true;
    m_condition.wakeAll();
    // Running threads will exit when they check m_shuttingDown
}

// ============================================================================
// add — Java lines 41-52
// Port of: public void add(final Task task)
// ============================================================================
void PrioritisedCachedThreadPool::add(Task *task)
{
    {
        QMutexLocker locker(&m_mutex);
        // Port of: mTasks.add(task);
        m_tasks.push_back(task);
        // Port of: mTasks.notifyAll();
        m_condition.wakeAll();

        // Port of: if(mIdleThreads < 1 && mRunningThreads < mMaxThreads)
        if (m_idleThreads < 1 && m_runningThreads < m_maxThreads) {
            // Port of: mRunningThreads++;
            m_runningThreads++;
            // Port of: new Thread(mExecutor, mThreadName + " " + (mThreadNameCount++)).start();
            auto *executor = new Executor(this);
            executor->setObjectName(m_threadName + " " + QString::number(m_threadNameCount++));
            executor->start();
        }
    }
}

// ============================================================================
// Executor — Constructor
// Port of: private final class Executor implements Runnable
// Java lines 61-63
// ============================================================================
PrioritisedCachedThreadPool::Executor::Executor(PrioritisedCachedThreadPool *pool)
    : m_pool(pool)
{
}

// ============================================================================
// Executor::run — Java lines 64-105
// Port of: @Override public void run()
// ============================================================================
void PrioritisedCachedThreadPool::Executor::run()
{
    // Port of: while(true) {
    while (true) {

        Task *taskToRun = nullptr;

        {
            QMutexLocker locker(&m_pool->m_mutex);

            // Port of: if(mTasks.isEmpty()) {
            if (!m_pool->m_shuttingDown && m_pool->m_tasks.empty()) {

                // Port of: mIdleThreads++;
                m_pool->m_idleThreads++;

                // Port of: try { mTasks.wait(30_000); }
                //         catch(InterruptedException e) { throw new RuntimeException(e); }
                //         finally { mIdleThreads--; }
                m_pool->m_condition.wait(&m_pool->m_mutex, 30000);

                // Port of: finally { mIdleThreads--; }
                m_pool->m_idleThreads--;

                // Port of: if(mTasks.isEmpty()) { mRunningThreads--; return; }
                if (m_pool->m_shuttingDown || m_pool->m_tasks.empty()) {
                    m_pool->m_runningThreads--;
                    return;
                }
            } else if (m_pool->m_shuttingDown) {
                m_pool->m_runningThreads--;
                return;
            }

            // Port of: int taskIndex = -1;
            //         for(int i = 0; i < mTasks.size(); i++) {
            //             if(taskToRun == null || mTasks.get(i).getPriority()
            //                     .isHigherPriorityThan(taskToRun.getPriority())) {
            //                 taskToRun = mTasks.get(i);
            //                 taskIndex = i;
            //             }
            //         }
            int taskIndex = -1;
            for (size_t i = 0; i < m_pool->m_tasks.size(); i++) {
                if (taskToRun == nullptr
                        || m_pool->m_tasks[i]->getPriority().isHigherPriorityThan(
                                taskToRun->getPriority())) {
                    taskToRun = m_pool->m_tasks[i];
                    taskIndex = static_cast<int>(i);
                }
            }

            // Port of: mTasks.remove(taskIndex);
            if (taskIndex >= 0) {
                m_pool->m_tasks.erase(m_pool->m_tasks.begin() + taskIndex);
            }
        }

        // Port of: assert taskToRun != null;
        //         taskToRun.run();
        if (taskToRun != nullptr) {
            taskToRun->run();
        }
    }
}

} // namespace PinkReader
