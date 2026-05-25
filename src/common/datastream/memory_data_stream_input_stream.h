// Origin: org/quantumbadger/redreader/common/datastream/MemoryDataStreamInputStream.java
#pragma once

#include <cstdint>

#include "seekable_input_stream.h"
#include "memory_data_stream.h"

namespace PinkReader {

class MemoryDataStreamInputStream : public SeekableInputStream {
private:
	MemoryDataStream& m_stream;
	int m_position;

public:
	explicit MemoryDataStreamInputStream(MemoryDataStream& stream);

	int read() override;

	int read(uint8_t* buf, int off, int len) override;

	long getPosition() override;

	void seek(long position) override;

	long skip(long offset) override;

	int available() override;

	void close() override;

	void readRemainingAsBytes(ByteArrayCallback& callback) override;
};

} // namespace PinkReader
