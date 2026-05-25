// Origin: RedReader/src/main/java/org/quantumbadger/redreader/reddit/prepared/RedditPreparedPost.java
#include "reddit_prepared_post.h"

namespace PinkReader {

RedditPreparedPost::RedditPreparedPost(
		void* /*context*/,
		void* /*cm*/,
		int /*listId*/,
		const RedditParsedPost& post,
		const TimestampUTC& /*timestamp*/,
		bool showSubreddit_,
		bool /*showThumbnails*/,
		bool /*allowHighResThumbnails*/,
		bool showInlinePreviews_)
	: src(&post)
	, mChangeDataManager(nullptr)
	, isArchived(post.isArchived())
	, isLocked(post.isLocked())
	, canModerate(post.getCanModerate())
	, hasThumbnail(false)
	, showSubreddit(showSubreddit_)
	, mIsProbablyAnImage(false)
	, mShowInlinePreviews(showInlinePreviews_)
	, thumbnailCache(nullptr)
	, thumbnailCallback(nullptr)
	, usageId(-1)
	, lastChange() {
}

bool RedditPreparedPost::isUpvoted() const {
	return false;
}

bool RedditPreparedPost::isDownvoted() const {
	return false;
}

bool RedditPreparedPost::isSaved() const {
	return false;
}

bool RedditPreparedPost::isHidden() const {
	return false;
}

bool RedditPreparedPost::isRead() const {
	return false;
}

int RedditPreparedPost::computeScore() const {
	return src->getScoreExcludingOwnVote();
}

bool RedditPreparedPost::shouldShowInlinePreview() const {
	return mShowInlinePreviews && src->isPreviewEnabled();
}

bool RedditPreparedPost::isVideoPreview() const {
	return src->isVideoPreview();
}

void RedditPreparedPost::performAction(void* /*activity*/, int /*action*/) {
}

void RedditPreparedPost::markAsRead(void* /*context*/, bool /*read*/) {
}

void* RedditPreparedPost::getThumbnail() {
	return thumbnailCache.load();
}

void RedditPreparedPost::setThumbnail(void* bitmap) {
	thumbnailCache.store(bitmap);
}

void RedditPreparedPost::setThumbnailCallback(void* callback) {
	thumbnailCallback = callback;
}

} // namespace PinkReader
