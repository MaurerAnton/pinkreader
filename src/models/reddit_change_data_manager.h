/*
 * PinkReader - GPLv3
 * File: reddit_change_data_manager.h
 * Exact port of RedditChangeDataManager.java (668 lines)
 *
 * Ports every line: Entry inner class, listener pattern, vote/save/read/hide tracking,
 * serialization (writeAllUsers/readAllUsers), pruning, per-user instance map.
 */

#pragma once

#include <QString>
#include <QHash>
#include <QSet>
#include <memory>
#include <mutex>
#include <functional>
#include <optional>

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

/**
 * @brief Port of RedditChangeDataManager (Java public final class, lines 52-668)
 *
 * Tracks per-user post/comment state: upvoted, downvoted, read, saved, hidden.
 * Inner class Entry stores the boolean flags and timestamp.
 * Static methods for serialization and global pruning.
 * Listener pattern via WeakReferenceListHashMapManager.
 */
class RedditChangeDataManager {
public:
    // ========================================================================
    // Listener interface — Java lines 219-221
    // ========================================================================
    class Listener {
    public:
        virtual ~Listener() = default;

        // Java line 220: void onRedditDataChange(final RedditIdAndType thingIdAndType);
        virtual void onRedditDataChange(const RedditIdAndType &thingIdAndType) = 0;
    };

    // ========================================================================
    // Entry inner class — Java lines 223-407 (private static final)
    // ========================================================================
    class Entry {
    public:
        // Java line 234: static final Entry CLEAR_ENTRY = new Entry();
        static const Entry &CLEAR_ENTRY();

        // Accessors — Java lines 287-305
        bool isUpvoted() const { return m_isUpvoted; }     // Java line 287-289
        bool isSaved() const { return m_isSaved; }           // Java line 291-293
        bool isRead() const { return m_isRead; }             // Java line 295-297
        std::optional<bool> isHidden() const { return m_isHidden; } // Java line 299-301
        bool isDownvoted() const { return m_isDownvoted; }   // Java line 303-305
        const TimestampUTC &timestamp() const { return m_timestamp; }

        // Mutators (return new Entry, since Entry is immutable like in Java) — Java lines 307-406
        Entry update(const TimestampUTC &timestamp, const RedditComment &comment) const; // Java lines 307-322
        Entry update(const TimestampUTC &timestamp, const RedditPost &post) const;       // Java lines 324-340
        Entry markUpvoted(const TimestampUTC &timestamp) const;    // Java lines 342-351
        Entry markDownvoted(const TimestampUTC &timestamp) const;  // Java lines 353-362
        Entry markUnvoted(const TimestampUTC &timestamp) const;    // Java lines 364-373
        Entry markRead(const TimestampUTC &timestamp, bool read) const;     // Java lines 375-384
        Entry markSaved(const TimestampUTC &timestamp, bool isSaved) const; // Java lines 386-395
        Entry markHidden(const TimestampUTC &timestamp, const std::optional<bool> &isHidden) const; // Java lines 397-406

        // Java line 261-268: constructor from ExtendedDataInputStream
        static Entry fromStream(ExtendedDataInputStream &dis);

        // Java line 270-277: writeTo(ExtendedDataOutputStream)
        void writeTo(ExtendedDataOutputStream &dos) const;

        // Java line 279-285: isClear()
        bool isClear() const;

    private:
        // Java lines 225-233: fields
        TimestampUTC m_timestamp;
        bool m_isUpvoted;
        bool m_isDownvoted;
        bool m_isRead;
        bool m_isSaved;
        std::optional<bool> m_isHidden; // null in Java = false for posts hidden, null for comments "collapsed"

        // Java lines 236-243: private Entry() — CLEAR_ENTRY constructor
        Entry();

        // Java lines 245-259: full constructor
        Entry(const TimestampUTC &timestamp,
              bool isUpvoted,
              bool isDownvoted,
              bool isRead,
              bool isSaved,
              const std::optional<bool> &isHidden);
    };

    // ========================================================================
    // Static API
    // ========================================================================

    // Java lines 61-75: getInstance(RedditAccount)
    static RedditChangeDataManager &getInstance(const RedditAccount &user);

    // Java lines 90-130: writeAllUsers(ExtendedDataOutputStream)
    static void writeAllUsers(ExtendedDataOutputStream &dos);

    // Java lines 132-194: readAllUsers(ExtendedDataInputStream, Context)
    static void readAllUsers(ExtendedDataInputStream &dis);

    // Java lines 196-198: pruneAllUsersDefaultMaxAge
    static void pruneAllUsersDefaultMaxAge();

    // Java lines 200-217: pruneAllUsersWhereOlderThan(TimeDuration)
    static void pruneAllUsersWhereOlderThan(const TimeDuration &maxAge);

    // ========================================================================
    // Instance API
    // ========================================================================

    // Java lines 427-432: addListener
    void addListener(const RedditIdAndType &thing, Listener *listener);

    // Java lines 434-439: removeListener
    void removeListener(const RedditIdAndType &thing, Listener *listener);

    // Java lines 496-503: update(TimestampUTC, RedditComment)
    void update(const TimestampUTC &timestamp, const RedditComment &comment);

    // Java lines 505-512: update(TimestampUTC, RedditPost)
    void update(const TimestampUTC &timestamp, const RedditPost &post);

    // Java lines 514-521: markUpvoted
    void markUpvoted(const TimestampUTC &timestamp, const RedditIdAndType &thing);

    // Java lines 523-532: markDownvoted
    void markDownvoted(const TimestampUTC &timestamp, const RedditIdAndType &thing);

    // Java lines 534-541: markUnvoted
    void markUnvoted(const TimestampUTC &timestamp, const RedditIdAndType &thing);

    // Java lines 543-553: markSaved
    void markSaved(const TimestampUTC &timestamp, const RedditIdAndType &thing, bool saved);

    // Java lines 555-565: markHidden
    void markHidden(const TimestampUTC &timestamp, const RedditIdAndType &thing,
                     const std::optional<bool> &hidden);

    // Java lines 567-577: markRead
    void markRead(const TimestampUTC &timestamp, const RedditIdAndType &thing,
                   const std::optional<bool> &read);

    // Java lines 579-583: isUpvoted
    bool isUpvoted(const RedditIdAndType &thing);

    // Java lines 585-589: isDownvoted
    bool isDownvoted(const RedditIdAndType &thing);

    // Java lines 591-595: isRead
    bool isRead(const RedditIdAndType &thing);

    // Java lines 597-601: isSaved
    bool isSaved(const RedditIdAndType &thing);

    // Java lines 603-607: isHidden
    std::optional<bool> isHidden(const RedditIdAndType &thing);

private:
    // ========================================================================
    // Construction — Java: no public constructor, only getInstance()
    // ========================================================================
    RedditChangeDataManager();
    ~RedditChangeDataManager();

    RedditChangeDataManager(const RedditChangeDataManager &) = delete;
    RedditChangeDataManager &operator=(const RedditChangeDataManager &) = delete;

    // ========================================================================
    // Private methods
    // ========================================================================

    // Java lines 441-450: get(RedditIdAndType) -> Entry
    const Entry &get(const RedditIdAndType &thing);

    // Java lines 452-472: set(RedditIdAndType, Entry existingValue, Entry newValue)
    void set(const RedditIdAndType &thing, const Entry &existingValue, const Entry &newValue);

    // Java lines 474-494: insertAll(HashMap<RedditIdAndType, Entry>)
    void insertAll(const QHash<RedditIdAndType, Entry> &entries);

    // Java lines 609-613: snapshot() -> HashMap<RedditIdAndType, Entry>
    QHash<RedditIdAndType, Entry> snapshot();

    // Java lines 615-667: prune(TimeDuration maxAge)
    void prune(const TimeDuration &maxAge);

    // Java lines 77-88: snapshotAllUsers() — static
    static QHash<RedditAccount, QHash<RedditIdAndType, Entry>> snapshotAllUsers();

    // ========================================================================
    // Fields
    // ========================================================================

    // Java line 54: TAG
    static const QString TAG;

    // Java line 56: MAX_ENTRY_COUNT = 10_000
    static constexpr int MAX_ENTRY_COUNT = 10000;

    // Java lines 58-59: INSTANCE_MAP (HashMap<RedditAccount, RedditChangeDataManager>)
    static QHash<const RedditAccount *, RedditChangeDataManager *> s_instanceMap;
    static std::mutex s_instanceMapMutex;

    // Java lines 421-422: mEntries (HashMap<RedditIdAndType, Entry>)
    QHash<RedditIdAndType, Entry> m_entries;

    // Java line 422: mLock (Object)
    mutable std::recursive_mutex m_lock;

    // Java lines 424-425: mListeners (WeakReferenceListHashMapManager<RedditIdAndType, Listener>)
    // In C++ we use a multimap; Java uses weak references
    QMultiHash<RedditIdAndType, Listener *> m_listeners;
};

} // namespace PinkReader
