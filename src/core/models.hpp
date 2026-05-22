#pragma once

#include <QString>
#include <QDateTime>
#include <QJsonObject>
#include <QJsonArray>
#include <QVector>
#include <QUrl>
#include <optional>

namespace PinkReader {

enum class PostType {
    Self,
    Link,
    Image,
    Video,
    Gallery,
    Crosspost,
    Unknown
};

enum class VoteState {
    None,
    Upvoted,
    Downvoted
};

struct MediaPreview {
    QUrl url;
    int width = 0;
    int height = 0;
};

struct Post {
    QString id;
    QString fullname;       // t3_xxxxx
    QString title;
    QString selftext;
    QString selftextHtml;
    QString author;
    QString subreddit;
    QString subredditId;
    QString permalink;
    QUrl url;
    QUrl thumbnailUrl;
    PostType type = PostType::Unknown;
    int score = 0;
    int upvoteRatio = 0;
    int commentCount = 0;
    VoteState voteState = VoteState::None;
    bool saved = false;
    bool hidden = false;
    bool stickied = false;
    bool locked = false;
    bool nsfw = false;
    bool spoiler = false;
    bool isSelf = false;
    bool isGallery = false;
    bool isVideo = false;
    QString domain;
    QDateTime createdUtc;
    QDateTime editedUtc;
    QString flairText;
    QString flairColor;
    QVector<MediaPreview> galleryImages;
    MediaPreview previewImage;
    QString videoUrl;
    int gildedCount = 0;
    QString distinguished;  // "moderator", "admin", "special"
    
    static Post fromJson(const QJsonObject& obj);
    QJsonObject toJson() const;
};

struct Comment {
    QString id;
    QString fullname;       // t1_xxxxx
    QString parentId;       // t1_xxxxx or t3_xxxxx
    QString linkId;         // t3_xxxxx of the post
    QString author;
    QString body;
    QString bodyHtml;
    int score = 0;
    VoteState voteState = VoteState::None;
    bool saved = false;
    bool stickied = false;
    bool collapsed = false;
    bool isSubmitter = false;
    QDateTime createdUtc;
    QDateTime editedUtc;
    int depth = 0;
    QVector<Comment> replies;
    QString distinguished;
    int gildedCount = 0;
    
    static Comment fromJson(const QJsonObject& obj, int depth = 0);
    QJsonObject toJson() const;
};

struct Subreddit {
    QString id;
    QString name;           // "programming" without r/
    QString title;
    QString description;
    QString publicDescription;
    QString headerTitle;
    QUrl iconUrl;
    QUrl bannerUrl;
    int subscriberCount = 0;
    int activeUserCount = 0;
    bool over18 = false;
    bool isDefault = false;
    bool userIsSubscriber = false;
    QDateTime createdUtc;
    QString submitText;
    QStringList moderators;
    
    static Subreddit fromJson(const QJsonObject& obj);
    QJsonObject toJson() const;
};

struct Listing {
    QString kind;           // "Listing"
    QString before;
    QString after;
    int count = 0;
    QVector<Post> posts;
    QVector<Comment> comments;
    QVector<Subreddit> subreddits;
    
    bool hasMore() const { return !after.isEmpty(); }
    static Listing fromJson(const QJsonObject& obj);
};

struct Account {
    QString username;
    QString accessToken;
    QString refreshToken;
    QString tokenType;
    int expiresIn = 0;
    QDateTime tokenExpiry;
    QString scope;
    bool isAnonymous = true;
    
    bool isExpired() const {
        return !isAnonymous && QDateTime::currentDateTimeUtc() > tokenExpiry;
    }
};

enum class SortOrder {
    Hot,
    New,
    Rising,
    Top,
    Controversial,
    Best,
    Relevance
};

enum class TimeRange {
    Hour,
    Day,
    Week,
    Month,
    Year,
    All
};

struct FeedRequest {
    QString subreddit;          // empty = frontpage
    SortOrder sort = SortOrder::Hot;
    TimeRange timeRange = TimeRange::Day;
    QString after;              // pagination token
    int limit = 25;
};

struct CommentRequest {
    QString postId;
    QString commentId;          // focus on this comment
    QString sort;               // "confidence", "top", "new", "controversial", "old", "qa"
    int limit = 200;
    int depth = 10;
};

} // namespace PinkReader

Q_DECLARE_METATYPE(PinkReader::Post)
Q_DECLARE_METATYPE(PinkReader::Comment)
Q_DECLARE_METATYPE(PinkReader::Subreddit)
Q_DECLARE_METATYPE(QVector<PinkReader::Post>)
Q_DECLARE_METATYPE(QVector<PinkReader::Comment>)
Q_DECLARE_METATYPE(QVector<PinkReader::Subreddit>)
