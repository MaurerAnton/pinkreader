#include "subreddit_model.hpp"

namespace PinkReader {

SubredditModel::SubredditModel(QObject* parent) : QAbstractListModel(parent) {}

int SubredditModel::rowCount(const QModelIndex& parent) const {
    return parent.isValid() ? 0 : m_names.size();
}

QVariant SubredditModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid())
        return {};
    if (role == NameRole)
        return m_names[index.row()];
    return {};
}

QHash<int, QByteArray> SubredditModel::roleNames() const {
    return {{NameRole, "name"}};
}

void SubredditModel::setSubreddits(const QStringList& names) {
    beginResetModel();
    m_names = names;
    endResetModel();
}

}  // namespace PinkReader
