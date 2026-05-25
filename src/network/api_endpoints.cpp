/*
 * PinkReader - Open source Reddit client for Android
 * Copyright (C) 2024-2026 PinkReader Contributors
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ... (full GPLv3 license) ...
 *
 * File: api_endpoints.cpp
 * Description: Implementation of Reddit API endpoint registry
 */

#include "network/api_endpoints.h"
#include "network/http_client.h"
#include "network/rate_limiter.h"
#include "utils/logging.h"

#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QEventLoop>
#include <QTimer>

namespace PinkReader {

ApiEndpoints::ApiEndpoints(QObject *parent)
    : QObject(parent)
{
}

ApiEndpoints::~ApiEndpoints()
{
    // Cancel any active requests
    for (QNetworkReply *reply : m_activeReplies) {
        if (reply) {
            reply->abort();
            reply->deleteLater();
        }
    }
    m_activeReplies.clear();
    m_pendingRequests.clear();
}

bool ApiEndpoints::initialize()
{
    if (m_initialized) {
        Logging::debug("Network::" + QString::fromLatin1("ApiEndpoints"),
            "Already initialized, skipping");
        return true;
    }

    Logging::info("Network::" + QString::fromLatin1("ApiEndpoints"),
        "Initializing ApiEndpoints...");

    // Validate configuration
    if (m_baseUrl.isEmpty()) {
        Logging::error("Network::" + QString::fromLatin1("ApiEndpoints"),
            "Base URL not set! Call setBaseUrl() before initialize().");
        return false;
    }

    // Validate URL format
    QUrl url(m_baseUrl);
    if (!url.isValid()) {
        Logging::error("Network::" + QString::fromLatin1("ApiEndpoints"),
            QString("Invalid base URL: %1").arg(m_baseUrl));
        return false;
    }

    m_initialized = true;
    Logging::info("Network::" + QString::fromLatin1("ApiEndpoints"),
        QString("Initialized successfully. Base URL: %1").arg(m_baseUrl));

    return true;
}

bool ApiEndpoints::isInitialized() const
{
    return m_initialized;
}

void ApiEndpoints::get(const QString &endpoint,
                        const QUrlQuery &params,
                        ResponseCallback callback)
{
    if (!m_initialized) {
        Logging::error("Network::" + QString::fromLatin1("ApiEndpoints"),
            "Cannot send GET request: not initialized");
        if (callback)
            callback(false, QJsonObject(),
                     QStringLiteral("Network component not initialized"));
        return;
    }

    RequestContext ctx;
    ctx.method = "GET";
    ctx.endpoint = endpoint;
    ctx.params = params;
    ctx.callback = std::move(callback);

    executeRequest(ctx);
}

void ApiEndpoints::post(const QString &endpoint,
                         const QJsonObject &body,
                         ResponseCallback callback)
{
    if (!m_initialized) {
        Logging::error("Network::" + QString::fromLatin1("ApiEndpoints"),
            "Cannot send POST request: not initialized");
        if (callback)
            callback(false, QJsonObject(),
                     QStringLiteral("Network component not initialized"));
        return;
    }

    RequestContext ctx;
    ctx.method = "POST";
    ctx.endpoint = endpoint;
    ctx.body = body;
    ctx.callback = std::move(callback);

    executeRequest(ctx);
}

void ApiEndpoints::put(const QString &endpoint,
                        const QJsonObject &body,
                        ResponseCallback callback)
{
    if (!m_initialized) {
        Logging::error("Network::" + QString::fromLatin1("ApiEndpoints"),
            "Cannot send PUT request: not initialized");
        if (callback)
            callback(false, QJsonObject(),
                     QStringLiteral("Network component not initialized"));
        return;
    }

    RequestContext ctx;
    ctx.method = "PUT";
    ctx.endpoint = endpoint;
    ctx.body = body;
    ctx.callback = std::move(callback);

    executeRequest(ctx);
}

void ApiEndpoints::deleteRequest(const QString &endpoint,
                                  ResponseCallback callback)
{
    if (!m_initialized) {
        Logging::error("Network::" + QString::fromLatin1("ApiEndpoints"),
            "Cannot send DELETE request: not initialized");
        if (callback)
            callback(false, QJsonObject(),
                     QStringLiteral("Network component not initialized"));
        return;
    }

    RequestContext ctx;
    ctx.method = "DELETE";
    ctx.endpoint = endpoint;
    ctx.callback = std::move(callback);

    executeRequest(ctx);
}

void ApiEndpoints::setBaseUrl(const QString &url)
{
    m_baseUrl = url;
    Logging::debug("Network::" + QString::fromLatin1("ApiEndpoints"),
        QString("Base URL set to: %1").arg(url));
}

QString ApiEndpoints::baseUrl() const
{
    return m_baseUrl;
}

void ApiEndpoints::setTimeout(int milliseconds)
{
    m_timeout = milliseconds;
    Logging::debug("Network::" + QString::fromLatin1("ApiEndpoints"),
        QString("Timeout set to: %1 ms").arg(milliseconds));
}

int ApiEndpoints::timeout() const
{
    return m_timeout;
}

void ApiEndpoints::setMaxRetries(int retries)
{
    m_maxRetries = qMax(0, qMin(retries, 10));
    Logging::debug("Network::" + QString::fromLatin1("ApiEndpoints"),
        QString("Max retries set to: %1").arg(m_maxRetries));
}

int ApiEndpoints::maxRetries() const
{
    return m_maxRetries;
}

int ApiEndpoints::pendingRequestCount() const
{
    QMutexLocker locker(&m_requestMutex);
    return m_activeReplies.size() + m_pendingRequests.size();
}

int ApiEndpoints::totalRequestsSent() const
{
    QMutexLocker locker(&m_statsMutex);
    return m_totalSent;
}

int ApiEndpoints::totalRequestsFailed() const
{
    QMutexLocker locker(&m_statsMutex);
    return m_totalFailed;
}

void ApiEndpoints::clearStatistics()
{
    QMutexLocker locker(&m_statsMutex);
    m_totalSent = 0;
    m_totalFailed = 0;
}

// ---------------------------------------------------------------------------
// Private Implementation
// ---------------------------------------------------------------------------

void ApiEndpoints::executeRequest(const RequestContext &ctx)
{
    // Build the full URL
    QUrl url(m_baseUrl + ctx.endpoint);

    if (!ctx.params.isEmpty()) {
        url.setQuery(ctx.params);
    }

    emit requestStarted(url.toString());

    // Create the network request
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                       QStringLiteral("application/json"));
    request.setTransferTimeout(m_timeout);

    // Execute the request
    QNetworkAccessManager *nam = HttpClient::instance()->networkManager();
    QNetworkReply *reply = nullptr;

    if (ctx.method == "GET") {
        reply = nam->get(request);
    } else if (ctx.method == "POST") {
        QJsonDocument doc(ctx.body);
        reply = nam->post(request, doc.toJson(QJsonDocument::Compact));
    } else if (ctx.method == "PUT") {
        QJsonDocument doc(ctx.body);
        reply = nam->put(request, doc.toJson(QJsonDocument::Compact));
    } else if (ctx.method == "DELETE") {
        reply = nam->deleteResource(request);
    }

    if (!reply) {
        Logging::error("Network::" + QString::fromLatin1("ApiEndpoints"),
            QString("Failed to create network reply for: %1").arg(url.toString()));
        {
            QMutexLocker locker(&m_statsMutex);
            m_totalFailed++;
        }
        if (ctx.callback) {
            ctx.callback(false, QJsonObject(),
                         QStringLiteral("Failed to create network request"));
        }
        return;
    }

    // Track active request
    {
        QMutexLocker locker(&m_requestMutex);
        m_activeReplies.append(reply);
    }

    // Connect reply signals
    RequestContext ctxCopy = ctx;
    connect(reply, &QNetworkReply::finished, this, [this, reply, ctxCopy]() {
        handleReply(reply, ctxCopy);
    });

    {
        QMutexLocker locker(&m_statsMutex);
        m_totalSent++;
    }
}

void ApiEndpoints::handleReply(QNetworkReply *reply, RequestContext ctx)
{
    reply->deleteLater();

    // Remove from active list
    {
        QMutexLocker locker(&m_requestMutex);
        m_activeReplies.removeAll(reply);
    }

    int statusCode = reply->attribute(
        QNetworkRequest::HttpStatusCodeAttribute).toInt();

    if (reply->error() == QNetworkReply::NoError && statusCode >= 200 && statusCode < 300) {
        // Success
        QByteArray responseData = reply->readAll();
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(responseData, &parseError);

        if (parseError.error == QJsonParseError::NoError) {
            emit requestCompleted(reply->url().toString(), true);
            if (ctx.callback) {
                ctx.callback(true, doc.object(),
                             QString());  // No error message on success
            }
        } else {
            Logging::error("Network::" + QString::fromLatin1("ApiEndpoints"),
                QString("JSON parse error: %1").arg(parseError.errorString()));
            emit requestCompleted(reply->url().toString(), false);
            if (ctx.callback) {
                ctx.callback(false, QJsonObject(),
                             QString("JSON parse error: %1")
                                 .arg(parseError.errorString()));
            }
        }
    } else {
        // Error
        QString errorString = reply->errorString();
        Logging::error("Network::" + QString::fromLatin1("ApiEndpoints"),
            QString("Request failed [%1]: %2")
                .arg(statusCode).arg(errorString));

        emit requestFailed(reply->url().toString(), errorString);

        {
            QMutexLocker locker(&m_statsMutex);
            m_totalFailed++;
        }

        // Retry logic for transient errors
        if (shouldRetry(statusCode) && ctx.retryCount < m_maxRetries) {
            Logging::debug("Network::" + QString::fromLatin1("ApiEndpoints"),
                QString("Retrying request (attempt %1/%2)...")
                    .arg(ctx.retryCount + 1).arg(m_maxRetries));
            retryRequest(ctx);
        } else {
            if (ctx.callback) {
                ctx.callback(false, QJsonObject(), errorString);
            }
        }
    }
}

void ApiEndpoints::retryRequest(RequestContext ctx)
{
    ctx.retryCount++;
    int delay = 1000 * ctx.retryCount;  // Linear backoff: 1s, 2s, 3s...
    QTimer::singleShot(delay, this, [this, ctx]() {
        executeRequest(ctx);
    });
}

bool ApiEndpoints::shouldRetry(int statusCode) const
{
    // Retry on server errors and rate limiting
    return statusCode >= 500 || statusCode == 429 || statusCode == 0;
}

} // namespace PinkReader
