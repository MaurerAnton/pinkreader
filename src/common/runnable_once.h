// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/RunnableOnce.java
#pragma once

#include <functional>
#include <atomic>

namespace PinkReader {

/**
 * A Runnable that executes only once, even if run() is called multiple times.
 * Equivalent to: org.quantumbadger.redreader.common.RunnableOnce
 */
class RunnableOnce {
private:
	std::atomic<bool> mAlreadyRun{false};
	std::function<void()> mRunnable;

public:
	/** A shared instance that does nothing. */
	static inline const std::function<void()> DO_NOTHING = []() {};

	/** Constructs a RunnableOnce with the given runnable. */
	explicit RunnableOnce(std::function<void()> runnable)
		: mRunnable(std::move(runnable)) {}

	/** Pre-made instance that does nothing. */
	static RunnableOnce doNothing() {
		return RunnableOnce(DO_NOTHING);
	}

	/**
	 * Runs the contained runnable exactly once.
	 * Subsequent calls are no-ops.
	 */
	void run() {
		bool expected = false;
		if (mAlreadyRun.compare_exchange_strong(expected, true)) {
			mRunnable();
		}
	}
};

} // namespace PinkReader
