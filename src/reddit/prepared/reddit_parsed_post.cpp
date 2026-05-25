// Origin: RedReader/src/main/java/org/quantumbadger/redreader/reddit/prepared/RedditParsedPost.kt
#include "reddit_parsed_post.h"

namespace PinkReader {

RedditParsedPost::RedditParsedPost(void* /*activity*/, const RedditPost& src, bool /*parseSelfText*/)
	: mSrc(&src), mTitle("[null]"), mDomain("") {
	// Full implementation requires:
	// - RedditPost field accessors
	// - UriString / UrlEncodedString::getDecoded()
	// - HtmlReader::parse()
	// - Preview image resolution selection logic
}

bool RedditParsedPost::hasSelfText() const {
	return mRawSelfTextMarkdown.has_value() && mRawSelfTextMarkdown->length() > 1;
}

const PostCommentSort* RedditParsedPost::getSuggestedCommentSort() const {
	return nullptr; // stub
}

std::string RedditParsedPost::getIdAlone() const {
	return mSrc->getIdAlone();
}

const RedditIdAndType& RedditParsedPost::getIdAndType() const {
	return mSrc->getIdAndType();
}

std::optional<RedditParsedPost::ImagePreviewDetails>
RedditParsedPost::getPreview(int /*minWidth*/, int /*minHeight*/) const {
	return std::nullopt;
}

std::optional<RedditParsedPost::ImagePreviewDetails>
RedditParsedPost::getPreviewMP4(int /*minWidth*/, int /*minHeight*/) const {
	return std::nullopt;
}

std::optional<RedditParsedPost::ImagePreviewDetails>
RedditParsedPost::getPreviewInternal(const void* /*image*/, int /*minWidth*/, int /*minHeight*/) const {
	return std::nullopt;
}

} // namespace PinkReader
