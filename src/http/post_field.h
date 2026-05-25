// Origin: RedReader http/PostField.kt
#pragma once

#include <string>
#include <vector>
#include <stdexcept>

namespace PinkReader {

struct PostField {
	std::string name;
	std::string value;

	PostField(std::string name, std::string value)
		: name(std::move(name)), value(std::move(value)) {}

	std::string Encode() const {
		// URL-encode name and value
		// Simplified version - full URL encoding deferred
		try {
			return UrlEncode(name) + "=" + UrlEncode(value);
		} catch (const std::runtime_error& e) {
			throw;
		}
	}

	static std::string EncodeList(const std::vector<PostField>& fields) {
		std::string result;

		for (const auto& field : fields) {
			if (!result.empty()) {
				result += '&';
			}

			result += field.Encode();
		}

		return result;
	}

private:
	static std::string UrlEncode(const std::string& value);
};

} // namespace PinkReader
