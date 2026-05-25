/*
 * PinkReader - GPLv3
 * File: account_switcher.cpp - Account switcher implementation
 */

#include "accounts/account_switcher.h"
#include "accounts/account_manager.h"
#include "utils/logging.h"

namespace PinkReader {

AccountSwitcher::AccountSwitcher(AccountManager *mgr, QObject *parent)
    : QObject(parent), m_accountManager(mgr)
{
}

AccountSwitcher::~AccountSwitcher() = default;

void AccountSwitcher::showSwitcher()
{
    Logging::debug("AccountSwitcher", "Showing account switcher");
    // UI dialog or popup would be shown here
}

QStringList AccountSwitcher::recentAccounts() const { return m_recentAccounts; }

void AccountSwitcher::addRecent(const QString &username)
{
    m_recentAccounts.removeAll(username);
    m_recentAccounts.prepend(username);
    if (m_recentAccounts.size() > MAX_RECENT)
        m_recentAccounts.resize(MAX_RECENT);
}

void AccountSwitcher::clearRecents() { m_recentAccounts.clear(); }

} // namespace PinkReader
