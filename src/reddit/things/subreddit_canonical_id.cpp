// Origin: RedReader/src/main/java/org/quantumbadger/redreader/reddit/things/SubredditCanonicalId.java
#include "subreddit_canonical_id.h"
#include <algorithm>
#include <cctype>

// Forward reference to RedditSubreddit static helpers which are needed
// RedditSubreddit::stripRPrefix and stripUserPrefix are used

namespace PinkReader {

// Helper: ASCII lowercase a string in-place
static std::string asciiLowercase(const std::string& s) {
	std::string result = s;
	// Trim
	result.erase(0, result.find_first_not_of(" \t\n\r\f\v"));
	result.erase(result.find_last_not_of(" \t\n\r\f\v") + 1);
	std::transform(result.begin(), result.end(), result.begin(),
		[](unsigned char c) { return std::tolower(c); });
	return result;
}

// Strip "/r/" prefix. Pattern: ((/)?r/)?([\\w\\+\\-\\.:]+)/?
static std::string stripRPrefix(const std::string& name) {
	std::string s = name;
	// Remove leading /
	if (!s.empty() && s[0] == '/') s = s.substr(1);
	// Remove leading "r/"
	if (s.size() >= 2 && s[0] == 'r' && s[1] == '/') s = s.substr(2);
	// Remove trailing /
	if (!s.empty() && s.back() == '/') s.pop_back();
	if (s.empty()) {
		throw InvalidSubredditNameException(name);
	}
	return s;
}

// Strip "/u/" or "/user/" prefix. Pattern: /?(u/|user/)([\\w\\+\\-\\.:]+)/?
static std::string stripUserPrefix(const std::string& name, bool& found) {
	std::string s = name;
	// Remove leading /
	if (!s.empty() && s[0] == '/') s = s.substr(1);
	if (s.size() >= 2 && s[0] == 'u' && s[1] == '/') {
		s = s.substr(2);
		found = true;
	} else if (s.size() >= 5 && s.substr(0, 5) == "user/") {
		s = s.substr(5);
		found = true;
	} else {
		found = false;
		return "";
	}
	// Remove trailing /
	if (!s.empty() && s.back() == '/') s.pop_back();
	return s;
}

SubredditCanonicalId::SubredditCanonicalId(const std::string& name) {
	std::string trimmed = asciiLowercase(name);
	bool isUser;
	std::string userSr = stripUserPrefix(trimmed, isUser);

	if (isUser) {
		mId = "/user/" + userSr;
	} else {
		mId = "/r/" + stripRPrefix(trimmed);
	}
}

std::string SubredditCanonicalId::getDisplayNameLowercase() const {
	if (mId.size() >= 6 && mId.substr(0, 6) == "/user/") {
		return mId;
	}
	return mId.substr(3); // strip "/r/"
}

} // namespace PinkReader
