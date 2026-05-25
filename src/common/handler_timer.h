// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/HandlerTimer.java
#pragma once

#include <functional>
#include <unordered_map>
#include <mutex>
#include <cstdint>

namespace PinkReader {

/**
 * Timer utility that posts delayed runnables and supports cancellation.
 * Equivalent to: org.quantumbadger.redreader.common.HandlerTimer
 *
 * Uses a post-delayed callback mechanism (abstracted from Android Handler).
 * The handler callback should execute the given function after the given delay in milliseconds.
 */
class HandlerTimer {
public:
	/** Callback type for posting delayed work: function, delay_ms */
	using PostDelayedFunc = std::function<void(std::function<void()>, int64_t)>;

private:
	PostDelayedFunc mPostDelayed;
	int mNextId = 0;
	std::unordered_map<int, bool> mTimers;
	std::mutex mMutex;

	/** Returns the next available timer ID (never returns 0). */
	int getNextId() {
		std::lock_guard<std::mutex> lock(mMutex);
		mNextId++;

		while (mTimers.count(mNextId) > 0 || mNextId == 0) {
			mNextId++;
		}

		return mNextId;
	}

public:
	/**
	 * Constructs a HandlerTimer with a post-delayed callback.
	 * @param postDelayed function that schedules a function to run after delayMs milliseconds
	 */
	explicit HandlerTimer(PostDelayedFunc postDelayed)
		: mPostDelayed(std::move(postDelayed)) {}

	// Not copyable
	HandlerTimer(const HandlerTimer&) = delete;
	HandlerTimer& operator=(const HandlerTimer&) = delete;

	/**
	 * Sets a timer that runs the given function after delayMs milliseconds.
	 * Returns a timer ID (never 0) that can be used to cancel.
	 * Equivalent to: HandlerTimer.setTimer(long, Runnable)
	 */
	int setTimer(int64_t delayMs, std::function<void()> runnable) {
		const int id = getNextId();

		{
			std::lock_guard<std::mutex> lock(mMutex);
			mTimers[id] = true;
		}

		mPostDelayed([this, id, runnable = std::move(runnable)]() {
			{
				std::lock_guard<std::mutex> lock(mMutex);
				auto it = mTimers.find(id);
				if (it == mTimers.end()) {
					return; // already cancelled
				}
				mTimers.erase(it);
			}

			runnable();
		}, delayMs);

		return id;
	}

	/**
	 * Cancels a previously set timer by its ID.
	 * Equivalent to: HandlerTimer.cancelTimer(int)
	 */
	void cancelTimer(int timerId) {
		std::lock_guard<std::mutex> lock(mMutex);
		mTimers.erase(timerId);
	}
};

} // namespace PinkReader
