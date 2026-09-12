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
 * File: reddit_gallery.h
 * Description: Reddit native gallery/image support
 */

#pragma once

#include "image_host.h"
#include <QObject>
#include <QString>
#include <QUrl>
#include <QJsonObject>
#include <QVector>
#include <functional>

namespace PinkReader {

// ImageInfo is defined once in image_host.h (shared).

/**
 * @brief Reddit native gallery/image support
 */
class RedditGallery : public QObject
{
    Q_OBJECT

public:
    using ImageCallback = std::function<void(
        bool success,
        const QVector<ImageInfo> &images,
        const QString &errorMessage
    )>;

    explicit RedditGallery(QObject *parent = nullptr);
    ~RedditGallery() override;

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
