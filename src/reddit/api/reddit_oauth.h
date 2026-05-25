// Origin: RedReader/src/main/java/org/quantumbadger/redreader/reddit/api/RedditOAuth.kt
// NOTE: Original is Kotlin (`object RedditOAuth`), ported as C++ namespace/class
#pragma once

#include <string>
#include <memory>
#include <optional>

namespace PinkReader {

// Forward declarations
class RedditAccount;
class UriString;

// Represents FetchRefreshTokenResult from original
enum class FetchRefreshTokenResultStatus {
	SUCCESS,
	USER_REFUSED_PERMISSION,
	INVALID_RESPONSE,
	INVALID_REQUEST,
	CONNECTION_ERROR,
	UNKNOWN_ERROR
};

struct FetchRefreshTokenResult {
	FetchRefreshTokenResultStatus status;
	void* error = nullptr;    // RRError*
	std::string refreshToken;
	std::string accessToken;

	// Success constructor
	FetchRefreshTokenResult(const std::string& refresh, const std::string& access)
		: status(FetchRefreshTokenResultStatus::SUCCESS)
		, refreshToken(refresh), accessToken(access) {}

	// Error constructor
	FetchRefreshTokenResult(FetchRefreshTokenResultStatus status_, void* error_)
		: status(status_), error(error_) {}
};

enum class FetchAccessTokenResultStatus {
	SUCCESS,
	CONNECTION_ERROR,
	UNKNOWN_ERROR
};

struct FetchAccessTokenResult {
	FetchAccessTokenResultStatus status;
	void* error = nullptr;
	std::string accessToken;

	FetchAccessTokenResult(const std::string& token)
		: status(FetchAccessTokenResultStatus::SUCCESS), accessToken(token) {}
	FetchAccessTokenResult(FetchAccessTokenResultStatus s, void* e)
		: status(s), error(e) {}
};

enum class FetchUserInfoResultStatus {
	SUCCESS,
	CONNECTION_ERROR,
	INVALID_RESPONSE,
	UNKNOWN_ERROR
};

struct FetchUserInfoResult {
	FetchUserInfoResultStatus status;
	void* error = nullptr;
	std::string username;

	FetchUserInfoResult(const std::string& name)
		: status(FetchUserInfoResultStatus::SUCCESS), username(name) {}
	FetchUserInfoResult(FetchUserInfoResultStatus s, void* e)
		: status(s), error(e) {}
};

// Simple token wrappers
struct RefreshToken { std::string token; };
struct AccessToken { std::string token; };

class RedditOAuth {
public:
	// Static-like namespace functions
	static RedditOAuth& instance();

	// URI building helpers
	static std::string getPromptUri();
	static std::string getAppId();

	void init(void* context);

	// Check if a user needs re-login (client ID changed)
	static bool needsRelogin(const RedditAccount& user);
	static bool anyNeedRelogin(void* context);

	// Token fetching and login flow
	FetchRefreshTokenResult fetchRefreshTokenSynchronous(
		void* context, const void* redirectUri);

	FetchAccessTokenResult fetchAccessTokenSynchronous(
		void* context, const void* redirectUri);

	FetchUserInfoResult fetchUserInfoSynchronous(
		void* context, const AccessToken& accessToken);

	void loginAsynchronous(void* context, void* callback);

private:
	RedditOAuth() = default;

	static const char* REDIRECT_URI_NEW;
	static const char* ALL_SCOPES;

	void* checkAccess(void* context, const RedditAccount* user);

	FetchRefreshTokenResult handleRefreshTokenError(
		void* exception, int* httpStatus, void* context, const void* uri);

	FetchAccessTokenResult handleAccessTokenError(
		void* exception, int* httpStatus, void* context, const void* uri);
};

} // namespace PinkReader
