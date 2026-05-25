/*
 * PinkReader - GPLv3
 * File: prioritised_cached_thread_pool.h
 * Exact port of PrioritisedCachedThreadPool.java (107 lines)
 *
 * Ports every field, method, and inner class (Task, Executor) exactly.
 */

#pragma once

#include <QString>
#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <vector>
#include <memory>

#include "priority.h"

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.common.PrioritisedCachedThreadPool
 *
 * A thread pool that caches idle threads and prioritizes tasks.
 * Every field, method, and inner class matches the Java original exactly.
 */
class PrioritisedCachedThreadPool {
public:
    // ========================================================================
    // Constructor — Java lines 36-39
    // Port of: public PrioritisedCachedThreadPool(final int threads,
    //                                              final String threadName)
    // ========================================================================
    PrioritisedCachedThreadPool(int threads, const QString &threadName);

    // ========================================================================
    // Destructor — shuts down the pool (C++ addition, no Java equivalent)
    // ========================================================================
    ~PrioritisedCachedThreadPool();

    // ========================================================================
    // add — Java lines 41-52
    // Port of: public void add(final Task task)
    // ========================================================================
    void add(class Task *task);

    // ========================================================================
    // Inner class: Task — Java lines 54-59
    // Port of: public static abstract class Task
    // ========================================================================
    class Task {
    public:
        virtual ~Task() = default;

        // Port of: @NonNull public abstract Priority getPriority();
        virtual Priority getPriority() = 0;

        // Port of: public abstract void run();
        virtual void run() = 0;
    };

private:
    // ========================================================================
    // Inner class: Executor — Java lines 61-106
    // Port of: private final class Executor implements Runnable
    // ========================================================================
    class Executor : public QThread {
    public:
        explicit Executor(PrioritisedCachedThreadPool *pool);

    protected:
        // Port of: @Override public void run()
        void run() override;

    private:
        PrioritisedCachedThreadPool *m_pool;
    };

    // ========================================================================
    // Fields — port exact from Java
    // ========================================================================

    // Port of: private final ArrayList<Task> mTasks = new ArrayList<>(16);
    std::vector<Task *> m_tasks;

    // Port of: private final Executor mExecutor = new Executor();
    // In C++, Executor instances are created on the fly as threads are spawned.
    // The "mExecutor" in Java was a Runnable; threads were created with it.

    // Port of: private final int mMaxThreads;
    int m_maxThreads;

    // Port of: private final String mThreadName;
    QString m_threadName;

    // Port of: private int mThreadNameCount = 0;
    int m_threadNameCount = 0;

    // Port of: private int mRunningThreads;
    int m_runningThreads = 0;

    // Port of: private int mIdleThreads;
    int m_idleThreads = 0;

    // Synchronization — C++ equivalents of Java synchronized(mTasks) and mTasks.wait/notifyAll
    QMutex m_mutex;
    QWaitCondition m_condition;

    bool m_shuttingDown = false; // C++ addition for graceful shutdown

    // Allow Executor to access private fields
    friend class Executor;
};

} // namespace PinkReader
