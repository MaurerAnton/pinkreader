// Origin: RedReader/src/main/java/org/quantumbadger/redreader/reddit/api/RedditAPIIndividualSubredditListRequester.java
#pragma once

#include <string>
#include <memory>

namespace PinkReader {

// Forward declarations
class RedditAccount;
class TimestampBound;
class RRError;
class WritableHashSet;

namespace RedditSubredditManager {
	enum class SubredditListType {
		DEFAULTS,
		MOST_POPULAR,
		SUBSCRIBED,
		MODERATED,
		MULTIREDDITS
	};
}

class RedditAPIIndividualSubredditListRequester {
public:
	RedditAPIIndividualSubredditListRequester(void* context, const RedditAccount& user);

	void performRequest(
		RedditSubredditManager::SubredditListType type,
		const TimestampBound& timestampBound,
		void* handler); // RequestResponseHandler<WritableHashSet, RRError>

	void performRequest(
		const void* keys, // Collection<SubredditListType>
		const TimestampBound& timestampBound,
		void* handler);

	void performWrite(const WritableHashSet& value);
	void performWrite(const void* values);

private:
	void* mContext;
	const RedditAccount* mUser;

	void doSubredditListRequest(
		RedditSubredditManager::SubredditListType type,
		void* handler,
		const std::string* after);
};

} // namespace PinkReader
