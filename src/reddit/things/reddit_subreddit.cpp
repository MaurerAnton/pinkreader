// Origin: RedReader/src/main/java/org/quantumbadger/redreader/reddit/things/RedditSubreddit.java
#include "reddit_subreddit.h"
#include <algorithm>
#include <cctype>
#include <sstream>

namespace PinkReader {

const std::regex RedditSubreddit::NAME_PATTERN(R"(((/)?r/)?([\w\+\-.:]+)/?)");
const std::regex RedditSubreddit::USER_PATTERN(R"(/?(u/|user/)([\w\+\-.:]+)/?)");

std::string RedditSubreddit::stripRPrefix(const std::string& name) {
	std::smatch match;
	if (std::regex_match(name, match, NAME_PATTERN)) {
		return match[3].str();
	}
	throw InvalidSubredditNameException(name);
}

std::string RedditSubreddit::stripUserPrefix(const std::string& name, bool& found) {
	std::smatch match;
	if (std::regex_match(name, match, USER_PATTERN)) {
		found = true;
		return match[2].str();
	}
	found = false;
	return "";
}

SubredditCanonicalId RedditSubreddit::getCanonicalId() const {
	return SubredditCanonicalId(url);
}

std::string RedditSubreddit::getSidebarHtmlStatic(bool nightMode, const std::string& htmlEscaped) {
	// Simple unescape of common HTML4 entities
	std::string unescaped = htmlEscaped;
	// Basic replacements for common entities
	auto replace_all = [](std::string& s, const std::string& from, const std::string& to) {
		size_t pos = 0;
		while ((pos = s.find(from, pos)) != std::string::npos) {
			s.replace(pos, from.length(), to);
			pos += to.length();
		}
	};
	replace_all(unescaped, "&amp;", "&");
	replace_all(unescaped, "&lt;", "<");
	replace_all(unescaped, "&gt;", ">");
	replace_all(unescaped, "&quot;", "\"");
	replace_all(unescaped, "&#39;", "'");

	std::ostringstream result;
	result << "<html>";
	result << "<head>";
	result << "<meta name=\"viewport\" content=\"width=device-width, user-scalable=yes\">";

	if (nightMode) {
		result << "<style>";
		result << "body {color: white; background-color: black;}";
		result << "a {color: #3399FF; background-color: 000033;}";
		result << "</style>";
	}

	result << "</head>";
	result << "<body>";
	result << unescaped;
	result << "</body>";
	result << "</html>";

	return result.str();
}

std::string RedditSubreddit::getSidebarHtml(bool nightMode) const {
	return getSidebarHtmlStatic(nightMode, description_html);
}

bool RedditSubreddit::hasSidebar() const {
	return !description_html.empty();
}

} // namespace PinkReader
