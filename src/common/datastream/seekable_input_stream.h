// Origin: org/quantumbadger/redreader/common/datastream/SeekableInputStream.java
#pragma once

#include <cstdint>
#include <stdexcept>

#include "byte_array_callback.h"

namespace PinkReader {

class SeekableInputStream {
private:
	int m_mark;

public:
	SeekableInputStream()
		: m_mark(0) {}

	virtual ~SeekableInputStream() = default;

	virtual long getPosition() = 0;

	virtual void seek(long position) = 0;

	void mark(int readlimit) {
		m_mark = static_cast<int>(getPosition());
	}

	void reset() {
		seek(m_mark);
	}

	bool markSupported() const {
		return true;
	}

	virtual int read() = 0;

	virtual int read(uint8_t* buf, int off, int len) = 0;

	virtual long skip(long offset) = 0;

	virtual int available() = 0;

	virtual void close() = 0;

	virtual void readRemainingAsBytes(ByteArrayCallback& callback) = 0;
};

} // namespace PinkReader
