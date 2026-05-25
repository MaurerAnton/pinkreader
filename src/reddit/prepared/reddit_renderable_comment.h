// Origin: RedReader/src/main/java/org/quantumbadger/redreader/reddit/prepared/RedditRenderableComment.java
#pragma once

#include "reddit_renderable_inbox_item.h"
#include <string>
#include <memory>
#include <optional>

namespace PinkReader {

// Forward declarations
class RedditParsedComment;
class RedditPreparedPost;
class RedditIdAndType;

class RedditRenderableComment : public RedditRenderableInboxItem {
public:
	RedditRenderableComment(
		const RedditParsedComment& parsedComment,
		const RedditPreparedPost* parentPost,
		int indentLevel,
		int collapsedIndentLevel,
		std::optional<int> minimumCommentScore);

	BetterSSB* getHeader(
		const RRThemeAttributes& theme,
		RedditChangeDataManager& changeDataManager,
		void* context,
		int commentAgeUnits,
		const TimestampUTC* postCreated,
		const TimestampUTC* parentCommentCreated) override;

	std::string getAccessibilityHeader(
		const RRThemeAttributes& theme,
		RedditChangeDataManager& changeDataManager,
		void* context,
		int commentAgeUnits,
		const TimestampUTC* postCreated,
		const TimestampUTC* parentCommentCreated,
		bool collapsed,
		const std::optional<int>& indentLevel) override;

	void* getBody(
		void* activity,
		const int* textColor,
		const float* textSize,
		bool showLinkButtons) override;

	void handleInboxClick(void* activity) override;
	void handleInboxLongClick(void* activity) override;

	const RedditParsedComment& getParsedComment() const;

private:
	const RedditParsedComment* mParsedComment;
	const RedditPreparedPost* mParentPost;
	int mIndentLevel;
	int mCollapsedIndentLevel;
	std::optional<int> mMinimumCommentScore;
};

} // namespace PinkReader
