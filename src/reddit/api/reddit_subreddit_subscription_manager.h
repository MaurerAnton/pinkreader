// Origin: RedReader/src/main/java/org/quantumbadger/redreader/reddit/api/RedditSubredditSubscriptionManager.java
#pragma once

#include <string>
#include <vector>
#include <unordered_set>
#include <memory>
#include <mutex>
#include "../api/subreddit_subscription_state.h"

namespace PinkReader {

// Forward declarations
class RedditAccount;
class SubredditCanonicalId;
class WritableHashSet;
class TimestampBound;
class TimestampUTC;
class RRError;
class RawObjectDB;

class RedditSubredditSubscriptionManager {
public:
	class SubredditSubscriptionStateChangeListener {
	public:
		virtual ~SubredditSubscriptionStateChangeListener() = default;
		virtual void onSubredditSubscriptionListUpdated(
			RedditSubredditSubscriptionManager& manager) = 0;
		virtual void onSubredditSubscriptionAttempted(
			RedditSubredditSubscriptionManager& manager) = 0;
		virtual void onSubredditUnsubscriptionAttempted(
			RedditSubredditSubscriptionManager& manager) = 0;
	};

	class ListenerContext {
	public:
		void removeListener();
	private:
		friend class RedditSubredditSubscriptionManager;
		ListenerContext(SubredditSubscriptionStateChangeListener& listener);
		SubredditSubscriptionStateChangeListener* mListener;
	};

	static RedditSubredditSubscriptionManager& getSingleton(
		void* context, const RedditAccount& account);

	ListenerContext addListener(SubredditSubscriptionStateChangeListener& listener);
	bool areSubscriptionsReady() const;

	const SubredditSubscriptionState* getSubscriptionState(
		const SubredditCanonicalId& id) const;

	std::vector<SubredditCanonicalId> getSubscriptionList() const;

	void triggerUpdateIfNotReady(void* onFailure = nullptr);
	void triggerUpdate(
		void* handler, // RequestResponseHandler*
		const TimestampBound& timestampBound);

	void subscribe(const SubredditCanonicalId& id, void* activity);
	void unsubscribe(const SubredditCanonicalId& id, void* activity);

private:
	RedditSubredditSubscriptionManager(const RedditAccount& user, void* context);

	enum class SubredditSubscriptionChangeType {
		LIST_UPDATED,
		SUBSCRIPTION_ATTEMPTED,
		UNSUBSCRIPTION_ATTEMPTED
	};

	const RedditAccount* user;
	void* context;

	static std::unique_ptr<RedditSubredditSubscriptionManager> sSingleton;
	static std::string sSingletonAccountUsername;

	std::unique_ptr<WritableHashSet> subscriptions;
	std::unordered_set<std::string> pendingSubscriptions;
	std::unordered_set<std::string> pendingUnsubscriptions;
	TimestampUTC* mLastUpdateRequestTime;

	void onSubscriptionAttempt(const SubredditCanonicalId& id);
	void onUnsubscriptionAttempt(const SubredditCanonicalId& id);
	void onSubscriptionChangeAttemptFailed(const SubredditCanonicalId& id);
	void onSubscriptionAttemptSuccess(const SubredditCanonicalId& id);
	void onUnsubscriptionAttemptSuccess(const SubredditCanonicalId& id);
	void onNewSubscriptionListReceived(
		const std::unordered_set<std::string>& newSubscriptions,
		const TimestampUTC& timestamp);

	static void addToHistory(const RedditAccount& account, const void* subscriptions);
};

} // namespace PinkReader
