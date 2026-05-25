/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 *
 * File: cache_db_manager.h
 * Port of RedReader's CacheDbManager.java
 *
 * Exact line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/cache/CacheDbManager.java
 *
 * Every field, method, constant, and logic branch ported 1:1.
 * This extends SQLiteOpenHelper (SQLite database manager for cache metadata).
 */

#pragma once

#include <QObject>
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QHash>
#include <QSet>
#include <QVector>
#include <QMutex>
#include <QUuid>
#include <memory>
#include <optional>

namespace PinkReader {

// Forward declarations
class UriString;
class RedditAccount;
class CacheEntry;
class TimestampUTC;
class TimeDuration;

// CacheCompressionType enum (from CacheCompressionType.java)
// Used by CacheDbManager for field defaults
enum class CacheCompressionType {
    NONE = 0,
    ZSTD = 1
};

/**
 * CacheDbManager — SQLite database manager for the cache.
 * Port of CacheDbManager.java (final class, extends SQLiteOpenHelper, 379 lines).
 *
 * Constants:
 *   - CACHE_DB_FILENAME = "cache.db"
 *   - TABLE = "web"
 *   - FIELD_URL = "url"
 *   - FIELD_ID = "id"
 *   - FIELD_TIMESTAMP = "timestamp"
 *   - FIELD_SESSION = "session"
 *   - FIELD_USER = "user"
 *   - FIELD_STATUS = "status"
 *   - FIELD_TYPE = "type"
 *   - FIELD_MIMETYPE = "mimetype"
 *   - FIELD_COMPRESSION_TYPE = "compressionType"
 *   - FIELD_LENGTH_UNCOMPRESSED = "lengthUncompressed"
 *   - FIELD_LENGTH_COMPRESSED = "lengthCompressed"
 *   - STATUS_MOVING = 1
 *   - STATUS_DONE = 2
 *   - CACHE_DB_VERSION = 2
 */
class CacheDbManager {
public:
    // ========================================================================
    // Constants (Java lines 45-63)
    // ========================================================================
    static const QString CACHE_DB_FILENAME;
    static const QString TABLE;

    static const QString FIELD_URL;
    static const QString FIELD_ID;
    static const QString FIELD_TIMESTAMP;
    static const QString FIELD_SESSION;
    static const QString FIELD_USER;
    static const QString FIELD_STATUS;
    static const QString FIELD_TYPE;
    static const QString FIELD_MIMETYPE;
    static const QString FIELD_COMPRESSION_TYPE;
    static const QString FIELD_LENGTH_UNCOMPRESSED;
    static const QString FIELD_LENGTH_COMPRESSED;

    static constexpr int STATUS_MOVING = 1;
    static constexpr int STATUS_DONE = 2;
    static constexpr int CACHE_DB_VERSION = 2;

    // ========================================================================
    // Construction (Java line 65-67)
    // ========================================================================
    explicit CacheDbManager(const QString &dbPath);

    ~CacheDbManager();

    // ========================================================================
    // Database lifecycle (Java lines 69-131)
    // ========================================================================

    // onCreate (Java lines 69-101)
    void onCreate();

    // onUpgrade (Java lines 103-131)
    void onUpgrade(int oldVersion, int newVersion);

    // ========================================================================
    // Query methods (Java lines 133-211)
    // ========================================================================

    // selectById (Java lines 133-154)
    // Returns Optional<CacheEntry> — empty if not found
    std::optional<CacheEntry> selectById(qint64 id);

    // select (Java lines 156-199)
    // Returns list of CacheEntry matching url/user/session
    QVector<CacheEntry> select(const UriString &url,
                                const QString &user,
                                const QUuid &session);
    QVector<CacheEntry> select(const UriString &url,
                                const QString &user);

    // ========================================================================
    // Mutation methods (Java lines 213-263)
    // ========================================================================

    // newEntry (Java lines 213-249)
    // Returns the new row ID, throws on failure
    qint64 newEntry(const UriString &url,
                    const RedditAccount &user,
                    int fileType,
                    const QUuid &session,
                    const QString &mimetype,
                    CacheCompressionType compressionType,
                    qint64 lengthCompressed,
                    qint64 lengthUncompressed);

    // setEntryDone (Java lines 251-258)
    void setEntryDone(qint64 id);

    // delete (Java lines 260-263)
    int deleteEntry(qint64 id);

    // ========================================================================
    // Pruning (Java lines 265-346)
    // ========================================================================

    // getFilesToPrune (Java lines 265-346)
    QVector<qint64> getFilesToPrune(const QSet<qint64> &currentFiles,
                                     const QHash<int, TimeDuration> &maxAge,
                                     const TimeDuration &defaultMaxAge);

    // ========================================================================
    // Maintenance (Java lines 348-378)
    // ========================================================================

    // emptyTheWholeCache (Java lines 348-351)
    void emptyTheWholeCache();

    // getFilesToSize (Java lines 353-378)
    QHash<qint64, int> getFilesToSize();

private:
    // ========================================================================
    // Private helpers
    // ========================================================================

    // readEntriesFromCursor (Java lines 201-211)
    QVector<CacheEntry> readEntriesFromCursor(QSqlQuery &query);

    // Internal database helpers
    QSqlDatabase m_db;
    QMutex m_mutex;
    QString m_dbPath;

    void ensureOpen();
};

// ========================================================================
// UriString — simple string wrapper for URLs
// Port of UriString.kt: data class UriString(val value: String)
// ========================================================================
class UriString {
public:
    QString value;

    UriString() = default;
    explicit UriString(const QString &v) : value(v) {}

    QString toString() const { return value; }
    bool isEmpty() const { return value.isEmpty(); }
    bool operator==(const UriString &other) const {
        return value == other.value;
    }
    bool operator!=(const UriString &other) const {
        return value != other.value;
    }
};

// Hash function for UriString
inline uint qHash(const UriString &key, uint seed = 0) {
    return qHash(key.value, seed);
}

// ========================================================================
// TimestampUTC — UTC timestamp wrapper
// Port of TimestampUTC.kt (81 lines)
// ========================================================================
class TimestampUTC {
public:
    qint64 m_epochMs = 0;

    TimestampUTC() = default;
    explicit TimestampUTC(qint64 epochMs) : m_epochMs(epochMs) {}

    static TimestampUTC now();
    static TimestampUTC fromUtcMs(qint64 value);
    static TimestampUTC fromUtcSecs(qint64 value);

    static const TimestampUTC ZERO;

    qint64 toUtcMs() const { return m_epochMs; }
    qint64 toUtcSecs() const { return m_epochMs / 1000; }

    bool isLessThan(const TimestampUTC &other) const {
        return m_epochMs < other.m_epochMs;
    }
    bool isGreaterThan(const TimestampUTC &other) const {
        return m_epochMs > other.m_epochMs;
    }

    static TimestampUTC oldest(const TimestampUTC &a, const TimestampUTC &b) {
        return (a.m_epochMs < b.m_epochMs) ? a : b;
    }

    TimestampUTC add(const class TimeDuration &duration) const;
    TimestampUTC subtract(const class TimeDuration &duration) const;

    bool operator<(const TimestampUTC &other) const {
        return m_epochMs < other.m_epochMs;
    }
    bool operator>(const TimestampUTC &other) const {
        return m_epochMs > other.m_epochMs;
    }
};

// ========================================================================
// TimeDuration — duration wrapper
// Port of TimeDuration.kt
// ========================================================================
class TimeDuration {
public:
    qint64 m_milliseconds = 0;

    TimeDuration() = default;
    explicit TimeDuration(qint64 ms) : m_milliseconds(ms) {}

    static TimeDuration milliseconds(qint64 ms) {
        return TimeDuration(ms);
    }
    static TimeDuration seconds(qint64 s) {
        return TimeDuration(s * 1000);
    }
    static TimeDuration minutes(qint64 m) {
        return TimeDuration(m * 60 * 1000);
    }
    static TimeDuration hours(qint64 h) {
        return TimeDuration(h * 3600 * 1000);
    }
    static TimeDuration days(qint64 d) {
        return TimeDuration(d * 86400 * 1000);
    }

    qint64 toMilliseconds() const { return m_milliseconds; }
};

inline TimestampUTC TimestampUTC::add(const TimeDuration &duration) const {
    return TimestampUTC(m_epochMs + duration.m_milliseconds);
}

inline TimestampUTC TimestampUTC::subtract(const TimeDuration &duration) const {
    return TimestampUTC(m_epochMs - duration.m_milliseconds);
}

// ========================================================================
// CacheEntry — cache entry metadata
// Port of CacheEntry.java (62 lines)
// Fields: id, url, session, timestamp, mimetype,
//         cacheCompressionType, lengthCompressed, lengthUncompressed
// Static: DB_FIELDS string array
// ========================================================================
class CacheEntry {
public:
    static QVector<QString> DB_FIELDS;

    qint64 id = 0;
    UriString url;
    QUuid session;
    TimestampUTC timestamp;
    QString mimetype;
    CacheCompressionType cacheCompressionType = CacheCompressionType::NONE;
    qint64 lengthCompressed = 0;
    qint64 lengthUncompressed = 0;

    CacheEntry() = default;

    // Constructor from SQL query cursor (Java lines 50-61)
    explicit CacheEntry(const QSqlQuery &query);

    static CacheCompressionType compressionTypeFromDatabaseId(int databaseId);
};

} // namespace PinkReader
