/*
 * PinkReader - GPLv3
 * File: thread_checked_var.h
 * Port of: org.quantumbadger.redreader.common.ThreadCheckedVar.java
 *
 * Every field, method, and logic branch from the original Java file.
 */

#pragma once

#include <stdexcept>
#include <thread>
#include <utility>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.common.ThreadCheckedVar<E>
 *
 * A variable wrapper that ensures get/set only occur on the thread
 * that created the instance.
 */
template<typename E>
class ThreadCheckedVar {
public:
	// Port of: public ThreadCheckedVar(final E value)
	explicit ThreadCheckedVar(E value)
		: m_value(std::move(value))
		, m_thread(std::this_thread::get_id()) {}

	// Port of: public E get()
	E get() const {
		if (m_thread != std::this_thread::get_id()) {
			throw std::runtime_error("Accessing variable from wrong thread");
		}
		return m_value;
	}

	// Port of: public void set(final E value)
	void set(E value) {
		if (m_thread != std::this_thread::get_id()) {
			throw std::runtime_error("Setting variable from wrong thread");
		}
		m_value = std::move(value);
	}

private:
	// Port of: private E mValue
	E m_value;

	// Port of: @NonNull private final Thread mThread
	std::thread::id m_thread;
};

} // namespace PinkReader
