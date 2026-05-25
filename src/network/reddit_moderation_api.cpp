/*
 * PinkReader - Open source Reddit client for Android
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: reddit_moderation_api.cpp - Implementation of Moderation API handler
 */

#include "network/reddit_moderation_api.h"
#include "network/reddit_oauth.h"
#include "network/http_client.h"
#include "network/rate_limiter.h"
#include "utils/logging.h"
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QTimer>

namespace PinkReader {

RedditModerationApi::RedditModerationApi(RedditOAuth *oauth, QObject *parent)
    : QObject(parent), m_oauth(oauth) {
    Logging::debug("Network::RedditModerationApi", "API handler created");
}

RedditModerationApi::~RedditModerationApi() {
    Logging::debug("Network::RedditModerationApi",
        QString("Destroyed. Sent: %1, Failed: %2").arg(m_totalSent).arg(m_totalFailed));
}

void RedditModerationApi::fetchItems(const QString &path, const QUrlQuery &params, ArrayCallback callback) {
    Logging::info("Network::RedditModerationApi", QString("Fetching: %1").arg(path));
    m_pendingCount++;
    QString fullUrl = QStringLiteral("https://oauth.reddit.com") + path;
    QUrl url(fullUrl);
    if (!params.isEmpty()) url.setQuery(params);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    request.setRawHeader("User-Agent", "PinkReader/1.0.0 (Android)");
    if (m_oauth && m_oauth->isAuthorized()) {
        request.setRawHeader("Authorization",
            QString("Bearer " + m_oauth->accessToken()).toUtf8());
    }
    QNetworkAccessManager *nam = HttpClient::instance()->networkManager();
    QNetworkReply *reply = nam->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply, path, callback]() {
        reply->deleteLater();
        m_pendingCount--;
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray data = reply->readAll();
            QJsonParseError pe;
            QJsonDocument doc = QJsonDocument::fromJson(data, &pe);
            if (pe.error == QJsonParseError::NoError) {
                QJsonObject root = doc.object();
                QJsonObject ld = root.value(QStringLiteral("data")).toObject();
                QJsonArray children = ld.value(QStringLiteral("children")).toArray();
                QJsonArray items;
                for (const QJsonValue &cv : children) {
                    QJsonObject child = cv.toObject();
                    QJsonObject cd = child.value(QStringLiteral("data")).toObject();
                    cd[QStringLiteral("_kind")] = child.value(QStringLiteral("kind")).toString();
                    items.append(cd);
                }
                QString after = ld.value(QStringLiteral("after")).toString();
                m_totalSent++;
                emit requestCompleted(path);
                if (callback) callback(true, items, after, QString());
            } else {
                m_totalFailed++;
                emit requestFailed(path, pe.errorString());
                if (callback) callback(false, QJsonArray(), QString(), pe.errorString());
            }
        } else {
            m_totalFailed++;
            QString err = reply->errorString();
            emit requestFailed(path, err);
            if (callback) callback(false, QJsonArray(), QString(), err);
        }
    });
    emit requestStarted(path);
}

void RedditModerationApi::submitAction(const QString &path, const QJsonObject &body, JsonCallback callback) {
    Logging::info("Network::RedditModerationApi", QString("Submitting: %1").arg(path));
    m_pendingCount++;
    QUrl url(QStringLiteral("https://oauth.reddit.com") + path);
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/x-www-form-urlencoded"));
    if (m_oauth && m_oauth->isAuthorized())
        request.setRawHeader("Authorization", QString("Bearer " + m_oauth->accessToken()).toUtf8());
    QUrlQuery formData;
    for (auto it = body.constBegin(); it != body.constEnd(); ++it)
        formData.addQueryItem(it.key(), it.value().toVariant().toString());
    QNetworkAccessManager *nam = HttpClient::instance()->networkManager();
    QNetworkReply *reply = nam->post(request, formData.toString(QUrl::FullyEncoded).toUtf8());
    connect(reply, &QNetworkReply::finished, this, [this, reply, path, callback]() {
        reply->deleteLater();
        m_pendingCount--;
        if (reply->error() == QNetworkReply::NoError) {
            QJsonParseError pe;
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll(), &pe);
            m_totalSent++;
            emit requestCompleted(path);
            if (callback) callback(true, doc.object(), QString());
        } else {
            m_totalFailed++;
            if (callback) callback(false, QJsonObject(), reply->errorString());
        }
    });
    emit requestStarted(path);
}

void RedditModerationApi::deleteItem(const QString &path, JsonCallback callback) {
    m_pendingCount++;
    QUrl url(QStringLiteral("https://oauth.reddit.com") + path);
    QNetworkRequest request(url);
    if (m_oauth && m_oauth->isAuthorized())
        request.setRawHeader("Authorization", QString("Bearer " + m_oauth->accessToken()).toUtf8());
    QNetworkAccessManager *nam = HttpClient::instance()->networkManager();
    QNetworkReply *reply = nam->deleteResource(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply, path, callback]() {
        reply->deleteLater();
        m_pendingCount--;
        if (reply->error() == QNetworkReply::NoError) {
            m_totalSent++;
            emit requestCompleted(path);
            if (callback) callback(true, QJsonObject(), QString());
        } else { m_totalFailed++; }
    });
}

void RedditModerationApi::setMaxRetries(int r) { m_maxRetries = qMax(0, qMin(r, 10)); }
int RedditModerationApi::maxRetries() const { return m_maxRetries; }
void RedditModerationApi::setRequestTimeout(int ms) { m_requestTimeout = qMax(1000, qMin(ms, 120000)); }
int RedditModerationApi::requestTimeout() const { return m_requestTimeout; }
int RedditModerationApi::pendingRequestCount() const { return m_pendingCount; }
int RedditModerationApi::totalRequestsSent() const { return m_totalSent; }
int RedditModerationApi::totalRequestsFailed() const { return m_totalFailed; }
void RedditModerationApi::resetStatistics() { m_totalSent = 0; m_totalFailed = 0; }

void RedditModerationApi::executeRequest(const QString &m, const QString &p, const QJsonObject &b, JsonCallback cb) {
    Q_UNUSED(m) Q_UNUSED(p) Q_UNUSED(b) Q_UNUSED(cb) }
void RedditModerationApi::retryRequest(const QString &m, const QString &p, const QJsonObject &b, JsonCallback cb, int a) {
    Q_UNUSED(m) Q_UNUSED(p) Q_UNUSED(b) Q_UNUSED(cb) Q_UNUSED(a) }
bool RedditModerationApi::isRetryable(int sc) const { return sc >= 500 || sc == 429 || sc == 0; }

} // namespace PinkReader