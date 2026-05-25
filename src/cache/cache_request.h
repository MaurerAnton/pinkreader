/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: cache_request.h - Port of RedReader's CacheRequest.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/cache/CacheRequest.java
 */

#pragma once

#include <QObject>
#include <QString>
#include <QUuid>
#include <functional>
#include <memory>
#include <optional>

namespace PinkReader {

class RedditAccount;
class Priority;
class DownloadStrategy;
class HTTPRequestBody;
class CacheDownload;
class CacheManager;
class RRError;

// Forward: SeekableInputStream factory
template<typename T>
class GenericFactory;

// Forward: TimestampUTC
class TimestampUTC;

// ============================================================================
// CacheRequestCallbacks — port of CacheRequestCallbacks.java
// ============================================================================

class CacheRequestCallbacks {
public:
    virtual ~CacheRequestCallbacks() = default;

    virtual void onDownloadNecessary() {}
    virtual void onDownloadStarted() {}

    virtual void onDataStreamAvailable(
            const GenericFactory<QByteArray> &streamFactory,
            const TimestampUTC &timestamp,
            const QUuid &session,
            bool fromCache,
            const std::optional<QString> &mimetype) {}

    virtual void onDataStreamComplete(
            const GenericFactory<QByteArray> &streamFactory,
            const TimestampUTC &timestamp,
            const QUuid &session,
            bool fromCache,
            const std::optional<QString> &mimetype) {}

    virtual void onProgress(
            bool authorizationInProgress,
            qint64 bytesRead,
            qint64 totalBytes) {}

    virtual void onFailure(const RRError &error) = 0;

    virtual void onCacheFileWritten(
            const CacheManager::ReadableCacheFile &cacheFile,
            const TimestampUTC &timestamp,
            const QUuid &session,
            bool fromCache,
            const std::optional<QString> &mimetype) {}
};

// ============================================================================
// CacheRequest — port of CacheRequest.java
// ============================================================================

class CacheRequest {
public:
    // ---- DownloadQueueType enum (Java line 44-50) ----
    enum class DownloadQueueType {
        REDDIT_API,
        IMGUR_API,
        IMMEDIATE,
        IMAGE_PRECACHE,
        REDGIFS_API_V2
    };

    // ---- RequestFailureType enum (Java line 52-65) ----
    enum class RequestFailureType {
        CONNECTION,
        REQUEST,
        STORAGE,
        CACHE_MISS,
        CANCELLED,
        MALFORMED_URL,
        PARSE,
        DISK_SPACE,
        REDDIT_REDIRECT,
        PARSE_IMGUR,
        UPLOAD_FAIL_IMGUR,
        CACHE_DIR_DOES_NOT_EXIST
    };

    // ---- Fields (Java lines 67-87) ----
    const QString url;
    const RedditAccount &user;
    const std::optional<QUuid> requestSession;
    const Priority &priority;
    const DownloadStrategy &downloadStrategy;
    const int fileType;
    const DownloadQueueType queueType;
    const std::optional<HTTPRequestBody> requestBody;
    const bool cache;
    // context is omitted in C++ port (Android-specific)

    // ---- Constructor 1: 7-arg with cache=true (Java lines 109-133) ----
    CacheRequest(
            const QString &url,
            const RedditAccount &user,
            const std::optional<QUuid> &requestSession,
            const Priority &priority,
            const DownloadStrategy &downloadStrategy,
            int fileType,
            DownloadQueueType queueType,
            const CacheRequestCallbacks &callbacks);

    // ---- Constructor 2: 7-arg without requestBody, cache default (Java lines 135-157) ----
    CacheRequest(
            const QString &url,
            const RedditAccount &user,
            const std::optional<QUuid> &requestSession,
            const Priority &priority,
            const DownloadStrategy &downloadStrategy,
            int fileType,
            DownloadQueueType queueType,
            bool cache,
            const CacheRequestCallbacks &callbacks);

    // ---- Constructor 3: 8-arg with requestBody, cache=false (Java lines 159-183) ----
    CacheRequest(
            const QString &url,
            const RedditAccount &user,
            const std::optional<QUuid> &requestSession,
            const Priority &priority,
            const DownloadStrategy &downloadStrategy,
            int fileType,
            DownloadQueueType queueType,
            const std::optional<HTTPRequestBody> &requestBody,
            const CacheRequestCallbacks &callbacks);

    // ---- Master constructor (Java lines 186-232) ----
    CacheRequest(
            const QString &url,
            const RedditAccount &user,
            const std::optional<QUuid> &requestSession,
            const Priority &priority,
            const DownloadStrategy &downloadStrategy,
            int fileType,
            DownloadQueueType queueType,
            const std::optional<HTTPRequestBody> &requestBody,
            bool cache,
            const CacheRequestCallbacks &callbacks);

    ~CacheRequest();

    // ---- Queue helpers ----
    int compareTo(const CacheRequest &another) const;

    // ---- Called by CacheDownload (Java line 90-96) ----
    bool setDownload(CacheDownload *download);

    // ---- Cancel (Java lines 99-107) ----
    void cancel();

    // ---- Callbacks (Java lines 245-336) ----
    void notifyDataStreamAvailable(
            const GenericFactory<QByteArray> &streamFactory,
            const TimestampUTC &timestamp,
            const QUuid &session,
            bool fromCache,
            const std::optional<QString> &mimetype);

    void notifyDataStreamComplete(
            const GenericFactory<QByteArray> &streamFactory,
            const TimestampUTC &timestamp,
            const QUuid &session,
            bool fromCache,
            const std::optional<QString> &mimetype);

    void notifyFailure(const RRError &error);

    void notifyProgress(
            bool authorizationInProgress,
            qint64 bytesRead,
            qint64 totalBytes);

    void notifyCacheFileWritten(
            const CacheManager::ReadableCacheFile &cacheFile,
            const TimestampUTC &timestamp,
            const QUuid &session,
            bool fromCache,
            const QString &mimetype);

    void notifyDownloadNecessary();
    void notifyDownloadStarted();

private:
    CacheDownload *m_download = nullptr;
    bool m_cancelled = false;
    const CacheRequestCallbacks &m_callbacks;

    void onCallbackException(const std::exception &t);
};

// ---- Comparison operators ----
bool operator<(const CacheRequest &a, const CacheRequest &b);

} // namespace PinkReader
