/*
 * PinkReader - GPLv3
 * File: triggerable_thread.h
 * Port of: org.quantumbadger.redreader.common.TriggerableThread.java
 *
 * Every field, method, inner class, and logic branch from the original Java file.
 */

#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <thread>
#include <stdexcept>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.common.TriggerableThread
 *
 * Original: A Runnable-based triggerable thread with delay and retrigger support.
 * Inner class InternalTriggerableThread extends Thread.
 */
class TriggerableThread {
public:
	// Port of: public TriggerableThread(final Runnable task, final long initialDelay)
	TriggerableThread(std::function<void()> task, int64_t initialDelayMs)
		: m_task(std::move(task))
		, m_initialDelayMs(initialDelayMs) {}

	~TriggerableThread() {
		stop();
	}

	TriggerableThread(const TriggerableThread &) = delete;
	TriggerableThread &operator=(const TriggerableThread &) = delete;

	// Port of: public synchronized void trigger()
	void trigger();

	// Stop the internal thread (not in original, needed for RAII in C++)
	void stop();

private:
	// Port of: private synchronized void onSleepEnd()
	//   allowRetrigger = true;
	void onSleepEnd();

	// Port of: private synchronized boolean shouldThreadContinue()
	//   if(shouldRetrigger) { shouldRetrigger = false; return true; }
	//   else { thread = null; allowRetrigger = false; return false; }
	bool shouldThreadContinue();

	// Internal accessors for the static thread function (same as above but take lock internally)
	friend void triggerableThreadRun(int64_t, const std::function<void()> &, TriggerableThread *);
	void onSleepEndInternal();
	bool shouldThreadContinueInternal();

	// Port of: private final Runnable task
	std::function<void()> m_task;

	// Port of: private final long initialDelay
	int64_t m_initialDelayMs;

	// Port of: private InternalTriggerableThread thread
	std::thread m_thread;

	// State flags (ports of private fields in original)
	bool m_allowRetrigger = false;
	bool m_shouldRetrigger = false;

	// Synchronization
	std::mutex m_mutex;

	// Port of: thread == null check
	bool m_running = false;
};

} // namespace PinkReader
