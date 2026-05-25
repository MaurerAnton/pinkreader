// Origin: org/quantumbadger/redreader/common/datastream/MemoryDataStream.java
#pragma once

#include <cstdint>
#include <cstring>
#include <mutex>
#include <condition_variable>
#include <stdexcept>
#include <memory>
#include <algorithm>
#include <vector>

#include "byte_array_callback.h"
#include "memory_data_stream_input_stream.h"

namespace PinkReader {

class MemoryDataStream {
private:
	std::mutex m_lock;
	std::condition_variable m_cv;

	uint8_t* m_data;
	int m_capacity;
	int m_size;

	std::runtime_error* m_failed;
	bool m_complete;

	void ensureCapacity(int desiredCapacity) {
		if(desiredCapacity <= m_capacity) {
			return;
		}

		if(desiredCapacity > (m_capacity * 2)) {
			realloc(desiredCapacity + (desiredCapacity / 2));
		} else {
			realloc(m_capacity * 2);
		}
	}

	void realloc(int newCapacity) {
		if(newCapacity < m_size) {
			throw std::runtime_error("Cannot shrink array");
		}

		uint8_t* newData = new uint8_t[newCapacity];
		std::memcpy(newData, m_data, m_size);
		delete[] m_data;
		m_data = newData;
		m_capacity = newCapacity;
	}

	bool notReadyForRead(int startingPosition) const {
		return !m_complete && m_failed == nullptr && m_size <= startingPosition;
	}

public:
	MemoryDataStream()
		: MemoryDataStream(64 * 1024) {}

	explicit MemoryDataStream(int initialCapacity)
		: m_capacity(initialCapacity)
		, m_size(0)
		, m_failed(nullptr)
		, m_complete(false) {
		if(initialCapacity < 1) {
			throw std::runtime_error("Initial capacity must be at least 1");
		}
		m_data = new uint8_t[initialCapacity];
	}

	explicit MemoryDataStream(const uint8_t* data, int length)
		: m_capacity(length)
		, m_size(length)
		, m_failed(nullptr)
		, m_complete(true) {
		m_data = new uint8_t[length];
		std::memcpy(m_data, data, length);
	}

	~MemoryDataStream() {
		delete[] m_data;
		delete m_failed;
	}

	// Non-copyable, non-movable due to mutex
	MemoryDataStream(const MemoryDataStream&) = delete;
	MemoryDataStream& operator=(const MemoryDataStream&) = delete;

	int size() {
		std::lock_guard<std::mutex> guard(m_lock);
		return m_size;
	}

	void writeBytes(const uint8_t* data, int offset, int length) {
		std::unique_lock<std::mutex> lock(m_lock);
		ensureCapacity(m_size + length);
		std::memcpy(m_data + m_size, data + offset, length);
		m_size += length;
		m_cv.notify_all();
	}

	void setComplete() {
		std::unique_lock<std::mutex> lock(m_lock);
		m_complete = true;
		m_cv.notify_all();
	}

	void setFailed(const std::runtime_error& e) {
		std::unique_lock<std::mutex> lock(m_lock);
		delete m_failed;
		m_failed = new std::runtime_error(e);
		m_cv.notify_all();
	}

	int blockingReadOneByte(int position) {
		std::unique_lock<std::mutex> lock(m_lock);

		while(notReadyForRead(position)) {
			m_cv.wait(lock);
		}

		if(m_failed != nullptr) {
			throw *m_failed;
		}

		if(m_size > position) {
			return static_cast<int>(m_data[position]);
		}

		if(m_complete) {
			return -1;
		}

		throw std::runtime_error("Internal error: ready conditions not true");
	}

	int blockingRead(int startingPosition, uint8_t* output, int offset, int maxLength) {
		if(maxLength == 0) {
			throw std::runtime_error("Attempted to read zero bytes");
		}

		std::unique_lock<std::mutex> lock(m_lock);

		while(notReadyForRead(startingPosition)) {
			m_cv.wait(lock);
		}

		if(m_failed != nullptr) {
			throw *m_failed;
		}

		if(m_size > startingPosition) {
			int bytesToRead = std::min(maxLength, m_size - startingPosition);
			std::memcpy(output + offset, m_data + startingPosition, bytesToRead);
			return bytesToRead;
		}

		if(m_complete) {
			return -1;
		}

		throw std::runtime_error("Internal error: ready conditions not true");
	}

	MemoryDataStreamInputStream* getInputStream();

	void getUnderlyingByteArrayWhenComplete(ByteArrayCallback& callback) {
		{
			std::unique_lock<std::mutex> lock(m_lock);

			while(!m_complete && m_failed == nullptr) {
				m_cv.wait(lock);
			}

			if(m_failed != nullptr) {
				throw *m_failed;
			}
		}

		callback.onByteArray(m_data, 0, m_size);
	}

	// Accessors needed by input stream
	const uint8_t* data() const { return m_data; }
	int dataSize() const { return m_size; }
	int capacity() const { return m_capacity; }
	std::mutex& mutex() { return m_lock; }
	std::condition_variable& cv() { return m_cv; }
	const std::runtime_error* failed() const { return m_failed; }
	bool complete() const { return m_complete; }
};

inline MemoryDataStreamInputStream* MemoryDataStream::getInputStream() {
	return new MemoryDataStreamInputStream(*this);
}

} // namespace PinkReader
