/*
 * PinkReader - Open source Reddit client for Android
 * Copyright (C) 2024-2026 PinkReader Contributors
 * GPLv3 License
 * File: precache_manager.cpp - Implementation of Image and comment precaching
 */

#include "cache/precache_manager.h"
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

PrecacheManager::PrecacheManager(QObject *parent)
    : QObject(parent)
{
}

PrecacheManager::~PrecacheManager()
{
    close();
}

bool PrecacheManager::initialize(const QString &basePath)
{
    if (m_initialized) {
        Logging::debug("Cache::PrecacheManager", "Already initialized");
        return true;
    }

    Logging::info("Cache::PrecacheManager",
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
            Logging::error("Cache::PrecacheManager",
                QString("Failed to create cache directory: %1").arg(m_basePath));
            emit cacheError(QString("Failed to create cache directory: %1")
                               .arg(m_basePath));
            return false;
        }
    }

    // Verify the directory is writable
    QFileInfo fi(m_basePath);
    if (!fi.isWritable()) {
        Logging::error("Cache::PrecacheManager",
            QString("Cache directory is not writable: %1").arg(m_basePath));
        emit cacheError(QString("Cache directory is not writable: %1")
                           .arg(m_basePath));
        return false;
    }

    m_initialized = true;
    emit cacheInitialized();

    Logging::info("Cache::PrecacheManager",
        QString("Initialized successfully. Path: %1, Max size: %2 MB")
            .arg(m_basePath)
            .arg(m_maxSize / (1024 * 1024)));

    return true;
}

bool PrecacheManager::isInitialized() const
{
    return m_initialized;
}

bool PrecacheManager::put(const QString &key, const QByteArray &data,
                         const QString &mimeType)
{
    if (!m_initialized) {
        Logging::error("Cache::PrecacheManager",
            "Cannot put entry: cache not initialized");
        return false;
    }

    if (key.isEmpty() || data.isEmpty()) {
        Logging::error("Cache::PrecacheManager",
            "Cannot put entry: key or data is empty");
        return false;
    }

    QMutexLocker locker(&m_mutex);

    QString filePath = entryPath(key);
    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly)) {
        Logging::error("Cache::PrecacheManager",
            QString("Failed to write cache entry: %1").arg(filePath));
        return false;
    }

    qint64 bytesWritten = file.write(data);
    file.close();

    if (bytesWritten != data.size()) {
        Logging::error("Cache::PrecacheManager",
            QString("Partial write: %1 of %2 bytes")
                .arg(bytesWritten).arg(data.size()));
        QFile::remove(filePath);
        return false;
    }

    emit entryAdded(key);
    return true;
}

QByteArray PrecacheManager::get(const QString &key) const
{
    if (!m_initialized) {
        Logging::error("Cache::PrecacheManager",
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
        Logging::error("Cache::PrecacheManager",
            QString("Failed to read cache entry: %1").arg(filePath));
        return QByteArray();
    }

    QByteArray data = file.readAll();
    file.close();

    return data;
}

bool PrecacheManager::remove(const QString &key)
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

bool PrecacheManager::contains(const QString &key) const
{
    if (!m_initialized)
        return false;

    QMutexLocker locker(&m_mutex);
    return QFile::exists(entryPath(key));
}

void PrecacheManager::clear()
{
    if (!m_initialized)
        return;

    QMutexLocker locker(&m_mutex);

    QDir dir(m_basePath);
    dir.removeRecursively();
    dir.mkpath(QStringLiteral("."));

    emit cacheCleared();
    Logging::info("Cache::PrecacheManager", "Cache cleared");
}

qint64 PrecacheManager::totalSize() const
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

qint64 PrecacheManager::entryCount() const
{
    if (!m_initialized)
        return 0;

    QMutexLocker locker(&m_mutex);

    QDir dir(m_basePath);
    return dir.entryList(QDir::Files | QDir::NoDotAndDotDot).count();
}

void PrecacheManager::setMaxSize(qint64 maxSizeBytes)
{
    m_maxSize = qMax(static_cast<qint64>(1024 * 1024), maxSizeBytes);
    Logging::debug("Cache::PrecacheManager",
        QString("Max size set to: %1 MB").arg(m_maxSize / (1024 * 1024)));
}

qint64 PrecacheManager::maxSize() const
{
    return m_maxSize;
}

void PrecacheManager::prune()
{
    if (!m_initialized)
        return;

    qint64 currentSize = totalSize();
    if (currentSize <= m_maxSize) {
        Logging::debug("Cache::PrecacheManager",
            "Cache size within limits, no pruning needed");
        return;
    }

    Logging::info("Cache::PrecacheManager",
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
    Logging::info("Cache::PrecacheManager",
        QString("Pruned %1 entries, freed %2 MB")
            .arg(removed).arg(freed / (1024 * 1024)));
}

void PrecacheManager::flush()
{
    // Ensure all pending writes are completed
    QMutexLocker locker(&m_mutex);
    // For file-based cache, flush is a no-op as writes are synchronous
}

void PrecacheManager::close()
{
    if (!m_initialized)
        return;

    Logging::info("Cache::PrecacheManager", "Closing cache...");
    flush();
    m_initialized = false;
    Logging::info("Cache::PrecacheManager", "Cache closed");
}

// Private helpers
QString PrecacheManager::entryPath(const QString &key) const
{
    return QDir(m_basePath).absoluteFilePath(key);
}

QVector<PrecacheManager::CacheEntry> PrecacheManager::loadIndex()
{
    return QVector<CacheEntry>();
}

void PrecacheManager::saveIndex(const QVector<CacheEntry> &entries)
{
    Q_UNUSED(entries)
}

void PrecacheManager::removeEntryFiles(const CacheEntry &entry)
{
    QFile::remove(entry.filePath);
}

} // namespace PinkReader
