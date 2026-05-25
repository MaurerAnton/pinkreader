/*
 * PinkReader - GPLv3
 * File: oauth_token_store.cpp - OAuth token store implementation
 */

#include "accounts/oauth_token_store.h"
#include "utils/logging.h"
#include "utils/encryption.h"

#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>

namespace PinkReader {

OAuthTokenStore::OAuthTokenStore(QObject *parent) : QObject(parent) {}
OAuthTokenStore::~OAuthTokenStore() = default;

bool OAuthTokenStore::initialize()
{
    if (m_initialized) return true;
    Logging::info("OAuthTokenStore", "Initializing token store...");
    QDir().mkpath(QFileInfo(storagePath()).absolutePath());
    m_initialized = true;
    return true;
}

bool OAuthTokenStore::isInitialized() const { return m_initialized; }

void OAuthTokenStore::storeTokens(const QString &username, const QString &accessToken,
                                   const QString &refreshToken, const QDateTime &expiry)
{
    QFile file(storagePath());
    QJsonObject store;
    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        if (doc.isObject()) store = doc.object();
        file.close();
    }
    QJsonObject userTokens;
    userTokens["access_token"] = accessToken;
    userTokens["refresh_token"] = refreshToken;
    userTokens["expiry"] = expiry.toString(Qt::ISODate);
    store[username] = userTokens;
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(store).toJson());
        file.close();
        emit tokensUpdated(username);
    }
}

QString OAuthTokenStore::accessToken(const QString &username) const
{
    QFile file(storagePath());
    if (!file.open(QIODevice::ReadOnly)) return QString();
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    return doc.object().value(username).toObject().value("access_token").toString();
}

QString OAuthTokenStore::refreshToken(const QString &username) const
{
    QFile file(storagePath());
    if (!file.open(QIODevice::ReadOnly)) return QString();
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    return doc.object().value(username).toObject().value("refresh_token").toString();
}

QDateTime OAuthTokenStore::tokenExpiry(const QString &username) const
{
    QFile file(storagePath());
    if (!file.open(QIODevice::ReadOnly)) return QDateTime();
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();
    QString s = doc.object().value(username).toObject().value("expiry").toString();
    return s.isEmpty() ? QDateTime() : QDateTime::fromString(s, Qt::ISODate);
}

void OAuthTokenStore::clearTokens(const QString &username)
{
    QFile file(storagePath());
    QJsonObject store;
    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        store = QJsonDocument::fromJson(file.readAll()).object();
        file.close();
    }
    store.remove(username);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(store).toJson());
        file.close();
    }
}

void OAuthTokenStore::clearAll()
{
    QFile::remove(storagePath());
}

QString OAuthTokenStore::storagePath() const
{
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation)
           + "/oauth_tokens.json";
}

} // namespace PinkReader
