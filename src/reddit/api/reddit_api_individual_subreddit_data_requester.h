// Origin: RedReader/src/main/java/org/quantumbadger/redreader/reddit/api/RedditAPIIndividualSubredditDataRequester.java
#pragma once

#include <string>
#include <memory>

namespace PinkReader {

// Forward declarations
class SubredditCanonicalId;
class RedditSubreddit;
class RedditAccount;
class TimestampBound;
class RRError;
template<typename K, typename V, typename E> class RequestResponseHandler;
template<typename K, typename V, typename E> class CacheDataSource;

class RedditAPIIndividualSubredditDataRequester {
public:
	RedditAPIIndividualSubredditDataRequester(void* context, const RedditAccount& user);

	void performRequest(
		const SubredditCanonicalId& subredditCanonicalId,
		const TimestampBound& timestampBound,
		void* handler); // RequestResponseHandler<RedditSubreddit, RRError>

	void performRequest(
		const void* subredditCanonicalIds, // Collection<SubredditCanonicalId>
		const TimestampBound& timestampBound,
		void* handler); // RequestResponseHandler<HashMap<SubredditCanonicalId, RedditSubreddit>, RRError>

	void performWrite(const RedditSubreddit& value);
	void performWrite(const void* values); // Collection<RedditSubreddit>

private:
	void* mContext;
	const RedditAccount* mUser;
};

} // namespace PinkReader
