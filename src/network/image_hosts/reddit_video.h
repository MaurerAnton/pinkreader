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
 * File: reddit_video.h
 * Description: Reddit native video (v.redd.it) support
 */

#pragma once

#include <QObject>
#include <QString>
#include <QUrl>
#include <QJsonObject>
#include <QVector>
#include <functional>

namespace PinkReader {

/**
 * @brief Image/video information returned by host APIs
 */
struct ImageInfo {
    QString url;           ///< Direct media URL
    QString thumbnailUrl;  ///< Thumbnail URL
    QString title;         ///< Media title/caption
    QString description;   ///< Media description
    int width = 0;         ///< Width in pixels
    int height = 0;        ///< Height in pixels
    qint64 sizeBytes = 0;  ///< File size in bytes
    QString mimeType;      ///< MIME type
    bool isVideo = false;  ///< Whether this is a video
    bool isGif = false;    ///< Whether this is a GIF
    bool hasAudio = false; ///< Whether video has audio track
    double duration = 0.0; ///< Duration in seconds (video only)
};

/**
 * @brief Reddit native video (v.redd.it) support
 */
class RedditVideo : public QObject
{
    Q_OBJECT

public:
    using ImageCallback = std::function<void(
        bool success,
        const QVector<ImageInfo> &images,
        const QString &errorMessage
    )>;

    explicit RedditVideo(QObject *parent = nullptr);
    ~RedditVideo() override;

    virtual bool canHandleUrl(const QUrl &url) const;
    virtual QString hostName() const;

    virtual void fetchImages(const QUrl &url,
                              ImageCallback callback);
    virtual void fetchImageInfo(const QUrl &url,
                                 ImageCallback callback);
    virtual QUrl directUrl(const QUrl &url) const;

    void setApiKey(const QString &key);
    QString apiKey() const;

    void setEnabled(bool enabled);
    bool isEnabled() const;

signals:
    void imageFetched(const QUrl &url, const ImageInfo &info);
    void fetchError(const QUrl &url, const QString &error);

protected:
    QString m_apiKey;
    bool m_enabled = true;
};

} // namespace PinkReader
