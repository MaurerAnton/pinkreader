/*
 * PinkReader - GPLv3
 * File: account_manager.cpp - Account manager implementation
 */

#include "accounts/account_manager.h"
#include "utils/logging.h"

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QFileInfo>
#include <QStandardPaths>

namespace PinkReader {

AccountManager::AccountManager(QObject *parent)
    : QObject(parent)
{
}

AccountManager::~AccountManager()
{
    saveAccounts();
}

void AccountManager::addAccount(const Account &account)
{
    // Replace if exists
    for (int i = 0; i < m_accounts.size(); ++i) {
        if (m_accounts[i].username() == account.username()) {
            m_accounts[i] = account;
            emit accountsChanged();
            return;
        }
    }
    m_accounts.append(account);
    emit accountAdded(account.username());
    emit accountsChanged();
}

void AccountManager::removeAccount(const QString &username)
{
    for (int i = 0; i < m_accounts.size(); ++i) {
        if (m_accounts[i].username() == username) {
            m_accounts.removeAt(i);
            if (m_currentIndex == i) {
                m_currentIndex = m_accounts.isEmpty() ? -1 : 0;
                emit currentAccountChanged(currentAccount());
            }
            emit accountRemoved(username);
            emit accountsChanged();
            return;
        }
    }
}

void AccountManager::updateAccount(const Account &account)
{
    for (int i = 0; i < m_accounts.size(); ++i) {
        if (m_accounts[i].username() == account.username()) {
            m_accounts[i] = account;
            emit accountsChanged();
            return;
        }
    }
}

Account *AccountManager::currentAccount()
{
    if (m_currentIndex >= 0 && m_currentIndex < m_accounts.size())
        return &m_accounts[m_currentIndex];
    return nullptr;
}

Account *AccountManager::account(const QString &username)
{
    for (auto &a : m_accounts) {
        if (a.username() == username)
            return &a;
    }
    return nullptr;
}

QVector<Account> AccountManager::allAccounts() const
{
    return m_accounts;
}

QStringList AccountManager::accountNames() const
{
    QStringList names;
    for (const auto &a : m_accounts)
        names.append(a.username());
    return names;
}

void AccountManager::setCurrentAccount(const QString &username)
{
    for (int i = 0; i < m_accounts.size(); ++i) {
        if (m_accounts[i].username() == username) {
            m_currentIndex = i;
            emit currentAccountChanged(&m_accounts[i]);
            return;
        }
    }
}

int AccountManager::accountCount() const
{
    return m_accounts.size();
}

bool AccountManager::hasAccounts() const
{
    return !m_accounts.isEmpty();
}

void AccountManager::loadAccounts()
{
    QFile file(storagePath());
    if (!file.exists() || !file.open(QIODevice::ReadOnly)) {
        Logging::debug("AccountManager", "No saved accounts file");
        return;
    }

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &err);
    file.close();

    if (err.error != QJsonParseError::NoError) {
        Logging::error("AccountManager", "Failed to parse accounts: " + err.errorString());
        return;
    }

    QJsonArray arr = doc.array();
    m_accounts.clear();
    for (const auto &v : arr)
        m_accounts.append(Account::fromJson(v.toObject()));

    if (!m_accounts.isEmpty())
        m_currentIndex = 0;

    Logging::info("AccountManager", QString("Loaded %1 accounts").arg(m_accounts.size()));
    emit accountsChanged();
}

void AccountManager::saveAccounts()
{
    QJsonArray arr;
    for (const auto &a : m_accounts)
        arr.append(a.toJson());

    QJsonDocument doc(arr);
    QDir().mkpath(QFileInfo(storagePath()).absolutePath());

    QFile file(storagePath());
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
    }
}

QString AccountManager::storagePath() const
{
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
           + "/accounts.json";
}

} // namespace PinkReader
