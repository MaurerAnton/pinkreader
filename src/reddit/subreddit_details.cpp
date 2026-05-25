// Origin: org/quantumbadger/redreader/reddit/SubredditDetails.java
#include "subreddit_details.h"
#include <stdexcept>

namespace PinkReader {

SubredditDetails::SubredditDetails(const RedditSubreddit& subreddit)
	: id(subreddit.getCanonicalId())
	, name(subreddit.display_name)
	, url(subreddit.getUrl())
	, publicDescriptionHtmlEscaped(subreddit.public_description_html)
	, subscribers(subreddit.subscribers)
{
	// NOTE: throws InvalidSubredditNameException in Java
}

SubredditDetails::SubredditDetails(const SubredditCanonicalId& subreddit)
	: id(subreddit)
	, name(subreddit.getDisplayNameLowercase())
	, url(UriString(subreddit.toString()))
	, publicDescriptionHtmlEscaped(std::nullopt)
	, subscribers(std::nullopt)
{
}

SubredditDetails SubredditDetails::newWithRuntimeException(const RedditSubreddit& subreddit) {
	try {
		return SubredditDetails(subreddit);
	} catch (const std::exception& e) {
		throw std::runtime_error(e.what());
	}
}

std::string SubredditDetails::getUniqueId() const {
	return id.toString();
}

bool SubredditDetails::hasSidebar() const {
	return publicDescriptionHtmlEscaped.has_value() && !publicDescriptionHtmlEscaped->empty();
}

void SubredditDetails::showSidebarActivity(AppCompatActivity& context) const {
	(void)context;
	// Original Java:
	// final Intent intent = new Intent(context, HtmlViewActivity.class);
	// intent.putExtra("html", RedditSubreddit.getSidebarHtmlStatic(
	//     PrefsUtility.isNightMode(), publicDescriptionHtmlEscaped));
	// intent.putExtra("title", String.format(Locale.US, "%s: %s",
	//     context.getString(R.string.sidebar_activity_title), url));
	// context.startActivityForResult(intent, 1);
}

} // namespace PinkReader
