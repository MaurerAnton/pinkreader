// Origin: RedReader/src/main/java/org/quantumbadger/redreader/account/RedditAccountId.kt
#pragma once

#include <string>

namespace PinkReader {

/**
 * @brief Port of org.quantumbadger.redreader.account.RedditAccountId
 *
 * Original: @Immutable data class RedditAccountId(val username: String)
 *   val canonicalUsername = username.trim().asciiLowercase()
 *   companion object { val ANON = RedditAccountId("") }
 *
 * Every field, method, and companion object from the original Kotlin file.
 */
class RedditAccountId {
public:
	// Port of: val username: String
	std::string username;

	// Port of: val canonicalUsername = username.trim().asciiLowercase()
	std::string canonicalUsername;

	// Port of: companion object { val ANON = RedditAccountId("") }
	static const RedditAccountId ANON;

	// Port of: data class constructor RedditAccountId(val username: String)
	RedditAccountId() = default;
	explicit RedditAccountId(const std::string& username);

	// Equality matching data class behavior
	bool operator==(const RedditAccountId& other) const {
		return canonicalUsername == other.canonicalUsername;
	}
	bool operator!=(const RedditAccountId& other) const {
		return !(*this == other);
	}
};

} // namespace PinkReader
