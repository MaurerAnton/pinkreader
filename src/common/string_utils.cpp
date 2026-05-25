// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/StringUtils.java

#include "string_utils.h"
#include <algorithm>
#include <cctype>

namespace PinkReader {

Optional<std::string> StringUtils::removePrefix(
		const std::string& input,
		const std::string& prefix) {

	if (input.size() >= prefix.size()
			&& input.compare(0, prefix.size(), prefix) == 0) {
		return Optional<std::string>::of(input.substr(prefix.size()));
	} else {
		return Optional<std::string>::empty();
	}
}

std::string StringUtils::asciiUppercase(const std::string& input) {
	std::string result = input;

	for (char& c : result) {
		if (c >= 'a' && c <= 'z') {
			c = static_cast<char>(c - 'a' + 'A');
		}
	}

	return result;
}

std::string StringUtils::asciiLowercase(const std::string& input) {
	std::string result = input;

	for (char& c : result) {
		if (c >= 'A' && c <= 'Z') {
			c = static_cast<char>(c - 'A' + 'a');
		}
	}

	return result;
}

std::string StringUtils::fromUTF8(const std::vector<uint8_t>& bytes) {
	return std::string(bytes.begin(), bytes.end());
}

} // namespace PinkReader
