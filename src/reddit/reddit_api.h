// Origin: org/quantumbadger/redreader/reddit/RedditAPI.java
#pragma once

#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <functional>

namespace PinkReader {

// Forward declarations for all types used
class Context;
class AppCompatActivity;
class CacheManager;
class RedditAccount;
class SubredditCanonicalId;
class RedditIdAndType;
class RRError;
class TimestampUTC;
class TimestampBound;
class TimeDuration;
class DownloadStrategy;
class DownloadStrategyAlways;
class DownloadStrategyIfTimestampOutsideBounds;
class Priority;
class UriString;
class PostField;
class HTTPRequestBody;
template<typename T> class Optional;
class SeekableInputStream;
class JsonValue;
class JsonArray;
class JsonString;
class JsonObject;
class FailedRequestBody;
class CacheRequest;
class CacheRequestCallbacks;
class CacheRequestJSONParser;
class RedditSubreddit;
class RedditThing;
class RedditUser;
class RedditFlairChoice;
template<typename T, typename E> class GenericFactory;
template<typename K, typename V, typename E> class RequestResponseHandler;
template<typename K, typename V> class UpdatedVersionListener;
template<typename E> class ValueResponseHandler;
class APIResponseHandler_APIFailureType; // APIFailureType from APIResponseHandler

class RedditAPI {
public:
	// Action constants
	static const int ACTION_UPVOTE = 0;
	static const int ACTION_UNVOTE = 1;
	static const int ACTION_DOWNVOTE = 2;
	static const int ACTION_SAVE = 3;
	static const int ACTION_HIDE = 4;
	static const int ACTION_UNSAVE = 5;
	static const int ACTION_UNHIDE = 6;
	static const int ACTION_REPORT = 7;
	static const int ACTION_DELETE = 8;

	// Subscription action constants
	static const int SUBSCRIPTION_ACTION_SUBSCRIBE = 0;
	static const int SUBSCRIPTION_ACTION_UNSUBSCRIBE = 1;

	// Response handler interfaces
	class BlockUserResponseHandler {
	public:
		virtual ~BlockUserResponseHandler() = default;
		virtual void onSuccess() = 0;
		virtual void onBlockUserPermissionDenied() = 0;
		virtual void onFailure(const RRError& error) = 0;
	};

	class FlairSelectorResponseHandler {
	public:
		virtual ~FlairSelectorResponseHandler() = default;
		virtual void onSuccess(const std::vector<RedditFlairChoice>& choices) = 0;
		virtual void onSubredditDoesNotExist() = 0;
		virtual void onSubredditPermissionDenied() = 0;
		virtual void onFailure(const RRError& error) = 0;
	};

	// SubredditListResponse
	class SubredditListResponse {
	public:
		std::vector<RedditSubreddit> subreddits;
		std::optional<std::string> after;

		SubredditListResponse(
				std::vector<RedditSubreddit> subreddits,
				std::optional<std::string> after)
			: subreddits(std::move(subreddits)), after(std::move(after)) {}
	};

	// flairSelectorForNewLink
	static void flairSelectorForNewLink(
			Context& context,
			CacheManager& cm,
			RedditAccount& user,
			const SubredditCanonicalId& subreddit,
			FlairSelectorResponseHandler& responseHandler);

	// submit
	static void submit(
			CacheManager& cm,
			class SubmitResponseHandler& responseHandler,
			RedditAccount& user,
			bool isSelfPost,
			const std::string& subreddit,
			const std::string& title,
			const std::string& body,
			bool sendRepliesToInbox,
			bool markAsNsfw,
			bool markAsSpoiler,
			const std::string* flairId,
			Context& context);

	// compose
	static void compose(
			CacheManager& cm,
			class ActionResponseHandler& responseHandler,
			RedditAccount& user,
			const std::string& recipient,
			const std::string& subject,
			const std::string& body,
			Context& context);

	// comment
	static void comment(
			CacheManager& cm,
			class SubmitResponseHandler& responseHandler,
			class ActionResponseHandler& inboxResponseHandler,
			RedditAccount& user,
			const RedditIdAndType& parentIdAndType,
			const std::string& markdown,
			bool sendRepliesToInbox,
			AppCompatActivity& context);

	// markAllAsRead
	static void markAllAsRead(
			CacheManager& cm,
			class ActionResponseHandler& responseHandler,
			RedditAccount& user,
			Context& context);

	// editComment
	static void editComment(
			CacheManager& cm,
			class ActionResponseHandler& responseHandler,
			RedditAccount& user,
			const RedditIdAndType& commentIdAndType,
			const std::string& markdown,
			Context& context);

	// action
	static void action(
			CacheManager& cm,
			class ActionResponseHandler& responseHandler,
			RedditAccount& user,
			const RedditIdAndType& idAndType,
			int action,
			Context& context);

	// subscriptionAction
	static void subscriptionAction(
			CacheManager& cm,
			class ActionResponseHandler& responseHandler,
			RedditAccount& user,
			const SubredditCanonicalId& subredditId,
			int action,
			Context& context);

	// getUser
	static void getUser(
			CacheManager& cm,
			const std::string& usernameToGet,
			class UserResponseHandler& responseHandler,
			RedditAccount& user,
			DownloadStrategy& downloadStrategy,
			Context& context);

	// unblockUser
	static void unblockUser(
			CacheManager& cm,
			const std::string& usernameToUnblock,
			const std::string& currentUserFullname,
			class ActionResponseHandler& responseHandler,
			RedditAccount& user,
			Context& context);

	// blockUser
	static void blockUser(
			CacheManager& cm,
			const std::string& usernameToBlock,
			BlockUserResponseHandler& responseHandler,
			RedditAccount& user,
			Context& context);

	// sendReplies
	static void sendReplies(
			CacheManager& cm,
			class ActionResponseHandler& responseHandler,
			RedditAccount& user,
			const std::string& fullname,
			bool state,
			Context& context);

	// popularSubreddits
	static void popularSubreddits(
			CacheManager& cm,
			RedditAccount& user,
			Context& context,
			class ValueResponseHandler<SubredditListResponse>& handler,
			const std::optional<std::string>& after);

	// searchSubreddits
	static void searchSubreddits(
			CacheManager& cm,
			RedditAccount& user,
			const std::string& queryString,
			Context& context,
			class ValueResponseHandler<SubredditListResponse>& handler,
			const std::optional<std::string>& after);

	// subscribedSubreddits
	static void subscribedSubreddits(
			CacheManager& cm,
			RedditAccount& user,
			AppCompatActivity& context,
			class ValueResponseHandler<std::vector<RedditSubreddit>>& handler);

	// requestSubredditList
	static void requestSubredditList(
			CacheManager& cm,
			const UriString& uri,
			RedditAccount& user,
			Context& context,
			class ValueResponseHandler<SubredditListResponse>& handler,
			DownloadStrategy& downloadStrategy);

private:
	// Inner classes
	class GenericResponseHandler;
	class SubmitJSONListener;

	// Helper methods
	static UriString prepareActionUri(int action, std::vector<PostField>& postFields);
	static UriString subscriptionPrepareActionUri(int action, std::vector<PostField>& postFields);
	static int findFailureType(const JsonValue& response);

	static void subscribedSubredditsInternal(
			CacheManager& cm,
			RedditAccount& user,
			AppCompatActivity& context,
			class ValueResponseHandler<std::vector<RedditSubreddit>>& handler,
			const std::optional<std::string>& after,
			std::vector<RedditSubreddit>& results);

	static CacheRequest* createPostRequest(
			const UriString& url,
			RedditAccount& user,
			const std::vector<PostField>& postFields,
			Context& context,
			CacheRequestJSONParser::Listener& handler);

	static CacheRequest* createPostRequestUnprocessedResponse(
			const UriString& url,
			RedditAccount& user,
			const std::vector<PostField>& postFields,
			Context& context,
			CacheRequestCallbacks& callbacks);

	static CacheRequest* createGetRequest(
			const UriString& url,
			RedditAccount& user,
			const Priority& priority,
			int fileType,
			DownloadStrategy& downloadStrategy,
			Context& context,
			CacheRequestJSONParser::Listener& handler);
};

} // namespace PinkReader
