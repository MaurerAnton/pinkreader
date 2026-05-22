#pragma once

#include <QObject>
#include <QVector>
#include <QSettings>
#include "models.hpp"

namespace PinkReader {

class AccountManager : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool hasAccounts READ hasAccounts NOTIFY accountsChanged)
    Q_PROPERTY(int accountCount READ accountCount NOTIFY accountsChanged)
    
public:
    explicit AccountManager(QObject* parent = nullptr);
    
    bool hasAccounts() const { return !m_accounts.isEmpty(); }
    int accountCount() const { return m_accounts.size(); }
    
    void addAccount(const Account& account);
    void removeAccount(const QString& username);
    void setActiveAccount(const QString& username);
    
    Account* activeAccount();
    Account* account(const QString& username);
    QVector<Account>& allAccounts() { return m_accounts; }
    
    QStringList accountNames() const;
    
    void loadAccounts();
    void saveAccounts();
    
    // OAuth constants
    static QString clientId() { return "pinkreader_oauth_client"; }
    static QString redirectUri() { return "pinkreader://oauth/callback"; }
    static QStringList scopes();
    
signals:
    void accountsChanged();
    void activeAccountChanged(const QString& username);
    void accountAdded(const QString& username);
    void accountRemoved(const QString& username);
    
private:
    QVector<Account> m_accounts;
    int m_activeIndex = -1;
};

} // namespace PinkReader
