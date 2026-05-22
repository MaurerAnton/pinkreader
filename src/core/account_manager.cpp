#include "account_manager.hpp"

#include <QStandardPaths>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace PinkReader {

AccountManager::AccountManager(QObject* parent)
    : QObject(parent)
{
    loadAccounts();
}

QStringList AccountManager::scopes() {
    return {
        "identity",
        "read",
        "vote",
        "save",
        "submit",
        "subscribe",
        "history",
        "mysubreddits",
        "privatemessages",
        "account"
    };
}

void AccountManager::addAccount(const Account& account) {
    // Replace if exists
    for (int i = 0; i < m_accounts.size(); ++i) {
        if (m_accounts[i].username == account.username) {
            m_accounts[i] = account;
            saveAccounts();
            emit accountsChanged();
            return;
        }
    }
    
    m_accounts.append(account);
    if (m_activeIndex < 0) {
        m_activeIndex = 0;
    }
    saveAccounts();
    emit accountAdded(account.username);
    emit accountsChanged();
}

void AccountManager::removeAccount(const QString& username) {
    for (int i = 0; i < m_accounts.size(); ++i) {
        if (m_accounts[i].username == username) {
            m_accounts.removeAt(i);
            if (m_activeIndex >= m_accounts.size()) {
                m_activeIndex = m_accounts.size() - 1;
            }
            saveAccounts();
            emit accountRemoved(username);
            emit accountsChanged();
            return;
        }
    }
}

void AccountManager::setActiveAccount(const QString& username) {
    for (int i = 0; i < m_accounts.size(); ++i) {
        if (m_accounts[i].username == username) {
            m_activeIndex = i;
            emit activeAccountChanged(username);
            return;
        }
    }
}

Account* AccountManager::activeAccount() {
    if (m_activeIndex >= 0 && m_activeIndex < m_accounts.size()) {
        return &m_accounts[m_activeIndex];
    }
    return nullptr;
}

Account* AccountManager::account(const QString& username) {
    for (auto& acc : m_accounts) {
        if (acc.username == username) return &acc;
    }
    return nullptr;
}

QStringList AccountManager::accountNames() const {
    QStringList names;
    for (const auto& acc : m_accounts) {
        names.append(acc.username);
    }
    return names;
}

void AccountManager::saveAccounts() {
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir().mkpath(path);
    
    QJsonArray arr;
    for (const auto& acc : m_accounts) {
        QJsonObject obj;
        obj["username"] = acc.username;
        obj["access_token"] = acc.accessToken;
        obj["refresh_token"] = acc.refreshToken;
        obj["token_type"] = acc.tokenType;
        obj["expires_in"] = acc.expiresIn;
        obj["scope"] = acc.scope;
        arr.append(obj);
    }
    
    QJsonDocument doc(arr);
    QFile file(path + "/accounts.json");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
    }
}

void AccountManager::loadAccounts() {
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/accounts.json";
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) return;
    
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    
    m_accounts.clear();
    for (const auto& val : doc.array()) {
        QJsonObject obj = val.toObject();
        Account acc;
        acc.username = obj["username"].toString();
        acc.accessToken = obj["access_token"].toString();
        acc.refreshToken = obj["refresh_token"].toString();
        acc.tokenType = obj["token_type"].toString();
        acc.expiresIn = obj["expires_in"].toInt();
        acc.scope = obj["scope"].toString();
        acc.isAnonymous = false;
        m_accounts.append(acc);
    }
}

} // namespace PinkReader
