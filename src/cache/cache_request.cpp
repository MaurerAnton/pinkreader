/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: cache_request.cpp - Implementation of CacheRequest port
 *
 * Line-by-line translation of Java CacheRequest.java
 */

#include "cache/cache_request.h"
#include "accounts/account.h"
#include "common/priority.h"
#include "common/rr_error.h"
#include "cache/download_strategy.h"
#include "cache/cache_download.h"
#include "http/http_request_body.h"
#include "utils/logging.h"

#include <QUuid>
#include <stdexcept>

namespace PinkReader {

using DownloadQueueType = CacheRequest::DownloadQueueType;
using RequestFailureType = CacheRequest::RequestFailureType;

// ============================================================================
// Constructor 1: 7-arg, cache defaults to true, no requestBody
// Java lines 109-133
// ============================================================================
CacheRequest::CacheRequest(
        const QString &url,
        const RedditAccount &user,
        const std::optional<QUuid> &requestSession,
        const Priority &priority,
        const DownloadStrategy &downloadStrategy,
        int fileType,
        DownloadQueueType queueType,
        const CacheRequestCallbacks &callbacks)
    : CacheRequest(url, user, requestSession, priority, downloadStrategy,
                   fileType, queueType, std::nullopt, true, callbacks)
{
}

// ============================================================================
// Constructor 2: 8-arg with cache param, no requestBody
// Java lines 135-157
// ============================================================================
CacheRequest::CacheRequest(
        const QString &url,
        const RedditAccount &user,
        const std::optional<QUuid> &requestSession,
        const Priority &priority,
        const DownloadStrategy &downloadStrategy,
        int fileType,
        DownloadQueueType queueType,
        bool cache,
        const CacheRequestCallbacks &callbacks)
    : CacheRequest(url, user, requestSession, priority, downloadStrategy,
                   fileType, queueType, std::nullopt, cache, callbacks)
{
}

// ============================================================================
// Constructor 3: 9-arg with requestBody, cache defaults to false
// Java lines 159-183
// ============================================================================
CacheRequest::CacheRequest(
        const QString &url,
        const RedditAccount &user,
        const std::optional<QUuid> &requestSession,
        const Priority &priority,
        const DownloadStrategy &downloadStrategy,
        int fileType,
        DownloadQueueType queueType,
        const std::optional<HTTPRequestBody> &requestBody,
        const CacheRequestCallbacks &callbacks)
    : CacheRequest(url, user, requestSession, priority, downloadStrategy,
                   fileType, queueType, requestBody, false, callbacks)
{
}

// ============================================================================
// Master constructor
// Java lines 186-232
// ============================================================================
CacheRequest::CacheRequest(
        const QString &url,
        const RedditAccount &user,
        const std::optional<QUuid> &requestSession,
        const Priority &priority,
        const DownloadStrategy &downloadStrategy,
        int fileType,
        DownloadQueueType queueType,
        const std::optional<HTTPRequestBody> &requestBody,
        bool cache,
        const CacheRequestCallbacks &callbacks)
    : url(url)
    , user(user)
    , requestSession(requestSession)
    , priority(priority)
    , downloadStrategy(downloadStrategy)
    , fileType(fileType)
    , queueType(queueType)
    , requestBody(requestBody)
    , cache((!requestBody.has_value()) && cache)
    , m_callbacks(callbacks)
{
    // Java line 199: this.context = context.getApplicationContext();
    // Skipped — Android-specific

    // Java line 200: mCallbacks = callbacks;
    // Already set via initializer list

    // Java lines 202-205: user null check
    // In C++ user is a reference, can't be null. Assert for sanity.
    if (user.username().isEmpty()) {
        throw std::invalid_argument(
            "User was null - set to empty string for anonymous");
    }

    // Java lines 207-210: downloadStrategy check for POST + cache
    if (!downloadStrategy.shouldDownloadWithoutCheckingCache()
            && requestBody.has_value()) {
        throw std::invalid_argument(
            "Should not perform cache lookup for POST requests");
    }

    // Java lines 212-220: field assignments done in initializer list

    // Java lines 222-231: url null check
    if (url.isEmpty()) {
        notifyFailure(RRError::generalErrorForFailure(
                RequestFailureType::MALFORMED_URL,
                std::nullopt,
                std::nullopt,
                std::nullopt,
                std::nullopt));
        cancel();
    }
}

CacheRequest::~CacheRequest() = default;

// ============================================================================
// setDownload — Java lines 90-96
// ============================================================================
bool CacheRequest::setDownload(CacheDownload *download)
{
    if (m_cancelled) {
        return false;
    }
    m_download = download;
    return true;
}

// ============================================================================
// cancel — Java lines 99-107
// ============================================================================
void CacheRequest::cancel()
{
    m_cancelled = true;

    if (m_download != nullptr) {
        m_download->cancel();
        m_download = nullptr;
    }
}

// ============================================================================
// compareTo — Java lines 237-241
// ============================================================================
int CacheRequest::compareTo(const CacheRequest &another) const
{
    if (priority.isHigherPriorityThan(another.priority)) {
        return -1;
    }
    if (another.priority.isHigherPriorityThan(priority)) {
        return 1;
    }
    return 0;
}

bool operator<(const CacheRequest &a, const CacheRequest &b)
{
    return a.compareTo(b) < 0;
}

// ============================================================================
// onCallbackException — Java lines 245-248
// ============================================================================
void CacheRequest::onCallbackException(const std::exception &t)
{
    Logging::error("CacheRequest",
        QString("Exception thrown from callback: %1").arg(t.what()));
    // BugReportActivity::handleGlobalError — omitted in C++ port
}

// ============================================================================
// notifyDataStreamAvailable — Java lines 250-258
// ============================================================================
void CacheRequest::notifyDataStreamAvailable(
        const GenericFactory<QByteArray> &streamFactory,
        const TimestampUTC &timestamp,
        const QUuid &session,
        bool fromCache,
        const std::optional<QString> &mimetype)
{
    m_callbacks.onDataStreamAvailable(
            streamFactory, timestamp, session, fromCache, mimetype);
}

// ============================================================================
// notifyDataStreamComplete — Java lines 260-268
// ============================================================================
void CacheRequest::notifyDataStreamComplete(
        const GenericFactory<QByteArray> &streamFactory,
        const TimestampUTC &timestamp,
        const QUuid &session,
        bool fromCache,
        const std::optional<QString> &mimetype)
{
    m_callbacks.onDataStreamComplete(
            streamFactory, timestamp, session, fromCache, mimetype);
}

// ============================================================================
// notifyFailure — Java lines 270-278
// ============================================================================
void CacheRequest::notifyFailure(const RRError &error)
{
    try {
        m_callbacks.onFailure(error);
    } catch (const std::exception &t1) {
        onCallbackException(t1);
    }
}

// ============================================================================
// notifyProgress — Java lines 280-289
// ============================================================================
void CacheRequest::notifyProgress(
        bool authorizationInProgress,
        qint64 bytesRead,
        qint64 totalBytes)
{
    try {
        m_callbacks.onProgress(authorizationInProgress, bytesRead, totalBytes);
    } catch (const std::exception &t) {
        onCallbackException(t);
    }
}

// ============================================================================
// notifyCacheFileWritten — Java lines 291-302
// ============================================================================
void CacheRequest::notifyCacheFileWritten(
        const CacheManager::ReadableCacheFile &cacheFile,
        const TimestampUTC &timestamp,
        const QUuid &session,
        bool fromCache,
        const QString &mimetype)
{
    try {
        m_callbacks.onCacheFileWritten(
                cacheFile, timestamp, session, fromCache, mimetype);
    } catch (const std::exception &t) {
        onCallbackException(t);
    }
}

// ============================================================================
// notifyDownloadNecessary — Java lines 304-319
// ============================================================================
void CacheRequest::notifyDownloadNecessary()
{
    try {
        m_callbacks.onDownloadNecessary();
    } catch (const std::exception &t1) {
        Logging::error("CacheRequest",
            QString("Exception thrown by onDownloadNecessary: %1").arg(t1.what()));

        try {
            onCallbackException(t1);
        } catch (const std::exception &t2) {
            Logging::error("CacheRequest",
                QString("Exception thrown by onCallbackException: %1").arg(t2.what()));
            // BugReportActivity::addGlobalError — omitted in C++
            // BugReportActivity::handleGlobalError — omitted in C++
        }
    }
}

// ============================================================================
// notifyDownloadStarted — Java lines 321-336
// ============================================================================
void CacheRequest::notifyDownloadStarted()
{
    try {
        m_callbacks.onDownloadStarted();
    } catch (const std::exception &t1) {
        Logging::error("CacheRequest",
            QString("Exception thrown by onDownloadStarted: %1").arg(t1.what()));

        try {
            onCallbackException(t1);
        } catch (const std::exception &t2) {
            Logging::error("CacheRequest",
                QString("Exception thrown by onCallbackException: %1").arg(t2.what()));
            // BugReportActivity::addGlobalError — omitted
            // BugReportActivity::handleGlobalError — omitted
        }
    }
}

} // namespace PinkReader
