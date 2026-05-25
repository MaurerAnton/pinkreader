/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 *
 * File: cache_db_manager.cpp
 * Port of RedReader's CacheDbManager.java
 */

#include "cache/cache_db_manager.h"
#include "accounts/reddit_account.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <QDebug>
#include <QUuid>
#include <stdexcept>

namespace PinkReader {

// ============================================================================
// Constants
// ============================================================================
const QString CacheDbManager::CACHE_DB_FILENAME = QStringLiteral("cache.db");
const QString CacheDbManager::TABLE = QStringLiteral("web");

const QString CacheDbManager::FIELD_URL = QStringLiteral("url");
const QString CacheDbManager::FIELD_ID = QStringLiteral("id");
const QString CacheDbManager::FIELD_TIMESTAMP = QStringLiteral("timestamp");
const QString CacheDbManager::FIELD_SESSION = QStringLiteral("session");
const QString CacheDbManager::FIELD_USER = QStringLiteral("user");
const QString CacheDbManager::FIELD_STATUS = QStringLiteral("status");
const QString CacheDbManager::FIELD_TYPE = QStringLiteral("type");
const QString CacheDbManager::FIELD_MIMETYPE = QStringLiteral("mimetype");
const QString CacheDbManager::FIELD_COMPRESSION_TYPE =
        QStringLiteral("compressionType");
const QString CacheDbManager::FIELD_LENGTH_UNCOMPRESSED =
        QStringLiteral("lengthUncompressed");
const QString CacheDbManager::FIELD_LENGTH_COMPRESSED =
        QStringLiteral("lengthCompressed");

// ============================================================================
// TimestampUTC static methods
// ============================================================================
const TimestampUTC TimestampUTC::ZERO(0);

TimestampUTC TimestampUTC::now() {
    return TimestampUTC(QDateTime::currentMSecsSinceEpoch());
}

TimestampUTC TimestampUTC::fromUtcMs(qint64 value) {
    return TimestampUTC(value);
}

TimestampUTC TimestampUTC::fromUtcSecs(qint64 value) {
    return TimestampUTC(value * 1000);
}

// ============================================================================
// CacheEntry static DB_FIELDS (Java lines 31-39)
// ============================================================================
QVector<QString> CacheEntry::DB_FIELDS = {
    CacheDbManager::FIELD_ID,
    CacheDbManager::FIELD_URL,
    CacheDbManager::FIELD_SESSION,
    CacheDbManager::FIELD_TIMESTAMP,
    CacheDbManager::FIELD_MIMETYPE,
    CacheDbManager::FIELD_COMPRESSION_TYPE,
    CacheDbManager::FIELD_LENGTH_COMPRESSED,
    CacheDbManager::FIELD_LENGTH_UNCOMPRESSED
};

// CacheEntry constructor from QSqlQuery (Java lines 50-61)
CacheEntry::CacheEntry(const QSqlQuery &query) {
    id = query.value(0).toLongLong();
    url = UriString(query.value(1).toString());
    session = QUuid::fromString(query.value(2).toString());
    timestamp = TimestampUTC::fromUtcMs(query.value(3).toLongLong());
    mimetype = query.value(4).toString();
    cacheCompressionType = compressionTypeFromDatabaseId(
            query.value(5).toInt());
    lengthCompressed = query.value(6).toLongLong();
    lengthUncompressed = query.value(7).toLongLong();
}

CacheCompressionType CacheEntry::compressionTypeFromDatabaseId(
        int databaseId) {
    if (databaseId == static_cast<int>(CacheCompressionType::NONE)) {
        return CacheCompressionType::NONE;
    }
    if (databaseId == static_cast<int>(CacheCompressionType::ZSTD)) {
        return CacheCompressionType::ZSTD;
    }
    throw std::runtime_error(
        std::string("Unknown compression type ") + std::to_string(databaseId));
}

// ============================================================================
// CacheDbManager construction (Java lines 65-67)
// ============================================================================
CacheDbManager::CacheDbManager(const QString &dbPath)
    : m_dbPath(dbPath) {
    m_db = QSqlDatabase::addDatabase(
        QStringLiteral("QSQLITE"),
        QStringLiteral("cache_db_connection"));
    m_db.setDatabaseName(dbPath);
}

CacheDbManager::~CacheDbManager() {
    if (m_db.isOpen()) {
        m_db.close();
    }
}

void CacheDbManager::ensureOpen() {
    if (!m_db.isOpen()) {
        if (!m_db.open()) {
            throw std::runtime_error(
                std::string("Failed to open cache DB: ")
                + m_db.lastError().text().toStdString());
        }
    }
}

// ============================================================================
// onCreate (Java lines 69-101)
// ============================================================================
void CacheDbManager::onCreate() {
    QMutexLocker locker(&m_mutex);
    ensureOpen();

    QSqlQuery query(m_db);
    const QString queryString = QString(
        "CREATE TABLE %1 ("
        "%2 INTEGER PRIMARY KEY AUTOINCREMENT,"
        "%3 TEXT NOT NULL,"
        "%4 TEXT NOT NULL,"
        "%5 TEXT NOT NULL,"
        "%6 INTEGER,"
        "%7 INTEGER,"
        "%8 INTEGER,"
        "%9 TEXT,"
        "%10 INTEGER,"
        "%11 INTEGER,"
        "%12 INTEGER,"
        "UNIQUE (%4, %3, %5) ON CONFLICT REPLACE)")
        .arg(TABLE,
             FIELD_ID,
             FIELD_URL,
             FIELD_USER,
             FIELD_SESSION,
             FIELD_TIMESTAMP,
             FIELD_STATUS,
             FIELD_TYPE,
             FIELD_MIMETYPE,
             FIELD_COMPRESSION_TYPE,
             FIELD_LENGTH_COMPRESSED,
             FIELD_LENGTH_UNCOMPRESSED);

    if (!query.exec(queryString)) {
        throw std::runtime_error(
            std::string("CacheDbManager.onCreate failed: ")
            + query.lastError().text().toStdString());
    }
}

// ============================================================================
// onUpgrade (Java lines 103-131)
// ============================================================================
void CacheDbManager::onUpgrade(int oldVersion, int newVersion) {
    QMutexLocker locker(&m_mutex);
    ensureOpen();

    if (oldVersion < 2) {
        QSqlQuery query(m_db);

        // ALTER TABLE web ADD COLUMN compressionType INTEGER NOT NULL DEFAULT 0
        QString sql = QString(
            "ALTER TABLE %1 ADD COLUMN %2 INTEGER NOT NULL DEFAULT %3")
            .arg(TABLE,
                 FIELD_COMPRESSION_TYPE,
                 QString::number(static_cast<int>(CacheCompressionType::NONE)));
        if (!query.exec(sql)) {
            qWarning() << "CacheDbManager.onUpgrade failed (compressionType):"
                       << query.lastError().text();
        }

        // ALTER TABLE web ADD COLUMN lengthUncompressed INTEGER NOT NULL DEFAULT 0
        sql = QString(
            "ALTER TABLE %1 ADD COLUMN %2 INTEGER NOT NULL DEFAULT 0")
            .arg(TABLE, FIELD_LENGTH_UNCOMPRESSED);
        if (!query.exec(sql)) {
            qWarning() << "CacheDbManager.onUpgrade failed (lengthUncompressed):"
                       << query.lastError().text();
        }

        // ALTER TABLE web ADD COLUMN lengthCompressed INTEGER NOT NULL DEFAULT 0
        sql = QString(
            "ALTER TABLE %1 ADD COLUMN %2 INTEGER NOT NULL DEFAULT 0")
            .arg(TABLE, FIELD_LENGTH_COMPRESSED);
        if (!query.exec(sql)) {
            qWarning() << "CacheDbManager.onUpgrade failed (lengthCompressed):"
                       << query.lastError().text();
        }
    }
}

// ============================================================================
// selectById (Java lines 133-154)
// ============================================================================
std::optional<CacheEntry> CacheDbManager::selectById(qint64 id) {
    QMutexLocker locker(&m_mutex);
    ensureOpen();

    QSqlQuery query(m_db);
    query.prepare(QString("SELECT * FROM %1 WHERE %2=? ORDER BY %3 DESC")
                      .arg(TABLE, FIELD_ID, FIELD_TIMESTAMP));
    query.addBindValue(id);

    if (!query.exec()) {
        qWarning() << "CacheDbManager.selectById failed:"
                   << query.lastError().text();
        return std::nullopt;
    }

    QVector<CacheEntry> entries = readEntriesFromCursor(query);

    if (entries.isEmpty()) {
        return std::nullopt;  // Optional.empty()
    }

    return entries.first();  // Optional.of(entries.get(0))
}

// ============================================================================
// readEntriesFromCursor (Java lines 201-211)
// ============================================================================
QVector<CacheEntry> CacheDbManager::readEntriesFromCursor(QSqlQuery &query) {
    QVector<CacheEntry> result;

    while (query.next()) {
        result.append(CacheEntry(query));
    }

    return result;
}

// ============================================================================
// select (with session — Java lines 156-199)
// ============================================================================
QVector<CacheEntry> CacheDbManager::select(
        const UriString &url,
        const QString &user,
        const QUuid &session) {
    QMutexLocker locker(&m_mutex);
    ensureOpen();

    QSqlQuery query(m_db);
    query.prepare(QString(
        "SELECT * FROM %1 WHERE %2=? AND %3=? AND %4=? AND %5=? "
        "ORDER BY %6 DESC")
        .arg(TABLE,
             FIELD_STATUS,
             FIELD_URL,
             FIELD_USER,
             FIELD_SESSION,
             FIELD_TIMESTAMP));
    query.addBindValue(STATUS_DONE);
    query.addBindValue(url.value);
    query.addBindValue(user);
    query.addBindValue(session.toString(QUuid::WithoutBraces));

    if (!query.exec()) {
        qWarning() << "CacheDbManager.select (with session) failed:"
                   << query.lastError().text();
        return {};
    }

    return readEntriesFromCursor(query);
}

QVector<CacheEntry> CacheDbManager::select(
        const UriString &url,
        const QString &user) {
    QMutexLocker locker(&m_mutex);
    ensureOpen();

    QSqlQuery query(m_db);
    query.prepare(QString(
        "SELECT * FROM %1 WHERE %2=? AND %3=? AND %4=? "
        "ORDER BY %5 DESC")
        .arg(TABLE,
             FIELD_STATUS,
             FIELD_URL,
             FIELD_USER,
             FIELD_TIMESTAMP));
    query.addBindValue(STATUS_DONE);
    query.addBindValue(url.value);
    query.addBindValue(user);

    if (!query.exec()) {
        qWarning() << "CacheDbManager.select (without session) failed:"
                   << query.lastError().text();
        return {};
    }

    return readEntriesFromCursor(query);
}

// ============================================================================
// newEntry (Java lines 213-249)
// ============================================================================
qint64 CacheDbManager::newEntry(
        const UriString &url,
        const RedditAccount &user,
        int fileType,
        const QUuid &session,
        const QString &mimetype,
        CacheCompressionType compressionType,
        qint64 lengthCompressed,
        qint64 lengthUncompressed) {
    if (session.isNull()) {
        throw std::runtime_error("No session to write");
    }

    QMutexLocker locker(&m_mutex);
    ensureOpen();

    QSqlQuery query(m_db);
    query.prepare(QString(
        "INSERT INTO %1 (%2,%3,%4,%5,%6,%7,%8,%9,%10,%11) "
        "VALUES (?,?,?,?,?,?,?,?,?,?)")
        .arg(TABLE,
             FIELD_URL,
             FIELD_USER,
             FIELD_SESSION,
             FIELD_TYPE,
             FIELD_STATUS,
             FIELD_TIMESTAMP,
             FIELD_MIMETYPE,
             FIELD_COMPRESSION_TYPE,
             FIELD_LENGTH_COMPRESSED,
             FIELD_LENGTH_UNCOMPRESSED));

    query.addBindValue(url.value);
    query.addBindValue(user.username);
    query.addBindValue(session.toString(QUuid::WithoutBraces));
    query.addBindValue(fileType);
    query.addBindValue(STATUS_MOVING);
    query.addBindValue(TimestampUTC::now().toUtcMs());
    query.addBindValue(mimetype);
    query.addBindValue(static_cast<int>(compressionType));
    query.addBindValue(lengthCompressed);
    query.addBindValue(lengthUncompressed);

    if (!query.exec()) {
        throw std::runtime_error(
            std::string("DB insert failed: ")
            + query.lastError().text().toStdString());
    }

    qint64 result = query.lastInsertId().toLongLong();

    if (result < 0) {
        throw std::runtime_error("DB insert failed (negative ID)");
    }

    return result;
}

// ============================================================================
// setEntryDone (Java lines 251-258)
// ============================================================================
void CacheDbManager::setEntryDone(qint64 id) {
    QMutexLocker locker(&m_mutex);
    ensureOpen();

    QSqlQuery query(m_db);
    query.prepare(QString("UPDATE %1 SET %2=? WHERE %3=?")
                      .arg(TABLE, FIELD_STATUS, FIELD_ID));
    query.addBindValue(STATUS_DONE);
    query.addBindValue(id);

    if (!query.exec()) {
        qWarning() << "CacheDbManager.setEntryDone failed:"
                   << query.lastError().text();
    }
}

// ============================================================================
// delete (Java lines 260-263)
// ============================================================================
int CacheDbManager::deleteEntry(qint64 id) {
    QMutexLocker locker(&m_mutex);
    ensureOpen();

    QSqlQuery query(m_db);
    query.prepare(QString("DELETE FROM %1 WHERE %2=?")
                      .arg(TABLE, FIELD_ID));
    query.addBindValue(id);

    if (!query.exec()) {
        qWarning() << "CacheDbManager.delete failed:"
                   << query.lastError().text();
        return 0;
    }

    return query.numRowsAffected();
}

// ============================================================================
// getFilesToPrune (Java lines 265-346)
// ============================================================================
QVector<qint64> CacheDbManager::getFilesToPrune(
        const QSet<qint64> &currentFiles,
        const QHash<int, TimeDuration> &maxAge,
        const TimeDuration &defaultMaxAge) {
    QMutexLocker locker(&m_mutex);
    ensureOpen();

    const TimestampUTC currentTime = TimestampUTC::now();

    QSqlQuery query(m_db);
    query.prepare(QString("SELECT %1,%2,%3 FROM %4")
                      .arg(FIELD_ID, FIELD_TIMESTAMP, FIELD_TYPE, TABLE));

    if (!query.exec()) {
        qWarning() << "CacheDbManager.getFilesToPrune query failed:"
                   << query.lastError().text();
        return {};
    }

    QSet<qint64> currentEntries;
    QVector<qint64> entriesToDelete;
    QVector<qint64> filesToDelete;
    filesToDelete.reserve(32);

    while (query.next()) {
        const qint64 id = query.value(0).toLongLong();
        const TimestampUTC timestamp =
                TimestampUTC::fromUtcMs(query.value(1).toLongLong());
        const int type = query.value(2).toInt();

        TimestampUTC pruneIfBeforeMs;

        if (maxAge.contains(type)) {
            pruneIfBeforeMs = currentTime.subtract(maxAge[type]);
        } else {
            qWarning() << "RR DEBUG cache"
                       << "Using default age! Filetype" << type;
            pruneIfBeforeMs = currentTime.subtract(defaultMaxAge);
        }

        if (!currentFiles.contains(id)) {
            entriesToDelete.append(id);
        } else if (timestamp.isLessThan(pruneIfBeforeMs)) {
            entriesToDelete.append(id);
            filesToDelete.append(id);
        } else {
            currentEntries.insert(id);
        }
    }

    // Check for files on disk not in database
    for (const qint64 id : currentFiles) {
        if (!currentEntries.contains(id)) {
            filesToDelete.append(id);
        }
    }

    // Delete entries from database
    if (!entriesToDelete.isEmpty()) {
        QString deleteQuery = QString("DELETE FROM %1 WHERE %2 IN (")
                                  .arg(TABLE, FIELD_ID);

        // Append first ID
        deleteQuery += QString::number(entriesToDelete.takeLast());

        for (const qint64 id : entriesToDelete) {
            deleteQuery += QStringLiteral(",") + QString::number(id);
            if (deleteQuery.length() > 512 * 1024) {
                break;
            }
        }

        deleteQuery += QStringLiteral(")");

        QSqlQuery delQuery(m_db);
        delQuery.exec(deleteQuery);
    }

    return filesToDelete;
}

// ============================================================================
// emptyTheWholeCache (Java lines 348-351)
// ============================================================================
void CacheDbManager::emptyTheWholeCache() {
    QMutexLocker locker(&m_mutex);
    ensureOpen();

    QSqlQuery query(m_db);
    query.exec(QString("DELETE FROM %1").arg(TABLE));
}

// ============================================================================
// getFilesToSize (Java lines 353-378)
// ============================================================================
QHash<qint64, int> CacheDbManager::getFilesToSize() {
    QMutexLocker locker(&m_mutex);
    ensureOpen();

    QSqlQuery query(m_db);
    query.prepare(QString("SELECT %1,%2 FROM %3")
                      .arg(FIELD_ID, FIELD_TYPE, TABLE));

    if (!query.exec()) {
        qWarning() << "CacheDbManager.getFilesToSize query failed:"
                   << query.lastError().text();
        return {};
    }

    QHash<qint64, int> filesToCheck;
    filesToCheck.reserve(32);

    while (query.next()) {
        const qint64 id = query.value(0).toLongLong();
        const int type = query.value(1).toInt();
        filesToCheck.insert(id, type);
    }

    return filesToCheck;
}

} // namespace PinkReader
