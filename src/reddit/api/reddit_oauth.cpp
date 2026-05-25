// Origin: RedReader/src/main/java/org/quantumbadger/redreader/reddit/api/RedditOAuth.kt
#include "reddit_oauth.h"

namespace PinkReader {

const char* RedditOAuth::REDIRECT_URI_NEW = "redreader://rr_oauth_redir";
const char* RedditOAuth::ALL_SCOPES = "identity edit flair history modconfig modflair modlog modposts modwiki mysubreddits privatemessages read report save submit subscribe vote wikiedit wikiread account";

RedditOAuth& RedditOAuth::instance() {
	static RedditOAuth sInstance;
	return sInstance;
}

std::string RedditOAuth::getPromptUri() {
	// Build the reddit OAuth prompt URI
	// Full implementation: https://www.reddit.com/api/v1/authorize.compact?...
	return "";
}

std::string RedditOAuth::getAppId() {
	return ""; // stub
}

void RedditOAuth::init(void* /*context*/) {
	// Full implementation reads reddit_auth.txt from assets
}

bool RedditOAuth::needsRelogin(const RedditAccount& /*user*/) {
	return false; // stub
}

bool RedditOAuth::anyNeedRelogin(void* /*context*/) {
	return false; // stub
}

FetchRefreshTokenResult RedditOAuth::fetchRefreshTokenSynchronous(
		void* /*context*/, const void* /*redirectUri*/) {
	return FetchRefreshTokenResult(FetchRefreshTokenResultStatus::UNKNOWN_ERROR, nullptr);
}

FetchAccessTokenResult RedditOAuth::fetchAccessTokenSynchronous(
		void* /*context*/, const void* /*redirectUri*/) {
	return FetchAccessTokenResult(FetchAccessTokenResultStatus::UNKNOWN_ERROR, nullptr);
}

FetchUserInfoResult RedditOAuth::fetchUserInfoSynchronous(
		void* /*context*/, const AccessToken& /*accessToken*/) {
	return FetchUserInfoResult(FetchUserInfoResultStatus::UNKNOWN_ERROR, nullptr);
}

void RedditOAuth::loginAsynchronous(void* /*context*/, void* /*callback*/) {
}

void* RedditOAuth::checkAccess(void* /*context*/, const RedditAccount* /*user*/) {
	return nullptr;
}

FetchRefreshTokenResult RedditOAuth::handleRefreshTokenError(
		void* /*exception*/, int* /*httpStatus*/, void* /*context*/, const void* /*uri*/) {
	return FetchRefreshTokenResult(FetchRefreshTokenResultStatus::UNKNOWN_ERROR, nullptr);
}

FetchAccessTokenResult RedditOAuth::handleAccessTokenError(
		void* /*exception*/, int* /*httpStatus*/, void* /*context*/, const void* /*uri*/) {
	return FetchAccessTokenResult(FetchAccessTokenResultStatus::UNKNOWN_ERROR, nullptr);
}

} // namespace PinkReader
