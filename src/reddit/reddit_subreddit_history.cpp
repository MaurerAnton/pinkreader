// Origin: org/quantumbadger/redreader/reddit/RedditSubredditHistory.java
#include "reddit_subreddit_history.h"
#include <algorithm>
#include <set>

namespace PinkReader {

std::map<const RedditAccount*, std::set<const SubredditCanonicalId*>> RedditSubredditHistory::m_subreddits;
std::mutex RedditSubredditHistory::m_mutex;

std::set<const SubredditCanonicalId*>& RedditSubredditHistory::getForAccount(const RedditAccount& account) {
	// HashMap<RedditAccount, HashSet<SubredditCanonicalId>>
	auto it = m_subreddits.find(&account);
	if (it == m_subreddits.end()) {
		// result = new HashSet<>(Constants.Reddit.DEFAULT_SUBREDDITS);
		std::set<const SubredditCanonicalId*> result;
		// NOTE: Constants.Reddit.DEFAULT_SUBREDDITS not yet ported
		auto [inserted, _] = m_subreddits.insert({&account, result});
		return inserted->second;
	}
	return it->second;
}

void RedditSubredditHistory::addSubreddit(const RedditAccount& account, const SubredditCanonicalId& id) {
	std::lock_guard<std::mutex> lock(m_mutex);
	getForAccount(account).insert(&id);
}

void RedditSubredditHistory::addSubreddits(const RedditAccount& account, const std::vector<const SubredditCanonicalId*>& ids) {
	std::lock_guard<std::mutex> lock(m_mutex);
	auto& set = getForAccount(account);
	for (const auto* id : ids) {
		set.insert(id);
	}
}

std::vector<const SubredditCanonicalId*> RedditSubredditHistory::getSubredditsSorted(const RedditAccount& account) {
	std::lock_guard<std::mutex> lock(m_mutex);
	auto& set = getForAccount(account);
	std::vector<const SubredditCanonicalId*> result(set.begin(), set.end());
	// Collections.sort(result); -- requires SubredditCanonicalId operator<
	std::sort(result.begin(), result.end(),
		[](const SubredditCanonicalId* a, const SubredditCanonicalId* b) {
			// Comparator stub: sort by display name or ID
			return a < b;
		});
	return result;
}

} // namespace PinkReader
