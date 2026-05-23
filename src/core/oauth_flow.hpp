#pragma once

#include <QObject>
#include <QUrl>

namespace PinkReader {

class OAuthFlow : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString authUrl READ authUrl NOTIFY authUrlReady)
    Q_PROPERTY(bool isAuthenticating READ isAuthenticating NOTIFY authenticatingChanged)

public:
    explicit OAuthFlow(QObject* parent = nullptr);

    QString authUrl() const { return m_authUrl; }
    bool isAuthenticating() const { return m_authenticating; }

    // Step 1: Generate auth URL (open in browser)
    Q_INVOKABLE void startAuth();
    // Step 2: Handle redirect URI
    Q_INVOKABLE void handleRedirect(const QUrl& redirectUrl);

    // Token getters
    QString accessToken() const { return m_accessToken; }
    QString refreshToken() const { return m_refreshToken; }
    int expiresIn() const { return m_expiresIn; }

    // Refresh expired access token
    Q_INVOKABLE void refreshAccessToken(const QString& refreshToken);

    static constexpr auto CLIENT_ID = "ohXpoqrYubFdZIfNu0vh85z4RWqTjLrQ";
    static constexpr auto REDIRECT_URI = "pinkreader://oauth/callback";
    static constexpr auto AUTH_URL = "https://www.reddit.com/api/v1/authorize";
    static constexpr auto TOKEN_URL = "https://www.reddit.com/api/v1/access_token";
    static constexpr auto USER_AGENT = "PinkReader/0.1";

signals:
    void authUrlReady();
    void authenticatingChanged();
    void authSuccess(const QString& accessToken, const QString& refreshToken, int expiresIn);
    void authError(const QString& error);
    void tokenRefreshed(const QString& accessToken, int expiresIn);
    void tokenRefreshError(const QString& error);

private:
    void exchangeCodeForToken(const QString& code);
    QByteArray generateRandomString(int length = 64);
    QByteArray base64UrlEncode(const QByteArray& input);
    QByteArray sha256(const QByteArray& input);

    QString m_codeVerifier;
    QString m_codeChallenge;
    QString m_authUrl;
    QString m_accessToken;
    QString m_refreshToken;
    int m_expiresIn = 0;
    bool m_authenticating = false;
};

}  // namespace PinkReader
