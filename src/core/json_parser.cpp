#include "json_parser.hpp"

#include <QDateTime>
#include <QRegularExpression>
#include <QUrl>

namespace PinkReader {
namespace JsonParser {

static inline QString safeString(const QJsonObject& obj, const QString& key) {
    return obj.contains(key) && !obj[key].isNull() ? obj[key].toString() : QString{};
}

static inline int safeInt(const QJsonObject& obj, const QString& key) {
    return obj.contains(key) ? obj[key].toInt() : 0;
}

static inline bool safeBool(const QJsonObject& obj, const QString& key) {
    return obj.contains(key) && obj[key].toBool();
}

static inline double safeDouble(const QJsonObject& obj, const QString& key) {
    return obj.contains(key) ? obj[key].toDouble() : 0.0;
}

static PostType detectPostType(const QJsonObject& obj) {
    if (safeBool(obj, "is_self"))
        return PostType::Self;
    if (safeBool(obj, "is_gallery"))
        return PostType::Gallery;
    if (safeBool(obj, "is_video"))
        return PostType::Video;

    QString url = safeString(obj, "url");
    QString domain = safeString(obj, "domain");
    QString hint = safeString(obj, "post_hint");

    if (hint == "image" || url.endsWith(".jpg") || url.endsWith(".png") || url.endsWith(".gif") ||
        url.endsWith(".webp"))
        return PostType::Image;
    if (hint == "hosted:video" || hint == "rich:video" || domain == "v.redd.it")
        return PostType::Video;
    if (hint == "link" || !url.isEmpty())
        return PostType::Link;

    return PostType::Unknown;
}

Post parsePost(const QJsonObject& data) {
    Post p;
    p.id = safeString(data, "id");
    p.fullname = safeString(data, "name");
    p.title = safeString(data, "title");
    p.selftext = safeString(data, "selftext");
    p.selftextHtml = safeString(data, "selftext_html");
    p.author = safeString(data, "author");
    p.subreddit = safeString(data, "subreddit");
    p.subredditId = safeString(data, "subreddit_id");
    p.permalink = safeString(data, "permalink");
    p.url = QUrl(safeString(data, "url"));
    p.thumbnailUrl = QUrl(safeString(data, "thumbnail"));
    p.type = detectPostType(data);
    p.score = safeInt(data, "score");
    p.upvoteRatio = static_cast<int>(safeDouble(data, "upvote_ratio") * 100);
    p.commentCount = safeInt(data, "num_comments");
    p.saved = safeBool(data, "saved");
    p.hidden = safeBool(data, "hidden");
    p.stickied = safeBool(data, "stickied");
    p.locked = safeBool(data, "locked");
    p.nsfw = safeBool(data, "over_18");
    p.spoiler = safeBool(data, "spoiler");
    p.isSelf = safeBool(data, "is_self");
    p.isGallery = safeBool(data, "is_gallery");
    p.isVideo = safeBool(data, "is_video");
    p.domain = safeString(data, "domain");
    p.flairText = safeString(data, "link_flair_text");
    p.distinguished = safeString(data, "distinguished");
    p.gildedCount = safeInt(data, "gilded");

    double created = safeDouble(data, "created_utc");
    p.createdUtc = QDateTime::fromSecsSinceEpoch(static_cast<qint64>(created), Qt::UTC);

    double edited = safeDouble(data, "edited");
    p.editedUtc = edited > 0 ? QDateTime::fromSecsSinceEpoch(static_cast<qint64>(edited), Qt::UTC) : QDateTime{};

    if (data.contains("likes")) {
        if (data["likes"].isNull())
            p.voteState = VoteState::None;
        else if (data["likes"].toBool())
            p.voteState = VoteState::Upvoted;
        else
            p.voteState = VoteState::Downvoted;
    }

    // Gallery images
    if (data.contains("gallery_data") && data.contains("media_metadata")) {
        QJsonObject gallery = data["gallery_data"].toObject();
        QJsonArray items = gallery["items"].toArray();
        QJsonObject meta = data["media_metadata"].toObject();
        for (const auto& item : items) {
            QString mediaId = item.toObject()["media_id"].toString();
            if (meta.contains(mediaId)) {
                QJsonObject mediaObj = meta[mediaId].toObject();
                QJsonArray previews = mediaObj["p"].toArray();
                if (!previews.isEmpty()) {
                    QJsonObject lastPreview = previews.last().toObject();
                    MediaPreview mp;
                    mp.url = QUrl(QString::fromUtf8(QByteArray::fromBase64(lastPreview["u"].toString().toUtf8())));
                    mp.width = lastPreview["x"].toInt();
                    mp.height = lastPreview["y"].toInt();
                    p.galleryImages.append(mp);
                }
            }
        }
    }

    // Preview image
    if (data.contains("preview")) {
        QJsonObject preview = data["preview"].toObject();
        QJsonArray images = preview["images"].toArray();
        if (!images.isEmpty()) {
            QJsonObject firstImage = images[0].toObject();
            QJsonArray resolutions = firstImage["resolutions"].toArray();
            if (!resolutions.isEmpty()) {
                QJsonObject lastRes = resolutions.last().toObject();
                p.previewImage.url = QUrl(QString::fromUtf8(QByteArray::fromBase64(lastRes["u"].toString().toUtf8())));
                p.previewImage.width = lastRes["width"].toInt(1080);
                p.previewImage.height = lastRes["height"].toInt(1080);
            }
        }
    }

    // Video URL from secure media
    if (data.contains("secure_media")) {
        QJsonObject secureMedia = data["secure_media"].toObject();
        if (secureMedia.contains("reddit_video")) {
            QJsonObject redditVideo = secureMedia["reddit_video"].toObject();
            p.videoUrl = QString::fromUtf8(QByteArray::fromBase64(redditVideo["hls_url"].toString().toUtf8()));
            if (p.videoUrl.isEmpty()) {
                p.videoUrl = QString::fromUtf8(QByteArray::fromBase64(redditVideo["dash_url"].toString().toUtf8()));
            }
            if (p.videoUrl.isEmpty()) {
                p.videoUrl = redditVideo["fallback_url"].toString();
            }
        }
    }

    return p;
}

Comment parseComment(const QJsonObject& data, int depth) {
    Comment c;
    c.id = safeString(data, "id");
    c.fullname = safeString(data, "name");
    c.parentId = safeString(data, "parent_id");
    c.linkId = safeString(data, "link_id");
    c.author = safeString(data, "author");
    c.body = safeString(data, "body");
    c.bodyHtml = safeString(data, "body_html");
    c.score = safeInt(data, "score");
    c.saved = safeBool(data, "saved");
    c.stickied = safeBool(data, "stickied");
    c.collapsed = safeBool(data, "collapsed");
    c.isSubmitter = safeBool(data, "is_submitter");
    c.depth = depth;
    c.distinguished = safeString(data, "distinguished");
    c.gildedCount = safeInt(data, "gilded");

    double created = safeDouble(data, "created_utc");
    c.createdUtc = QDateTime::fromSecsSinceEpoch(static_cast<qint64>(created), Qt::UTC);

    if (data.contains("likes")) {
        if (data["likes"].isNull())
            c.voteState = VoteState::None;
        else if (data["likes"].toBool())
            c.voteState = VoteState::Upvoted;
        else
            c.voteState = VoteState::Downvoted;
    }

    double edited = safeDouble(data, "edited");
    c.editedUtc = edited > 0 ? QDateTime::fromSecsSinceEpoch(static_cast<qint64>(edited), Qt::UTC) : QDateTime{};

    // Recursively parse replies
    if (data.contains("replies") && !data["replies"].isNull()) {
        QJsonObject replies = data["replies"].toObject();
        if (replies["kind"].toString() == "Listing") {
            QJsonObject repliesData = replies["data"].toObject();
            QJsonArray children = repliesData["children"].toArray();
            for (const auto& child : children) {
                QJsonObject childObj = child.toObject();
                if (childObj["kind"].toString() == "t1") {
                    c.replies.append(parseComment(childObj["data"].toObject(), depth + 1));
                }
            }
        }
    }

    return c;
}

Subreddit parseSubreddit(const QJsonObject& data) {
    Subreddit sr;
    sr.id = safeString(data, "id");
    sr.name = safeString(data, "display_name");
    sr.title = safeString(data, "title");
    sr.description = safeString(data, "description");
    sr.publicDescription = safeString(data, "public_description");
    sr.headerTitle = safeString(data, "header_title");
    sr.subscriberCount = safeInt(data, "subscribers");
    sr.activeUserCount = safeInt(data, "active_user_count");
    sr.over18 = safeBool(data, "over18");
    sr.userIsSubscriber = safeBool(data, "user_is_subscriber");

    double created = safeDouble(data, "created_utc");
    sr.createdUtc = QDateTime::fromSecsSinceEpoch(static_cast<qint64>(created), Qt::UTC);

    QString icon = safeString(data, "icon_img");
    if (!icon.isEmpty())
        sr.iconUrl = QUrl(icon);

    QString banner = safeString(data, "banner_img");
    if (!banner.isEmpty())
        sr.bannerUrl = QUrl(banner);

    sr.submitText = safeString(data, "submit_text");

    return sr;
}

Listing parseListing(const QJsonDocument& doc) {
    Listing listing;
    QJsonObject root = doc.object();
    listing.kind = root["kind"].toString();

    if (root["kind"] == "Listing") {
        QJsonObject data = root["data"].toObject();
        listing.before = data["before"].toString();
        listing.after = data["after"].toString();
        listing.count = static_cast<int>(data["dist"].toDouble());

        QJsonArray children = data["children"].toArray();
        for (const auto& child : children) {
            QJsonObject childObj = child.toObject();
            QString kind = childObj["kind"].toString();
            QJsonObject childData = childObj["data"].toObject();

            if (kind == "t3") {
                listing.posts.append(parsePost(childData));
            } else if (kind == "t1") {
                listing.comments.append(parseComment(childData));
            } else if (kind == "t5") {
                listing.subreddits.append(parseSubreddit(childData));
            }
        }
    }

    return listing;
}

}  // namespace JsonParser
}  // namespace PinkReader
