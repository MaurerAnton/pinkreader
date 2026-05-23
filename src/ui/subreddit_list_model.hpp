#pragma once

#include "../core/models.hpp"

#include <QAbstractListModel>
#include <QSortFilterProxyModel>
#include <QVector>

namespace PinkReader {

class SubredditListModel : public QAbstractListModel {
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(QString sortBy READ sortBy WRITE setSortBy NOTIFY sortByChanged)

public:
    enum Roles {
        NameRole = Qt::UserRole + 1,
        TitleRole,
        SubscribersRole,
        ActiveUsersRole,
        PostsPerDayRole,
        DescriptionRole,
        IconRole,
        Over18Role,
        CreatedRole
    };

    enum SortMode { SortRelevance, SortSubscribers, SortActiveUsers, SortPostsPerDay, SortName };
    Q_ENUM(SortMode)

    explicit SubredditListModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    int count() const { return m_items.size(); }
    QString sortBy() const { return m_sortBy; }
    void setSortBy(const QString& sort);

    void setSubreddits(const QVector<Subreddit>& subreddits);
    void clear();

signals:
    void countChanged();
    void sortByChanged();

private:
    void resort();

    struct Item {
        Subreddit sr;
        qreal postsPerDay = 0;
    };

    QVector<Item> m_items;
    QString m_sortBy = "relevance";
};

}  // namespace PinkReader
