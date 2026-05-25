/*
 * PinkReader - Open source Reddit client for Android
 * Copyright (C) 2024-2026 PinkReader Contributors
 * GPLv3 License
 * File: cache_stats.cpp - Implementation of Cache statistics and monitoring
 */

#include "cache/cache_stats.h"
#include "utils/logging.h"
#include "utils/file_utils.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStandardPaths>
#include <QDateTime>

namespace PinkReader {

CacheStats::CacheStats(QObject *parent)
    : QObject(parent)
{
}

CacheStats::~CacheStats()
{
    close();
}

bool CacheStats::initialize(const QString &basePath)
{
    if (m_initialized) {
        Logging::debug("Cache::CacheStats", "Already initialized");
        return true;
    }

    Logging::info("Cache::CacheStats",
        QString("Initializing with base path: %1").arg(basePath));

    if (basePath.isEmpty()) {
        m_basePath = QStandardPaths::writableLocation(
            QStandardPaths::CacheLocation);
    } else {
        m_basePath = basePath;
    }

    // Create cache directory if it doesn't exist
    QDir dir(m_basePath);
    if (!dir.exists()) {
        if (!dir.mkpath(QStringLiteral("."))) {
            Logging::error("Cache::CacheStats",
                QString("Failed to create cache directory: %1").arg(m_basePath));
            emit cacheError(QString("Failed to create cache directory: %1")
                               .arg(m_basePath));
            return false;
        }
    }

    // Verify the directory is writable
    QFileInfo fi(m_basePath);
    if (!fi.isWritable()) {
        Logging::error("Cache::CacheStats",
            QString("Cache directory is not writable: %1").arg(m_basePath));
        emit cacheError(QString("Cache directory is not writable: %1")
                           .arg(m_basePath));
        return false;
    }

    m_initialized = true;
    emit cacheInitialized();

    Logging::info("Cache::CacheStats",
        QString("Initialized successfully. Path: %1, Max size: %2 MB")
            .arg(m_basePath)
            .arg(m_maxSize / (1024 * 1024)));

    return true;
}

bool CacheStats::isInitialized() const
{
    return m_initialized;
}

bool CacheStats::put(const QString &key, const QByteArray &data,
                         const QString &mimeType)
{
    if (!m_initialized) {
        Logging::error("Cache::CacheStats",
            "Cannot put entry: cache not initialized");
        return false;
    }

    if (key.isEmpty() || data.isEmpty()) {
        Logging::error("Cache::CacheStats",
            "Cannot put entry: key or data is empty");
        return false;
    }

    QMutexLocker locker(&m_mutex);

    QString filePath = entryPath(key);
    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly)) {
        Logging::error("Cache::CacheStats",
            QString("Failed to write cache entry: %1").arg(filePath));
        return false;
    }

    qint64 bytesWritten = file.write(data);
    file.close();

    if (bytesWritten != data.size()) {
        Logging::error("Cache::CacheStats",
            QString("Partial write: %1 of %2 bytes")
                .arg(bytesWritten).arg(data.size()));
        QFile::remove(filePath);
        return false;
    }

    emit entryAdded(key);
    return true;
}

QByteArray CacheStats::get(const QString &key) const
{
    if (!m_initialized) {
        Logging::error("Cache::CacheStats",
            "Cannot get entry: cache not initialized");
        return QByteArray();
    }

    QMutexLocker locker(&m_mutex);

    QString filePath = entryPath(key);
    QFile file(filePath);

    if (!file.exists()) {
        return QByteArray();
    }

    if (!file.open(QIODevice::ReadOnly)) {
        Logging::error("Cache::CacheStats",
            QString("Failed to read cache entry: %1").arg(filePath));
        return QByteArray();
    }

    QByteArray data = file.readAll();
    file.close();

    return data;
}

bool CacheStats::remove(const QString &key)
{
    if (!m_initialized)
        return false;

    QMutexLocker locker(&m_mutex);
    QString filePath = entryPath(key);

    if (QFile::exists(filePath)) {
        if (QFile::remove(filePath)) {
            emit entryRemoved(key);
            return true;
        }
    }

    return false;
}

bool CacheStats::contains(const QString &key) const
{
    if (!m_initialized)
        return false;

    QMutexLocker locker(&m_mutex);
    return QFile::exists(entryPath(key));
}

void CacheStats::clear()
{
    if (!m_initialized)
        return;

    QMutexLocker locker(&m_mutex);

    QDir dir(m_basePath);
    dir.removeRecursively();
    dir.mkpath(QStringLiteral("."));

    emit cacheCleared();
    Logging::info("Cache::CacheStats", "Cache cleared");
}

qint64 CacheStats::totalSize() const
{
    if (!m_initialized)
        return 0;

    QMutexLocker locker(&m_mutex);

    qint64 total = 0;
    QDir dir(m_basePath);
    QFileInfoList files = dir.entryInfoList(
        QDir::Files | QDir::NoDotAndDotDot);

    for (const QFileInfo &fi : files) {
        total += fi.size();
    }

    return total;
}

qint64 CacheStats::entryCount() const
{
    if (!m_initialized)
        return 0;

    QMutexLocker locker(&m_mutex);

    QDir dir(m_basePath);
    return dir.entryList(QDir::Files | QDir::NoDotAndDotDot).count();
}

void CacheStats::setMaxSize(qint64 maxSizeBytes)
{
    m_maxSize = qMax(static_cast<qint64>(1024 * 1024), maxSizeBytes);
    Logging::debug("Cache::CacheStats",
        QString("Max size set to: %1 MB").arg(m_maxSize / (1024 * 1024)));
}

qint64 CacheStats::maxSize() const
{
    return m_maxSize;
}

void CacheStats::prune()
{
    if (!m_initialized)
        return;

    qint64 currentSize = totalSize();
    if (currentSize <= m_maxSize) {
        Logging::debug("Cache::CacheStats",
            "Cache size within limits, no pruning needed");
        return;
    }

    Logging::info("Cache::CacheStats",
        QString("Pruning cache: %1 MB -> %2 MB limit")
            .arg(currentSize / (1024 * 1024))
            .arg(m_maxSize / (1024 * 1024)));

    QMutexLocker locker(&m_mutex);

    // Get all cache files sorted by last access time (oldest first)
    QDir dir(m_basePath);
    QFileInfoList files = dir.entryInfoList(
        QDir::Files | QDir::NoDotAndDotDot);

    std::sort(files.begin(), files.end(),
        [](const QFileInfo &a, const QFileInfo &b) {
            return a.lastRead() < b.lastRead();
        });

    int removed = 0;
    qint64 freed = 0;

    for (const QFileInfo &fi : files) {
        if (currentSize - freed <= m_maxSize)
            break;

        freed += fi.size();
        QFile::remove(fi.absoluteFilePath());
        removed++;
    }

    emit pruneCompleted(removed, freed);
    Logging::info("Cache::CacheStats",
        QString("Pruned %1 entries, freed %2 MB")
            .arg(removed).arg(freed / (1024 * 1024)));
}

void CacheStats::flush()
{
    // Ensure all pending writes are completed
    QMutexLocker locker(&m_mutex);
    // For file-based cache, flush is a no-op as writes are synchronous
}

void CacheStats::close()
{
    if (!m_initialized)
        return;

    Logging::info("Cache::CacheStats", "Closing cache...");
    flush();
    m_initialized = false;
    Logging::info("Cache::CacheStats", "Cache closed");
}

// Private helpers
QString CacheStats::entryPath(const QString &key) const
{
    return QDir(m_basePath).absoluteFilePath(key);
}

QVector<CacheStats::CacheEntry> CacheStats::loadIndex()
{
    return QVector<CacheEntry>();
}

void CacheStats::saveIndex(const QVector<CacheEntry> &entries)
{
    Q_UNUSED(entries)
}

void CacheStats::removeEntryFiles(const CacheEntry &entry)
{
    QFile::remove(entry.filePath);
}

} // namespace PinkReader
