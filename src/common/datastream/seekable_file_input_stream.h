// Origin: org/quantumbadger/redreader/common/datastream/SeekableFileInputStream.java
#pragma once

#include <QFile>
#include <cstdint>

#include "seekable_input_stream.h"

namespace PinkReader {

class SeekableFileInputStream : public SeekableInputStream {
private:
	QFile m_file;
	long m_position;

public:
	explicit SeekableFileInputStream(const QString& filePath);

	long getPosition() override;

	void seek(long position) override;

	void readRemainingAsBytes(ByteArrayCallback& callback) override;

	int read() override;

	int read(uint8_t* buf, int off, int len) override;

	long skip(long n) override;

	int available() override;

	void close() override;
};

} // namespace PinkReader
