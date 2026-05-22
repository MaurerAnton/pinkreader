#include "oauth_flow.hpp"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QCryptographicHash>
#include <QRandomGenerator>
#include <QDesktopServices>
#include <QCoreApplication>

namespace PinkReader {

OAuthFlow::OAuthFlow(QObject* parent) : QObject(parent) {}

QByteArray OAuthFlow::generateRandomString(int length) {
    QByteArray result;
    result.resize(length);
    const char chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~";
    const int numChars = sizeof(chars) - 1;
    for (int i = 0; i < length; ++i) {
        result[i] = chars[QRandomGenerator::global()->bounded(numChars)];
    }
    return result;
}

QByteArray OAuthFlow::base64UrlEncode(const QByteArray& input) {
    return input.toBase64(QByteArray::Base64UrlEncoding);
}

QByteArray OAuthFlow::sha256(const QByteArray& input) {
    return QCryptographicHash::hash(input, QCryptographicHash::Sha256);
}

void OAuthFlow::startAuth() {
    m_authenticating = true;
    emit authenticatingChanged();

    // PKCE: generate code_verifier and code_challenge
    m_codeVerifier = QString::fromLatin1(generateRandomString(64));
    QByteArray challenge = base64UrlEncode(sha256(m_codeVerifier.toUtf8()));

    // Build authorization URL
    QUrl url(AUTH_URL);
    QUrlQuery query;
    query.addQueryItem("client_id", CLIENT_ID);
    query.addQueryItem("response_type", "code");
    query.addQueryItem("state", QString::fromLatin1(generateRandomString(16)));
    query.addQueryItem("redirect_uri", REDIRECT_URI);
    query.addQueryItem("duration", "permanent");
    query.addQueryItem("scope",
        "identity edit flair history modconfig modflair modlog "
        "modposts modwiki mysubreddits privatemessages read report "
        "save submit subscribe vote wikiedit wikiread");
    query.addQueryItem("code_challenge", QString::fromLatin1(challenge));
    query.addQueryItem("code_challenge_method", "S256");

    url.setQuery(query);
    m_authUrl = url.toString();
    emit authUrlReady();
}

void OAuthFlow::handleRedirect(const QUrl& redirectUrl) {
    if (!m_authenticating) return;

    QUrlQuery query(redirectUrl);
    QString error = query.queryItemValue("error");
    if (!error.isEmpty()) {
        m_authenticating = false;
        emit authenticatingChanged();
        emit authError(error);
        return;
    }

    QString code = query.queryItemValue("code");
    if (code.isEmpty()) {
        m_authenticating = false;
        emit authenticatingChanged();
        emit authError("No authorization code received");
        return;
    }

    exchangeCodeForToken(code);
}

void OAuthFlow::exchangeCodeForToken(const QString& code) {
    auto* nam = new QNetworkAccessManager(this);

    QUrl url(TOKEN_URL);
    QNetworkRequest req(url);
    req.setRawHeader("User-Agent", USER_AGENT);
    req.setRawHeader("Authorization",
        "Basic " + QByteArray(QString(CLIENT_ID).toUtf8()).toBase64());
    req.setHeader(QNetworkRequest::ContentTypeHeader,
        "application/x-www-form-urlencoded");

    QUrlQuery body;
    body.addQueryItem("grant_type", "authorization_code");
    body.addQueryItem("code", code);
    body.addQueryItem("redirect_uri", REDIRECT_URI);
    body.addQueryItem("code_verifier", m_codeVerifier);

    auto* reply = nam->post(req, body.toString(QUrl::FullyEncoded).toUtf8());

    connect(reply, &QNetworkReply::finished, this, [this, reply, nam]() {
        reply->deleteLater();
        nam->deleteLater();
        m_authenticating = false;
        emit authenticatingChanged();

        if (reply->error() != QNetworkReply::NoError) {
            QString errBody = reply->readAll();
            emit authError(errBody.isEmpty() ? reply->errorString() : errBody);
            return;
        }

        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject obj = doc.object();

        m_accessToken = obj["access_token"].toString();
        m_refreshToken = obj["refresh_token"].toString();
        m_expiresIn = obj["expires_in"].toInt(3600);

        if (m_accessToken.isEmpty()) {
            emit authError("No access token in response");
            return;
        }

        emit authSuccess(m_accessToken, m_refreshToken, m_expiresIn);
    });
}

void OAuthFlow::refreshAccessToken(const QString& refreshTokenStr) {
    auto* nam = new QNetworkAccessManager(this);

    QUrl url(TOKEN_URL);
    QNetworkRequest req(url);
    req.setRawHeader("User-Agent", USER_AGENT);
    req.setRawHeader("Authorization",
        "Basic " + QByteArray(QString(CLIENT_ID).toUtf8()).toBase64());
    req.setHeader(QNetworkRequest::ContentTypeHeader,
        "application/x-www-form-urlencoded");

    QUrlQuery body;
    body.addQueryItem("grant_type", "refresh_token");
    body.addQueryItem("refresh_token", refreshTokenStr);

    auto* reply = nam->post(req, body.toString(QUrl::FullyEncoded).toUtf8());

    connect(reply, &QNetworkReply::finished, this, [this, reply, nam]() {
        reply->deleteLater();
        nam->deleteLater();

        if (reply->error() != QNetworkReply::NoError) {
            emit tokenRefreshError(reply->errorString());
            return;
        }

        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject obj = doc.object();

        m_accessToken = obj["access_token"].toString();
        m_expiresIn = obj["expires_in"].toInt(3600);

        emit tokenRefreshed(m_accessToken, m_expiresIn);
    });
}

} // namespace PinkReader
