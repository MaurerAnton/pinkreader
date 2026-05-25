// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/Locker.java
#pragma once

#include <mutex>

namespace PinkReader {

/**
 * RAII lock wrapper that implements a "Closeable" pattern.
 * Equivalent to: org.quantumbadger.redreader.common.Locker
 *
 * Usage:
 *   {
 *     Locker locker(myMutex);
 *     // critical section
 *   } // automatically unlocked
 */
class Locker {
private:
	std::mutex& mLock;

public:
	/** Acquires the lock immediately. */
	explicit Locker(std::mutex& lock) : mLock(lock) {
		mLock.lock();
	}

	/** Not copyable. */
	Locker(const Locker&) = delete;
	Locker& operator=(const Locker&) = delete;

	/** Not movable (mutex reference would be invalidated). */
	Locker(Locker&&) = delete;
	Locker& operator=(Locker&&) = delete;

	/** Releases the lock. Equivalent to Closeable.close(). */
	~Locker() {
		mLock.unlock();
	}
};

} // namespace PinkReader
