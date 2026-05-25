#pragma once

#include <QAbstractListModel>
#include <QJsonObject>
#include <QJsonArray>
#include <QVector>
#include <QMutex>
#include <QHash>

namespace PinkReader {

class PostListAdapter : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles {
        IdRole = Qt::UserRole + 1,
        TitleRole, SubtitleRole, IconRole, ImageRole,
        ScoreRole, AuthorRole, TimestampRole, UrlRole,
        DataRole, MetadataRole, TypeRole, StatusRole
    };

    explicit PostListAdapter(QObject *parent = nullptr);
    ~PostListAdapter() override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setItems(const QJsonArray &items);
    void addItems(const QJsonArray &items);
    void prependItems(const QJsonArray &items);
    void removeItem(int row);
    void removeItemById(const QString &id);
    void clear();

    int findItemById(const QString &id) const;
    QJsonObject itemAt(int row) const;
    int itemCount() const;
    bool isEmpty() const;

    void sort(int role, Qt::SortOrder order = Qt::AscendingOrder);
    void filter(const QString &query);

signals:
    void itemsLoaded(int count);
    void itemsAppended(int count);
    void itemsCleared();

private:
    QVector<QJsonObject> m_items;
    QString m_filterQuery;
    bool m_hasActiveFilter = false;
    mutable QMutex m_dataMutex;
    QVariant getItemData(const QJsonObject &item, int role) const;
};

} // namespace PinkReader
