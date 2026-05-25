// Origin: org/quantumbadger/redreader/common/datastream/MemoryDataStreamInputStream.java
#include "memory_data_stream_input_stream.h"

#include <algorithm>
#include <stdexcept>
#include <functional>

namespace PinkReader {

// Wrapper callback to adapt getUnderlyingByteArrayWhenComplete for lambda usage
namespace {
class LambdaByteArrayCallback : public ByteArrayCallback {
private:
	std::function<void(const uint8_t*, int, int)> m_func;
public:
	explicit LambdaByteArrayCallback(std::function<void(const uint8_t*, int, int)> func)
		: m_func(std::move(func)) {}
	void onByteArray(const uint8_t* buf, int offset, int length) override {
		m_func(buf, offset, length);
	}
};
} // anonymous namespace

MemoryDataStreamInputStream::MemoryDataStreamInputStream(MemoryDataStream& stream)
	: m_stream(stream)
	, m_position(0) {}

int MemoryDataStreamInputStream::read() {
	const int result = m_stream.blockingReadOneByte(m_position);

	if(result >= 0) {
		m_position++;
	}

	return result;
}

int MemoryDataStreamInputStream::read(uint8_t* buf, int off, int len) {
	const int bytesRead = m_stream.blockingRead(m_position, buf, off, len);

	if(bytesRead > 0) {
		m_position += bytesRead;
	}

	return bytesRead;
}

long MemoryDataStreamInputStream::getPosition() {
	return m_position;
}

void MemoryDataStreamInputStream::seek(long position) {
	if(position < 0) {
		throw std::runtime_error("Attempted to seek before zero");
	}

	m_position = static_cast<int>(position);
}

long MemoryDataStreamInputStream::skip(long offset) {
	const int bytesToSkip = static_cast<int>(
		std::min(offset, std::max(0L, static_cast<long>(m_stream.size()) - m_position)));
	m_position += bytesToSkip;
	return bytesToSkip;
}

int MemoryDataStreamInputStream::available() {
	return m_stream.size();
}

void MemoryDataStreamInputStream::close() {
	// Nothing to do here
}

void MemoryDataStreamInputStream::readRemainingAsBytes(ByteArrayCallback& callback) {
	LambdaByteArrayCallback wrapper(
		[&callback, this](const uint8_t* buf, int offset, int length) {
			callback.onByteArray(buf, offset + m_position, length - m_position);
		});
	m_stream.getUnderlyingByteArrayWhenComplete(wrapper);
}

} // namespace PinkReader
