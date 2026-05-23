#include "content_resolver.hpp"

#include <QNetworkReply>
#include <QRegularExpression>

namespace PinkReader {

ContentResolver::ContentResolver(QObject* parent) : QObject(parent), m_nam(new QNetworkAccessManager(this)) {}

QString ContentResolver::resolveImageUrl(const QString& originalUrl) {
    QString url = originalUrl;

    // Imgur: replace .gifv with .mp4, handle i.imgur.com
    if (url.contains("imgur.com")) {
        url.replace(".gifv", ".mp4");
        url.replace("imgur.com/", "i.imgur.com/");
        if (!url.contains("."))
            url += ".jpg";
    }

    // Reddit preview images: change &amp; to &
    url.replace("&amp;", "&");

    // Remove Reddit tracking
    QRegularExpression trackRe("[?&]utm_[^&]+");
    url.remove(trackRe);

    return url;
}

QString ContentResolver::resolveVideoUrl(const QString& originalUrl) {
    QString url = originalUrl;

    // v.redd.it -> extract DASH/HLS
    if (url.contains("v.redd.it")) {
        return resolveRedditVideo(url);
    }

    // YouTube: convert to embed URL for preview
    QRegularExpression ytRe("(?:youtube\\.com/watch\\?v=|youtu\\.be/)([\\w-]+)");
    auto match = ytRe.match(url);
    if (match.hasMatch()) {
        return "https://img.youtube.com/vi/" + match.captured(1) + "/hqdefault.jpg";
    }

    return url;
}

QString ContentResolver::resolveGifvToMp4(const QString& imgurUrl) {
    QString url = imgurUrl;
    url.replace(".gifv", ".mp4");
    return url;
}

QString ContentResolver::resolveRedditVideo(const QString& url) {
    // v.redd.it links: video is in post data, not in URL
    // Return audio URL for merging
    QRegularExpression re("v\\.redd\\.it/(\\w+)");
    auto match = re.match(url);
    if (match.hasMatch()) {
        QString id = match.captured(1);
        return "https://v.redd.it/" + id + "/DASHPlaylist.mpd";
    }
    return url;
}

bool ContentResolver::isSupportedImage(const QString& url) {
    static const QStringList exts = {"jpg", "jpeg", "png", "gif", "webp", "bmp", "svg"};
    for (const auto& ext : exts) {
        if (url.endsWith("." + ext, Qt::CaseInsensitive))
            return true;
    }
    return url.contains("i.redd.it") || url.contains("i.imgur.com") || url.contains("preview.redd.it");
}

bool ContentResolver::isSupportedVideo(const QString& url) {
    static const QStringList exts = {"mp4", "webm", "gifv", "mov", "mkv"};
    for (const auto& ext : exts) {
        if (url.endsWith("." + ext, Qt::CaseInsensitive))
            return true;
    }
    return url.contains("v.redd.it") || url.contains("youtube.com") || url.contains("youtu.be");
}

void ContentResolver::prefetchThumbnail(const QString& url) {
    QNetworkRequest req{QUrl{url}};
    req.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);
    auto* reply = m_nam->get(req);
    connect(reply, &QNetworkReply::finished, this, [this, reply, url]() {
        reply->deleteLater();
        if (reply->error() == QNetworkReply::NoError) {
            emit thumbnailReady(url, {});
        }
    });
}

void ContentResolver::fetchMedia(const QString& url, MediaCallback callback) {
    QNetworkRequest req{QUrl{url}};
    req.setRawHeader("User-Agent", "PinkReader/0.1");

    auto* reply = m_nam->get(req);
    connect(reply, &QNetworkReply::finished, this, [reply, callback]() {
        reply->deleteLater();
        if (reply->error() == QNetworkReply::NoError) {
            QString contentType = reply->header(QNetworkRequest::ContentTypeHeader).toString();
            callback(true, reply->readAll(), contentType);
        } else {
            callback(false, {}, {});
        }
    });
}

}  // namespace PinkReader
