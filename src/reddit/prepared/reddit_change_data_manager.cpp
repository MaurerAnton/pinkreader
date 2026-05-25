// Origin: RedReader/src/main/java/org/quantumbadger/redreader/reddit/prepared/RedditChangeDataManager.java
#include "reddit_change_data_manager.h"

namespace PinkReader {

// Static members
std::mutex RedditChangeDataManager::sInstanceMutex;
std::map<std::string, std::unique_ptr<RedditChangeDataManager>> RedditChangeDataManager::sInstanceMap;

// Entry defaults
const RedditChangeDataManager::Entry RedditChangeDataManager::Entry::CLEAR_ENTRY;

RedditChangeDataManager::Entry::Entry()
	: mTimestamp() // ZERO
	, mIsUpvoted(false)
	, mIsDownvoted(false)
	, mIsRead(false)
	, mIsSaved(false)
	, mIsHidden(std::nullopt) {
}

RedditChangeDataManager::Entry::Entry(const TimestampUTC& timestamp, bool isUpvoted, bool isDownvoted,
		bool isRead, bool isSaved, std::optional<bool> isHidden)
	: mTimestamp(timestamp)
	, mIsUpvoted(isUpvoted)
	, mIsDownvoted(isDownvoted)
	, mIsRead(isRead)
	, mIsSaved(isSaved)
	, mIsHidden(std::move(isHidden)) {
}

bool RedditChangeDataManager::Entry::isClear() const {
	return !mIsUpvoted && !mIsDownvoted && !mIsRead && !mIsSaved && !mIsHidden.has_value();
}

RedditChangeDataManager::Entry RedditChangeDataManager::Entry::markUpvoted(const TimestampUTC& timestamp) const {
	return Entry(timestamp, true, false, mIsRead, mIsSaved, mIsHidden);
}

RedditChangeDataManager::Entry RedditChangeDataManager::Entry::markDownvoted(const TimestampUTC& timestamp) const {
	return Entry(timestamp, false, true, mIsRead, mIsSaved, mIsHidden);
}

RedditChangeDataManager::Entry RedditChangeDataManager::Entry::markUnvoted(const TimestampUTC& timestamp) const {
	return Entry(timestamp, false, false, mIsRead, mIsSaved, mIsHidden);
}

RedditChangeDataManager::Entry RedditChangeDataManager::Entry::markRead(const TimestampUTC& timestamp, bool read) const {
	return Entry(timestamp, mIsUpvoted, mIsDownvoted, read, mIsSaved, mIsHidden);
}

RedditChangeDataManager::Entry RedditChangeDataManager::Entry::markSaved(const TimestampUTC& timestamp, bool isSaved) const {
	return Entry(timestamp, mIsUpvoted, mIsDownvoted, mIsRead, isSaved, mIsHidden);
}

RedditChangeDataManager::Entry RedditChangeDataManager::Entry::markHidden(const TimestampUTC& timestamp, std::optional<bool> isHidden) const {
	return Entry(timestamp, mIsUpvoted, mIsDownvoted, mIsRead, mIsSaved, std::move(isHidden));
}

RedditChangeDataManager::Entry RedditChangeDataManager::Entry::update(const TimestampUTC& timestamp, const RedditComment& comment) const {
	// stub
	return *this;
}

RedditChangeDataManager::Entry RedditChangeDataManager::Entry::update(const TimestampUTC& timestamp, const RedditPost& post) const {
	// stub
	return *this;
}

RedditChangeDataManager::Entry::Entry(ExtendedDataInputStream& /*dis*/) {
}

void RedditChangeDataManager::Entry::writeTo(ExtendedDataOutputStream& /*dos*/) const {
}

// RedditChangeDataManager methods

RedditChangeDataManager& RedditChangeDataManager::getInstance(const RedditAccount& /*user*/) {
	// stub: returns a static instance
	static RedditChangeDataManager instance;
	return instance;
}

void RedditChangeDataManager::writeAllUsers(ExtendedDataOutputStream& /*dos*/) {
}

void RedditChangeDataManager::readAllUsers(ExtendedDataInputStream& /*dis*/, void* /*context*/) {
}

void RedditChangeDataManager::pruneAllUsersDefaultMaxAge() {
}

void RedditChangeDataManager::pruneAllUsersWhereOlderThan(const TimeDuration& /*maxAge*/) {
}

void RedditChangeDataManager::addListener(const RedditIdAndType& /*thing*/, Listener& /*listener*/) {
}

void RedditChangeDataManager::removeListener(const RedditIdAndType& /*thing*/, Listener& /*listener*/) {
}

void RedditChangeDataManager::update(const TimestampUTC& timestamp, const RedditComment& /*comment*/) {
}

void RedditChangeDataManager::update(const TimestampUTC& timestamp, const RedditPost& /*post*/) {
}

void RedditChangeDataManager::markUpvoted(const TimestampUTC& timestamp, const RedditIdAndType& thing) {
}

void RedditChangeDataManager::markDownvoted(const TimestampUTC& timestamp, const RedditIdAndType& thing) {
}

void RedditChangeDataManager::markUnvoted(const TimestampUTC& timestamp, const RedditIdAndType& thing) {
}

void RedditChangeDataManager::markSaved(const TimestampUTC& timestamp, const RedditIdAndType& thing, bool isSaved) {
}

void RedditChangeDataManager::markRead(const TimestampUTC& timestamp, const RedditIdAndType& thing, bool isRead) {
}

void RedditChangeDataManager::markHidden(const TimestampUTC& timestamp, const RedditIdAndType& thing, std::optional<bool> isHidden) {
}

bool RedditChangeDataManager::isUpvoted(const RedditIdAndType& /*thing*/) const {
	return false;
}

bool RedditChangeDataManager::isDownvoted(const RedditIdAndType& /*thing*/) const {
	return false;
}

bool RedditChangeDataManager::isSaved(const RedditIdAndType& /*thing*/) const {
	return false;
}

bool RedditChangeDataManager::isRead(const RedditIdAndType& /*thing*/) const {
	return false;
}

std::optional<bool> RedditChangeDataManager::isHidden(const RedditIdAndType& /*thing*/) const {
	return std::nullopt;
}

RedditChangeDataManager::Entry RedditChangeDataManager::get(const RedditIdAndType& /*thing*/) const {
	return Entry::CLEAR_ENTRY;
}

void RedditChangeDataManager::set(const RedditIdAndType& /*thing*/, const Entry& /*existingValue*/, const Entry& /*newValue*/) {
}

void RedditChangeDataManager::insertAll(const std::unordered_map<std::string, Entry>& /*entries*/) {
}

} // namespace PinkReader
