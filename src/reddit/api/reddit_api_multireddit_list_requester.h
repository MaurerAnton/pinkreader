// Origin: RedReader/src/main/java/org/quantumbadger/redreader/reddit/api/RedditAPIMultiredditListRequester.java
#pragma once

#include <memory>

namespace PinkReader {

// Forward declarations
class RedditAccount;
class TimestampBound;
class RRError;
class WritableHashSet;

class RedditAPIMultiredditListRequester {
public:
	class Key {
	public:
		static const Key INSTANCE;
	private:
		Key() = default;
	};

	RedditAPIMultiredditListRequester(void* context, const RedditAccount& user);

	void performRequest(
		const Key& key,
		const TimestampBound& timestampBound,
		void* handler); // RequestResponseHandler<WritableHashSet, RRError>

	void performRequest(
		const void* keys, // Collection<Key>
		const TimestampBound& timestampBound,
		void* handler);

	void performWrite(const WritableHashSet& value);
	void performWrite(const void* values);

private:
	void* mContext;
	const RedditAccount* mUser;

	void doRequest(void* handler);
};

} // namespace PinkReader
