// Origin: RedReader/src/main/java/org/quantumbadger/redreader/reddit/prepared/RedditParsedComment.java
// Minimal stub .cpp -- full implementation requires Android/Java infrastructure
#include "reddit_parsed_comment.h"

namespace PinkReader {

RedditParsedComment::RedditParsedComment(const RedditComment& comment, void* /*activity*/)
	: mSrc(&comment), mBody(nullptr), mFlair(nullptr) {
	// Full implementation requires:
	// - HtmlReader::parse(comment.getBody_html().getDecoded(), activity)
	// - Comment flair text processing
	// - Emote downloading
}

const BodyElement& RedditParsedComment::getBody() const {
	return *mBody;
}

BetterSSB* RedditParsedComment::getFlair() const {
	return mFlair.get();
}

std::string RedditParsedComment::getIdAlone() const {
	return mSrc->getIdAlone();
}

const RedditIdAndType& RedditParsedComment::getIdAndType() const {
	return mSrc->getIdAndType();
}

const RedditComment& RedditParsedComment::getRawComment() const {
	return *mSrc;
}

void RedditParsedComment::getFlairEmotes(void* /*flairRichtext*/, void* /*activity*/) {
	// Full implementation requires Android Bitmap, ImageSpan, CacheManager
}

} // namespace PinkReader
