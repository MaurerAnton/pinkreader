// Origin: RedReader/src/main/java/org/quantumbadger/redreader/reddit/prepared/RedditPreparedMessage.java
#include "reddit_prepared_message.h"

namespace PinkReader {

RedditPreparedMessage::RedditPreparedMessage(
		void* /*activity*/,
		const RedditMessage& message,
		int inboxType_)
	: src(&message)
	, inboxType(inboxType_) {
}

BetterSSB* RedditPreparedMessage::getHeader(
		const RRThemeAttributes& /*theme*/,
		RedditChangeDataManager& /*changeDataManager*/,
		void* /*context*/,
		int /*commentAgeUnits*/,
		const TimestampUTC* /*postCreated*/,
		const TimestampUTC* /*parentCommentCreated*/) {
	return header.get();
}

std::string RedditPreparedMessage::getAccessibilityHeader(
		const RRThemeAttributes& /*theme*/,
		RedditChangeDataManager& /*changeDataManager*/,
		void* /*context*/,
		int /*commentAgeUnits*/,
		const TimestampUTC* /*postCreated*/,
		const TimestampUTC* /*parentCommentCreated*/,
		bool /*collapsed*/,
		const std::optional<int>& /*indentLevel*/) {
	return ""; // stub
}

void* RedditPreparedMessage::getBody(
		void* /*activity*/,
		const int* /*textColor*/,
		const float* /*textSize*/,
		bool /*showLinkButtons*/) {
	return nullptr; // stub
}

void RedditPreparedMessage::handleInboxClick(void* /*activity*/) {
}

void RedditPreparedMessage::handleInboxLongClick(void* /*activity*/) {
}

const BetterSSB& RedditPreparedMessage::getHeader() const {
	return *header;
}

void RedditPreparedMessage::openReplyActivity(void* /*activity*/) {
}

} // namespace PinkReader
