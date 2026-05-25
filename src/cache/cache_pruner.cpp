/*
 * PinkReader - Open source Reddit client for Android
 * Copyright (C) 2024-2026 PinkReader Contributors
 * GPLv3 License
 * File: cache_pruner.cpp - Implementation of Background cache cleanup and pruning
 */

#include "cache/cache_pruner.h"
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

CachePruner::CachePruner(QObject *parent)
    : QObject(parent)
{
}

CachePruner::~CachePruner()
{
    close();
}

bool CachePruner::initialize(const QString &basePath)
{
    if (m_initialized) {
        Logging::debug("Cache::CachePruner", "Already initialized");
        return true;
    }

    Logging::info("Cache::CachePruner",
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
            Logging::error("Cache::CachePruner",
                QString("Failed to create cache directory: %1").arg(m_basePath));
            emit cacheError(QString("Failed to create cache directory: %1")
                               .arg(m_basePath));
            return false;
        }
    }

    // Verify the directory is writable
    QFileInfo fi(m_basePath);
    if (!fi.isWritable()) {
        Logging::error("Cache::CachePruner",
            QString("Cache directory is not writable: %1").arg(m_basePath));
        emit cacheError(QString("Cache directory is not writable: %1")
                           .arg(m_basePath));
        return false;
    }

    m_initialized = true;
    emit cacheInitialized();

    Logging::info("Cache::CachePruner",
        QString("Initialized successfully. Path: %1, Max size: %2 MB")
            .arg(m_basePath)
            .arg(m_maxSize / (1024 * 1024)));

    return true;
}

bool CachePruner::isInitialized() const
{
    return m_initialized;
}

bool CachePruner::put(const QString &key, const QByteArray &data,
                         const QString &mimeType)
{
    if (!m_initialized) {
        Logging::error("Cache::CachePruner",
            "Cannot put entry: cache not initialized");
        return false;
    }

    if (key.isEmpty() || data.isEmpty()) {
        Logging::error("Cache::CachePruner",
            "Cannot put entry: key or data is empty");
        return false;
    }

    QMutexLocker locker(&m_mutex);

    QString filePath = entryPath(key);
    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly)) {
        Logging::error("Cache::CachePruner",
            QString("Failed to write cache entry: %1").arg(filePath));
        return false;
    }

    qint64 bytesWritten = file.write(data);
    file.close();

    if (bytesWritten != data.size()) {
        Logging::error("Cache::CachePruner",
            QString("Partial write: %1 of %2 bytes")
                .arg(bytesWritten).arg(data.size()));
        QFile::remove(filePath);
        return false;
    }

    emit entryAdded(key);
    return true;
}

QByteArray CachePruner::get(const QString &key) const
{
    if (!m_initialized) {
        Logging::error("Cache::CachePruner",
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
        Logging::error("Cache::CachePruner",
            QString("Failed to read cache entry: %1").arg(filePath));
        return QByteArray();
    }

    QByteArray data = file.readAll();
    file.close();

    return data;
}

bool CachePruner::remove(const QString &key)
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

bool CachePruner::contains(const QString &key) const
{
    if (!m_initialized)
        return false;

    QMutexLocker locker(&m_mutex);
    return QFile::exists(entryPath(key));
}

void CachePruner::clear()
{
    if (!m_initialized)
        return;

    QMutexLocker locker(&m_mutex);

    QDir dir(m_basePath);
    dir.removeRecursively();
    dir.mkpath(QStringLiteral("."));

    emit cacheCleared();
    Logging::info("Cache::CachePruner", "Cache cleared");
}

qint64 CachePruner::totalSize() const
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

qint64 CachePruner::entryCount() const
{
    if (!m_initialized)
        return 0;

    QMutexLocker locker(&m_mutex);

    QDir dir(m_basePath);
    return dir.entryList(QDir::Files | QDir::NoDotAndDotDot).count();
}

void CachePruner::setMaxSize(qint64 maxSizeBytes)
{
    m_maxSize = qMax(static_cast<qint64>(1024 * 1024), maxSizeBytes);
    Logging::debug("Cache::CachePruner",
        QString("Max size set to: %1 MB").arg(m_maxSize / (1024 * 1024)));
}

qint64 CachePruner::maxSize() const
{
    return m_maxSize;
}

void CachePruner::prune()
{
    if (!m_initialized)
        return;

    qint64 currentSize = totalSize();
    if (currentSize <= m_maxSize) {
        Logging::debug("Cache::CachePruner",
            "Cache size within limits, no pruning needed");
        return;
    }

    Logging::info("Cache::CachePruner",
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
    Logging::info("Cache::CachePruner",
        QString("Pruned %1 entries, freed %2 MB")
            .arg(removed).arg(freed / (1024 * 1024)));
}

void CachePruner::flush()
{
    // Ensure all pending writes are completed
    QMutexLocker locker(&m_mutex);
    // For file-based cache, flush is a no-op as writes are synchronous
}

void CachePruner::close()
{
    if (!m_initialized)
        return;

    Logging::info("Cache::CachePruner", "Closing cache...");
    flush();
    m_initialized = false;
    Logging::info("Cache::CachePruner", "Cache closed");
}

// Private helpers
QString CachePruner::entryPath(const QString &key) const
{
    return QDir(m_basePath).absoluteFilePath(key);
}

QVector<CachePruner::CacheEntry> CachePruner::loadIndex()
{
    return QVector<CacheEntry>();
}

void CachePruner::saveIndex(const QVector<CacheEntry> &entries)
{
    Q_UNUSED(entries)
}

void CachePruner::removeEntryFiles(const CacheEntry &entry)
{
    QFile::remove(entry.filePath);
}

} // namespace PinkReader
