// Origin: RedReader/src/main/java/org/quantumbadger/redreader/reddit/prepared/RedditRenderableInboxItem.java
// NOTE: This is an interface / pure virtual class in Java
#pragma once

#include <string>
#include <memory>
#include <optional>

namespace PinkReader {

// Forward declarations
class BetterSSB;
class RRThemeAttributes;
class RedditChangeDataManager;
class TimestampUTC;
class BodyElement;

class RedditRenderableInboxItem {
public:
	virtual ~RedditRenderableInboxItem() = default;

	virtual BetterSSB* getHeader(
		const RRThemeAttributes& theme,
		RedditChangeDataManager& changeDataManager,
		void* context,
		int commentAgeUnits,
		const TimestampUTC* postCreated,
		const TimestampUTC* parentCommentCreated) = 0;

	virtual std::string getAccessibilityHeader(
		const RRThemeAttributes& theme,
		RedditChangeDataManager& changeDataManager,
		void* context,
		int commentAgeUnits,
		const TimestampUTC* postCreated,
		const TimestampUTC* parentCommentCreated,
		bool collapsed,
		const std::optional<int>& indentLevel) = 0;

	virtual void* getBody(
		void* activity,
		const int* textColor,
		const float* textSize,
		bool showLinkButtons) = 0;

	virtual void handleInboxClick(void* activity) = 0;
	virtual void handleInboxLongClick(void* activity) = 0;
};

} // namespace PinkReader
