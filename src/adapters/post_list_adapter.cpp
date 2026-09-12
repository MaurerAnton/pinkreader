#include "adapters/post_list_adapter.h"

#include <QJsonArray>
#include <QJsonObject>

namespace PinkReader {

PostListAdapter::PostListAdapter(QObject *parent)
    : QAbstractListModel(parent)
{
}

PostListAdapter::~PostListAdapter() = default;

int PostListAdapter::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;
    return m_items.size();
}

QVariant PostListAdapter::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_items.size()) return {};
    return getItemData(m_items[index.row()], role);
}

QHash<int, QByteArray> PostListAdapter::roleNames() const
{
    return {
        {IdRole, "id"},
        {TitleRole, "title"},
        {SubtitleRole, "subtitle"},
        {IconRole, "icon"},
        {ImageRole, "image"},
        {ScoreRole, "score"},
        {AuthorRole, "author"},
        {TimestampRole, "timestamp"},
        {UrlRole, "url"},
        {DataRole, "data"},
        {MetadataRole, "metadata"},
        {TypeRole, "type"},
        {StatusRole, "status"}
    };
}

void PostListAdapter::setItems(const QJsonArray &items)
{
    beginResetModel();
    m_items.clear();
    for (const auto &item : items) m_items.append(item.toObject());
    endResetModel();
    emit itemsLoaded(m_items.size());
}

void PostListAdapter::addItems(const QJsonArray &items)
{
    const int start = m_items.size();
    const int end = start + items.size() - 1;
    beginInsertRows(QModelIndex(), start, end);
    for (const auto &item : items) m_items.append(item.toObject());
    endInsertRows();
    emit itemsAppended(items.size());
}

void PostListAdapter::prependItems(const QJsonArray &items)
{
    beginInsertRows(QModelIndex(), 0, items.size() - 1);
    for (const auto &item : items) m_items.prepend(item.toObject());
    endInsertRows();
    emit itemsAppended(items.size());
}

void PostListAdapter::removeItem(int row)
{
    if (row < 0 || row >= m_items.size()) return;
    beginRemoveRows(QModelIndex(), row, row);
    m_items.removeAt(row);
    endRemoveRows();
}

void PostListAdapter::removeItemById(const QString &id)
{
    const int row = findItemById(id);
    if (row >= 0) removeItem(row);
}

void PostListAdapter::clear()
{
    beginResetModel();
    m_items.clear();
    endResetModel();
    emit itemsCleared();
}

int PostListAdapter::findItemById(const QString &id) const
{
    for (int i = 0; i < m_items.size(); ++i) {
        if (m_items[i].value("id").toString() == id) return i;
    }
    return -1;
}

QJsonObject PostListAdapter::itemAt(int row) const
{
    if (row < 0 || row >= m_items.size()) return {};
    return m_items[row];
}

int PostListAdapter::itemCount() const
{
    return m_items.size();
}

bool PostListAdapter::isEmpty() const
{
    return m_items.isEmpty();
}

void PostListAdapter::sort(int role, Qt::SortOrder order)
{
    (void)role; (void)order;
    // TODO: implement sort
}

void PostListAdapter::filter(const QString &query)
{
    (void)query;
    // TODO: implement filter
}

QVariant PostListAdapter::getItemData(const QJsonObject &item, int role) const
{
    switch (role) {
        case IdRole: return item.value("id");
        case TitleRole: return item.value("title");
        case SubtitleRole: return item.value("subtitle");
        case IconRole: return item.value("icon");
        case ImageRole: return item.value("image");
        case ScoreRole: return item.value("score");
        case AuthorRole: return item.value("author");
        case TimestampRole: return item.value("timestamp");
        case UrlRole: return item.value("url");
        case DataRole: return item.value("data");
        case MetadataRole: return item.value("metadata");
        case TypeRole: return item.value("type");
        case StatusRole: return item.value("status");
    }
    return {};
}

} // namespace PinkReader