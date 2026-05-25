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
 * File: reddit_requester.h
 * Description: HTTP request execution and queuing
 */

#pragma once

#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QMap>
#include <QVector>
#include <QTimer>
#include <QMutex>
#include <functional>
#include <memory>

namespace PinkReader {

// Forward declarations
class RedditOAuth;

/**
 * @brief HTTP request execution and queuing
 *
 * @note Part of the PinkReader network layer
 */
class RedditRequester : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Callback type for API responses
     * @param success Whether the request was successful
     * @param data The response data as a JSON object
     * @param errorMessage Error description if success is false
     */
    using ResponseCallback = std::function<void(
        bool success,
        const QJsonObject &data,
        const QString &errorMessage
    )>;

    /**
     * @brief Callback type for raw string responses
     * @param success Whether the request was successful
     * @param data The raw response string
     * @param errorMessage Error description if not successful
     */
    using RawResponseCallback = std::function<void(
        bool success,
        const QString &data,
        const QString &errorMessage
    )>;

    explicit RedditRequester(QObject *parent = nullptr);
    ~RedditRequester() override;

    // Initialization
    bool initialize();
    bool isInitialized() const;

    // Request methods
    void get(const QString &endpoint,
             const QUrlQuery &params,
             ResponseCallback callback);

    void post(const QString &endpoint,
              const QJsonObject &body,
              ResponseCallback callback);

    void put(const QString &endpoint,
             const QJsonObject &body,
             ResponseCallback callback);

    void deleteRequest(const QString &endpoint,
                       ResponseCallback callback);

    // Configuration
    void setBaseUrl(const QString &url);
    QString baseUrl() const;

    void setTimeout(int milliseconds);
    int timeout() const;

    void setMaxRetries(int retries);
    int maxRetries() const;

    // Statistics
    int pendingRequestCount() const;
    int totalRequestsSent() const;
    int totalRequestsFailed() const;

    void clearStatistics();

signals:
    void requestStarted(const QString &url);
    void requestCompleted(const QString &url, bool success);
    void requestFailed(const QString &url, const QString &error);
    void networkError(const QString &error);

private:
    struct RequestContext {
        QString endpoint;
        QUrlQuery params;
        QJsonObject body;
        ResponseCallback callback;
        RawResponseCallback rawCallback;
        int retryCount = 0;
        QString method = "GET";
    };

    void executeRequest(const RequestContext &ctx);
    void handleReply(QNetworkReply *reply, RequestContext ctx);
    void retryRequest(RequestContext ctx);
    bool shouldRetry(int statusCode) const;

    // Private data
    QString m_baseUrl;
    int m_timeout = 30000;     // 30 seconds default
    int m_maxRetries = 3;      // 3 retries default
    bool m_initialized = false;

    // Statistics tracking
    int m_totalSent = 0;
    int m_totalFailed = 0;
    mutable QMutex m_statsMutex;

    // Active request tracking
    QVector<QNetworkReply *> m_activeReplies;
    QVector<RequestContext> m_pendingRequests;
    QMutex m_requestMutex;

    static constexpr int MAX_PENDING_REQUESTS = 100;
    static constexpr int REQUEST_COOLDOWN_MS = 100;
};

} // namespace PinkReader
