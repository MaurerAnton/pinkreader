/*
 * PinkReader - Open source Reddit client for Android
 * Copyright (C) 2024-2026 PinkReader Contributors
 * GPLv3 License
 * File: cache_compression.cpp - Implementation of Zstd compression for cached data
 */

#include "cache/cache_compression.h"
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

CacheCompression::CacheCompression(QObject *parent)
    : QObject(parent)
{
}

CacheCompression::~CacheCompression()
{
    close();
}

bool CacheCompression::initialize(const QString &basePath)
{
    if (m_initialized) {
        Logging::debug("Cache::CacheCompression", "Already initialized");
        return true;
    }

    Logging::info("Cache::CacheCompression",
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
            Logging::error("Cache::CacheCompression",
                QString("Failed to create cache directory: %1").arg(m_basePath));
            emit cacheError(QString("Failed to create cache directory: %1")
                               .arg(m_basePath));
            return false;
        }
    }

    // Verify the directory is writable
    QFileInfo fi(m_basePath);
    if (!fi.isWritable()) {
        Logging::error("Cache::CacheCompression",
            QString("Cache directory is not writable: %1").arg(m_basePath));
        emit cacheError(QString("Cache directory is not writable: %1")
                           .arg(m_basePath));
        return false;
    }

    m_initialized = true;
    emit cacheInitialized();

    Logging::info("Cache::CacheCompression",
        QString("Initialized successfully. Path: %1, Max size: %2 MB")
            .arg(m_basePath)
            .arg(m_maxSize / (1024 * 1024)));

    return true;
}

bool CacheCompression::isInitialized() const
{
    return m_initialized;
}

bool CacheCompression::put(const QString &key, const QByteArray &data,
                         const QString &mimeType)
{
    if (!m_initialized) {
        Logging::error("Cache::CacheCompression",
            "Cannot put entry: cache not initialized");
        return false;
    }

    if (key.isEmpty() || data.isEmpty()) {
        Logging::error("Cache::CacheCompression",
            "Cannot put entry: key or data is empty");
        return false;
    }

    QMutexLocker locker(&m_mutex);

    QString filePath = entryPath(key);
    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly)) {
        Logging::error("Cache::CacheCompression",
            QString("Failed to write cache entry: %1").arg(filePath));
        return false;
    }

    qint64 bytesWritten = file.write(data);
    file.close();

    if (bytesWritten != data.size()) {
        Logging::error("Cache::CacheCompression",
            QString("Partial write: %1 of %2 bytes")
                .arg(bytesWritten).arg(data.size()));
        QFile::remove(filePath);
        return false;
    }

    emit entryAdded(key);
    return true;
}

QByteArray CacheCompression::get(const QString &key) const
{
    if (!m_initialized) {
        Logging::error("Cache::CacheCompression",
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
        Logging::error("Cache::CacheCompression",
            QString("Failed to read cache entry: %1").arg(filePath));
        return QByteArray();
    }

    QByteArray data = file.readAll();
    file.close();

    return data;
}

bool CacheCompression::remove(const QString &key)
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

bool CacheCompression::contains(const QString &key) const
{
    if (!m_initialized)
        return false;

    QMutexLocker locker(&m_mutex);
    return QFile::exists(entryPath(key));
}

void CacheCompression::clear()
{
    if (!m_initialized)
        return;

    QMutexLocker locker(&m_mutex);

    QDir dir(m_basePath);
    dir.removeRecursively();
    dir.mkpath(QStringLiteral("."));

    emit cacheCleared();
    Logging::info("Cache::CacheCompression", "Cache cleared");
}

qint64 CacheCompression::totalSize() const
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

qint64 CacheCompression::entryCount() const
{
    if (!m_initialized)
        return 0;

    QMutexLocker locker(&m_mutex);

    QDir dir(m_basePath);
    return dir.entryList(QDir::Files | QDir::NoDotAndDotDot).count();
}

void CacheCompression::setMaxSize(qint64 maxSizeBytes)
{
    m_maxSize = qMax(static_cast<qint64>(1024 * 1024), maxSizeBytes);
    Logging::debug("Cache::CacheCompression",
        QString("Max size set to: %1 MB").arg(m_maxSize / (1024 * 1024)));
}

qint64 CacheCompression::maxSize() const
{
    return m_maxSize;
}

void CacheCompression::prune()
{
    if (!m_initialized)
        return;

    qint64 currentSize = totalSize();
    if (currentSize <= m_maxSize) {
        Logging::debug("Cache::CacheCompression",
            "Cache size within limits, no pruning needed");
        return;
    }

    Logging::info("Cache::CacheCompression",
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
    Logging::info("Cache::CacheCompression",
        QString("Pruned %1 entries, freed %2 MB")
            .arg(removed).arg(freed / (1024 * 1024)));
}

void CacheCompression::flush()
{
    // Ensure all pending writes are completed
    QMutexLocker locker(&m_mutex);
    // For file-based cache, flush is a no-op as writes are synchronous
}

void CacheCompression::close()
{
    if (!m_initialized)
        return;

    Logging::info("Cache::CacheCompression", "Closing cache...");
    flush();
    m_initialized = false;
    Logging::info("Cache::CacheCompression", "Cache closed");
}

// Private helpers
QString CacheCompression::entryPath(const QString &key) const
{
    return QDir(m_basePath).absoluteFilePath(key);
}

QVector<CacheCompression::CacheEntry> CacheCompression::loadIndex()
{
    return QVector<CacheEntry>();
}

void CacheCompression::saveIndex(const QVector<CacheEntry> &entries)
{
    Q_UNUSED(entries)
}

void CacheCompression::removeEntryFiles(const CacheEntry &entry)
{
    QFile::remove(entry.filePath);
}

} // namespace PinkReader
