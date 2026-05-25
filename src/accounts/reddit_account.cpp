// Origin: RedReader/src/main/java/org/quantumbadger/redreader/account/RedditAccount.java
#include "reddit_account.h"
#include "../common/string_utils.h"
#include <algorithm>
#include <functional>
#include <cstring>

namespace PinkReader {

// Port of: public RedditAccount(@NonNull final String username,
//   final RedditOAuth.RefreshToken refreshToken, final long priority,
//   @Nullable final String clientId)
RedditAccount::RedditAccount(const std::string& username, const std::string& refreshToken,
                             int64_t priority, const std::string& clientId)
	: refreshToken(refreshToken)
	, priority(priority)
	, clientId(clientId) {

	// Port of: if(username == null) { throw new RuntimeException("Null user in RedditAccount"); }
	// (std::string cannot be null, so we skip null check)

	// Port of: this.username = username.trim();
	std::string trimmed = username;
	trimmed.erase(0, trimmed.find_first_not_of(" \t\n\r\f\v"));
	trimmed.erase(trimmed.find_last_not_of(" \t\n\r\f\v") + 1);
	this->username = trimmed;

	// Port of: this.canonicalUsername = StringUtils.asciiLowercase(this.username);
	this->canonicalUsername = StringUtils::asciiLowercase(this->username);
}

// Port of: public boolean isAnonymous() { return username.isEmpty(); }
bool RedditAccount::isAnonymous() const {
	return username.empty();
}

// Port of: public boolean isNotAnonymous() { return !isAnonymous(); }
bool RedditAccount::isNotAnonymous() const {
	return !isAnonymous();
}

// Port of: public String getCanonicalUsername() { return canonicalUsername; }
const std::string& RedditAccount::getCanonicalUsername() const {
	return canonicalUsername;
}

// Port of: public synchronized RedditOAuth.AccessToken getMostRecentAccessToken()
std::string RedditAccount::getMostRecentAccessToken() const {
	// Port of: return accessToken;
	// NOTE: synchronized in Java; in C++ we assume single-threaded or caller-managed sync
	return accessToken;
}

// Port of: public synchronized void setAccessToken(final RedditOAuth.AccessToken token)
void RedditAccount::setAccessToken(const std::string& token) {
	// Port of: accessToken = token;
	accessToken = token;
}

// Port of: @Override public boolean equals(final Object o)
bool RedditAccount::equals(const RedditAccount& other) const {
	// Port of: if(!(o instanceof RedditAccount)) return false;
	// (handled by C++ type system)

	// Port of: return canonicalUsername.equalsIgnoreCase(other.canonicalUsername)
	//   && Objects.equals(clientId, other.clientId)
	//   && Objects.equals(refreshToken, other.refreshToken);
	// equalsIgnoreCase: compare lowercase
	std::string thisLower = StringUtils::asciiLowercase(canonicalUsername);
	std::string otherLower = StringUtils::asciiLowercase(other.canonicalUsername);
	return thisLower == otherLower
		&& clientId == other.clientId
		&& refreshToken == other.refreshToken;
}

// Port of: @Override public int hashCode()
size_t RedditAccount::hashCode() const {
	// Port of: return getCanonicalUsername().hashCode();
	return std::hash<std::string>{}(canonicalUsername);
}

} // namespace PinkReader
