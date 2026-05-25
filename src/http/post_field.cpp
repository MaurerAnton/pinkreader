// Origin: RedReader http/PostField.kt
#include "post_field.h"
#include <iomanip>
#include <sstream>

namespace PinkReader {

std::string PostField::UrlEncode(const std::string& value) {
	std::ostringstream escaped;
	escaped.fill('0');
	escaped << std::hex;

	for (auto c : value) {
		// Keep alphanumeric and other acceptable characters intact
		if (std::isalnum(static_cast<unsigned char>(c)) || c == '-' || c == '_' || c == '.' || c == '~') {
			escaped << c;
		} else if (c == ' ') {
			escaped << '+';
		} else {
			escaped << std::uppercase;
			escaped << '%' << std::setw(2) << static_cast<int>(static_cast<unsigned char>(c));
			escaped << std::nouppercase;
		}
	}

	return escaped.str();
}

} // namespace PinkReader
