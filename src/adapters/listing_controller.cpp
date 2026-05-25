/*
 * PinkReader - GPLv3
 * File: listing_controller.cpp - Implementation of Pagination listing controller
 */

#include "adapters/listing_controller.h"
#include "utils/logging.h"
#include <algorithm>

namespace PinkReader {

ListingController::ListingController(QObject *parent) : QAbstractListModel(parent) {}
ListingController::~ListingController() = default;

int ListingController::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;
    QMutexLocker locker(&m_mutex);
    return m_items.size();
}

QVariant ListingController::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();
    QMutexLocker locker(&m_mutex);
    if (index.row() < 0 || index.row() >= m_items.size()) return QVariant();
    return getData(m_items.at(index.row()), role);
}

QHash<int, QByteArray> ListingController::roleNames() const
{
    QHash<int, QByteArray> r;
    r[IdRole] = "id";
    r[TitleRole] = "title";
    r[SubtitleRole] = "subtitle";
    r[ScoreRole] = "score";
    r[AuthorRole] = "author";
    return r;
}

void ListingController::setItems(const QJsonArray &items)
{
    beginResetModel();
    { QMutexLocker l(&m_mutex); m_items.clear(); for (auto &v : items) m_items.append(v.toObject()); }
    endResetModel();
    emit itemsLoaded(m_items.size());
}

void ListingController::addItems(const QJsonArray &items)
{
    if (items.isEmpty()) return;
    int old = m_items.size();
    beginInsertRows(QModelIndex(), old, old + items.size() - 1);
    { QMutexLocker l(&m_mutex); for (auto &v : items) m_items.append(v.toObject()); }
    endInsertRows();
    emit itemsLoaded(m_items.size());
}

void ListingController::clear()
{
    beginResetModel();
    { QMutexLocker l(&m_mutex); m_items.clear(); }
    endResetModel();
    emit itemsCleared();
}

int ListingController::itemCount() const
{
    QMutexLocker l(&m_mutex);
    return m_items.size();
}

bool ListingController::isEmpty() const
{
    QMutexLocker l(&m_mutex);
    return m_items.isEmpty();
}

QVariant ListingController::getData(const QJsonObject &item, int role) const
{
    switch (role) {
        case IdRole: return item["id"].toString();
        case TitleRole: return item["title"].toString();
        case SubtitleRole: return item["subtitle"].toString();
        case IconRole: return item["icon"].toString();
        case ImageRole: return item["image"].toString();
        case ScoreRole: return item["score"].toInt();
        case AuthorRole: return item["author"].toString();
        case TimestampRole: return item["timestamp"].toString();
        case UrlRole: return item["url"].toString();
        case DataRole: return QVariant::fromValue(item);
        case Qt::DisplayRole: return item["title"].toString();
        default: return QVariant();
    }
}


void ListingController::setAfter(const QString &after) { m_after = after; }
QString ListingController::after() const { return m_after; }
bool ListingController::hasMore() const { return m_hasMore; }
void ListingController::setSortOrder(const QString &order) { m_sortOrder = order; }
QString ListingController::sortOrder() const { return m_sortOrder; }
int ListingController::currentPage() const { return m_currentPage; }
void ListingController::resetPagination() { m_after.clear(); m_currentPage = 0; m_hasMore = true; }

} // namespace PinkReader
