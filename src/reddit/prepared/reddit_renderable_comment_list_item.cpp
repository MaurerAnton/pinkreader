// Origin: RedReader/src/main/java/org/quantumbadger/redreader/reddit/prepared/RedditRenderableCommentListItem.java
#include "reddit_renderable_comment_list_item.h"
#include "reddit_renderable_comment.h"

namespace PinkReader {

RedditRenderableCommentListItem::RedditRenderableCommentListItem(
		const RedditParsedComment& parsedComment,
		const RedditPreparedPost* parentPost,
		int indentLevel,
		int collapsedIndentLevel,
		std::optional<int> minimumCommentScore)
	: mComment(std::make_unique<RedditRenderableComment>(
		parsedComment, parentPost, indentLevel, collapsedIndentLevel, std::move(minimumCommentScore))) {
}

BetterSSB* RedditRenderableCommentListItem::getHeader(
		const RRThemeAttributes& theme,
		RedditChangeDataManager& changeDataManager,
		void* context,
		int commentAgeUnits,
		const TimestampUTC* postCreated,
		const TimestampUTC* parentCommentCreated) {
	return mComment->getHeader(theme, changeDataManager, context, commentAgeUnits, postCreated, parentCommentCreated);
}

std::string RedditRenderableCommentListItem::getAccessibilityHeader(
		const RRThemeAttributes& theme,
		RedditChangeDataManager& changeDataManager,
		void* context,
		int commentAgeUnits,
		const TimestampUTC* postCreated,
		const TimestampUTC* parentCommentCreated,
		bool collapsed,
		const std::optional<int>& indentLevel) {
	return mComment->getAccessibilityHeader(theme, changeDataManager, context, commentAgeUnits, postCreated, parentCommentCreated, collapsed, indentLevel);
}

void* RedditRenderableCommentListItem::getBody(
		void* activity,
		const int* textColor,
		const float* textSize,
		bool showLinkButtons) {
	return mComment->getBody(activity, textColor, textSize, showLinkButtons);
}

void RedditRenderableCommentListItem::handleInboxClick(void* activity) {
	mComment->handleInboxClick(activity);
}

void RedditRenderableCommentListItem::handleInboxLongClick(void* activity) {
	mComment->handleInboxLongClick(activity);
}

const RedditRenderableComment& RedditRenderableCommentListItem::getComment() const {
	return *mComment;
}

} // namespace PinkReader
