// Origin: org/quantumbadger/redreader/reddit/RedditSubredditManager.java
#pragma once

#include <string>
#include <vector>
#include <map>
#include <mutex>

namespace PinkReader {

// Forward declarations
class RedditAccount;
class RedditSubreddit;
class SubredditCanonicalId;
class TimestampUTC;
class TimestampBound;
template<typename K, typename V, typename E> class RequestResponseHandler;
template<typename K, typename V> class UpdatedVersionListener;
template<typename K, typename V, typename E> class WeakCache;
class Context;

class RedditSubredditManager {
public:
	enum class SubredditListType {
		SUBSCRIBED,
		MODERATED,
		MULTIREDDITS,
		MOST_POPULAR,
		DEFAULTS
	};

	void offerRawSubredditData(
			const std::vector<RedditSubreddit>& toWrite,
			const TimestampUTC& timestamp);

	static RedditSubredditManager& getInstance(const Context& context, const RedditAccount& user);

	void getSubreddit(
			const SubredditCanonicalId& subredditCanonicalId,
			const TimestampBound& timestampBound,
			RequestResponseHandler<SubredditCanonicalId, RedditSubreddit, int>& handler,
			UpdatedVersionListener<SubredditCanonicalId, RedditSubreddit>* updatedVersionListener);

	void getSubreddits(
			const std::vector<SubredditCanonicalId>& ids,
			const TimestampBound& timestampBound,
			RequestResponseHandler<
					std::map<SubredditCanonicalId, RedditSubreddit>,
					int>& handler);

private:
	RedditSubredditManager(const Context& context, const RedditAccount& user);

	static std::string getDbFilename(const std::string& type, const RedditAccount& user);

	// WeakCache<SubredditCanonicalId, RedditSubreddit, RRError> subredditCache;
	WeakCache<SubredditCanonicalId, RedditSubreddit, int>* m_subredditCache;

	static RedditSubredditManager* s_singleton;
	static const RedditAccount* s_singletonUser;
	static std::mutex s_mutex;
};

} // namespace PinkReader
