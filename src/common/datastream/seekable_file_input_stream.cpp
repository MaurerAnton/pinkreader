// Origin: org/quantumbadger/redreader/common/datastream/SeekableFileInputStream.java
#include "seekable_file_input_stream.h"

#include <QByteArray>
#include <algorithm>
#include <stdexcept>

namespace PinkReader {

SeekableFileInputStream::SeekableFileInputStream(const QString& filePath)
	: m_file(filePath)
	, m_position(0) {
	if(!m_file.open(QIODevice::ReadOnly)) {
		throw std::runtime_error("Failed to open file: " + filePath.toStdString());
	}
}

long SeekableFileInputStream::getPosition() {
	return m_position;
}

void SeekableFileInputStream::seek(long position) {
	m_file.seek(position);
	m_position = position;
}

void SeekableFileInputStream::readRemainingAsBytes(ByteArrayCallback& callback) {
	const qint64 remaining = m_file.size() - m_position;
	QByteArray result = m_file.read(remaining);
	callback.onByteArray(
		reinterpret_cast<const uint8_t*>(result.constData()),
		0,
		result.size());
}

int SeekableFileInputStream::read() {
	char byte;
	if(m_file.read(&byte, 1) < 1) {
		return -1;
	}

	const int result = static_cast<uint8_t>(byte);

	if(result >= 0) {
		m_position++;
	}

	return result;
}

int SeekableFileInputStream::read(uint8_t* buf, int off, int len) {
	if(len == 0) {
		throw std::runtime_error("Attempted to read zero bytes");
	}

	const int result = static_cast<int>(
		m_file.read(reinterpret_cast<char*>(buf + off), len));

	if(result > 0) {
		m_position += result;
	}

	return result;
}

long SeekableFileInputStream::skip(long n) {
	const long bytesToSkip = std::min(n, static_cast<long>(available()));
	seek(m_position + bytesToSkip);
	return bytesToSkip;
}

int SeekableFileInputStream::available() {
	return static_cast<int>(m_file.size() - m_position);
}

void SeekableFileInputStream::close() {
	m_file.close();
}

} // namespace PinkReader
