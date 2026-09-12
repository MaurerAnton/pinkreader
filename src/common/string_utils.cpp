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

namespace {

// ASCII-only fold of one QChar (mirrors the std::string overloads above:
// the class deliberately folds ASCII only, never full Unicode case).
QChar asciiLowerQChar(QChar c) {
	ushort u = c.unicode();
	if (u >= 'A' && u <= 'Z') {
		return QChar(u - 'A' + 'a');
	}
	return c;
}

QChar asciiUpperQChar(QChar c) {
	ushort u = c.unicode();
	if (u >= 'a' && u <= 'z') {
		return QChar(u - 'a' + 'A');
	}
	return c;
}

} // anonymous namespace

QString StringUtils::asciiLowercase(const QString& input) {
	QString result = input;
	for (int i = 0; i < result.size(); ++i) {
		result[i] = asciiLowerQChar(result.at(i));
	}
	return result;
}

QString StringUtils::asciiUppercase(const QString& input) {
	QString result = input;
	for (int i = 0; i < result.size(); ++i) {
		result[i] = asciiUpperQChar(result.at(i));
	}
	return result;
}

bool StringUtils::asciiLowercaseContains(const QString& haystack,
		const QString& needle) {
	return asciiLowercase(haystack).contains(asciiLowercase(needle));
}

} // namespace PinkReader
