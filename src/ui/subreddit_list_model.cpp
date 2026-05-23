#include "subreddit_list_model.hpp"

#include <QDateTime>
#include <algorithm>

namespace PinkReader {

SubredditListModel::SubredditListModel(QObject* parent) : QAbstractListModel(parent) {}

int SubredditListModel::rowCount(const QModelIndex& parent) const {
    return parent.isValid() ? 0 : m_items.size();
}

void SubredditListModel::setSortBy(const QString& sort) {
    if (m_sortBy != sort) {
        m_sortBy = sort;
        emit sortByChanged();
        resort();
    }
}

QVariant SubredditListModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() >= m_items.size())
        return {};
    const auto& item = m_items[index.row()];
    const auto& sr = item.sr;

    switch (role) {
        case NameRole:
            return sr.name;
        case TitleRole:
            return sr.title.isEmpty() ? sr.name : sr.title;
        case SubscribersRole:
            return sr.subscriberCount;
        case ActiveUsersRole:
            return sr.activeUserCount;
        case PostsPerDayRole:
            return item.postsPerDay;
        case DescriptionRole:
            return sr.publicDescription;
        case IconRole:
            return sr.iconUrl.toString();
        case Over18Role:
            return sr.over18;
        case CreatedRole:
            return sr.createdUtc.toString(Qt::ISODate);
        default:
            return {};
    }
}

QHash<int, QByteArray> SubredditListModel::roleNames() const {
    return {{NameRole, "name"},
            {TitleRole, "title"},
            {SubscribersRole, "subscribers"},
            {ActiveUsersRole, "activeUsers"},
            {PostsPerDayRole, "postsPerDay"},
            {DescriptionRole, "description"},
            {IconRole, "iconUrl"},
            {Over18Role, "over18"},
            {CreatedRole, "created"}};
}

void SubredditListModel::setSubreddits(const QVector<Subreddit>& subreddits) {
    beginResetModel();
    m_items.clear();
    m_items.reserve(subreddits.size());

    auto now = QDateTime::currentSecsSinceEpoch();
    for (const auto& sr : subreddits) {
        Item item;
        item.sr = sr;

        // Estimate posts per day from active users
        // activeUserCount is a rough proxy; actual p/day ≈ activeUsers * 0.3-0.5
        // For display, we use activeUserCount as "activity level"
        qint64 age = now - sr.createdUtc.toSecsSinceEpoch();
        qint64 ageDays = qMax(age / 86400, 1LL);
        item.postsPerDay = qreal(sr.activeUserCount) / qreal(ageDays);

        m_items.append(item);
    }

    resort();
    endResetModel();
    emit countChanged();
}

void SubredditListModel::clear() {
    beginResetModel();
    m_items.clear();
    endResetModel();
    emit countChanged();
}

void SubredditListModel::resort() {
    if (m_items.isEmpty())
        return;

    if (m_sortBy == "relevance")
        return;  // keep API order

    std::sort(m_items.begin(), m_items.end(), [this](const Item& a, const Item& b) {
        if (m_sortBy == "subscribers")
            return a.sr.subscriberCount > b.sr.subscriberCount;
        if (m_sortBy == "active")
            return a.sr.activeUserCount > b.sr.activeUserCount;
        if (m_sortBy == "posts")
            return a.postsPerDay > b.postsPerDay;
        if (m_sortBy == "name")
            return a.sr.name.toLower() < b.sr.name.toLower();
        return false;
    });

    emit dataChanged(index(0), index(m_items.size() - 1));
}

}  // namespace PinkReader
