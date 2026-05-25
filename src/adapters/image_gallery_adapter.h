/*
 * PinkReader - GPLv3
 * File: image_gallery_adapter.h - Image gallery adapter
 */

#pragma once

#include <QAbstractListModel>
#include <QJsonObject>
#include <QJsonArray>
#include <QVector>
#include <QMutex>
#include <QHash>

namespace PinkReader {

class ImageGalleryAdapter : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        TitleRole, SubtitleRole, IconRole, ImageRole,
        ScoreRole, AuthorRole, TimestampRole, UrlRole,
        DataRole, MetadataRole
    };

    explicit ImageGalleryAdapter(QObject *parent = nullptr);
    ~ImageGalleryAdapter() override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setItems(const QJsonArray &items);
    void addItems(const QJsonArray &items);
    void clear();
    int itemCount() const;
    bool isEmpty() const;

signals:
    void itemsLoaded(int count);
    void itemsCleared();

private:
    QVector<QJsonObject> m_items;
    mutable QMutex m_mutex;
    QVariant getData(const QJsonObject &item, int role) const;
};

} // namespace PinkReader
