/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: reddit_api_action.h - Port of RedReader's RedditAPI.java action methods
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/reddit/RedditAPI.java
 *
 * Contains: action constants, prepareActionUri, vote/save/hide/report/delete
 * endpoints, subscription actions, createPostRequest, createGetRequest,
 * findFailureType, and all inner response handler types.
 */

#pragma once

#include <QString>
#include <QStringList>
#include <QUuid>
#include <functional>
#include <memory>
#include <optional>
#include <vector>

namespace PinkReader {

// Forward declarations
class RedditAccount;
class CacheManager;
class CacheRequest;
class CacheRequestCallbacks;
class CacheRequestJSONParser;
class Priority;
class DownloadStrategy;
class HTTPRequestBody;
class TimestampUTC;
class RRError;
class JsonValue;
class RedditIdAndType;
class SubredditCanonicalId;
class RedditFlairChoice;
struct PostField;

// ============================================================================
// APIResponseHandler::APIFailureType — port of enum inside APIResponseHandler
// ============================================================================
enum class APIFailureType {
    INVALID_USER,
    BAD_CAPTCHA,
    NOTALLOWED,
    SUBREDDIT_REQUIRED,
    URL_REQUIRED,
    UNKNOWN,
    TOO_FAST,
    TOO_LONG,
    ALREADY_SUBMITTED,
    POST_FLAIR_REQUIRED
};

// ============================================================================
// APIResponseHandler base — abstract handler
// ============================================================================
class APIResponseHandler {
public:
    virtual ~APIResponseHandler() = default;

protected:
    virtual void onCallbackException(const std::exception &t) = 0;
    virtual void onFailure(const RRError &error) = 0;

public:
    void notifyFailure(const RRError &error);
    void notifyFailure(APIFailureType type,
                       const std::optional<QString> &debuggingContext,
                       const std::optional<QString> &responseBody);
};

// ============================================================================
// ActionResponseHandler — for actions (vote, save, etc.)
// ============================================================================
class ActionResponseHandler : public APIResponseHandler {
public:
    virtual void notifySuccess() = 0;
};

// ============================================================================
// SubmitResponseHandler — for submit/comment
// ============================================================================
class SubmitResponseHandler : public APIResponseHandler {
public:
    virtual void onSubmitErrors(const std::vector<QString> &errors) = 0;
    virtual void onSuccess(const std::optional<QString> &redirectUrl,
                           const std::optional<QString> &thingId) = 0;
};

// ============================================================================
// UserResponseHandler — for getUser
// ============================================================================
class UserResponseHandler : public APIResponseHandler {
public:
    virtual void notifySuccess(const class RedditUser &user,
                               const TimestampUTC &timestamp) = 0;
};

// ============================================================================
// ValueResponseHandler<T> — generic typed response handler
// ============================================================================
template<typename T>
class ValueResponseHandler : public APIResponseHandler {
public:
    virtual void onSuccess(const T &value) = 0;
};

// ============================================================================
// BlockUserResponseHandler — Java lines 155-159
// ============================================================================
class BlockUserResponseHandler {
public:
    virtual ~BlockUserResponseHandler() = default;
    virtual void onSuccess() = 0;
    virtual void onBlockUserPermissionDenied() = 0;
    virtual void onFailure(const RRError &error) = 0;
};

// ============================================================================
// FlairSelectorResponseHandler — Java lines 161-170
// ============================================================================
class FlairSelectorResponseHandler {
public:
    virtual ~FlairSelectorResponseHandler() = default;
    virtual void onSuccess(const std::vector<RedditFlairChoice> &choices) = 0;
    virtual void onSubredditDoesNotExist() = 0;
    virtual void onSubredditPermissionDenied() = 0;
    virtual void onFailure(const RRError &error) = 0;
};

// ============================================================================
// SubredditListResponse — Java lines 885-895
// ============================================================================
class SubredditListResponse {
public:
    // Note: RedditSubreddit is forward-declared; actual type comes from models
    std::vector<std::shared_ptr<class RedditSubreddit>> subreddits;
    std::optional<QString> after;

    SubredditListResponse() = default;
    SubredditListResponse(std::vector<std::shared_ptr<class RedditSubreddit>> subreddits,
                          std::optional<QString> after)
        : subreddits(std::move(subreddits)), after(std::move(after)) {}
};

// ============================================================================
// RedditAPI — static API action methods
// Port of RedditAPI.java (1121 lines)
// ============================================================================

class RedditAPI {
public:
    // ========================================================================
    // Action constants — Java lines 77-85
    // ========================================================================
    static constexpr int ACTION_UPVOTE   = 0;
    static constexpr int ACTION_UNVOTE   = 1;
    static constexpr int ACTION_DOWNVOTE = 2;
    static constexpr int ACTION_SAVE     = 3;
    static constexpr int ACTION_HIDE     = 4;
    static constexpr int ACTION_UNSAVE   = 5;
    static constexpr int ACTION_UNHIDE   = 6;
    static constexpr int ACTION_REPORT   = 7;
    static constexpr int ACTION_DELETE   = 8;

    // ========================================================================
    // Subscription action constants — Java lines 87-88
    // ========================================================================
    static constexpr int SUBSCRIPTION_ACTION_SUBSCRIBE   = 0;
    static constexpr int SUBSCRIPTION_ACTION_UNSUBSCRIBE = 1;

    // ========================================================================
    // Reddit API path constants — from Constants.Reddit
    // ========================================================================
    static constexpr const char *SCHEME_HTTPS    = "https";
    static constexpr const char *DOMAIN_HTTPS    = "oauth.reddit.com";
    static constexpr const char *PATH_VOTE       = "/api/vote";
    static constexpr const char *PATH_SAVE       = "/api/save";
    static constexpr const char *PATH_HIDE       = "/api/hide";
    static constexpr const char *PATH_UNSAVE     = "/api/unsave";
    static constexpr const char *PATH_UNHIDE     = "/api/unhide";
    static constexpr const char *PATH_REPORT     = "/api/report";
    static constexpr const char *PATH_DELETE     = "/api/del";
    static constexpr const char *PATH_SUBSCRIBE  = "/api/subscribe";
    static constexpr const char *PATH_COMMENTS   = "/comments/";
    static constexpr const char *PATH_ME         = "/api/v1/me";
    static constexpr const char *PATH_SUBREDDITS_POPULAR = "/subreddits/popular.json";
    static constexpr const char *PATH_SUBREDDITS_MINE_SUBSCRIBER = "/subreddits/mine/subscriber.json?limit=100";
    static constexpr const char *PATH_SUBREDDITS_MINE_MODERATOR  = "/subreddits/mine/moderator.json?limit=100";
    static constexpr const char *PATH_MULTIREDDITS_MINE = "/api/multi/mine.json";

    // ========================================================================
    // URL builders — port of Constants.Reddit.getUri / getUriBuilder
    // ========================================================================
    static QString getApiUrl(const QString &path);
    static QString getNonApiUrl(const QString &path);

    // ========================================================================
    // API error detection — port of Constants.Reddit isApiError* methods
    // ========================================================================
    static bool isApiErrorUser(const QString &str);
    static bool isApiErrorCaptcha(const QString &str);
    static bool isApiErrorNotAllowed(const QString &str);
    static bool isApiErrorSubredditRequired(const QString &str);
    static bool isApiErrorURLRequired(const QString &str);
    static bool isApiTooFast(const QString &str);
    static bool isApiTooLong(const QString &str);
    static bool isApiAlreadySubmitted(const QString &str);
    static bool isPostFlairRequired(const QString &str);
    static bool isApiError(const QString &str);

    // ========================================================================
    // main action() method — Java lines 510-529
    // ========================================================================
    static void action(
            CacheManager &cm,
            ActionResponseHandler &responseHandler,
            const RedditAccount &user,
            const RedditIdAndType &idAndType,
            int action,
            void *context); // context is platform-specific (Android Context/Activity)

    // ========================================================================
    // prepareActionUri — Java lines 531-563
    // ========================================================================
    static QString prepareActionUri(int action, std::vector<PostField> &postFields);

    // ========================================================================
    // subscriptionAction — Java lines 565-603
    // ========================================================================
    static void subscriptionAction(
            CacheManager &cm,
            ActionResponseHandler &responseHandler,
            const RedditAccount &user,
            const SubredditCanonicalId &subredditId,
            int action,
            void *context);

    // ========================================================================
    // subscriptionPrepareActionUri — Java lines 605-620
    // ========================================================================
    static QString subscriptionPrepareActionUri(
            int action, std::vector<PostField> &postFields);

    // ========================================================================
    // submit — Java lines 336-377
    // ========================================================================
    static void submit(
            CacheManager &cm,
            SubmitResponseHandler &responseHandler,
            const RedditAccount &user,
            bool isSelfPost,
            const QString &subreddit,
            const QString &title,
            const QString &body,
            bool sendRepliesToInbox,
            bool markAsNsfw,
            bool markAsSpoiler,
            const std::optional<QString> &flairId,
            void *context);

    // ========================================================================
    // compose — Java lines 379-400
    // ========================================================================
    static void compose(
            CacheManager &cm,
            ActionResponseHandler &responseHandler,
            const RedditAccount &user,
            const QString &recipient,
            const QString &subject,
            const QString &body,
            void *context);

    // ========================================================================
    // comment — Java lines 402-456
    // ========================================================================
    static void comment(
            CacheManager &cm,
            SubmitResponseHandler &responseHandler,
            ActionResponseHandler &inboxResponseHandler,
            const RedditAccount &user,
            const RedditIdAndType &parentIdAndType,
            const QString &markdown,
            bool sendRepliesToInbox,
            void *context);

    // ========================================================================
    // markAllAsRead — Java lines 458-488
    // ========================================================================
    static void markAllAsRead(
            CacheManager &cm,
            ActionResponseHandler &responseHandler,
            const RedditAccount &user,
            void *context);

    // ========================================================================
    // editComment — Java lines 490-508
    // ========================================================================
    static void editComment(
            CacheManager &cm,
            ActionResponseHandler &responseHandler,
            const RedditAccount &user,
            const RedditIdAndType &commentIdAndType,
            const QString &markdown,
            void *context);

    // ========================================================================
    // getUser — Java lines 622-669
    // ========================================================================
    static void getUser(
            CacheManager &cm,
            const QString &usernameToGet,
            UserResponseHandler &responseHandler,
            const RedditAccount &user,
            const DownloadStrategy &downloadStrategy,
            void *context);

    // ========================================================================
    // unblockUser — Java lines 671-690
    // ========================================================================
    static void unblockUser(
            CacheManager &cm,
            const QString &usernameToUnblock,
            const QString &currentUserFullname,
            ActionResponseHandler &responseHandler,
            const RedditAccount &user,
            void *context);

    // ========================================================================
    // blockUser — Java lines 692-731
    // ========================================================================
    static void blockUser(
            CacheManager &cm,
            const QString &usernameToBlock,
            BlockUserResponseHandler &responseHandler,
            const RedditAccount &user,
            void *context);

    // ========================================================================
    // sendReplies — Java lines 733-750
    // ========================================================================
    static void sendReplies(
            CacheManager &cm,
            ActionResponseHandler &responseHandler,
            const RedditAccount &user,
            const QString &fullname,
            bool state,
            void *context);

    // ========================================================================
    // flairSelectorForNewLink — Java lines 172-260
    // ========================================================================
    static void flairSelectorForNewLink(
            CacheManager &cm,
            const RedditAccount &user,
            const SubredditCanonicalId &subreddit,
            FlairSelectorResponseHandler &responseHandler,
            void *context);

    // ========================================================================
    // popularSubreddits — Java lines 752-779
    // ========================================================================
    static void popularSubreddits(
            CacheManager &cm,
            const RedditAccount &user,
            ValueResponseHandler<SubredditListResponse> &handler,
            const std::optional<QString> &after,
            void *context);

    // ========================================================================
    // searchSubreddits — Java lines 781-814
    // ========================================================================
    static void searchSubreddits(
            CacheManager &cm,
            const RedditAccount &user,
            const QString &queryString,
            ValueResponseHandler<SubredditListResponse> &handler,
            const std::optional<QString> &after,
            void *context);

    // ========================================================================
    // subscribedSubreddits — Java lines 816-830
    // ========================================================================
    static void subscribedSubreddits(
            CacheManager &cm,
            const RedditAccount &user,
            ValueResponseHandler<std::vector<std::shared_ptr<class RedditSubreddit>>> &handler,
            void *context);

    // ========================================================================
    // requestSubredditList — Java lines 897-960
    // ========================================================================
    static void requestSubredditList(
            CacheManager &cm,
            const QString &uri,
            const RedditAccount &user,
            ValueResponseHandler<SubredditListResponse> &handler,
            const DownloadStrategy &downloadStrategy,
            void *context);

    // ========================================================================
    // findFailureType — Java lines 962-1060
    // ========================================================================
    static std::optional<APIFailureType> findFailureType(const JsonValue &response);

    // ========================================================================
    // createPostRequest — Java lines 1062-1076
    // ========================================================================
    static CacheRequest createPostRequest(
            const QString &url,
            const RedditAccount &user,
            const std::vector<PostField> &postFields,
            const CacheRequestJSONParser &parser,
            void *context);

    // ========================================================================
    // createPostRequestUnprocessedResponse — Java lines 1078-1097
    // ========================================================================
    static CacheRequest createPostRequestUnprocessedResponse(
            const QString &url,
            const RedditAccount &user,
            const std::vector<PostField> &postFields,
            const CacheRequestCallbacks &callbacks,
            void *context);

    // ========================================================================
    // createGetRequest — Java lines 1099-1120
    // ========================================================================
    static CacheRequest createGetRequest(
            const QString &url,
            const RedditAccount &user,
            const Priority &priority,
            int fileType,
            const DownloadStrategy &downloadStrategy,
            const CacheRequestJSONParser &parser,
            void *context);

private:
    // ========================================================================
    // subscribedSubredditsInternal — Java lines 832-883
    // ========================================================================
    static void subscribedSubredditsInternal(
            CacheManager &cm,
            const RedditAccount &user,
            ValueResponseHandler<std::vector<std::shared_ptr<class RedditSubreddit>>> &handler,
            const std::optional<QString> &after,
            std::vector<std::shared_ptr<class RedditSubreddit>> &results,
            void *context);
};

} // namespace PinkReader
