#pragma once

#include "../core/models.hpp"

#include <QAbstractListModel>

namespace PinkReader {

class AccountModel : public QAbstractListModel {
    Q_OBJECT
public:
    enum Roles { UsernameRole = Qt::UserRole + 1, IsActiveRole };

    explicit AccountModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setAccounts(const QVector<Account>& accounts, int activeIndex);

private:
    QVector<Account> m_accounts;
    int m_activeIndex = -1;
};

}  // namespace PinkReader
