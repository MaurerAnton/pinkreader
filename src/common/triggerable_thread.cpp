/*
 * PinkReader - GPLv3
 * File: triggerable_thread.cpp
 * Port of: org.quantumbadger.redreader.common.TriggerableThread.java (method bodies)
 *
 * Every field, method, inner class, and logic branch from the original Java file.
 */

#include "triggerable_thread.h"
#include <chrono>

namespace PinkReader {

// Port of: private final class InternalTriggerableThread extends Thread
// In the original, run() does:
//   do { Thread.sleep(initialDelay); onSleepEnd(); task.run(); } while(shouldThreadContinue());
void triggerableThreadRun(
		int64_t initialDelayMs,
		const std::function<void()> &task,
		TriggerableThread *owner)
{
	do {
		// Port of: try { Thread.sleep(initialDelay); }
		//   catch(InterruptedException e) { throw new RuntimeException(e); }
		try {
			std::this_thread::sleep_for(std::chrono::milliseconds(initialDelayMs));
		} catch (...) {
			throw std::runtime_error("InternalTriggerableThread interrupted");
		}

		// Port of: onSleepEnd();
		owner->onSleepEndInternal();

		// Port of: task.run();
		task();

	} while (owner->shouldThreadContinueInternal());
}

// Port of: public synchronized void trigger()
void TriggerableThread::trigger() {
	std::lock_guard<std::mutex> lock(m_mutex);

	// Port of: if(thread == null)
	if (!m_running) {
		// Port of: thread = new InternalTriggerableThread(); thread.start();
		m_running = true;
		m_thread = std::thread(triggerableThreadRun,
			m_initialDelayMs, std::cref(m_task), this);

	} else if (m_allowRetrigger) {
		// Port of: else if(allowRetrigger) { shouldRetrigger = true; }
		m_shouldRetrigger = true;
	}
}

void TriggerableThread::stop() {
	std::lock_guard<std::mutex> lock(m_mutex);
	m_shouldRetrigger = false;
	m_allowRetrigger = false;

	if (m_running && m_thread.joinable()) {
		m_mutex.unlock();
		m_thread.join();
		m_mutex.lock();
		m_running = false;
	}
}

// Port of: private synchronized void onSleepEnd()
void TriggerableThread::onSleepEnd() {
	std::lock_guard<std::mutex> lock(m_mutex);
	// Port of: allowRetrigger = true;
	m_allowRetrigger = true;
}

void TriggerableThread::onSleepEndInternal() {
	onSleepEnd();
}

// Port of: private synchronized boolean shouldThreadContinue()
bool TriggerableThread::shouldThreadContinue() {
	std::lock_guard<std::mutex> lock(m_mutex);

	// Port of: if(shouldRetrigger)
	if (m_shouldRetrigger) {
		// Port of: shouldRetrigger = false; return true;
		m_shouldRetrigger = false;
		return true;

	} else {
		// Port of: thread = null; allowRetrigger = false; return false;
		m_running = false;
		m_allowRetrigger = false;
		return false;
	}
}

bool TriggerableThread::shouldThreadContinueInternal() {
	return shouldThreadContinue();
}

} // namespace PinkReader
