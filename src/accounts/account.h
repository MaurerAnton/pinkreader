/*
 * PinkReader - GPLv3
 * File: account.h - Reddit account model
 */

#pragma once

#include <QString>
#include <QDateTime>
#include <QJsonObject>
#include <QMetaType>

namespace PinkReader {

/**
 * @brief Represents a single Reddit user account with OAuth tokens.
 */
class Account
{
public:
    Account();
    explicit Account(const QString &username);
    Account(const Account &other) = default;
    Account &operator=(const Account &other) = default;
    ~Account() = default;

    // Identity
    QString username() const { return m_username; }
    void setUsername(const QString &v) { m_username = v; }

    QString displayName() const { return m_displayName.isEmpty() ? m_username : m_displayName; }
    void setDisplayName(const QString &v) { m_displayName = v; }

    // OAuth tokens
    QString accessToken() const { return m_accessToken; }
    void setAccessToken(const QString &v) { m_accessToken = v; }

    QString refreshToken() const { return m_refreshToken; }
    void setRefreshToken(const QString &v) { m_refreshToken = v; }

    QDateTime tokenExpiry() const { return m_tokenExpiry; }
    void setTokenExpiry(const QDateTime &v) { m_tokenExpiry = v; }

    // Token status
    bool isAuthorized() const { return !m_accessToken.isEmpty() && !isTokenExpired(); }
    bool isTokenExpired() const;
    bool needsRefresh() const;

    // Account metadata
    qint64 createdUtc() const { return m_createdUtc; }
    void setCreatedUtc(qint64 v) { m_createdUtc = v; }

    int linkKarma() const { return m_linkKarma; }
    void setLinkKarma(int v) { m_linkKarma = v; }

    int commentKarma() const { return m_commentKarma; }
    void setCommentKarma(int v) { m_commentKarma = v; }

    bool hasVerifiedEmail() const { return m_hasVerifiedEmail; }
    void setHasVerifiedEmail(bool v) { m_hasVerifiedEmail = v; }

    bool isGold() const { return m_isGold; }
    void setIsGold(bool v) { m_isGold = v; }

    bool isMod() const { return m_isMod; }
    void setIsMod(bool v) { m_isMod = v; }

    // Over18 / NSFW
    bool over18() const { return m_over18; }
    void setOver18(bool v) { m_over18 = v; }

    // Serialization
    QJsonObject toJson() const;
    static Account fromJson(const QJsonObject &json);
    bool isValid() const;

    // Comparison
    bool operator==(const Account &other) const;
    bool operator!=(const Account &other) const;

private:
    QString m_username;
    QString m_displayName;
    QString m_accessToken;
    QString m_refreshToken;
    QDateTime m_tokenExpiry;
    qint64 m_createdUtc = 0;
    int m_linkKarma = 0;
    int m_commentKarma = 0;
    bool m_hasVerifiedEmail = false;
    bool m_isGold = false;
    bool m_isMod = false;
    bool m_over18 = false;
};

} // namespace PinkReader

Q_DECLARE_METATYPE(PinkReader::Account)
