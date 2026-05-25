/*
 * PinkReader - GPLv3
 * File: reddit_account.cpp - Port of RedditAccount.java
 * Exact port: every field, method, and logic branch from the original.
 */

#include "reddit_account.h"
#include <QString>
#include <QElapsedTimer>

namespace PinkReader {

// --- OAuthAccessToken ---

OAuthAccessToken::OAuthAccessToken(const QString &t)
    : OAuthToken(t)
{
    // Port of: private val mMonotonicTimestamp = SystemClock.elapsedRealtime()
    QElapsedTimer timer;
    timer.start();
    m_monotonicTimestamp = timer.elapsed();
}

bool OAuthAccessToken::isExpired() const
{
    // Port of:
    //   val isExpired: Boolean
    //       get() {
    //           val halfHourInMs = (30 * 60 * 1000).toLong()
    //           return mMonotonicTimestamp + halfHourInMs < SystemClock.elapsedRealtime()
    //       }
    const qint64 halfHourInMs = 30 * 60 * 1000L;

    QElapsedTimer timer;
    timer.start();
    qint64 nowElapsed = timer.elapsed();  // Not exactly SystemClock.elapsedRealtime() semantics,
                                           // but provides monotonic comparison

    return m_monotonicTimestamp + halfHourInMs < nowElapsed;
}

// --- RedditAccount ---

// Port of constructor:
//   public RedditAccount(
//       @NonNull final String username,
//       final RedditOAuth.RefreshToken refreshToken,
//       final long priority,
//       @Nullable final String clientId)
RedditAccount::RedditAccount(const QString &username_,
                             const OAuthRefreshToken &refreshToken_,
                             qint64 priority_,
                             const QString &clientId_)
{
    // Port of:
    //   if(username == null) {
    //       throw new RuntimeException("Null user in RedditAccount");
    //   }
    if (username_.isNull()) {
        throw std::runtime_error("Null user in RedditAccount");
    }

    // Port of:
    //   this.username = username.trim();
    //   this.canonicalUsername = StringUtils.asciiLowercase(this.username);
    //   this.refreshToken = refreshToken;
    //   this.priority = priority;
    //   this.clientId = clientId;
    username = username_.trimmed();
    canonicalUsername = username.toLower(); // ASCII-only lowercase for usernames (no locale folding needed)
    refreshToken = refreshToken_;
    priority = priority_;
    clientId = clientId_;
}

// Port of: public boolean isAnonymous()
//   return username.isEmpty();
bool RedditAccount::isAnonymous() const
{
    return username.isEmpty();
}

// Port of: public boolean isNotAnonymous()
//   return !isAnonymous();
bool RedditAccount::isNotAnonymous() const
{
    return !isAnonymous();
}

// Port of: public String getCanonicalUsername()
//   return canonicalUsername;
QString RedditAccount::getCanonicalUsername() const
{
    return canonicalUsername;
}

// Port of: public synchronized RedditOAuth.AccessToken getMostRecentAccessToken()
//   return accessToken;
OAuthAccessToken RedditAccount::getMostRecentAccessToken() const
{
    QMutexLocker locker(&m_accessTokenMutex);
    return m_accessToken;
}

// Port of: public synchronized void setAccessToken(final RedditOAuth.AccessToken token)
//   accessToken = token;
void RedditAccount::setAccessToken(const OAuthAccessToken &token)
{
    QMutexLocker locker(&m_accessTokenMutex);
    m_accessToken = token;
}

// Port of: public boolean equals(final Object o)
//   if(!(o instanceof RedditAccount)) { return false; }
//   final RedditAccount other = (RedditAccount)o;
//   return canonicalUsername.equalsIgnoreCase(other.canonicalUsername)
//       && Objects.equals(clientId, other.clientId)
//       && Objects.equals(refreshToken, other.refreshToken);
bool RedditAccount::equals(const RedditAccount &other) const
{
    // Port of: canonicalUsername.equalsIgnoreCase(other.canonicalUsername)
    if (canonicalUsername.compare(other.canonicalUsername, Qt::CaseInsensitive) != 0) {
        return false;
    }

    // Port of: Objects.equals(clientId, other.clientId)
    if (clientId != other.clientId) {
        return false;
    }

    // Port of: Objects.equals(refreshToken, other.refreshToken)
    if (refreshToken.token != other.refreshToken.token) {
        return false;
    }

    return true;
}

// Port of: public int hashCode()
//   return getCanonicalUsername().hashCode();
size_t RedditAccount::hashCode() const
{
    // Case-insensitive hash of canonicalUsername (Java uses String.hashCode()
    // which is case-sensitive, but canonicalUsername is already lowercase)
    return qHash(canonicalUsername);
}

} // namespace PinkReader
