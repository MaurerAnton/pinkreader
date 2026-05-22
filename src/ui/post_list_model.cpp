#include "post_list_model.hpp"

#include <QDateTime>

namespace PinkReader {

PostListModel::PostListModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

int PostListModel::rowCount(const QModelIndex& parent) const {
    return parent.isValid() ? 0 : m_posts.size();
}

QVariant PostListModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() >= m_posts.size()) return {};
    
    const auto& post = m_posts[index.row()];
    
    switch (role) {
    case IdRole:          return post.id;
    case TitleRole:        return post.title;
    case AuthorRole:       return post.author;
    case SubredditRole:    return post.subreddit;
    case ScoreRole:        return post.score;
    case CommentCountRole: return post.commentCount;
    case ThumbnailRole:    return post.thumbnailUrl.toString();
    case SelfTextRole:     return post.selftext;
    case PermalinkRole:    return post.permalink;
    case UrlRole:          return post.url.toString();
    case DomainRole:       return post.domain;
    case CreatedRole:      return post.createdUtc.toString(Qt::ISODate);
    case NsfwRole:         return post.nsfw;
    case SpoilerRole:      return post.spoiler;
    case StickiedRole:     return post.stickied;
    case VoteStateRole:    return static_cast<int>(post.voteState);
    case SavedRole:        return post.saved;
    case TypeRole:         return static_cast<int>(post.type);
    case FlairRole:        return post.flairText;
    case GildedRole:       return post.gildedCount;
    case PostHintRole: {
        // Detect post_hint from type
        if (post.isSelf) return QStringLiteral("self");
        if (post.isGallery) return QStringLiteral("gallery");
        if (post.isVideo) return QStringLiteral("video");
        if (post.type == PostType::Image) return QStringLiteral("image");
        return QString{};
    }
    case IsGalleryRole:    return post.isGallery;
    case IsVideoRole:      return post.isVideo;
    case GalleryArrayRole: {
        QVariantList list;
        for (const auto& img : post.galleryImages) {
            QVariantMap map;
            map["url"] = img.url.toString();
            map["width"] = img.width;
            map["height"] = img.height;
            list.append(map);
        }
        return list;
    }
    default:               return {};
    }
}

QHash<int, QByteArray> PostListModel::roleNames() const {
    return {
        {IdRole, "postId"},
        {TitleRole, "title"},
        {AuthorRole, "author"},
        {SubredditRole, "subreddit"},
        {ScoreRole, "score"},
        {CommentCountRole, "commentCount"},
        {ThumbnailRole, "thumbnail"},
        {SelfTextRole, "selfText"},
        {PermalinkRole, "permalink"},
        {UrlRole, "url"},
        {DomainRole, "domain"},
        {CreatedRole, "created"},
        {NsfwRole, "nsfw"},
        {SpoilerRole, "spoiler"},
        {StickiedRole, "stickied"},
        {VoteStateRole, "voteState"},
        {SavedRole, "saved"},
        {TypeRole, "postType"},
        {FlairRole, "flair"},
        {GildedRole, "gilded"},
        {PostHintRole, "postHint"},
        {IsGalleryRole, "isGallery"},
        {IsVideoRole, "isVideo"},
        {GalleryArrayRole, "galleryArray"}
    };
}

void PostListModel::setPosts(const QVector<Post>& posts, const QString& after) {
    beginResetModel();
    m_posts = posts;
    m_after = after;
    m_hasMore = !after.isEmpty();
    endResetModel();
    emit countChanged();
    emit hasMoreChanged();
}

void PostListModel::appendPosts(const QVector<Post>& posts, const QString& after) {
    if (posts.isEmpty()) return;
    beginInsertRows(QModelIndex(), m_posts.size(), m_posts.size() + posts.size() - 1);
    m_posts.append(posts);
    m_after = after;
    m_hasMore = !after.isEmpty();
    endInsertRows();
    emit countChanged();
    emit hasMoreChanged();
}

void PostListModel::clear() {
    beginResetModel();
    m_posts.clear();
    m_after.clear();
    m_hasMore = false;
    endResetModel();
    emit countChanged();
    emit hasMoreChanged();
}

Post* PostListModel::postAt(int index) {
    if (index >= 0 && index < m_posts.size()) return &m_posts[index];
    return nullptr;
}

} // namespace PinkReader
