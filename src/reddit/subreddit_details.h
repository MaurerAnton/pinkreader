// Origin: org/quantumbadger/redreader/reddit/SubredditDetails.java
#pragma once

#include <string>
#include <optional>

namespace PinkReader {

// Forward declarations
class RedditSubreddit;
class SubredditCanonicalId;
class UriString;
class AppCompatActivity;

class SubredditDetails {
public:
	const SubredditCanonicalId id;
	const std::string name;
	const UriString url;
	const std::optional<std::string> publicDescriptionHtmlEscaped;
	const std::optional<int> subscribers;

	// Constructor from RedditSubreddit (throws InvalidSubredditNameException in Java)
	SubredditDetails(const RedditSubreddit& subreddit);

	// Constructor from SubredditCanonicalId
	SubredditDetails(const SubredditCanonicalId& subreddit);

	// newWithRuntimeException
	static SubredditDetails newWithRuntimeException(const RedditSubreddit& subreddit);

	// getUniqueId() - HasUniqueId interface
	std::string getUniqueId() const;

	// hasSidebar()
	bool hasSidebar() const;

	// showSidebarActivity
	void showSidebarActivity(AppCompatActivity& context) const;
};

} // namespace PinkReader
