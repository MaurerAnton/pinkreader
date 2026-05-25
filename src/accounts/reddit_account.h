/*
 * PinkReader - GPLv3
 * File: reddit_account.h - Port of RedditAccount.java
 * Exact port: every field, method, and logic branch from the original.
 */

#pragma once

#include <QString>
#include <QDateTime>
#include <QElapsedTimer>
#include <QMutex>
#include <string>
#include <memory>

namespace PinkReader {

// Port of RedditOAuth.Token / RefreshToken / AccessToken
// Original (RedditOAuth.kt):
//   open class Token(@JvmField val token: String)
//   class RefreshToken(token: String?) : Token(token!!)
//   class AccessToken(token: String?) : Token(token!!) {
//       private val mMonotonicTimestamp = SystemClock.elapsedRealtime()
//       val isExpired: Boolean
//           get() { ... halfHourInMs ... }
//   }

class OAuthToken {
public:
    QString token;

    OAuthToken() = default;
    explicit OAuthToken(const QString &t) : token(t) {}
    QString toString() const { return token; }
};

class OAuthAccessToken : public OAuthToken {
public:
    OAuthAccessToken() = default;
    explicit OAuthAccessToken(const QString &t);

    bool isExpired() const;

private:
    qint64 m_monotonicTimestamp = 0; // elapsed msec at creation
};

class OAuthRefreshToken : public OAuthToken {
public:
    OAuthRefreshToken() = default;
    explicit OAuthRefreshToken(const QString &t) : OAuthToken(t) {}
};

// Port of RedditAccount.java
// Every field, method, and logic branch ported exactly.

class RedditAccount {
public:
    // Fields (exact port of Java fields)
    // public final String username;
    // public final String canonicalUsername;
    // public final RedditOAuth.RefreshToken refreshToken;
    // private RedditOAuth.AccessToken accessToken;
    // public final long priority;
    // @Nullable public final String clientId;
    QString username;
    QString canonicalUsername;
    OAuthRefreshToken refreshToken;
    qint64 priority = 0;
    QString clientId; // empty = null

    // Constructor: RedditAccount(username, refreshToken, priority, clientId)
    RedditAccount() = default;
    RedditAccount(const QString &username_,
                  const OAuthRefreshToken &refreshToken_,
                  qint64 priority_,
                  const QString &clientId_);

    // Port: public boolean isAnonymous()
    bool isAnonymous() const;

    // Port: public boolean isNotAnonymous()
    bool isNotAnonymous() const;

    // Port: public String getCanonicalUsername()
    QString getCanonicalUsername() const;

    // Port: public synchronized RedditOAuth.AccessToken getMostRecentAccessToken()
    OAuthAccessToken getMostRecentAccessToken() const;

    // Port: public synchronized void setAccessToken(final RedditOAuth.AccessToken token)
    void setAccessToken(const OAuthAccessToken &token);

    // Port: public boolean equals(final Object o)
    bool equals(const RedditAccount &other) const;
    bool operator==(const RedditAccount &other) const { return equals(other); }
    bool operator!=(const RedditAccount &other) const { return !equals(other); }

    // Port: public int hashCode()
    // Returns std::hash of canonicalUsername case-insensitive
    size_t hashCode() const;

private:
    mutable QMutex m_accessTokenMutex;
    OAuthAccessToken m_accessToken;
};

// Hash specialization for use in unordered containers
struct RedditAccountHash {
    size_t operator()(const RedditAccount &a) const { return a.hashCode(); }
};

} // namespace PinkReader
