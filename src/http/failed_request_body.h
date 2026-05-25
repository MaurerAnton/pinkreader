// Origin: RedReader http/FailedRequestBody.java
#pragma once

#include <optional>
#include <string>
#include <vector>
#include <cstdint>
#include <mutex>

namespace PinkReader {

// Forward declarations
class JsonValue;
template<typename T, typename E>
class GenericFactory;
class SeekableInputStream;

class FailedRequestBody {
private:
	std::optional<std::vector<uint8_t>> mBytes;
	std::optional<std::string> mString;
	std::optional<JsonValue> mJson;
	bool mAttemptedParse;
	std::mutex mMutex;

public:
	explicit FailedRequestBody(const std::vector<uint8_t>& bytes)
		: mBytes(bytes)
		, mString(std::nullopt)
		, mJson(std::nullopt)
		, mAttemptedParse(false) {}

	explicit FailedRequestBody(const std::string& value)
		: mBytes(std::nullopt)
		, mString(value)
		, mJson(std::nullopt)
		, mAttemptedParse(false) {}

	explicit FailedRequestBody(const JsonValue& value)
		: mBytes(std::nullopt)
		, mString(std::nullopt)
		, mJson(value)
		, mAttemptedParse(false) {}

	static std::optional<FailedRequestBody> From(std::istream& is);
	static std::optional<FailedRequestBody> From(GenericFactory<SeekableInputStream, std::ios_base::failure>& factory);

	std::string ToString();
	std::vector<uint8_t> ToBytes();
	std::optional<JsonValue> ToJson();
};

} // namespace PinkReader
