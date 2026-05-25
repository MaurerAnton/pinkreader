// Origin: org/quantumbadger/redreader/common/datastream/ByteArrayCallback.java
#pragma once

#include <cstdint>
#include <stdexcept>

namespace PinkReader {

class ByteArrayCallback {
public:
	virtual ~ByteArrayCallback() = default;

	virtual void onByteArray(
			const uint8_t* buf,
			int offset,
			int length) = 0;
};

} // namespace PinkReader
