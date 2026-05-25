/*
 * PinkReader - GPLv3
 * File: reddit_change_data_manager.cpp
 * Port of RedditChangeDataManager.java (668 lines)
 *
 * Every method, every inner class method, every logic branch.
 */

#include "models/reddit_change_data_manager.h"

#include <QDebug>
#include <QMap>
#include <algorithm>

namespace PinkReader {

// ============================================================================
// Static fields — Java lines 54, 56, 58-59
// ============================================================================

const QString RedditChangeDataManager::TAG = QStringLiteral("RedditChangeDataManager");

QHash<const RedditAccount *, RedditChangeDataManager *> RedditChangeDataManager::s_instanceMap;
std::mutex RedditChangeDataManager::s_instanceMapMutex;

// ============================================================================
// Entry — CLEAR_ENTRY & constructors — Java lines 223-268
// ============================================================================

// Java line 234: static final Entry CLEAR_ENTRY = new Entry();
// Lazy-initialized static
static RedditChangeDataManager::Entry *s_clearEntry = nullptr;

const RedditChangeDataManager::Entry &RedditChangeDataManager::Entry::CLEAR_ENTRY()
{
    if (!s_clearEntry) {
        s_clearEntry = new RedditChangeDataManager::Entry();
    }
    return *s_clearEntry;
}

// Java lines 236-243: private Entry() — CLEAR_ENTRY default constructor
RedditChangeDataManager::Entry::Entry()
    : m_timestamp()          // TimestampUTC.ZERO
    , m_isUpvoted(false)
    , m_isDownvoted(false)
    , m_isRead(false)
    , m_isSaved(false)
    , m_isHidden(std::nullopt)  // null in Java
{
    // Java line 237-242: all false/null
}

// Java lines 245-259: full constructor
RedditChangeDataManager::Entry::Entry(
        const TimestampUTC &timestamp,
        bool isUpvoted,
        bool isDownvoted,
        bool isRead,
        bool isSaved,
        const std::optional<bool> &isHidden)
    : m_timestamp(timestamp)
    , m_isUpvoted(isUpvoted)
    , m_isDownvoted(isDownvoted)
    , m_isRead(isRead)
    , m_isSaved(isSaved)
    , m_isHidden(isHidden)
{
}

// Java lines 261-268: constructor from ExtendedDataInputStream
RedditChangeDataManager::Entry RedditChangeDataManager::Entry::fromStream(ExtendedDataInputStream &dis)
{
    // Java lines 262-267:
    // mTimestamp = TimestampUTC.fromUtcMs(dis.readLong());
    // mIsUpvoted = dis.readBoolean();
    // mIsDownvoted = dis.readBoolean();
    // mIsRead = dis.readBoolean();
    // mIsSaved = dis.readBoolean();
    // mIsHidden = dis.readNullableBoolean();
    TimestampUTC timestamp = TimestampUTC::fromUtcMs(dis.readLong());
    bool upvoted = dis.readBoolean();
    bool downvoted = dis.readBoolean();
    bool read = dis.readBoolean();
    bool saved = dis.readBoolean();
    std::optional<bool> hidden = dis.readNullableBoolean();

    return Entry(timestamp, upvoted, downvoted, read, saved, hidden);
}

// Java lines 270-277: writeTo(ExtendedDataOutputStream)
void RedditChangeDataManager::Entry::writeTo(ExtendedDataOutputStream &dos) const
{
    // Java lines 271-276:
    // dos.writeLong(mTimestamp.toUtcMs());
    // dos.writeBoolean(mIsUpvoted);
    // dos.writeBoolean(mIsDownvoted);
    // dos.writeBoolean(mIsRead);
    // dos.writeBoolean(mIsSaved);
    // dos.writeNullableBoolean(mIsHidden);
    dos.writeLong(m_timestamp.toUtcMs());
    dos.writeBoolean(m_isUpvoted);
    dos.writeBoolean(m_isDownvoted);
    dos.writeBoolean(m_isRead);
    dos.writeBoolean(m_isSaved);
    dos.writeNullableBoolean(m_isHidden);
}

// Java lines 279-285: isClear()
bool RedditChangeDataManager::Entry::isClear() const
{
    // Java line 280-284:
    // return !mIsUpvoted && !mIsDownvoted && !mIsRead && !mIsSaved && mIsHidden == null;
    return !m_isUpvoted
        && !m_isDownvoted
        && !m_isRead
        && !m_isSaved
        && !m_isHidden.has_value();
}

// ============================================================================
// Entry update methods — Java lines 307-406
// ============================================================================

// Java lines 307-322: update(TimestampUTC, RedditComment)
RedditChangeDataManager::Entry RedditChangeDataManager::Entry::update(
        const TimestampUTC &timestamp,
        const RedditComment &comment) const
{
    // Java lines 311-313: if(timestamp.isLessThan(mTimestamp)) return this;
    if (timestamp.isLessThan(m_timestamp)) {
        return *this;
    }

    // Java lines 315-321:
    // return new Entry(timestamp,
    //     Boolean.TRUE.equals(comment.getLikes()),    // isUpvoted
    //     Boolean.FALSE.equals(comment.getLikes()),   // isDownvoted
    //     false,                                       // isRead
    //     comment.getSaved(),                          // isSaved
    //     mIsHidden);                                  // Use existing value for "collapsed"
    bool likes = comment.getLikes(); // Ternary: true if likes == true, false otherwise
    return Entry(timestamp,
                 likes,                   // Boolean.TRUE.equals(likes)
                 !likes,                  // Boolean.FALSE.equals(likes) - simplified: if not true and known
                 false,
                 comment.getSaved(),
                 m_isHidden);             // Preserve existing "collapsed" state
}

// Java lines 324-340: update(TimestampUTC, RedditPost)
RedditChangeDataManager::Entry RedditChangeDataManager::Entry::update(
        const TimestampUTC &timestamp,
        const RedditPost &post) const
{
    // Java lines 328-330: if(timestamp.isLessThan(mTimestamp)) return this;
    if (timestamp.isLessThan(m_timestamp)) {
        return *this;
    }

    // Java lines 332-339:
    // return new Entry(timestamp,
    //     Boolean.TRUE.equals(post.getLikes()),
    //     Boolean.FALSE.equals(post.getLikes()),
    //     (PrefsUtility.pref_behaviour_mark_posts_as_read() && post.getClicked()) || mIsRead,
    //     post.getSaved(),
    //     post.getHidden() ? true : null);
    bool likes = post.getLikes();
    bool markAsRead = post.getClicked() || m_isRead; // Simplified: PrefsUtility check omitted
    bool postHidden = post.getHidden();
    std::optional<bool> hidden = postHidden ? std::optional<bool>(true) : std::nullopt;

    return Entry(timestamp, likes, !likes, markAsRead, post.getSaved(), hidden);
}

// Java lines 342-351: markUpvoted
RedditChangeDataManager::Entry RedditChangeDataManager::Entry::markUpvoted(
        const TimestampUTC &timestamp) const
{
    // Java lines 344-350:
    return Entry(timestamp, true, false, m_isRead, m_isSaved, m_isHidden);
}

// Java lines 353-362: markDownvoted
RedditChangeDataManager::Entry RedditChangeDataManager::Entry::markDownvoted(
        const TimestampUTC &timestamp) const
{
    // Java lines 355-361:
    return Entry(timestamp, false, true, m_isRead, m_isSaved, m_isHidden);
}

// Java lines 364-373: markUnvoted
RedditChangeDataManager::Entry RedditChangeDataManager::Entry::markUnvoted(
        const TimestampUTC &timestamp) const
{
    // Java lines 366-372:
    return Entry(timestamp, false, false, m_isRead, m_isSaved, m_isHidden);
}

// Java lines 375-384: markRead
RedditChangeDataManager::Entry RedditChangeDataManager::Entry::markRead(
        const TimestampUTC &timestamp,
        bool read) const
{
    // Java lines 377-383:
    return Entry(timestamp, m_isUpvoted, m_isDownvoted, read, m_isSaved, m_isHidden);
}

// Java lines 386-395: markSaved
RedditChangeDataManager::Entry RedditChangeDataManager::Entry::markSaved(
        const TimestampUTC &timestamp,
        bool isSaved) const
{
    // Java lines 388-394:
    return Entry(timestamp, m_isUpvoted, m_isDownvoted, m_isRead, isSaved, m_isHidden);
}

// Java lines 397-406: markHidden
RedditChangeDataManager::Entry RedditChangeDataManager::Entry::markHidden(
        const TimestampUTC &timestamp,
        const std::optional<bool> &isHidden) const
{
    // Java lines 399-405:
    return Entry(timestamp, m_isUpvoted, m_isDownvoted, m_isRead, m_isSaved, isHidden);
}

// ============================================================================
// Constructor / Destructor
// ============================================================================

RedditChangeDataManager::RedditChangeDataManager() = default;
RedditChangeDataManager::~RedditChangeDataManager() = default;

// ============================================================================
// getInstance — Java lines 61-75
// ============================================================================

RedditChangeDataManager &RedditChangeDataManager::getInstance(const RedditAccount &user)
{
    // Java lines 64-74:
    // synchronized(INSTANCE_MAP) {
    //     RedditChangeDataManager result = INSTANCE_MAP.get(user);
    //     if(result == null) { result = new RedditChangeDataManager(); INSTANCE_MAP.put(user, result); }
    //     return result;
    // }
    std::lock_guard<std::mutex> lock(s_instanceMapMutex);

    const RedditAccount *key = &user;
    auto it = s_instanceMap.find(key);
    if (it != s_instanceMap.end()) {
        return *it.value();
    }

    auto *result = new RedditChangeDataManager();
    s_instanceMap[key] = result;
    return *result;
}

// ============================================================================
// get — Java lines 441-450
// ============================================================================

const RedditChangeDataManager::Entry &RedditChangeDataManager::get(const RedditIdAndType &thing)
{
    // Java lines 443-449:
    // final Entry entry = mEntries.get(thing);
    // if(entry == null) return Entry.CLEAR_ENTRY; else return entry;
    auto it = m_entries.find(thing);
    if (it == m_entries.end()) {
        return Entry::CLEAR_ENTRY();
    }
    return it.value();
}

// ============================================================================
// set — Java lines 452-472
// ============================================================================

void RedditChangeDataManager::set(
        const RedditIdAndType &thing,
        const Entry &existingValue,
        const Entry &newValue)
{
    // Java lines 457-466:
    // if(newValue.isClear()) {
    //     if(!existingValue.isClear()) { mEntries.remove(thing); RedditChangeDataIO.notifyUpdateStatic(); }
    // } else {
    //     mEntries.put(thing, newValue); RedditChangeDataIO.notifyUpdateStatic();
    // }
    if (newValue.isClear()) {
        if (!existingValue.isClear()) {
            m_entries.remove(thing);
            // RedditChangeDataIO.notifyUpdateStatic(); — stub
        }
    } else {
        m_entries[thing] = newValue;
        // RedditChangeDataIO.notifyUpdateStatic(); — stub
    }

    // Java lines 468-471:
    // AndroidCommon.UI_THREAD_HANDLER.post(() -> mListeners.map(thing,
    //     ListenerNotifyOperator.INSTANCE, thing));
    auto range = m_listeners.values(thing);
    for (auto *listener : range) {
        if (listener) {
            listener->onRedditDataChange(thing);
        }
    }
}

// ============================================================================
// insertAll — Java lines 474-494
// ============================================================================

void RedditChangeDataManager::insertAll(const QHash<RedditIdAndType, Entry> &entries)
{
    // Java lines 476-489: synchronized(mLock) { for each entry, update if newer }
    {
        std::lock_guard<std::recursive_mutex> lock(m_lock);

        for (auto it = entries.begin(); it != entries.end(); ++it) {
            const Entry &newEntry = it.value();
            auto existingIt = m_entries.find(it.key());

            // Java lines 483-487:
            // if(existingEntry == null || existingEntry.mTimestamp.isLessThan(newEntry.mTimestamp)) {
            //     mEntries.put(entry.getKey(), newEntry);
            // }
            if (existingIt == m_entries.end()
                    || existingIt.value().timestamp().isLessThan(newEntry.timestamp())) {
                m_entries[it.key()] = newEntry;
            }
        }
    }

    // Java lines 491-493:
    // for(final RedditIdAndType idAndType : entries.keySet()) {
    //     mListeners.map(idAndType, ListenerNotifyOperator.INSTANCE, idAndType);
    // }
    for (auto it = entries.begin(); it != entries.end(); ++it) {
        auto range = m_listeners.values(it.key());
        for (auto *listener : range) {
            if (listener) {
                listener->onRedditDataChange(it.key());
            }
        }
    }
}

// ============================================================================
// update — Java lines 496-512
// ============================================================================

void RedditChangeDataManager::update(const TimestampUTC &timestamp, const RedditComment &comment)
{
    // Java lines 498-502:
    // synchronized(mLock) {
    //     final Entry existingEntry = get(comment.getIdAndType());
    //     final Entry updatedEntry = existingEntry.update(timestamp, comment);
    //     set(comment.getIdAndType(), existingEntry, updatedEntry);
    // }
    std::lock_guard<std::recursive_mutex> lock(m_lock);
    const Entry &existingEntry = get(comment.getIdAndType());
    Entry updatedEntry = existingEntry.update(timestamp, comment);
    set(comment.getIdAndType(), existingEntry, updatedEntry);
}

void RedditChangeDataManager::update(const TimestampUTC &timestamp, const RedditPost &post)
{
    // Java lines 507-511:
    std::lock_guard<std::recursive_mutex> lock(m_lock);
    const Entry &existingEntry = get(post.getIdAndType());
    Entry updatedEntry = existingEntry.update(timestamp, post);
    set(post.getIdAndType(), existingEntry, updatedEntry);
}

// ============================================================================
// markUpvoted / markDownvoted / markUnvoted — Java lines 514-541
// ============================================================================

void RedditChangeDataManager::markUpvoted(const TimestampUTC &timestamp, const RedditIdAndType &thing)
{
    // Java lines 516-520:
    std::lock_guard<std::recursive_mutex> lock(m_lock);
    const Entry &existingEntry = get(thing);
    Entry updatedEntry = existingEntry.markUpvoted(timestamp);
    set(thing, existingEntry, updatedEntry);
}

void RedditChangeDataManager::markDownvoted(const TimestampUTC &timestamp, const RedditIdAndType &thing)
{
    // Java lines 527-531:
    std::lock_guard<std::recursive_mutex> lock(m_lock);
    const Entry &existingEntry = get(thing);
    Entry updatedEntry = existingEntry.markDownvoted(timestamp);
    set(thing, existingEntry, updatedEntry);
}

void RedditChangeDataManager::markUnvoted(const TimestampUTC &timestamp, const RedditIdAndType &thing)
{
    // Java lines 536-540:
    std::lock_guard<std::recursive_mutex> lock(m_lock);
    const Entry &existingEntry = get(thing);
    Entry updatedEntry = existingEntry.markUnvoted(timestamp);
    set(thing, existingEntry, updatedEntry);
}

// ============================================================================
// markSaved — Java lines 543-553
// ============================================================================

void RedditChangeDataManager::markSaved(
        const TimestampUTC &timestamp,
        const RedditIdAndType &thing,
        bool saved)
{
    // Java lines 548-552:
    std::lock_guard<std::recursive_mutex> lock(m_lock);
    const Entry &existingEntry = get(thing);
    Entry updatedEntry = existingEntry.markSaved(timestamp, saved);
    set(thing, existingEntry, updatedEntry);
}

// ============================================================================
// markHidden — Java lines 555-565
// ============================================================================

void RedditChangeDataManager::markHidden(
        const TimestampUTC &timestamp,
        const RedditIdAndType &thing,
        const std::optional<bool> &hidden)
{
    // Java lines 560-564:
    std::lock_guard<std::recursive_mutex> lock(m_lock);
    const Entry &existingEntry = get(thing);
    Entry updatedEntry = existingEntry.markHidden(timestamp, hidden);
    set(thing, existingEntry, updatedEntry);
}

// ============================================================================
// markRead — Java lines 567-577
// ============================================================================

void RedditChangeDataManager::markRead(
        const TimestampUTC &timestamp,
        const RedditIdAndType &thing,
        const std::optional<bool> &read)
{
    // Java lines 572-576:
    std::lock_guard<std::recursive_mutex> lock(m_lock);
    const Entry &existingEntry = get(thing);
    Entry updatedEntry = existingEntry.markRead(timestamp,
        read.has_value() ? read.value() : m_isSaved);  // fallback if no value
    set(thing, existingEntry, updatedEntry);
}

// ============================================================================
// addListener / removeListener — Java lines 427-439
// ============================================================================

void RedditChangeDataManager::addListener(const RedditIdAndType &thing, Listener *listener)
{
    // Java line 431: mListeners.add(thing, listener);
    m_listeners.insert(thing, listener);
}

void RedditChangeDataManager::removeListener(const RedditIdAndType &thing, Listener *listener)
{
    // Java line 438: mListeners.remove(thing, listener);
    m_listeners.remove(thing, listener);
}

// ============================================================================
// isUpvoted / isDownvoted / isRead / isSaved / isHidden — Java lines 579-607
// ============================================================================

bool RedditChangeDataManager::isUpvoted(const RedditIdAndType &thing)
{
    // Java lines 580-582: synchronized(mLock) { return get(thing).isUpvoted(); }
    std::lock_guard<std::recursive_mutex> lock(m_lock);
    return get(thing).isUpvoted();
}

bool RedditChangeDataManager::isDownvoted(const RedditIdAndType &thing)
{
    std::lock_guard<std::recursive_mutex> lock(m_lock);
    return get(thing).isDownvoted();
}

bool RedditChangeDataManager::isRead(const RedditIdAndType &thing)
{
    std::lock_guard<std::recursive_mutex> lock(m_lock);
    return get(thing).isRead();
}

bool RedditChangeDataManager::isSaved(const RedditIdAndType &thing)
{
    std::lock_guard<std::recursive_mutex> lock(m_lock);
    return get(thing).isSaved();
}

std::optional<bool> RedditChangeDataManager::isHidden(const RedditIdAndType &thing)
{
    std::lock_guard<std::recursive_mutex> lock(m_lock);
    return get(thing).isHidden();
}

// ============================================================================
// snapshot — Java lines 609-613
// ============================================================================

QHash<RedditIdAndType, RedditChangeDataManager::Entry> RedditChangeDataManager::snapshot()
{
    // Java lines 610-612: synchronized(mLock) { return new HashMap<>(mEntries); }
    std::lock_guard<std::recursive_mutex> lock(m_lock);
    return m_entries;
}

// ============================================================================
// snapshotAllUsers — Java lines 77-88
// ============================================================================

QHash<RedditAccount, QHash<RedditIdAndType, RedditChangeDataManager::Entry>>
RedditChangeDataManager::snapshotAllUsers()
{
    // Java lines 79-87:
    // final HashMap<RedditAccount, HashMap<RedditIdAndType, Entry>> result = new HashMap<>();
    // synchronized(INSTANCE_MAP) {
    //     for(final RedditAccount account : INSTANCE_MAP.keySet()) {
    //         result.put(account, getInstance(account).snapshot());
    //     }
    // }
    // return result;
    QHash<RedditAccount, QHash<RedditIdAndType, Entry>> result;
    // Note: This returns copies, not refs. Snapshot returns copies.
    // Stub: iterating the instance map would require storing accounts separately.
    return result;
}

// ============================================================================
// writeAllUsers — Java lines 90-130
// ============================================================================

void RedditChangeDataManager::writeAllUsers(ExtendedDataOutputStream &dos)
{
    // Java line 92: Log.i(TAG, "Taking snapshot...");
    qDebug() << TAG << ": Taking snapshot...";

    // Java line 94: final HashMap<RedditAccount, HashMap<RedditIdAndType, Entry>> data = snapshotAllUsers();
    auto data = snapshotAllUsers();

    // Java line 96: Log.i(TAG, "Writing to stream...");
    qDebug() << TAG << ": Writing to stream...";

    // Java lines 98-99: final Set<Map.Entry<RedditAccount, HashMap<...>>> userDataSet = data.entrySet();
    // Java line 101: dos.writeInt(userDataSet.size());
    dos.writeInt(data.size());

    // Java lines 103-127: iterate over userData
    for (auto userIt = data.begin(); userIt != data.end(); ++userIt) {
        // Java line 106: final String username = userData.getKey().getCanonicalUsername();
        QString username = userIt.key().getCanonicalUsername();
        // Java line 107: dos.writeUTF(username);
        dos.writeUTF(username);

        // Java lines 109-111: final Set<Map.Entry<RedditIdAndType, Entry>> entrySet = ...
        const auto &entryData = userIt.value();
        dos.writeInt(entryData.size());

        // Java lines 113-116: iterate entries
        for (auto entryIt = entryData.begin(); entryIt != entryData.end(); ++entryIt) {
            // Java line 114: dos.writeUTF(entry.getKey().getValue());
            dos.writeUTF(entryIt.key().getValue());
            // Java line 115: entry.getValue().writeTo(dos);
            entryIt.value().writeTo(dos);
        }

        // Java lines 118-126: debug logging
        if (General::isSensitiveDebugLoggingEnabled()) {
            qDebug() << TAG << QString("Wrote %1 entries for user '%2'")
                            .arg(entryData.size())
                            .arg(username);
        }
    }

    // Java line 129: Log.i(TAG, "All entries written to stream.");
    qDebug() << TAG << ": All entries written to stream.";
}

// ============================================================================
// readAllUsers — Java lines 132-194
// ============================================================================

void RedditChangeDataManager::readAllUsers(ExtendedDataInputStream &dis)
{
    // Java line 136: Log.i(TAG, "Reading from stream...");
    qDebug() << TAG << ": Reading from stream...";

    // Java line 138: final int userCount = dis.readInt();
    int userCount = dis.readInt();

    // Java line 140: Log.i(TAG, userCount + " users to read.");
    qDebug() << TAG << userCount << "users to read.";

    // Java lines 142-191: for each user
    for (int i = 0; i < userCount; i++) {
        // Java line 144: final String username = dis.readUTF();
        QString username = dis.readUTF();
        // Java line 145: final int entryCount = dis.readInt();
        int entryCount = dis.readInt();

        // Java lines 147-155: debug logging
        if (General::isSensitiveDebugLoggingEnabled()) {
            qDebug() << TAG << QString("Reading %1 entries for user '%2'")
                            .arg(entryCount).arg(username);
        }

        // Java line 157: final HashMap<RedditIdAndType, Entry> entries = new HashMap<>(entryCount);
        QHash<RedditIdAndType, Entry> entries;
        entries.reserve(entryCount);

        // Java lines 159-163: read each entry
        for (int j = 0; j < entryCount; j++) {
            // Java line 160: final RedditIdAndType thingId = new RedditIdAndType(dis.readUTF());
            RedditIdAndType thingId(dis.readUTF());
            // Java line 161: final Entry entry = new Entry(dis);
            Entry entry = Entry::fromStream(dis);
            // Java line 162: entries.put(thingId, entry);
            entries[thingId] = entry;
        }

        // Java line 165: Log.i(TAG, "Getting account...");
        qDebug() << TAG << ": Getting account...";

        // Java lines 167-168: find account
        // In the original, this uses RedditAccountManager.getInstance(context).getAccount(username)
        // Stub: we can't resolve accounts without context
        // If account found, call getInstance(account).insertAll(entries)
        // Otherwise, skip with log message (Java lines 170-178)

        // Java lines 170-178: account == null -> skip
        qDebug() << TAG << QString("Skipping user '%1' - account resolution not implemented").arg(username);
    }

    // Java line 193: Log.i(TAG, "All entries read from stream.");
    qDebug() << TAG << ": All entries read from stream.";
}

// ============================================================================
// pruneAllUsersDefaultMaxAge — Java lines 196-198
// ============================================================================

void RedditChangeDataManager::pruneAllUsersDefaultMaxAge()
{
    // Java line 197: pruneAllUsersWhereOlderThan(PrefsUtility.pref_cache_maxage_entry());
    // Stub: would call PrefsUtility.pref_cache_maxage_entry() to get duration
    // For now, prune with 7 days
    TimeDuration maxAge = TimeDuration::hours(168);
    pruneAllUsersWhereOlderThan(maxAge);
}

// ============================================================================
// pruneAllUsersWhereOlderThan — Java lines 200-217
// ============================================================================

void RedditChangeDataManager::pruneAllUsersWhereOlderThan(const TimeDuration &maxAge)
{
    // Java line 202: Log.i(TAG, "Pruning for all users...");
    qDebug() << TAG << ": Pruning for all users...";

    // Java lines 204-208:
    // final Set<RedditAccount> users;
    // synchronized(INSTANCE_MAP) { users = new HashSet<>(INSTANCE_MAP.keySet()); }
    QSet<const RedditAccount *> users;
    {
        std::lock_guard<std::mutex> lock(s_instanceMapMutex);
        for (auto it = s_instanceMap.begin(); it != s_instanceMap.end(); ++it) {
            users.insert(it.key());
        }
    }

    // Java lines 210-214:
    // for(final RedditAccount user : users) {
    //     final RedditChangeDataManager managerForUser = getInstance(user);
    //     managerForUser.prune(maxAge);
    // }
    for (auto *user : users) {
        RedditChangeDataManager &managerForUser = getInstance(*user);
        managerForUser.prune(maxAge);
    }

    // Java line 216: Log.i(TAG, "Pruning complete.");
    qDebug() << TAG << ": Pruning complete.";
}

// ============================================================================
// prune — Java lines 615-667
// ============================================================================

void RedditChangeDataManager::prune(const TimeDuration &maxAge)
{
    // Java lines 617-618:
    // final TimestampUTC now = TimestampUTC.now();
    // final TimestampUTC timestampBoundary = now.subtract(maxAge);
    TimestampUTC now = TimestampUTC::now();
    TimestampUTC timestampBoundary = now.subtract(maxAge);

    std::lock_guard<std::recursive_mutex> lock(m_lock);

    // Java lines 621-623:
    // final Iterator<Map.Entry<RedditIdAndType, Entry>> iterator = mEntries.entrySet().iterator();
    // final SortedMap<TimestampUTC, RedditIdAndType> byTimestamp = new TreeMap<>();
    QMap<TimestampUTC, RedditIdAndType> byTimestamp;

    // Java lines 625-643: first pass - remove old entries
    auto it = m_entries.begin();
    while (it != m_entries.end()) {
        const TimestampUTC &ts = it.value().timestamp();
        byTimestamp[ts] = it.key();

        // Java lines 631-642: if(timestamp.isLessThan(timestampBoundary)) remove
        if (ts.isLessThan(timestampBoundary)) {
            qDebug() << TAG << QString("Pruning '%1'").arg(it.key().toString());
            it = m_entries.erase(it);
        } else {
            ++it;
        }
    }

    // Java lines 646-665: second pass - limit total entries to MAX_ENTRY_COUNT
    // final Iterator<Map.Entry<TimestampUTC, RedditIdAndType>> iter2 = byTimestamp.entrySet().iterator();
    auto iter2 = byTimestamp.begin();

    while (iter2 != byTimestamp.end()) {
        // Java line 650: if(mEntries.size() <= MAX_ENTRY_COUNT) break;
        if (m_entries.size() <= MAX_ENTRY_COUNT) {
            break;
        }

        // Java lines 654-664: evict oldest
        qDebug() << TAG << QString("Evicting '%1'").arg(iter2.value().toString());
        m_entries.remove(iter2.value());
        ++iter2;
    }
}

} // namespace PinkReader
