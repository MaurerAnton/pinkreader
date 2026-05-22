#pragma once

#include <QObject>
#include <QQuickImageProvider>
#include <QImage>
#include <QPixmap>
#include <QHash>
#include <QMutex>
#include <QNetworkAccessManager>
#include <QNetworkReply>

namespace PinkReader {

class ImageCache : public QObject {
    Q_OBJECT
public:
    explicit ImageCache(const QString& cacheDir, QObject* parent = nullptr);

    QString cacheDir() const { return m_cacheDir; }
    void setMaxSize(qint64 bytes) { m_maxCacheSize = bytes; }
    qint64 maxSize() const { return m_maxCacheSize; }
    qint64 currentSize() const;

    QString localPath(const QString& url) const;
    bool isCached(const QString& url) const;

    Q_INVOKABLE QString cacheUrl(const QString& remoteUrl);
    Q_INVOKABLE void prefetch(const QString& url);
    Q_INVOKABLE void clear();
    Q_INVOKABLE qint64 sizeBytes() const { return currentSize(); }

    QImage loadImage(const QString& url);
    QPixmap loadPixmap(const QString& url);

signals:
    void imageCached(const QString& url, const QString& localPath);
    void downloadError(const QString& url, const QString& error);
    void downloadProgress(const QString& url, qint64 received, qint64 total);

private:
    void downloadImage(const QString& url);
    QString hashUrl(const QString& url) const;
    void evictOldest(qint64 targetSize);

    QString m_cacheDir;
    qint64 m_maxCacheSize = 200 * 1024 * 1024; // 200 MB
    QNetworkAccessManager* m_nam;
    QHash<QString, QNetworkReply*> m_activeDownloads;
    mutable QMutex m_mutex;
};

class PinkImageProvider : public QQuickImageProvider {
public:
    explicit PinkImageProvider(ImageCache* cache);
    QImage requestImage(const QString& id, QSize* size,
                        const QSize& requestedSize) override;
    QPixmap requestPixmap(const QString& id, QSize* size,
                          const QSize& requestedSize) override;
private:
    ImageCache* m_cache;
};

} // namespace PinkReader
