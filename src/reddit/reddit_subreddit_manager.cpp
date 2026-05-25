// Origin: org/quantumbadger/redreader/reddit/RedditSubredditManager.java
#include "reddit_subreddit_manager.h"

namespace PinkReader {

RedditSubredditManager* RedditSubredditManager::s_singleton = nullptr;
const RedditAccount* RedditSubredditManager::s_singletonUser = nullptr;
std::mutex RedditSubredditManager::s_mutex;

RedditSubredditManager::RedditSubredditManager(const Context& context, const RedditAccount& user)
	: m_subredditCache(nullptr)
{
	(void)context;
	(void)user;
	// Original Java:
	// final RawObjectDB<SubredditCanonicalId, RedditSubreddit> subredditDb
	//     = new RawObjectDB<>(context, getDbFilename("subreddits", user), RedditSubreddit.class);
	// final ThreadedRawObjectDB<...> subredditDbWrapper
	//     = new ThreadedRawObjectDB<>(subredditDb, new RedditAPIIndividualSubredditDataRequester(context, user));
	// subredditCache = new WeakCache<>(subredditDbWrapper);
}

RedditSubredditManager& RedditSubredditManager::getInstance(const Context& context, const RedditAccount& user) {
	std::lock_guard<std::mutex> lock(s_mutex);
	if (s_singleton == nullptr || !(user == *s_singletonUser)) {
		s_singletonUser = &user;
		delete s_singleton;
		s_singleton = new RedditSubredditManager(context, user);
	}
	return *s_singleton;
}

void RedditSubredditManager::offerRawSubredditData(
		const std::vector<RedditSubreddit>& toWrite,
		const TimestampUTC& timestamp) {
	(void)timestamp;
	// subredditCache.performWrite(toWrite);
	(void)toWrite;
}

void RedditSubredditManager::getSubreddit(
		const SubredditCanonicalId& subredditCanonicalId,
		const TimestampBound& timestampBound,
		RequestResponseHandler<SubredditCanonicalId, RedditSubreddit, int>& handler,
		UpdatedVersionListener<SubredditCanonicalId, RedditSubreddit>* updatedVersionListener) {
	(void)subredditCanonicalId;
	(void)timestampBound;
	(void)handler;
	(void)updatedVersionListener;
	// subredditCache.performRequest(subredditCanonicalId, timestampBound, handler, updatedVersionListener);
}

void RedditSubredditManager::getSubreddits(
		const std::vector<SubredditCanonicalId>& ids,
		const TimestampBound& timestampBound,
		RequestResponseHandler<
				std::map<SubredditCanonicalId, RedditSubreddit>,
				int>& handler) {
	(void)ids;
	(void)timestampBound;
	(void)handler;
	// subredditCache.performRequest(ids, timestampBound, handler);
}

std::string RedditSubredditManager::getDbFilename(const std::string& type, const RedditAccount& user) {
	(void)user;
	// return General.sha1(user.username.getBytes()) + "_" + type + "_subreddits.db";
	return type + "_subreddits.db";
}

} // namespace PinkReader
