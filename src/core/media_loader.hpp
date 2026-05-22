#pragma once

#include <QObject>
#include <QUrl>
#include "../core/models.hpp"

namespace PinkReader {

struct MediaInfo {
    enum Type { Unknown, Image, Gif, Video, Gallery, YouTube, Imgur };

    Type type = Unknown;
    QUrl url;
    QUrl thumbnailUrl;
    QString videoUrl;   // mp4/hls for playback
    int width = 0;
    int height = 0;
    bool isLooping = false;
    bool isMuted = true;
    QVector<QUrl> galleryUrls;
    QVector<int> galleryWidths;
    QVector<int> galleryHeights;
};

class MediaLoader : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool preferLowBandwidth READ preferLowBandwidth WRITE setPreferLowBandwidth NOTIFY preferLowBandwidthChanged)

public:
    explicit MediaLoader(QObject* parent = nullptr);

    bool preferLowBandwidth() const { return m_lowBandwidth; }
    void setPreferLowBandwidth(bool low);

    Q_INVOKABLE MediaInfo resolveMedia(const QString& url, const QString& domain,
                                       const QString& postHint, bool isGallery,
                                       bool isVideo, bool isSelf);

    Q_INVOKABLE QUrl previewUrl(const QString& url, const QString& thumbnail);
    Q_INVOKABLE bool isVideo(const QString& url, const QString& domain);
    Q_INVOKABLE bool isImage(const QString& url, const QString& domain, const QString& postHint);
    Q_INVOKABLE bool isGif(const QString& url, const QString& domain);

    // Extract video URL from various services
    Q_INVOKABLE QString extractVideoUrl(const QString& url, const QString& domain);
    Q_INVOKABLE QString youtubeThumbnail(const QString& url);
    Q_INVOKABLE QString imgurDirectUrl(const QString& url);
    Q_INVOKABLE QString redditVideoFallback(const QString& videoUrl, const QString& dashUrl, const QString& hlsUrl);

    // Get best playable URL for video (resolves mp4/stream)
    Q_INVOKABLE QUrl playableVideoUrl(const QString& postUrl, const QString& domain,
                                      const QString& videoFallbackUrl, const QString& hlsUrl,
                                      const QString& dashUrl);

signals:
    void preferLowBandwidthChanged();

private:
    bool m_lowBandwidth = false;
};

} // namespace PinkReader

Q_DECLARE_METATYPE(PinkReader::MediaInfo)
