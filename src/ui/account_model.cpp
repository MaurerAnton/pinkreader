#include "account_model.hpp"

namespace PinkReader {

AccountModel::AccountModel(QObject* parent) : QAbstractListModel(parent) {}

int AccountModel::rowCount(const QModelIndex& parent) const {
    return parent.isValid() ? 0 : m_accounts.size();
}

QVariant AccountModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) return {};
    if (role == UsernameRole) return m_accounts[index.row()].username;
    if (role == IsActiveRole) return index.row() == m_activeIndex;
    return {};
}

QHash<int, QByteArray> AccountModel::roleNames() const {
    return {{UsernameRole, "username"}, {IsActiveRole, "isActive"}};
}

void AccountModel::setAccounts(const QVector<Account>& accounts, int activeIndex) {
    beginResetModel();
    m_accounts = accounts;
    m_activeIndex = activeIndex;
    endResetModel();
}

} // namespace PinkReader
