// Origin: RedReader/src/main/java/org/quantumbadger/redreader/reddit/api/RedditAPIMultiredditListRequester.java
#include "reddit_api_multireddit_list_requester.h"
#include <stdexcept>

namespace PinkReader {

const RedditAPIMultiredditListRequester::Key RedditAPIMultiredditListRequester::Key::INSTANCE;

RedditAPIMultiredditListRequester::RedditAPIMultiredditListRequester(void* context, const RedditAccount& user)
	: mContext(context), mUser(&user) {
}

void RedditAPIMultiredditListRequester::performRequest(
		const Key& /*key*/,
		const TimestampBound& /*timestampBound*/,
		void* /*handler*/) {
	// Full implementation:
	// - If anonymous: return empty WritableHashSet
	// - Else: doRequest() -> CacheRequest -> JSON parse -> return multireddit names
}

void RedditAPIMultiredditListRequester::performRequest(
		const void* /*keys*/,
		const TimestampBound& /*timestampBound*/,
		void* /*handler*/) {
	throw std::runtime_error("UnsupportedOperationException");
}

void RedditAPIMultiredditListRequester::performWrite(const WritableHashSet& /*value*/) {
	throw std::runtime_error("UnsupportedOperationException");
}

void RedditAPIMultiredditListRequester::performWrite(const void* /*values*/) {
	throw std::runtime_error("UnsupportedOperationException");
}

void RedditAPIMultiredditListRequester::doRequest(void* /*handler*/) {
}

} // namespace PinkReader
