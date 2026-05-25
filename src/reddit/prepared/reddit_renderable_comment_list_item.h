// Origin: RedReader/src/main/java/org/quantumbadger/redreader/reddit/prepared/RedditRenderableCommentListItem.java
#pragma once

#include "reddit_renderable_inbox_item.h"
#include <string>
#include <memory>
#include <optional>

namespace PinkReader {

// Forward declarations
class RedditParsedComment;
class RedditPreparedPost;
class RedditRenderableComment;

class RedditRenderableCommentListItem : public RedditRenderableInboxItem {
public:
	RedditRenderableCommentListItem(
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

	const RedditRenderableComment& getComment() const;

private:
	std::unique_ptr<RedditRenderableComment> mComment;
};

} // namespace PinkReader
