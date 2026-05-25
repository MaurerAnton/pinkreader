/*
 * PinkReader - GPLv3
 * File: listing_controller.h - Pagination listing controller
 */

#pragma once

#include <QAbstractListModel>
#include <QJsonObject>
#include <QJsonArray>
#include <QVector>
#include <QMutex>
#include <QHash>

namespace PinkReader {

class ListingController : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        TitleRole, SubtitleRole, IconRole, ImageRole,
        ScoreRole, AuthorRole, TimestampRole, UrlRole,
        DataRole, MetadataRole
    };

    explicit ListingController(QObject *parent = nullptr);
    ~ListingController() override;

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

    void setAfter(const QString &after);
    QString after() const;
    bool hasMore() const;
    void setSortOrder(const QString &order);
    QString sortOrder() const;
    int currentPage() const;
    void resetPagination();

private:
    QVector<QJsonObject> m_items;
    QString m_after;
    QString m_sortOrder = "hot";
    int m_currentPage = 0;
    bool m_hasMore = true;
    mutable QMutex m_mutex;
    QVariant getData(const QJsonObject &item, int role) const;
};

} // namespace PinkReader
