// Origin: RedReader/src/main/java/org/quantumbadger/redreader/account/RedditAccountId.kt
#include "reddit_account_id.h"
#include "../common/string_utils_kt.h"
#include <algorithm>

namespace PinkReader {

// Port of: companion object { val ANON = RedditAccountId("") }
const RedditAccountId RedditAccountId::ANON("");

// Port of: data class constructor RedditAccountId(val username: String)
RedditAccountId::RedditAccountId(const std::string& username)
	: username(username) {
	// Port of: val canonicalUsername = username.trim().asciiLowercase()
	// Trim whitespace
	std::string trimmed = username;
	trimmed.erase(0, trimmed.find_first_not_of(" \t\n\r\f\v"));
	trimmed.erase(trimmed.find_last_not_of(" \t\n\r\f\v") + 1);
	canonicalUsername = asciiLowercase(trimmed);
}

} // namespace PinkReader
