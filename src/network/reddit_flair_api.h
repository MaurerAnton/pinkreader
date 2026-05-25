/*
 * PinkReader - Open source Reddit client for Android
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: reddit_flair_api.h - Flair management API handler
 */

#pragma once

#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrlQuery>
#include <functional>

namespace PinkReader {
class RedditOAuth;

/**
 * @brief Flair management API handler
 */
class RedditFlairApi : public QObject
{
    Q_OBJECT
public:
    using JsonCallback = std::function<void(bool, const QJsonObject&, const QString&)>;
    using ArrayCallback = std::function<void(bool, const QJsonArray&, const QString&, const QString&)>;

    explicit RedditFlairApi(RedditOAuth *oauth, QObject *parent = nullptr);
    ~RedditFlairApi() override;

    void fetchItems(const QString &path, const QUrlQuery &params, ArrayCallback callback);
    void submitAction(const QString &path, const QJsonObject &body, JsonCallback callback);
    void deleteItem(const QString &path, JsonCallback callback);

    void setMaxRetries(int retries);
    int maxRetries() const;
    void setRequestTimeout(int ms);
    int requestTimeout() const;
    int pendingRequestCount() const;
    int totalRequestsSent() const;
    int totalRequestsFailed() const;
    void resetStatistics();

signals:
    void requestStarted(const QString &endpoint);
    void requestCompleted(const QString &endpoint);
    void requestFailed(const QString &endpoint, const QString &error);
    void rateLimited(int retryAfterSeconds);
    void authenticationFailed();

private:
    RedditOAuth *m_oauth = nullptr;
    int m_maxRetries = 3;
    int m_requestTimeout = 30000;
    int m_totalSent = 0;
    int m_totalFailed = 0;
    int m_pendingCount = 0;

    void executeRequest(const QString &method, const QString &path,
                        const QJsonObject &body, JsonCallback callback);
    void retryRequest(const QString &method, const QString &path,
                      const QJsonObject &body, JsonCallback cb, int attempt);
    bool isRetryable(int statusCode) const;
};

} // namespace PinkReader