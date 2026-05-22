#pragma once

#include <QAbstractListModel>
#include <QStringList>

namespace PinkReader {

class SubredditModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum Roles { NameRole = Qt::UserRole + 1 };
    
    explicit SubredditModel(QObject* parent = nullptr);
    
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;
    
    void setSubreddits(const QStringList& names);
    
private:
    QStringList m_names;
};

} // namespace PinkReader
