/*
 * PinkReader - GPLv3
 * File: triggerable_thread_group.h
 * Port of: org.quantumbadger.redreader.common.TriggerableThreadGroup.java
 *
 * Every field, method, and logic branch from the original Java file.
 */

#pragma once

#include "triggerable_thread.h"
#include <functional>
#include <memory>
#include <vector>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.common.TriggerableThreadGroup
 *
 * Original: Manages an array of TriggerableThread instances and
 * round-robins trigger() calls across them.
 */
class TriggerableThreadGroup {
public:
	// Port of: public TriggerableThreadGroup(final int threads, final Runnable task)
	//   mThreads = new TriggerableThread[threads];
	//   for(int i = 0; i < threads; i++) { mThreads[i] = new TriggerableThread(task, 0); }
	TriggerableThreadGroup(int threadCount, std::function<void()> task)
		: m_nextThreadToTrigger(0)
	{
		m_threads.reserve(static_cast<size_t>(threadCount));
		for (int i = 0; i < threadCount; i++) {
			m_threads.emplace_back(new TriggerableThread(task, 0));
		}
	}

	// Port of: public void triggerOne()
	//   mThreads[mNextThreadToTrigger].trigger();
	//   mNextThreadToTrigger = (mNextThreadToTrigger + 1) % mThreads.length;
	void triggerOne() {
		m_threads[m_nextThreadToTrigger]->trigger();
		m_nextThreadToTrigger =
			(m_nextThreadToTrigger + 1) % static_cast<int>(m_threads.size());
	}

private:
	// Port of: private final TriggerableThread[] mThreads
	std::vector<std::unique_ptr<TriggerableThread>> m_threads;

	// Port of: private int mNextThreadToTrigger = 0
	int m_nextThreadToTrigger;
};

} // namespace PinkReader
