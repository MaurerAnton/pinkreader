// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/HexUtils.java
#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <cstdint>

namespace PinkReader {

/**
 * Utility class for hex encoding/decoding.
 * Equivalent to: org.quantumbadger.redreader.common.HexUtils
 */
class HexUtils {
public:
	HexUtils() = delete;

	/**
	 * Converts a byte array to a hex string (uppercase).
	 * Equivalent to: HexUtils.toHex(byte[])
	 */
	static std::string toHex(const std::vector<uint8_t>& input);

	/**
	 * Converts a single hex character to its integer value.
	 * Equivalent to: HexUtils.fromHex(char)
	 *
	 * @throws std::runtime_error if the character is not a valid hex digit
	 */
	static int fromHex(char digit);

	/**
	 * Converts a hex string to a byte array.
	 * Equivalent to: HexUtils.fromHex(String)
	 *
	 * @throws std::runtime_error if the string is invalid
	 */
	static std::vector<uint8_t> fromHex(const std::string& input);
};

} // namespace PinkReader
