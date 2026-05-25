/*
 * PinkReader - GPLv3
 * File: account.cpp - Account model implementation
 */

#include "accounts/account.h"
#include <QJsonObject>
#include <QJsonValue>
#include <QDebug>

namespace PinkReader {

Account::Account() = default;

Account::Account(const QString &username)
    : m_username(username)
{
}

bool Account::isTokenExpired() const
{
    if (!m_tokenExpiry.isValid())
        return false;
    return QDateTime::currentDateTimeUtc() >= m_tokenExpiry;
}

bool Account::needsRefresh() const
{
    if (!m_tokenExpiry.isValid())
        return false;
    // Refresh if token expires within 5 minutes
    return QDateTime::currentDateTimeUtc().addSecs(300) >= m_tokenExpiry;
}

QJsonObject Account::toJson() const
{
    QJsonObject obj;
    obj["username"] = m_username;
    obj["display_name"] = m_displayName;
    obj["access_token"] = m_accessToken;
    obj["refresh_token"] = m_refreshToken;
    if (m_tokenExpiry.isValid())
        obj["token_expiry"] = m_tokenExpiry.toString(Qt::ISODate);
    obj["created_utc"] = static_cast<double>(m_createdUtc);
    obj["link_karma"] = m_linkKarma;
    obj["comment_karma"] = m_commentKarma;
    obj["has_verified_email"] = m_hasVerifiedEmail;
    obj["is_gold"] = m_isGold;
    obj["is_mod"] = m_isMod;
    obj["over_18"] = m_over18;
    return obj;
}

Account Account::fromJson(const QJsonObject &json)
{
    Account a;
    a.m_username = json.value("username").toString();
    a.m_displayName = json.value("display_name").toString();
    a.m_accessToken = json.value("access_token").toString();
    a.m_refreshToken = json.value("refresh_token").toString();
    QString expiry = json.value("token_expiry").toString();
    if (!expiry.isEmpty())
        a.m_tokenExpiry = QDateTime::fromString(expiry, Qt::ISODate);
    a.m_createdUtc = static_cast<qint64>(json.value("created_utc").toDouble());
    a.m_linkKarma = json.value("link_karma").toInt();
    a.m_commentKarma = json.value("comment_karma").toInt();
    a.m_hasVerifiedEmail = json.value("has_verified_email").toBool();
    a.m_isGold = json.value("is_gold").toBool();
    a.m_isMod = json.value("is_mod").toBool();
    a.m_over18 = json.value("over_18").toBool();
    return a;
}

bool Account::isValid() const
{
    return !m_username.isEmpty();
}

bool Account::operator==(const Account &other) const
{
    return m_username == other.m_username;
}

bool Account::operator!=(const Account &other) const
{
    return !(*this == other);
}

} // namespace PinkReader
