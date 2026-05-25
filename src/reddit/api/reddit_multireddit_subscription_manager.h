// Origin: RedReader/src/main/java/org/quantumbadger/redreader/reddit/api/RedditMultiredditSubscriptionManager.java
#pragma once

#include <string>
#include <vector>
#include <memory>
#include <mutex>

namespace PinkReader {

// Forward declarations
class RedditAccount;
class WritableHashSet;
class TimestampBound;
class TimestampUTC;
class RRError;
class RawObjectDB;

class RedditMultiredditSubscriptionManager {
public:
	class MultiredditListChangeListener {
	public:
		virtual ~MultiredditListChangeListener() = default;
		virtual void onMultiredditListUpdated(
			RedditMultiredditSubscriptionManager& manager) = 0;
	};

	static RedditMultiredditSubscriptionManager& getSingleton(
		void* context, const RedditAccount& account);

	void addListener(MultiredditListChangeListener& listener);
	bool areSubscriptionsReady() const;
	std::vector<std::string> getSubscriptionList();

	void triggerUpdate(
		void* handler, // RequestResponseHandler<HashSet<String>, RRError>*
		const TimestampBound& timestampBound);

private:
	RedditMultiredditSubscriptionManager(const RedditAccount& user, void* context);

	const RedditAccount* mUser;
	void* mContext;

	static std::unique_ptr<RedditMultiredditSubscriptionManager> sSingleton;
	static std::string sSingletonAccountUsername;

	std::unique_ptr<WritableHashSet> mMultireddits;

	void onNewSubscriptionListReceived(
		const std::vector<std::string>& newSubscriptions,
		const TimestampUTC& timestamp);
};

} // namespace PinkReader
