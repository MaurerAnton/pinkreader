#pragma once

#include <QObject>
#include <QUrl>
#include <QNetworkAccessManager>
#include <functional>

namespace PinkReader {

class ContentResolver : public QObject {
    Q_OBJECT
public:
    using MediaCallback = std::function<void(bool success, const QByteArray& data, const QString& contentType)>;
    
    explicit ContentResolver(QObject* parent = nullptr);
    
    void setPreferLowBandwidth(bool low) { m_lowBandwidth = low; }
    void setProxyUrl(const QString& proxy) { m_proxyUrl = proxy; }
    
    QString resolveImageUrl(const QString& originalUrl);
    QString resolveVideoUrl(const QString& originalUrl);
    
    void prefetchThumbnail(const QString& url);
    void fetchMedia(const QString& url, MediaCallback callback);
    
    QString resolveGifvToMp4(const QString& imgurUrl);
    QString resolveRedditVideo(const QString& url);
    bool isSupportedImage(const QString& url);
    bool isSupportedVideo(const QString& url);
    
signals:
    void thumbnailReady(const QString& originalUrl, const QString& cachedPath);
    
private:
    QNetworkAccessManager* m_nam;
    bool m_lowBandwidth = false;
    QString m_proxyUrl;
};

} // namespace PinkReader
