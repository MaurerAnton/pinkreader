/*
 * PinkReader - GPLv3
 * File: oauth_token_store.h - OAuth token secure storage
 */

#pragma once

#include <QObject>
#include <QString>
#include <QDateTime>

namespace PinkReader {

class OAuthTokenStore : public QObject
{
    Q_OBJECT
public:
    explicit OAuthTokenStore(QObject *parent = nullptr);
    ~OAuthTokenStore() override;

    void storeTokens(const QString &username, const QString &accessToken,
                     const QString &refreshToken, const QDateTime &expiry);
    QString accessToken(const QString &username) const;
    QString refreshToken(const QString &username) const;
    QDateTime tokenExpiry(const QString &username) const;
    void clearTokens(const QString &username);
    void clearAll();

    bool initialize();
    bool isInitialized() const;

signals:
    void tokensUpdated(const QString &username);

private:
    QString storagePath() const;
    bool m_initialized = false;
};

} // namespace PinkReader
