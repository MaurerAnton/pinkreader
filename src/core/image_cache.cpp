#include "image_cache.hpp"

#include <QCryptographicHash>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>
#include <QUrl>

namespace PinkReader {

ImageCache::ImageCache(const QString& cacheDir, QObject* parent)
    : QObject(parent), m_cacheDir(cacheDir), m_nam(new QNetworkAccessManager(this)) {
    QDir().mkpath(m_cacheDir);
    m_nam->setRedirectPolicy(QNetworkRequest::NoLessSafeRedirectPolicy);
}

QString ImageCache::hashUrl(const QString& url) const {
    return QString::fromUtf8(QCryptographicHash::hash(url.toUtf8(), QCryptographicHash::Sha256).toHex());
}

QString ImageCache::localPath(const QString& url) const {
    return m_cacheDir + "/" + hashUrl(url);
}

bool ImageCache::isCached(const QString& url) const {
    QString path = localPath(url);
    QFileInfo fi(path);
    return fi.exists() && fi.size() > 0;
}

QString ImageCache::cacheUrl(const QString& remoteUrl) {
    if (remoteUrl.isEmpty())
        return {};
    if (remoteUrl.startsWith("image://"))
        return remoteUrl;
    if (remoteUrl.startsWith("qrc:"))
        return remoteUrl;

    QString local = localPath(remoteUrl);

    if (!isCached(remoteUrl)) {
        downloadImage(remoteUrl);
    }

    return "image://pink/" + remoteUrl;
}

void ImageCache::prefetch(const QString& url) {
    if (url.isEmpty() || isCached(url))
        return;
    if (m_activeDownloads.contains(url))
        return;
    downloadImage(url);
}

void ImageCache::clear() {
    QMutexLocker lock(&m_mutex);
    QDir dir(m_cacheDir);
    dir.removeRecursively();
    QDir().mkpath(m_cacheDir);
}

qint64 ImageCache::currentSize() const {
    qint64 total = 0;
    QDir dir(m_cacheDir);
    for (const auto& entry : dir.entryInfoList(QDir::Files)) {
        total += entry.size();
    }
    return total;
}

void ImageCache::downloadImage(const QString& url) {
    if (m_activeDownloads.contains(url))
        return;

    QNetworkRequest req{QUrl{url}};
    req.setRawHeader("User-Agent", "PinkReader/0.1");

    auto* reply = m_nam->get(req);
    m_activeDownloads.insert(url, reply);

    connect(reply, &QNetworkReply::finished, this, [this, reply, url]() {
        reply->deleteLater();
        m_activeDownloads.remove(url);

        if (reply->error() != QNetworkReply::NoError) {
            emit downloadError(url, reply->errorString());
            return;
        }

        QByteArray data = reply->readAll();
        if (data.isEmpty()) {
            emit downloadError(url, "Empty response");
            return;
        }

        QString path = localPath(url);
        QFile file(path);
        if (file.open(QIODevice::WriteOnly)) {
            file.write(data);
            file.close();
            emit imageCached(url, path);

            // Evict if over limit
            if (currentSize() > m_maxCacheSize) {
                evictOldest(m_maxCacheSize * 0.8);
            }
        } else {
            emit downloadError(url, "Failed to write: " + file.errorString());
        }
    });

    connect(reply, &QNetworkReply::downloadProgress, this,
            [this, url](qint64 received, qint64 total) { emit downloadProgress(url, received, total); });
}

void ImageCache::evictOldest(qint64 targetSize) {
    QDir dir(m_cacheDir);
    auto entries = dir.entryInfoList(QDir::Files, QDir::Time | QDir::Reversed);
    qint64 total = currentSize();

    for (const auto& entry : entries) {
        if (total <= targetSize)
            break;
        total -= entry.size();
        QFile::remove(entry.absoluteFilePath());
    }
}

QImage ImageCache::loadImage(const QString& url) {
    if (!isCached(url))
        return {};
    return QImage(localPath(url));
}

QPixmap ImageCache::loadPixmap(const QString& url) {
    if (!isCached(url))
        return {};
    return QPixmap(localPath(url));
}

// --- PinkImageProvider ---

PinkImageProvider::PinkImageProvider(ImageCache* cache)
    : QQuickImageProvider(QQmlImageProviderBase::Image), m_cache(cache) {}

QImage PinkImageProvider::requestImage(const QString& id, QSize* size, const QSize& requestedSize) {
    QImage img = m_cache->loadImage(id);
    if (img.isNull()) {
        m_cache->prefetch(id);
        return {};
    }
    if (size)
        *size = img.size();
    if (requestedSize.isValid()) {
        return img.scaled(requestedSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    return img;
}

QPixmap PinkImageProvider::requestPixmap(const QString& id, QSize* size, const QSize& requestedSize) {
    QPixmap pix = m_cache->loadPixmap(id);
    if (pix.isNull()) {
        m_cache->prefetch(id);
        return {};
    }
    if (size)
        *size = pix.size();
    if (requestedSize.isValid()) {
        return pix.scaled(requestedSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }
    return pix;
}

}  // namespace PinkReader
