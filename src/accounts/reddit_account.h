// Origin: RedReader/src/main/java/org/quantumbadger/redreader/account/RedditAccount.java
#pragma once

#include <string>
#include <cstdint>

namespace PinkReader {

// Forward declaration
namespace RedditOAuth {
	struct RefreshToken;
	struct AccessToken;
}

/**
 * @brief Port of org.quantumbadger.redreader.account.RedditAccount
 *
 * Represents a Reddit user account with username, refresh token, priority, and client ID.
 * Every field and method from the original Java file.
 */
class RedditAccount {
public:
	// Port of: @NonNull public final String username;
	std::string username;
	// Port of: @NonNull public final String canonicalUsername;
	std::string canonicalUsername;
	// Port of: public final RedditOAuth.RefreshToken refreshToken;
	// Nullable: empty string token = null
	std::string refreshToken;
	// Port of: public final long priority;
	int64_t priority;
	// Port of: @Nullable public final String clientId;
	std::string clientId; // empty = null

	// Port of: private RedditOAuth.AccessToken accessToken;
	// (synchronized access via getMostRecentAccessToken/setAccessToken)
	std::string accessToken; // empty = null

	// Port of: public RedditAccount(@NonNull final String username,
	//   final RedditOAuth.RefreshToken refreshToken, final long priority,
	//   @Nullable final String clientId)
	//   if(username == null) { throw new RuntimeException("Null user in RedditAccount"); }
	//   this.username = username.trim();
	//   this.canonicalUsername = StringUtils.asciiLowercase(this.username);
	//   this.refreshToken = refreshToken;
	//   this.priority = priority;
	//   this.clientId = clientId;
	RedditAccount(const std::string& username, const std::string& refreshToken,
	              int64_t priority, const std::string& clientId);
	RedditAccount() = default;

	// Port of: public boolean isAnonymous() { return username.isEmpty(); }
	bool isAnonymous() const;

	// Port of: public boolean isNotAnonymous() { return !isAnonymous(); }
	bool isNotAnonymous() const;

	// Port of: public String getCanonicalUsername() { return canonicalUsername; }
	const std::string& getCanonicalUsername() const;

	// Port of: public synchronized RedditOAuth.AccessToken getMostRecentAccessToken()
	//   return accessToken;
	std::string getMostRecentAccessToken() const;

	// Port of: public synchronized void setAccessToken(final RedditOAuth.AccessToken token)
	//   accessToken = token;
	void setAccessToken(const std::string& token);

	// Port of: @Override public boolean equals(final Object o)
	//   if(!(o instanceof RedditAccount)) return false;
	//   final RedditAccount other = (RedditAccount)o;
	//   return canonicalUsername.equalsIgnoreCase(other.canonicalUsername)
	//     && Objects.equals(clientId, other.clientId)
	//     && Objects.equals(refreshToken, other.refreshToken);
	bool equals(const RedditAccount& other) const;

	// Port of: @Override public int hashCode()
	//   return getCanonicalUsername().hashCode();
	size_t hashCode() const;
};

} // namespace PinkReader
