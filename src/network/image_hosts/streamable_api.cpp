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
 * File: streamable_api.cpp
 * Description: Implementation of Streamable.com video support
 */

#include "network/image_hosts/streamable_api.h"
#include "utils/logging.h"
#include "network/http_client.h"

#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QUrlQuery>

namespace PinkReader {

StreamableApi::StreamableApi(QObject *parent)
    : QObject(parent)
{
    Logging::debug("ImageHosts",
        QString("Created StreamableApi handler"));
}

StreamableApi::~StreamableApi()
{
    Logging::debug("ImageHosts",
        QString("Destroyed StreamableApi handler"));
}

bool StreamableApi::canHandleUrl(const QUrl &url) const
{
    if (!m_enabled)
        return false;

    QString host = url.host().toLower();
    // Default: check if the host matches the service domain
    // Override in subclasses for specific domain matching
    return !host.isEmpty();
}

QString StreamableApi::hostName() const
{
    return QStringLiteral("streamable_api");
}

void StreamableApi::fetchImages(const QUrl &url,
                                 ImageCallback callback)
{
    if (!m_enabled) {
        Logging::debug("ImageHosts",
            QString("StreamableApi is disabled"));
        if (callback)
            callback(false, QVector<ImageInfo>(),
                     QStringLiteral("Image host is disabled"));
        return;
    }

    Logging::info("ImageHosts",
        QString("Fetching images from StreamableApi: %1").arg(url.toString()));

    // Build API request
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                       QStringLiteral("application/json"));

    if (!m_apiKey.isEmpty()) {
        request.setRawHeader("Authorization",
                              QString("Client-ID %1").arg(m_apiKey).toUtf8());
    }

    QNetworkAccessManager *nam = HttpClient::instance()->networkManager();
    QNetworkReply *reply = nam->get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply, callback]() {
        reply->deleteLater();

        if (reply->error() != QNetworkReply::NoError) {
            QString error = reply->errorString();
            Logging::error("ImageHosts",
                QString("StreamableApi fetch failed: %1").arg(error));
            emit fetchError(reply->url(), error);
            if (callback)
                callback(false, QVector<ImageInfo>(), error);
            return;
        }

        QByteArray data = reply->readAll();
        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

        if (parseError.error != QJsonParseError::NoError) {
            Logging::error("ImageHosts",
                QString("StreamableApi JSON parse error: %1")
                    .arg(parseError.errorString()));
            if (callback)
                callback(false, QVector<ImageInfo>(),
                         parseError.errorString());
            return;
        }

        QVector<ImageInfo> images;
        QJsonObject root = doc.object();

        // Parse images from the response
        // Subclasses override this parsing logic for their specific API format
        QJsonArray items = root.value(QStringLiteral("data")).toArray();
        if (items.isEmpty()) {
            items = root.value(QStringLiteral("items")).toArray();
        }

        for (const QJsonValue &val : items) {
            QJsonObject item = val.toObject();
            ImageInfo info;
            info.url = item.value(QStringLiteral("link")).toString();
            info.title = item.value(QStringLiteral("title")).toString();
            info.description = item.value(QStringLiteral("description")).toString();

            if (!info.url.isEmpty()) {
                images.append(info);
                emit imageFetched(QUrl(info.url), info);
            }
        }

        Logging::info("ImageHosts",
            QString("StreamableApi fetched %1 images").arg(images.size()));

        if (callback)
            callback(true, images, QString());
    });
}

void StreamableApi::fetchImageInfo(const QUrl &url,
                                    ImageCallback callback)
{
    // Default implementation: delegate to fetchImages
    fetchImages(url, callback);
}

QUrl StreamableApi::directUrl(const QUrl &url) const
{
    // Default: return the URL as-is
    return url;
}

void StreamableApi::setApiKey(const QString &key)
{
    m_apiKey = key;
    Logging::debug("ImageHosts",
        QString("StreamableApi API key %1")
            .arg(key.isEmpty() ? QStringLiteral("cleared")
                              : QStringLiteral("set")));
}

QString StreamableApi::apiKey() const
{
    return m_apiKey;
}

void StreamableApi::setEnabled(bool enabled)
{
    m_enabled = enabled;
    Logging::debug("ImageHosts",
        QString("StreamableApi %1")
            .arg(enabled ? QStringLiteral("enabled")
                        : QStringLiteral("disabled")));
}

bool StreamableApi::isEnabled() const
{
    return m_enabled;
}

} // namespace PinkReader
