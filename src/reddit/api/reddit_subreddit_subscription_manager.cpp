// Origin: RedReader/src/main/java/org/quantumbadger/redreader/reddit/api/RedditSubredditSubscriptionManager.java
#include "reddit_subreddit_subscription_manager.h"

namespace PinkReader {

// Static members
std::unique_ptr<RedditSubredditSubscriptionManager> RedditSubredditSubscriptionManager::sSingleton;
std::string RedditSubredditSubscriptionManager::sSingletonAccountUsername;

// ListenerContext
RedditSubredditSubscriptionManager::ListenerContext::ListenerContext(SubredditSubscriptionStateChangeListener& listener)
	: mListener(&listener) {
}

void RedditSubredditSubscriptionManager::ListenerContext::removeListener() {
	// Would need access to parent's listener list
}

// RedditSubredditSubscriptionManager
RedditSubredditSubscriptionManager::RedditSubredditSubscriptionManager(const RedditAccount& user_, void* context_)
	: user(&user_), context(context_), mLastUpdateRequestTime(nullptr) {
}

RedditSubredditSubscriptionManager& RedditSubredditSubscriptionManager::getSingleton(
		void* /*context*/, const RedditAccount& /*account*/) {
	// Stub
	static RedditSubredditSubscriptionManager instance(
		*static_cast<const RedditAccount*>(nullptr), nullptr);
	return instance;
}

RedditSubredditSubscriptionManager::ListenerContext
RedditSubredditSubscriptionManager::addListener(SubredditSubscriptionStateChangeListener& listener) {
	return ListenerContext(listener);
}

bool RedditSubredditSubscriptionManager::areSubscriptionsReady() const {
	return subscriptions != nullptr;
}

const SubredditSubscriptionState* RedditSubredditSubscriptionManager::getSubscriptionState(
		const SubredditCanonicalId& /*id*/) const {
	return nullptr;
}

std::vector<SubredditCanonicalId> RedditSubredditSubscriptionManager::getSubscriptionList() const {
	return {};
}

void RedditSubredditSubscriptionManager::triggerUpdateIfNotReady(void* /*onFailure*/) {
}

void RedditSubredditSubscriptionManager::triggerUpdate(
		void* /*handler*/,
		const TimestampBound& /*timestampBound*/) {
}

void RedditSubredditSubscriptionManager::subscribe(const SubredditCanonicalId& /*id*/, void* /*activity*/) {
}

void RedditSubredditSubscriptionManager::unsubscribe(const SubredditCanonicalId& /*id*/, void* /*activity*/) {
}

void RedditSubredditSubscriptionManager::onSubscriptionAttempt(const SubredditCanonicalId& /*id*/) {
}

void RedditSubredditSubscriptionManager::onUnsubscriptionAttempt(const SubredditCanonicalId& /*id*/) {
}

void RedditSubredditSubscriptionManager::onSubscriptionChangeAttemptFailed(const SubredditCanonicalId& /*id*/) {
}

void RedditSubredditSubscriptionManager::onSubscriptionAttemptSuccess(const SubredditCanonicalId& /*id*/) {
}

void RedditSubredditSubscriptionManager::onUnsubscriptionAttemptSuccess(const SubredditCanonicalId& /*id*/) {
}

void RedditSubredditSubscriptionManager::onNewSubscriptionListReceived(
		const std::unordered_set<std::string>& /*newSubscriptions*/,
		const TimestampUTC& /*timestamp*/) {
}

void RedditSubredditSubscriptionManager::addToHistory(const RedditAccount& /*account*/, const void* /*subscriptions*/) {
}

} // namespace PinkReader
