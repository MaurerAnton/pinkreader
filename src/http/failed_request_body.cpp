// Origin: RedReader http/FailedRequestBody.java
#include "failed_request_body.h"
#include <sstream>
#include <stdexcept>

namespace PinkReader {

std::optional<FailedRequestBody> FailedRequestBody::From(std::istream& is) {
	try {
		// Read whole stream into bytes
		std::vector<uint8_t> data;
		char buf[4096];
		while (is.read(buf, sizeof(buf)) || is.gcount() > 0) {
			data.insert(data.end(),
				reinterpret_cast<uint8_t*>(buf),
				reinterpret_cast<uint8_t*>(buf) + is.gcount());
		}
		return FailedRequestBody(data);
	} catch (const std::ios_base::failure&) {
		return std::nullopt;
	}
}

std::optional<FailedRequestBody> FailedRequestBody::From(
		GenericFactory<SeekableInputStream, std::ios_base::failure>& factory) {
	try {
		auto stream = factory.Create();
		return From(stream);
	} catch (const std::ios_base::failure&) {
		return std::nullopt;
	}
}

std::string FailedRequestBody::ToString() {
	std::lock_guard<std::mutex> lock(mMutex);

	if (!mString.has_value()) {
		if (mBytes.has_value()) {
			const auto& bytes = mBytes.value();
			mString = std::string(reinterpret_cast<const char*>(bytes.data()), bytes.size());
		} else if (mJson.has_value()) {
			mString = mJson.value().ToString();
		} else {
			throw std::runtime_error("No data present");
		}
	}

	return mString.value();
}

std::vector<uint8_t> FailedRequestBody::ToBytes() {
	std::lock_guard<std::mutex> lock(mMutex);

	if (!mBytes.has_value()) {
		const std::string& str = ToString();
		mBytes = std::vector<uint8_t>(str.begin(), str.end());
	}

	return mBytes.value();
}

std::optional<JsonValue> FailedRequestBody::ToJson() {
	std::lock_guard<std::mutex> lock(mMutex);

	if (!mJson.has_value() && !mAttemptedParse) {
		mAttemptedParse = true;

		try {
			std::istringstream iss(ToString());
			// mJson = JsonValue::Parse(iss);
			// Deferred: JsonValue::Parse is not yet ported
		} catch (const std::ios_base::failure&) {
			// Ignore this
		}
	}

	return mJson;
}

} // namespace PinkReader
