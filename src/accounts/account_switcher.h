/*
 * PinkReader - GPLv3
 * File: account_switcher.h - Quick account switching
 */

#pragma once

#include <QObject>
#include <QStringList>

namespace PinkReader {

class AccountManager;

class AccountSwitcher : public QObject
{
    Q_OBJECT
public:
    explicit AccountSwitcher(AccountManager *mgr, QObject *parent = nullptr);
    ~AccountSwitcher() override;

    void showSwitcher();
    QStringList recentAccounts() const;
    void addRecent(const QString &username);
    void clearRecents();

signals:
    void switchRequested(const QString &username);
    void addAccountRequested();

private:
    AccountManager *m_accountManager = nullptr;
    QStringList m_recentAccounts;
    static constexpr int MAX_RECENT = 10;
};

} // namespace PinkReader
