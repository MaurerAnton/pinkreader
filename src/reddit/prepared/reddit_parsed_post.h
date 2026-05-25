// Origin: RedReader/src/main/java/org/quantumbadger/redreader/reddit/prepared/RedditParsedPost.kt
#pragma once

#include <string>
#include <memory>
#include <optional>
#include <vector>
#include "../things/reddit_thing_with_id_and_type.h"

namespace PinkReader {

// Forward declarations
class RedditPost;
class UriString;
class BodyElement;
class RedditIdAndType;
enum class PostCommentSort;

class RedditParsedPost : public RedditThingWithIdAndType {
public:
	struct ImagePreviewDetails {
		// UriString url;
		std::string url;
		int width = 0;
		int height = 0;
	};

	RedditParsedPost(void* activity, const RedditPost& src, bool parseSelfText);

	const RedditPost& getSrc() const { return *mSrc; }

	const std::string& getTitle() const { return mTitle; }
	// const UriString& getUrl() const;
	const BodyElement* getSelfText() const { return mSelfText.get(); }
	const std::string* getFlairText() const { return mFlairText ? &*mFlairText : nullptr; }
	const std::string& getDomain() const { return mDomain; }

	bool isStickied() const { return mIsStickied; }
	bool isArchived() const { return mIsArchived; }
	bool isLocked() const { return mIsLocked; }
	bool getCanModerate() const { return mCanModerate; }
	bool isPreviewEnabled() const { return mIsPreviewEnabled; }
	bool isVideoPreview() const { return mIsVideoPreview; }
	bool isSpoiler() const { return mIsSpoiler; }
	bool isNsfw() const { return mIsNsfw; }
	bool isSelfPost() const { return mIsSelfPost; }

	const std::string* getAuthor() const { return mAuthor ? &*mAuthor : nullptr; }
	const std::string* getDistinguished() const { return mDistinguished ? &*mDistinguished : nullptr; }
	const std::string& getSubreddit() const { return mSubreddit; }
	int getScoreExcludingOwnVote() const { return mScoreExcludingOwnVote; }
	int getCommentCount() const { return mCommentCount; }
	int getGoldAmount() const { return mGoldAmount; }
	const std::string* isCrosspost() const { return mIsCrosspost ? &*mIsCrosspost : nullptr; }
	// TimestampUTC getCreatedTimeUTC();
	int getUpvotePercentage() const { return mUpvotePercentage; }
	const std::string& getPermalink() const { return mPermalink; }
	const std::string* getRawSelfTextMarkdown() const { return mRawSelfTextMarkdown ? &*mRawSelfTextMarkdown : nullptr; }
	bool hasSelfText() const;
	const PostCommentSort* getSuggestedCommentSort() const;

	std::string getIdAlone() const override;
	const RedditIdAndType& getIdAndType() const override;

	std::optional<ImagePreviewDetails> getPreview(int minWidth, int minHeight) const;
	std::optional<ImagePreviewDetails> getPreviewMP4(int minWidth, int minHeight) const;

private:
	const RedditPost* mSrc;
	std::string mTitle;
	// UriString mUrl;
	std::unique_ptr<BodyElement> mSelfText;
	std::optional<std::string> mFlairText;
	std::string mDomain;
	std::string mPermalink;
	bool mIsStickied = false;
	bool mIsPreviewEnabled = false;
	bool mIsVideoPreview = false;
	bool mIsArchived = false;
	bool mIsLocked = false;
	bool mCanModerate = false;
	bool mIsSpoiler = false;
	bool mIsNsfw = false;
	bool mIsSelfPost = false;
	std::optional<std::string> mAuthor;
	std::optional<std::string> mDistinguished;
	std::optional<std::string> mRawSelfTextMarkdown;
	std::string mSubreddit;
	int mScoreExcludingOwnVote = 0;
	int mCommentCount = 0;
	int mGoldAmount = 0;
	std::optional<std::string> mIsCrosspost;
	int mUpvotePercentage = 0;

	std::optional<ImagePreviewDetails> getPreviewInternal(
		const void* image, int minWidth, int minHeight) const;
};

} // namespace PinkReader
