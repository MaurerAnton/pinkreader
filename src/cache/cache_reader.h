/*
 * PinkReader - Open source Reddit client for Android
 * Copyright (C) 2024-2026 PinkReader Contributors
 * GPLv3 License
 * File: cache_reader.h - Cache data reader with decompression
 */

#pragma once

#include <QObject>
#include <QString>
#include <QByteArray>
#include <QDateTime>
#include <QSqlDatabase>
#include <QDir>
#include <QMutex>
#include <QQueue>
#include <QThread>
#include <functional>
#include <memory>

namespace PinkReader {

class CacheReader : public QObject
{
    Q_OBJECT

public:
    explicit CacheReader(QObject *parent = nullptr);
    ~CacheReader() override;

    bool initialize(const QString &basePath);
    bool isInitialized() const;

    // Cache operations
    bool put(const QString &key, const QByteArray &data, const QString &mimeType = QString());
    QByteArray get(const QString &key) const;
    bool remove(const QString &key);
    bool contains(const QString &key) const;
    void clear();

    // Size management
    qint64 totalSize() const;
    qint64 entryCount() const;
    void setMaxSize(qint64 maxSizeBytes);
    qint64 maxSize() const;

    // Lifecycle
    void prune();
    void flush();
    void close();

signals:
    void cacheInitialized();
    void cacheError(const QString &error);
    void entryAdded(const QString &key);
    void entryRemoved(const QString &key);
    void cacheCleared();
    void pruneCompleted(int entriesRemoved, qint64 bytesFreed);
    void sizeWarning(qint64 currentSize, qint64 maxSize);

private:
    QString m_basePath;
    bool m_initialized = false;
    qint64 m_maxSize = 512 * 1024 * 1024;  // 512 MB default
    QMutex m_mutex;

    struct CacheEntry {
        QString key;
        QString filePath;
        qint64 size = 0;
        QDateTime createdAt;
        QDateTime lastAccessed;
        QString mimeType;
        int accessCount = 0;
    };

    QVector<CacheEntry> loadIndex();
    void saveIndex(const QVector<CacheEntry> &entries);
    QString entryPath(const QString &key) const;
    void removeEntryFiles(const CacheEntry &entry);
};

} // namespace PinkReader
