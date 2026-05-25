// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/HexUtils.java

#include "hex_utils.h"
#include <sstream>
#include <iomanip>
#include <cctype>
#include <algorithm>

namespace PinkReader {

std::string HexUtils::toHex(const std::vector<uint8_t>& input) {
	std::ostringstream result;
	result << std::hex << std::uppercase << std::setfill('0');

	for (const auto b : input) {
		result << std::setw(2) << static_cast<int>(b);
	}

	return result.str();
}

int HexUtils::fromHex(char digit) {
	if (digit >= '0' && digit <= '9') {
		return digit - '0';
	}

	if (digit >= 'A' && digit <= 'F') {
		return digit + 10 - 'A';
	}

	if (digit >= 'a' && digit <= 'f') {
		return digit + 10 - 'a';
	}

	throw std::runtime_error(std::string("Invalid hex digit '") + digit + "'");
}

std::vector<uint8_t> HexUtils::fromHex(const std::string& input) {
	// Trim whitespace
	std::string inputTrimmed = input;
	inputTrimmed.erase(inputTrimmed.begin(),
		std::find_if(inputTrimmed.begin(), inputTrimmed.end(),
			[](unsigned char ch) { return !std::isspace(ch); }));
	inputTrimmed.erase(
		std::find_if(inputTrimmed.rbegin(), inputTrimmed.rend(),
			[](unsigned char ch) { return !std::isspace(ch); }).base(),
		inputTrimmed.end());

	if (inputTrimmed.length() % 2 != 0) {
		throw std::runtime_error("Hex string length is not even: '" + inputTrimmed + "'");
	}

	std::vector<uint8_t> result(inputTrimmed.length() / 2);

	for (size_t i = 0; i < result.size(); i++) {
		result[i] = static_cast<uint8_t>(
			(fromHex(inputTrimmed[i * 2]) << 4) | fromHex(inputTrimmed[i * 2 + 1]));
	}

	return result;
}

} // namespace PinkReader
