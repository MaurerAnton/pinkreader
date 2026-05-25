// Origin: RedReader/src/main/java/org/quantumbadger/redreader/reddit/things/RedditSubreddit.java
#pragma once

#include <string>
#include <cstdint>
#include <optional>
#include <regex>
#include "subreddit_canonical_id.h"
#include "invalid_subreddit_name_exception.h"

namespace PinkReader {

class UriString;
class RedditThing;

class RedditSubreddit {
public:
	static constexpr int DB_VERSION = 1;

	std::string header_img;
	std::string header_title;
	std::string description;
	std::string description_html;
	std::string public_description_html;
	std::string id;
	std::string name;
	std::string title;
	std::string display_name;
	std::string url;
	int64_t created = 0;
	int64_t created_utc = 0;
	std::optional<int32_t> accounts_active;
	std::optional<int32_t> subscribers;
	std::optional<bool> over18;

	int64_t downloadTime = 0;

	RedditSubreddit() = default;

	RedditSubreddit(const std::string& url_, const std::string& title_, bool /*isSortable*/)
		: url(url_), title(title_) {}

	// Constructor with creation data (timestamp)
	explicit RedditSubreddit(int64_t timestamp)
		: downloadTime(timestamp) {}

	static std::string stripRPrefix(const std::string& name);
	static std::string stripUserPrefix(const std::string& name, bool& found);

	SubredditCanonicalId getCanonicalId() const;
	// UriString getUrl() const;

	static std::string getSidebarHtmlStatic(bool nightMode, const std::string& htmlEscaped);
	std::string getSidebarHtml(bool nightMode) const;
	bool hasSidebar() const;

	int compareTo(const RedditSubreddit& another) const {
		// Case-insensitive comparison on display_name
		std::string a = display_name;
		std::string b = another.display_name;
		std::transform(a.begin(), a.end(), a.begin(), ::tolower);
		std::transform(b.begin(), b.end(), b.begin(), ::tolower);
		if (a < b) return -1;
		if (a > b) return 1;
		return 0;
	}

	std::string getUniqueId() const { return id; }

private:
	static const std::regex NAME_PATTERN;
	static const std::regex USER_PATTERN;
};

} // namespace PinkReader
