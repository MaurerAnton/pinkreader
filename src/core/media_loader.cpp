#include "media_loader.hpp"

#include <QRegularExpression>
#include <QUrlQuery>

namespace PinkReader {

MediaLoader::MediaLoader(QObject* parent) : QObject(parent) {}

void MediaLoader::setPreferLowBandwidth(bool low) {
    if (m_lowBandwidth != low) {
        m_lowBandwidth = low;
        emit preferLowBandwidthChanged();
    }
}

MediaInfo MediaLoader::resolveMedia(const QString& urlStr, const QString& domain,
                                     const QString& postHint, bool isGallery,
                                     bool isVideo, bool isSelf) {
    MediaInfo info;
    info.url = QUrl(urlStr);

    if (isSelf) {
        info.type = MediaInfo::Unknown;
        return info;
    }
    if (isGallery) {
        info.type = MediaInfo::Gallery;
        return info;
    }
    if (isVideo) {
        info.type = MediaInfo::Video;
        info.videoUrl = urlStr;
        return info;
    }

    // Detect by post_hint
    if (postHint == "image") {
        info.type = MediaInfo::Image;
    } else if (postHint == "hosted:video" || postHint == "rich:video") {
        info.type = MediaInfo::Video;
    } else if (postHint == "link") {
        info.type = isGif(urlStr, domain) ? MediaInfo::Gif : MediaInfo::Image;
    } else {
        // Detect by URL/domain patterns
        if (isGif(urlStr, domain)) {
            info.type = MediaInfo::Gif;
        } else if (isImage(urlStr, domain, postHint)) {
            info.type = MediaInfo::Image;
        } else if (domain.contains("youtube.com") || domain.contains("youtu.be")) {
            info.type = MediaInfo::YouTube;
        } else if (domain.contains("imgur.com")) {
            info.type = MediaInfo::Imgur;
        } else if (domain.contains("v.redd.it")) {
            info.type = MediaInfo::Video;
        }
    }

    // Generate thumbnail
    if (info.type == MediaInfo::YouTube) {
        info.thumbnailUrl = QUrl(youtubeThumbnail(urlStr));
    } else if (info.type == MediaInfo::Imgur) {
        info.url = QUrl(imgurDirectUrl(urlStr));
    } else if (info.type == MediaInfo::Video && domain.contains("v.redd.it")) {
        info.thumbnailUrl = QUrl(urlStr + "/DASH_720.mp4");
    } else if (info.type == MediaInfo::Gif) {
        info.url = QUrl(extractVideoUrl(urlStr, domain));
        info.isLooping = true;
        info.isMuted = true;
    }

    return info;
}

QUrl MediaLoader::previewUrl(const QString& url, const QString& thumbnail) {
    if (!thumbnail.isEmpty() &&
        thumbnail != "self" && thumbnail != "default" &&
        thumbnail != "nsfw" && thumbnail != "spoiler") {
        return QUrl(thumbnail);
    }
    return QUrl(url);
}

bool MediaLoader::isVideo(const QString& url, const QString& domain) {
    if (domain.contains("v.redd.it")) return true;
    if (domain.contains("youtube.com") || domain.contains("youtu.be")) return true;
    if (url.endsWith(".mp4", Qt::CaseInsensitive)) return true;
    if (url.endsWith(".webm", Qt::CaseInsensitive)) return true;
    if (url.endsWith(".gifv", Qt::CaseInsensitive)) return true;
    return false;
}

bool MediaLoader::isImage(const QString& url, const QString& domain,
                           const QString& postHint) {
    if (postHint == "image") return true;
    if (domain.contains("i.redd.it")) return true;
    if (domain.contains("i.imgur.com")) return true;
    if (domain.contains("preview.redd.it")) return true;

    static const QStringList imageExts = {
        ".jpg", ".jpeg", ".png", ".webp", ".bmp", ".svg"
    };
    for (const auto& ext : imageExts) {
        if (url.endsWith(ext, Qt::CaseInsensitive)) return true;
    }
    return false;
}

bool MediaLoader::isGif(const QString& url, const QString& domain) {
    if (url.endsWith(".gif", Qt::CaseInsensitive)) return true;
    if (url.contains(".gifv")) return true;
    if (domain == "gfycat.com" || domain == "www.gfycat.com") return true;
    if (domain == "redgifs.com" || domain == "www.redgifs.com") return true;
    return false;
}

QString MediaLoader::extractVideoUrl(const QString& url, const QString& domain) {
    // imgur gifv -> mp4
    if (domain.contains("imgur.com")) {
        QString fixed = url;
        fixed.replace(".gifv", ".mp4");
        return fixed;
    }

    // reddit video
    if (domain.contains("v.redd.it")) {
        return url;
    }

    // generic
    return url;
}

QString MediaLoader::youtubeThumbnail(const QString& url) {
    QRegularExpression re("(?:youtube\\.com/watch\\?v=|youtu\\.be/|youtube\\.com/embed/)([\\w-]{11})");
    auto m = re.match(url);
    if (m.hasMatch()) {
        return "https://img.youtube.com/vi/" + m.captured(1) + "/mqdefault.jpg";
    }
    return {};
}

QString MediaLoader::imgurDirectUrl(const QString& url) {
    QString fixed = url;
    fixed.replace("imgur.com/", "i.imgur.com/");
    fixed.replace(".gifv", ".mp4");
    if (!fixed.contains('.')) fixed += ".jpg";
    return fixed;
}

QString MediaLoader::redditVideoFallback(const QString& videoUrl,
                                          const QString& dashUrl,
                                          const QString& hlsUrl) {
    if (!videoUrl.isEmpty()) return videoUrl;
    if (!dashUrl.isEmpty()) return QString::fromUtf8(QByteArray::fromBase64(dashUrl.toUtf8()));
    if (!hlsUrl.isEmpty()) return QString::fromUtf8(QByteArray::fromBase64(hlsUrl.toUtf8()));
    return {};
}

QUrl MediaLoader::playableVideoUrl(const QString& postUrl, const QString& domain,
                                    const QString& videoFallbackUrl, const QString& hlsUrl,
                                    const QString& dashUrl) {
    // v.redd.it: try fallback mp4 first
    if (domain.contains("v.redd.it")) {
        QString fallback = QString::fromUtf8(
            QByteArray::fromBase64(videoFallbackUrl.toUtf8()));
        if (!fallback.isEmpty()) return QUrl(fallback);

        // Try DASH -> might not play in Qt, but try
        QString dash = QString::fromUtf8(
            QByteArray::fromBase64(dashUrl.toUtf8()));
        if (!dash.isEmpty()) return QUrl(dash);

        // Try HLS
        QString hls = QString::fromUtf8(
            QByteArray::fromBase64(hlsUrl.toUtf8()));
        if (!hls.isEmpty()) return QUrl(hls);

        return QUrl(postUrl + "/DASH_720.mp4");
    }

    // imgur gifv -> mp4
    if (postUrl.contains(".gifv")) {
        QString mp4 = postUrl;
        mp4.replace(".gifv", ".mp4");
        return QUrl(mp4);
    }

    // gfycat / redgifs
    if (domain.contains("gfycat.com") || domain.contains("redgifs.com")) {
        return QUrl(postUrl);
    }

    // YouTube embed URL
    if (domain.contains("youtube.com") || domain.contains("youtu.be")) {
        QRegularExpression re("(?:watch\\?v=|youtu\\.be/|embed/)([\\w-]{11})");
        auto m = re.match(postUrl);
        if (m.hasMatch()) {
            return QUrl("https://www.youtube.com/embed/" + m.captured(1) + "?autoplay=1");
        }
    }

    // Direct mp4/webm
    if (postUrl.endsWith(".mp4") || postUrl.endsWith(".webm")) {
        return QUrl(postUrl);
    }

    return QUrl(postUrl);
}

} // namespace PinkReader
