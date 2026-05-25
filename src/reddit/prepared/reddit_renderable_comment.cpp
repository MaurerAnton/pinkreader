// Origin: RedReader/src/main/java/org/quantumbadger/redreader/reddit/prepared/RedditRenderableComment.java
#include "reddit_renderable_comment.h"

namespace PinkReader {

RedditRenderableComment::RedditRenderableComment(
		const RedditParsedComment& parsedComment,
		const RedditPreparedPost* parentPost,
		int indentLevel,
		int collapsedIndentLevel,
		std::optional<int> minimumCommentScore)
	: mParsedComment(&parsedComment)
	, mParentPost(parentPost)
	, mIndentLevel(indentLevel)
	, mCollapsedIndentLevel(collapsedIndentLevel)
	, mMinimumCommentScore(std::move(minimumCommentScore)) {
}

BetterSSB* RedditRenderableComment::getHeader(
		const RRThemeAttributes& /*theme*/,
		RedditChangeDataManager& /*changeDataManager*/,
		void* /*context*/,
		int /*commentAgeUnits*/,
		const TimestampUTC* /*postCreated*/,
		const TimestampUTC* /*parentCommentCreated*/) {
	return nullptr; // stub
}

std::string RedditRenderableComment::getAccessibilityHeader(
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

void* RedditRenderableComment::getBody(
		void* /*activity*/,
		const int* /*textColor*/,
		const float* /*textSize*/,
		bool /*showLinkButtons*/) {
	return nullptr; // stub
}

void RedditRenderableComment::handleInboxClick(void* /*activity*/) {
}

void RedditRenderableComment::handleInboxLongClick(void* /*activity*/) {
}

const RedditParsedComment& RedditRenderableComment::getParsedComment() const {
	return *mParsedComment;
}

} // namespace PinkReader
