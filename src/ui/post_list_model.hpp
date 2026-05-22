#pragma once

#include <QAbstractListModel>
#include <QVector>
#include "../core/models.hpp"

namespace PinkReader {

class PostListModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(bool hasMore READ hasMore NOTIFY hasMoreChanged)
    
public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        TitleRole,
        AuthorRole,
        SubredditRole,
        ScoreRole,
        CommentCountRole,
        ThumbnailRole,
        SelfTextRole,
        PermalinkRole,
        UrlRole,
        DomainRole,
        CreatedRole,
        NsfwRole,
        SpoilerRole,
        StickiedRole,
        VoteStateRole,
        SavedRole,
        TypeRole,
        FlairRole,
        GildedRole
    };
    
    explicit PostListModel(QObject* parent = nullptr);
    
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    
    int count() const { return m_posts.size(); }
    bool hasMore() const { return m_hasMore; }
    
    void setPosts(const QVector<Post>& posts, const QString& after);
    void appendPosts(const QVector<Post>& posts, const QString& after);
    void clear();
    Post* postAt(int index);
    
signals:
    void countChanged();
    void hasMoreChanged();
    
private:
    QVector<Post> m_posts;
    QString m_after;
    bool m_hasMore = false;
};

} // namespace PinkReader
