// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/StringUtils.java
#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include "optional.h"

namespace PinkReader {

/**
 * String utility methods.
 * Equivalent to: org.quantumbadger.redreader.common.StringUtils
 */
class StringUtils {
public:
	StringUtils() = delete;

	/**
	 * If the input starts with prefix, returns Optional containing the remainder.
	 * Otherwise returns empty Optional.
	 * Equivalent to: StringUtils.removePrefix(String, String)
	 */
	static Optional<std::string> removePrefix(
		const std::string& input,
		const std::string& prefix);

	/**
	 * Converts ASCII lowercase letters to uppercase.
	 * Equivalent to: StringUtils.asciiUppercase(String)
	 */
	static std::string asciiUppercase(const std::string& input);

	/**
	 * Converts ASCII uppercase letters to lowercase.
	 * Equivalent to: StringUtils.asciiLowercase(String)
	 */
	static std::string asciiLowercase(const std::string& input);

	/**
	 * Joins a collection of elements with a separator.
	 * Equivalent to: StringUtils.join(Collection, CharSequence)
	 */
	template<typename T>
	static std::string join(const std::vector<T>& elements, const std::string& separator) {
		std::string result;
		bool first = true;

		for (const auto& element : elements) {
			if (!first) {
				result += separator;
			}
			result += element; // requires operator<< or implicit conversion
			first = false;
		}

		return result;
	}

	/**
	 * Returns true if the string is null or empty.
	 * Equivalent to: StringUtils.isEmpty(CharSequence)
	 */
	static bool isEmpty(const std::string* value) {
		return value == nullptr || value->empty();
	}

	/**
	 * Returns true if the string is empty.
	 */
	static bool isEmpty(const std::string& value) {
		return value.empty();
	}

	/**
	 * Converts a UTF-8 byte array to a string.
	 * Equivalent to: StringUtils.fromUTF8(byte[])
	 */
	static std::string fromUTF8(const std::vector<uint8_t>& bytes);
};

} // namespace PinkReader
