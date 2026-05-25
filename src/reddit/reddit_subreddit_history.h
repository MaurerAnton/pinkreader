// Origin: org/quantumbadger/redreader/reddit/RedditSubredditHistory.java
#pragma once

#include <vector>
#include <set>
#include <map>
#include <mutex>

// Forward declarations
namespace PinkReader {
class RedditAccount;
class SubredditCanonicalId;
}

namespace PinkReader {

// Keeps an in-memory list of all known subreddits per account
class RedditSubredditHistory {
private:
	static std::map<const RedditAccount*, std::set<const SubredditCanonicalId*>> m_subreddits;
	static std::mutex m_mutex;

	static std::set<const SubredditCanonicalId*>& getForAccount(const RedditAccount& account);

public:
	static void addSubreddit(const RedditAccount& account, const SubredditCanonicalId& id);
	static void addSubreddits(const RedditAccount& account, const std::vector<const SubredditCanonicalId*>& ids);
	static std::vector<const SubredditCanonicalId*> getSubredditsSorted(const RedditAccount& account);
};

} // namespace PinkReader
