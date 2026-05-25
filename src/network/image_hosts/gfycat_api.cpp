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
 * File: gfycat_api.cpp
 * Description: Implementation of Gfycat API integration
 */

#include "network/image_hosts/gfycat_api.h"
#include "utils/logging.h"
#include "network/http_client.h"

#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QUrlQuery>

namespace PinkReader {

GfycatApi::GfycatApi(QObject *parent)
    : QObject(parent)
{
    Logging::debug("ImageHosts",
        QString("Created GfycatApi handler"));
}

GfycatApi::~GfycatApi()
{
    Logging::debug("ImageHosts",
        QString("Destroyed GfycatApi handler"));
}

bool GfycatApi::canHandleUrl(const QUrl &url) const
{
    if (!m_enabled)
        return false;

    QString host = url.host().toLower();
    // Default: check if the host matches the service domain
    // Override in subclasses for specific domain matching
    return !host.isEmpty();
}

QString GfycatApi::hostName() const
{
    return QStringLiteral("gfycat_api");
}

void GfycatApi::fetchImages(const QUrl &url,
                                 ImageCallback callback)
{
    if (!m_enabled) {
        Logging::debug("ImageHosts",
            QString("GfycatApi is disabled"));
        if (callback)
            callback(false, QVector<ImageInfo>(),
                     QStringLiteral("Image host is disabled"));
        return;
    }

    Logging::info("ImageHosts",
        QString("Fetching images from GfycatApi: %1").arg(url.toString()));

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
                QString("GfycatApi fetch failed: %1").arg(error));
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
                QString("GfycatApi JSON parse error: %1")
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
            QString("GfycatApi fetched %1 images").arg(images.size()));

        if (callback)
            callback(true, images, QString());
    });
}

void GfycatApi::fetchImageInfo(const QUrl &url,
                                    ImageCallback callback)
{
    // Default implementation: delegate to fetchImages
    fetchImages(url, callback);
}

QUrl GfycatApi::directUrl(const QUrl &url) const
{
    // Default: return the URL as-is
    return url;
}

void GfycatApi::setApiKey(const QString &key)
{
    m_apiKey = key;
    Logging::debug("ImageHosts",
        QString("GfycatApi API key %1")
            .arg(key.isEmpty() ? QStringLiteral("cleared")
                              : QStringLiteral("set")));
}

QString GfycatApi::apiKey() const
{
    return m_apiKey;
}

void GfycatApi::setEnabled(bool enabled)
{
    m_enabled = enabled;
    Logging::debug("ImageHosts",
        QString("GfycatApi %1")
            .arg(enabled ? QStringLiteral("enabled")
                        : QStringLiteral("disabled")));
}

bool GfycatApi::isEnabled() const
{
    return m_enabled;
}

} // namespace PinkReader
