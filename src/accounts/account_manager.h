/*
 * PinkReader - GPLv3
 * File: account_manager.h - Multi-account manager
 */

#pragma once

#include <QObject>
#include <QString>
#include <QVector>
#include <QSettings>
#include "accounts/account.h"

namespace PinkReader {

/**
 * @brief Manages multiple Reddit accounts with OAuth token persistence.
 */
class AccountManager : public QObject
{
    Q_OBJECT

public:
    explicit AccountManager(QObject *parent = nullptr);
    ~AccountManager() override;

    // Account CRUD
    void addAccount(const Account &account);
    void removeAccount(const QString &username);
    void updateAccount(const Account &account);

    // Access
    Account *currentAccount();
    Account *account(const QString &username);
    QVector<Account> allAccounts() const;
    QStringList accountNames() const;

    // Current account switching
    void setCurrentAccount(const QString &username);
    int accountCount() const;
    bool hasAccounts() const;

    // Persistence
    void loadAccounts();
    void saveAccounts();
    QString storagePath() const;

signals:
    void currentAccountChanged(Account *account);
    void accountAdded(const QString &username);
    void accountRemoved(const QString &username);
    void accountsChanged();

private:
    QVector<Account> m_accounts;
    int m_currentIndex = -1;
};

} // namespace PinkReader
