// Origin: RedReader/src/main/java/org/quantumbadger/redreader/reddit/prepared/RedditPreparedPost.java
#pragma once

#include <string>
#include <memory>
#include <mutex>
#include <atomic>

namespace PinkReader {

// Forward declarations
class RedditParsedPost;
class RedditChangeDataManager;
class TimestampUTC;
class RedditPostActions;
class BetterSSB;

class RedditPreparedPost {
public:
	RedditPreparedPost(
		void* context,
		void* cm, // CacheManager
		int listId,
		const RedditParsedPost& post,
		const TimestampUTC& timestamp,
		bool showSubreddit,
		bool showThumbnails,
		bool allowHighResThumbnails,
		bool showInlinePreviews);

	const RedditParsedPost& getSrc() const { return *src; }
	bool isUpvoted() const;
	bool isDownvoted() const;
	bool isSaved() const;
	bool isHidden() const;
	bool isRead() const;

	int computeScore() const;

	const bool isArchived;
	const bool isLocked;
	const bool canModerate;
	const bool hasThumbnail;
	const bool showSubreddit;

	bool shouldShowInlinePreview() const;
	bool isVideoPreview() const;
	void performAction(void* activity, int action);
	// BetterSSB buildSubtitle(void* context, bool headerMode);
	// std::string buildAccessibilitySubtitle(void* context, bool headerMode);

	void markAsRead(void* context, bool read);

	// Thumbnail management
	void* getThumbnail();
	void setThumbnail(void* bitmap);
	void setThumbnailCallback(void* callback);

	TimestampUTC lastChange;

private:
	const RedditParsedPost* src;
	RedditChangeDataManager* mChangeDataManager;
	bool mIsProbablyAnImage;
	bool mShowInlinePreviews;
	std::atomic<void*> thumbnailCache;
	void* thumbnailCallback;
	int usageId;
};

} // namespace PinkReader
