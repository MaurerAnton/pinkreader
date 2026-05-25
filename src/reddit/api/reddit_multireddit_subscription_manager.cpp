// Origin: RedReader/src/main/java/org/quantumbadger/redreader/reddit/api/RedditMultiredditSubscriptionManager.java
#include "reddit_multireddit_subscription_manager.h"

namespace PinkReader {

std::unique_ptr<RedditMultiredditSubscriptionManager> RedditMultiredditSubscriptionManager::sSingleton;
std::string RedditMultiredditSubscriptionManager::sSingletonAccountUsername;

RedditMultiredditSubscriptionManager::RedditMultiredditSubscriptionManager(const RedditAccount& user, void* context)
	: mUser(&user), mContext(context) {
}

RedditMultiredditSubscriptionManager& RedditMultiredditSubscriptionManager::getSingleton(
		void* /*context*/, const RedditAccount& /*account*/) {
	// Stub: creates a static instance
	static RedditMultiredditSubscriptionManager instance(
		*static_cast<const RedditAccount*>(nullptr), nullptr);
	return instance;
}

void RedditMultiredditSubscriptionManager::addListener(MultiredditListChangeListener& /*listener*/) {
}

bool RedditMultiredditSubscriptionManager::areSubscriptionsReady() const {
	return mMultireddits != nullptr;
}

std::vector<std::string> RedditMultiredditSubscriptionManager::getSubscriptionList() {
	return {};
}

void RedditMultiredditSubscriptionManager::triggerUpdate(
		void* /*handler*/,
		const TimestampBound& /*timestampBound*/) {
}

void RedditMultiredditSubscriptionManager::onNewSubscriptionListReceived(
		const std::vector<std::string>& /*newSubscriptions*/,
		const TimestampUTC& /*timestamp*/) {
}

} // namespace PinkReader
