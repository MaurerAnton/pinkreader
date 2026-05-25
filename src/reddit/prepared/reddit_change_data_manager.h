// Origin: RedReader/src/main/java/org/quantumbadger/redreader/reddit/prepared/RedditChangeDataManager.java
#pragma once

#include <string>
#include <map>
#include <unordered_map>
#include <memory>
#include <optional>
#include <functional>
#include <mutex>

namespace PinkReader {

// Forward declarations
class RedditAccount;
class RedditIdAndType;
class RedditComment;
class RedditPost;
class TimestampUTC;
class TimeDuration;
class ExtendedDataOutputStream;
class ExtendedDataInputStream;

class RedditChangeDataManager {
public:
	class Listener {
	public:
		virtual ~Listener() = default;
		virtual void onRedditDataChange(const RedditIdAndType& thingIdAndType) = 0;
	};

	// Singleton per user
	static RedditChangeDataManager& getInstance(const RedditAccount& user);

	// Serialization
	static void writeAllUsers(ExtendedDataOutputStream& dos);
	static void readAllUsers(ExtendedDataInputStream& dis, void* context);

	// Pruning
	static void pruneAllUsersDefaultMaxAge();
	static void pruneAllUsersWhereOlderThan(const TimeDuration& maxAge);

	// Listener management
	void addListener(const RedditIdAndType& thing, Listener& listener);
	void removeListener(const RedditIdAndType& thing, Listener& listener);

	// Update from Reddit objects
	void update(const TimestampUTC& timestamp, const RedditComment& comment);
	void update(const TimestampUTC& timestamp, const RedditPost& post);

	// Mark individual states
	void markUpvoted(const TimestampUTC& timestamp, const RedditIdAndType& thing);
	void markDownvoted(const TimestampUTC& timestamp, const RedditIdAndType& thing);
	void markUnvoted(const TimestampUTC& timestamp, const RedditIdAndType& thing);
	void markSaved(const TimestampUTC& timestamp, const RedditIdAndType& thing, bool isSaved);
	void markRead(const TimestampUTC& timestamp, const RedditIdAndType& thing, bool isRead);
	void markHidden(const TimestampUTC& timestamp, const RedditIdAndType& thing, std::optional<bool> isHidden);

	// Query states
	bool isUpvoted(const RedditIdAndType& thing) const;
	bool isDownvoted(const RedditIdAndType& thing) const;
	bool isSaved(const RedditIdAndType& thing) const;
	bool isRead(const RedditIdAndType& thing) const;
	std::optional<bool> isHidden(const RedditIdAndType& thing) const;

private:
	class Entry {
	public:
		static const Entry CLEAR_ENTRY;

		Entry();
		Entry(const TimestampUTC& timestamp, bool isUpvoted, bool isDownvoted,
			bool isRead, bool isSaved, std::optional<bool> isHidden);
		explicit Entry(ExtendedDataInputStream& dis);

		void writeTo(ExtendedDataOutputStream& dos) const;

		bool isClear() const;
		const TimestampUTC& getTimestamp() const { return mTimestamp; }
		bool isUpvoted() const { return mIsUpvoted; }
		bool isDownvoted() const { return mIsDownvoted; }
		bool isRead() const { return mIsRead; }
		bool isSaved() const { return mIsSaved; }
		std::optional<bool> isHidden() const { return mIsHidden; }

		Entry update(const TimestampUTC& timestamp, const RedditComment& comment) const;
		Entry update(const TimestampUTC& timestamp, const RedditPost& post) const;
		Entry markUpvoted(const TimestampUTC& timestamp) const;
		Entry markDownvoted(const TimestampUTC& timestamp) const;
		Entry markUnvoted(const TimestampUTC& timestamp) const;
		Entry markRead(const TimestampUTC& timestamp, bool read) const;
		Entry markSaved(const TimestampUTC& timestamp, bool isSaved) const;
		Entry markHidden(const TimestampUTC& timestamp, std::optional<bool> isHidden) const;

	private:
		TimestampUTC mTimestamp;
		bool mIsUpvoted;
		bool mIsDownvoted;
		bool mIsRead;
		bool mIsSaved;
		std::optional<bool> mIsHidden;
	};

	RedditChangeDataManager() = default;

	static std::mutex sInstanceMutex;
	static std::map<std::string, std::unique_ptr<RedditChangeDataManager>> sInstanceMap;

	std::mutex mLock;
	std::unordered_map<std::string, Entry> mEntries; // keyed by id string

	Entry get(const RedditIdAndType& thing) const;
	void set(const RedditIdAndType& thing, const Entry& existingValue, const Entry& newValue);
	void insertAll(const std::unordered_map<std::string, Entry>& entries);
};

} // namespace PinkReader
