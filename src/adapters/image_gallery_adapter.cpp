/*
 * PinkReader - GPLv3
 * File: image_gallery_adapter.cpp - Implementation of Image gallery adapter
 */

#include "adapters/image_gallery_adapter.h"
#include "utils/logging.h"
#include <algorithm>

namespace PinkReader {

ImageGalleryAdapter::ImageGalleryAdapter(QObject *parent) : QAbstractListModel(parent) {}
ImageGalleryAdapter::~ImageGalleryAdapter() = default;

int ImageGalleryAdapter::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;
    QMutexLocker locker(&m_mutex);
    return m_items.size();
}

QVariant ImageGalleryAdapter::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();
    QMutexLocker locker(&m_mutex);
    if (index.row() < 0 || index.row() >= m_items.size()) return QVariant();
    return getData(m_items.at(index.row()), role);
}

QHash<int, QByteArray> ImageGalleryAdapter::roleNames() const
{
    QHash<int, QByteArray> r;
    r[IdRole] = "id";
    r[TitleRole] = "title";
    r[SubtitleRole] = "subtitle";
    r[ScoreRole] = "score";
    r[AuthorRole] = "author";
    return r;
}

void ImageGalleryAdapter::setItems(const QJsonArray &items)
{
    beginResetModel();
    { QMutexLocker l(&m_mutex); m_items.clear(); for (auto &v : items) m_items.append(v.toObject()); }
    endResetModel();
    emit itemsLoaded(m_items.size());
}

void ImageGalleryAdapter::addItems(const QJsonArray &items)
{
    if (items.isEmpty()) return;
    int old = m_items.size();
    beginInsertRows(QModelIndex(), old, old + items.size() - 1);
    { QMutexLocker l(&m_mutex); for (auto &v : items) m_items.append(v.toObject()); }
    endInsertRows();
    emit itemsLoaded(m_items.size());
}

void ImageGalleryAdapter::clear()
{
    beginResetModel();
    { QMutexLocker l(&m_mutex); m_items.clear(); }
    endResetModel();
    emit itemsCleared();
}

int ImageGalleryAdapter::itemCount() const
{
    QMutexLocker l(&m_mutex);
    return m_items.size();
}

bool ImageGalleryAdapter::isEmpty() const
{
    QMutexLocker l(&m_mutex);
    return m_items.isEmpty();
}

QVariant ImageGalleryAdapter::getData(const QJsonObject &item, int role) const
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

} // namespace PinkReader
