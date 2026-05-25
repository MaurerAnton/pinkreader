// Origin: RedReader/src/main/java/org/quantumbadger/redreader/reddit/prepared/RedditPreparedMessage.java
#pragma once

#include "reddit_renderable_inbox_item.h"
#include <string>
#include <memory>
#include <optional>

namespace PinkReader {

// Forward declarations
class BetterSSB;
class BodyElement;
class RedditIdAndType;
class RedditMessage;
enum class InboxType;

class RedditPreparedMessage : public RedditRenderableInboxItem {
public:
	RedditPreparedMessage(
		void* activity,
		const RedditMessage& message,
		int inboxType);

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

	const BetterSSB& getHeader() const;

private:
	std::unique_ptr<BetterSSB> header;
	std::unique_ptr<BodyElement> body;
	// RedditIdAndType idAndType;
	const RedditMessage* src;
	int inboxType;

	void openReplyActivity(void* activity);
};

} // namespace PinkReader
