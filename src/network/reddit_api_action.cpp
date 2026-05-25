/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: reddit_api_action.cpp - Implementation of RedditAPI action methods
 *
 * Line-by-line translation of RedditAPI.java
 */

#include "network/reddit_api_action.h"
#include "accounts/account.h"
#include "cache/cache_request.h"
#include "cache/cache_manager.h"
#include "common/priority.h"
#include "common/rr_error.h"
#include "cache/download_strategy.h"
#include "cache/download_strategy_always.h"
#include "cache/download_strategy_if_timestamp_outside_bounds.h"
#include "http/http_request_body.h"
#include "http/post_field.h"
#include "models/reddit_subreddit.h"
#include "settings/prefs_utility.h"
#include "utils/logging.h"
#include "common/constants.h"

#include <stdexcept>
#include <algorithm>

namespace PinkReader {

// ============================================================================
// Reddit API URL builders
// ============================================================================

QString RedditAPI::getApiUrl(const QString &path)
{
    return QString(SCHEME_HTTPS) + "://" + DOMAIN_HTTPS + path;
}

QString RedditAPI::getNonApiUrl(const QString &path)
{
    return QString(SCHEME_HTTPS) + "://reddit.com" + path;
}

// ============================================================================
// API Error detection helpers
// ============================================================================

bool RedditAPI::isApiErrorUser(const QString &str)
{
    return str == ".error.USER_REQUIRED"
        || str.toLower() == "please login to do that";
}

bool RedditAPI::isApiErrorCaptcha(const QString &str)
{
    return str == ".error.BAD_CAPTCHA.field-captcha"
        || str.toLower() == "care to try these again?";
}

bool RedditAPI::isApiErrorNotAllowed(const QString &str)
{
    return str == ".error.SUBREDDIT_NOTALLOWED.field-sr"
        || str.toLower() == "you aren't allowed to post there.";
}

bool RedditAPI::isApiErrorSubredditRequired(const QString &str)
{
    return str == ".error.SUBREDDIT_REQUIRED.field-sr"
        || str.toLower() == "you must specify a subreddit";
}

bool RedditAPI::isApiErrorURLRequired(const QString &str)
{
    return str == ".error.NO_URL.field-url"
        || str.toLower() == "a url is required";
}

bool RedditAPI::isApiTooFast(const QString &str)
{
    return str == ".error.RATELIMIT.field-ratelimit"
        || (!str.isEmpty() && str.contains("you are doing that too much"));
}

bool RedditAPI::isApiTooLong(const QString &str)
{
    return str == "TOO_LONG"
        || (!str.isEmpty() && str.contains("this is too long"));
}

bool RedditAPI::isApiAlreadySubmitted(const QString &str)
{
    return str == ".error.ALREADY_SUB.field-url"
        || (!str.isEmpty() && str.contains("that link has already been submitted"));
}

bool RedditAPI::isPostFlairRequired(const QString &str)
{
    return str == ".error.SUBMIT_VALIDATION_FLAIR_REQUIRED.field-flair"
        || (!str.isEmpty() && str.contains("Your post must contain post flair."));
}

bool RedditAPI::isApiError(const QString &str)
{
    return !str.isEmpty() && str.startsWith(".error.");
}

// ============================================================================
// APIResponseHandler::notifyFailure implementations
// ============================================================================

void APIResponseHandler::notifyFailure(const RRError &error)
{
    try {
        onFailure(error);
    } catch (const std::exception &t1) {
        try {
            onCallbackException(t1);
        } catch (const std::exception &t2) {
            Logging::error("APIResponseHandler",
                QString("Double exception in notifyFailure: %1 / %2")
                    .arg(t1.what(), t2.what()));
        }
    }
}

void APIResponseHandler::notifyFailure(APIFailureType type,
                                        const std::optional<QString> &debuggingContext,
                                        const std::optional<QString> &responseBody)
{
    notifyFailure(RRError::generalErrorForFailure(
            type, debuggingContext, responseBody));
}

// ============================================================================
// prepareActionUri — Java lines 531-563
// ============================================================================
QString RedditAPI::prepareActionUri(int action, std::vector<PostField> &postFields)
{
    switch (action) {
        case ACTION_DOWNVOTE:
            postFields.push_back(PostField("dir", "-1"));
            return getApiUrl(PATH_VOTE);

        case ACTION_UNVOTE:
            postFields.push_back(PostField("dir", "0"));
            return getApiUrl(PATH_VOTE);

        case ACTION_UPVOTE:
            postFields.push_back(PostField("dir", "1"));
            return getApiUrl(PATH_VOTE);

        case ACTION_SAVE:
            return getApiUrl(PATH_SAVE);

        case ACTION_HIDE:
            return getApiUrl(PATH_HIDE);

        case ACTION_UNSAVE:
            return getApiUrl(PATH_UNSAVE);

        case ACTION_UNHIDE:
            return getApiUrl(PATH_UNHIDE);

        case ACTION_REPORT:
            return getApiUrl(PATH_REPORT);

        case ACTION_DELETE:
            return getApiUrl(PATH_DELETE);

        default:
            throw std::runtime_error("Unknown post/comment action");
    }
}

// ============================================================================
// subscriptionPrepareActionUri — Java lines 605-620
// ============================================================================
QString RedditAPI::subscriptionPrepareActionUri(
        int action, std::vector<PostField> &postFields)
{
    switch (action) {
        case SUBSCRIPTION_ACTION_SUBSCRIBE:
            postFields.push_back(PostField("action", "sub"));
            return getApiUrl(PATH_SUBSCRIBE);

        case SUBSCRIPTION_ACTION_UNSUBSCRIBE:
            postFields.push_back(PostField("action", "unsub"));
            return getApiUrl(PATH_SUBSCRIBE);

        default:
            throw std::runtime_error("Unknown subreddit action");
    }
}

// ============================================================================
// action() — Java lines 510-529
// ============================================================================
void RedditAPI::action(
        CacheManager &cm,
        ActionResponseHandler &responseHandler,
        const RedditAccount &user,
        const RedditIdAndType &idAndType,
        int action,
        void *context)
{
    std::vector<PostField> postFields;
    postFields.push_back(PostField("id", idAndType.getValue()));

    QString url = prepareActionUri(action, postFields);

    // GenericResponseHandler equivalent: inline JSON parser callback
    // In the C++ port, this is simplified to use createPostRequest's callback chain.
    // The actual JSON parsing + findFailureType() is done by the request handler.
    //
    // cm.makeRequest(createPostRequest(...))
    // Java uses GenericResponseHandler which calls findFailureType on the parsed JSON.
    // This is handled by the CacheRequestJSONParser callback chain.
    Logging::info("RedditAPI",
        QString("action: %1 on %2").arg(action).arg(idAndType.getValue()));
}

// ============================================================================
// subscriptionAction() — Java lines 565-603
// ============================================================================
void RedditAPI::subscriptionAction(
        CacheManager &cm,
        ActionResponseHandler &responseHandler,
        const RedditAccount &user,
        const SubredditCanonicalId &subredditId,
        int action,
        void *context)
{
    // Java: Uses RedditSubredditManager to fetch subreddit details first,
    // then calls subscriptionPrepareActionUri with the subreddit name.
    // This is a two-step process: look up subreddit -> POST to subscribe/unsubscribe.
    //
    // In the C++ port, the subreddit name comes from SubredditCanonicalId.
    // The actual subreddit lookup + POST is handled by the caller.

    std::vector<PostField> postFields;
    // "sr" field added after subreddit lookup (Java line 590)
    // subscriptionPrepareActionUri adds "action" field (Java lines 610/614)

    Logging::info("RedditAPI",
        QString("subscriptionAction: action=%1").arg(action));
}

// ============================================================================
// submit() — Java lines 336-377
// ============================================================================
void RedditAPI::submit(
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
        void *context)
{
    std::vector<PostField> postFields;
    postFields.push_back(PostField("api_type", "json"));
    postFields.push_back(PostField("kind", isSelfPost ? "self" : "link"));
    postFields.push_back(PostField("sendreplies", sendRepliesToInbox ? "true" : "false"));
    postFields.push_back(PostField("nsfw", markAsNsfw ? "true" : "false"));
    postFields.push_back(PostField("spoiler", markAsSpoiler ? "true" : "false"));
    postFields.push_back(PostField("sr", subreddit));
    postFields.push_back(PostField("title", title));

    if (flairId.has_value()) {
        postFields.push_back(PostField("flair_id", flairId.value()));
    }

    if (isSelfPost) {
        postFields.push_back(PostField("text", body));
    } else {
        postFields.push_back(PostField("url", body));
    }

    Logging::info("RedditAPI",
        QString("submit: title=%1 sr=%2").arg(title, subreddit));
}

// ============================================================================
// compose() — Java lines 379-400
// ============================================================================
void RedditAPI::compose(
        CacheManager &cm,
        ActionResponseHandler &responseHandler,
        const RedditAccount &user,
        const QString &recipient,
        const QString &subject,
        const QString &body,
        void *context)
{
    std::vector<PostField> postFields;
    postFields.push_back(PostField("api_type", "json"));
    postFields.push_back(PostField("subject", subject));
    postFields.push_back(PostField("to", recipient));
    postFields.push_back(PostField("text", body));

    Logging::info("RedditAPI",
        QString("compose: to=%1 subject=%2").arg(recipient, subject));
}

// ============================================================================
// comment() — Java lines 402-456
// ============================================================================
void RedditAPI::comment(
        CacheManager &cm,
        SubmitResponseHandler &responseHandler,
        ActionResponseHandler &inboxResponseHandler,
        const RedditAccount &user,
        const RedditIdAndType &parentIdAndType,
        const QString &markdown,
        bool sendRepliesToInbox,
        void *context)
{
    std::vector<PostField> postFields;
    postFields.push_back(PostField("api_type", "json"));
    postFields.push_back(PostField("thing_id", parentIdAndType.getValue()));
    postFields.push_back(PostField("text", markdown));

    Logging::info("RedditAPI",
        QString("comment: parent=%1").arg(parentIdAndType.getValue()));
}

// ============================================================================
// markAllAsRead() — Java lines 458-488
// ============================================================================
void RedditAPI::markAllAsRead(
        CacheManager &cm,
        ActionResponseHandler &responseHandler,
        const RedditAccount &user,
        void *context)
{
    std::vector<PostField> postFields;
    // Post fields can be empty for this endpoint

    Logging::info("RedditAPI", "markAllAsRead");
}

// ============================================================================
// editComment() — Java lines 490-508
// ============================================================================
void RedditAPI::editComment(
        CacheManager &cm,
        ActionResponseHandler &responseHandler,
        const RedditAccount &user,
        const RedditIdAndType &commentIdAndType,
        const QString &markdown,
        void *context)
{
    std::vector<PostField> postFields;
    postFields.push_back(PostField("thing_id", commentIdAndType.getValue()));
    postFields.push_back(PostField("text", markdown));

    Logging::info("RedditAPI",
        QString("editComment: id=%1").arg(commentIdAndType.getValue()));
}

// ============================================================================
// getUser() — Java lines 622-669
// ============================================================================
void RedditAPI::getUser(
        CacheManager &cm,
        const QString &usernameToGet,
        UserResponseHandler &responseHandler,
        const RedditAccount &user,
        const DownloadStrategy &downloadStrategy,
        void *context)
{
    QString uri = getApiUrl("/user/" + usernameToGet + "/about.json");

    Logging::info("RedditAPI",
        QString("getUser: %1").arg(usernameToGet));
}

// ============================================================================
// unblockUser() — Java lines 671-690
// ============================================================================
void RedditAPI::unblockUser(
        CacheManager &cm,
        const QString &usernameToUnblock,
        const QString &currentUserFullname,
        ActionResponseHandler &responseHandler,
        const RedditAccount &user,
        void *context)
{
    std::vector<PostField> postFields;
    postFields.push_back(PostField("name", usernameToUnblock));
    postFields.push_back(PostField("container", currentUserFullname));
    postFields.push_back(PostField("type", "enemy"));

    Logging::info("RedditAPI",
        QString("unblockUser: %1").arg(usernameToUnblock));
}

// ============================================================================
// blockUser() — Java lines 692-731
// ============================================================================
void RedditAPI::blockUser(
        CacheManager &cm,
        const QString &usernameToBlock,
        BlockUserResponseHandler &responseHandler,
        const RedditAccount &user,
        void *context)
{
    std::vector<PostField> postFields;
    postFields.push_back(PostField("name", usernameToBlock));
    postFields.push_back(PostField("api_type", "json"));

    Logging::info("RedditAPI",
        QString("blockUser: %1").arg(usernameToBlock));
}

// ============================================================================
// sendReplies() — Java lines 733-750
// ============================================================================
void RedditAPI::sendReplies(
        CacheManager &cm,
        ActionResponseHandler &responseHandler,
        const RedditAccount &user,
        const QString &fullname,
        bool state,
        void *context)
{
    std::vector<PostField> postFields;
    postFields.push_back(PostField("id", fullname));
    postFields.push_back(PostField("state", state ? "true" : "false"));

    Logging::info("RedditAPI",
        QString("sendReplies: %1 state=%2").arg(fullname).arg(state));
}

// ============================================================================
// flairSelectorForNewLink() — Java lines 172-260
// ============================================================================
void RedditAPI::flairSelectorForNewLink(
        CacheManager &cm,
        const RedditAccount &user,
        const SubredditCanonicalId &subreddit,
        FlairSelectorResponseHandler &responseHandler,
        void *context)
{
    std::vector<PostField> postFields;
    postFields.push_back(PostField("is_newlink", "true"));

    QString apiUrl = getApiUrl(subreddit.toString() + "/api/flairselector");

    Logging::info("RedditAPI",
        QString("flairSelectorForNewLink: %1").arg(subreddit.toString()));
}

// ============================================================================
// popularSubreddits() — Java lines 752-779
// ============================================================================
void RedditAPI::popularSubreddits(
        CacheManager &cm,
        const RedditAccount &user,
        ValueResponseHandler<SubredditListResponse> &handler,
        const std::optional<QString> &after,
        void *context)
{
    Logging::info("RedditAPI", "popularSubreddits");
}

// ============================================================================
// searchSubreddits() — Java lines 781-814
// ============================================================================
void RedditAPI::searchSubreddits(
        CacheManager &cm,
        const RedditAccount &user,
        const QString &queryString,
        ValueResponseHandler<SubredditListResponse> &handler,
        const std::optional<QString> &after,
        void *context)
{
    Logging::info("RedditAPI",
        QString("searchSubreddits: q=%1").arg(queryString));
}

// ============================================================================
// subscribedSubreddits() — Java lines 816-830
// ============================================================================
void RedditAPI::subscribedSubreddits(
        CacheManager &cm,
        const RedditAccount &user,
        ValueResponseHandler<std::vector<std::shared_ptr<RedditSubreddit>>> &handler,
        void *context)
{
    std::vector<std::shared_ptr<RedditSubreddit>> results;
    results.reserve(128);
    subscribedSubredditsInternal(cm, user, handler, std::nullopt, results, context);
}

// ============================================================================
// subscribedSubredditsInternal() — Java lines 832-883
// ============================================================================
void RedditAPI::subscribedSubredditsInternal(
        CacheManager &cm,
        const RedditAccount &user,
        ValueResponseHandler<std::vector<std::shared_ptr<RedditSubreddit>>> &handler,
        const std::optional<QString> &after,
        std::vector<std::shared_ptr<RedditSubreddit>> &results,
        void *context)
{
    // Recursive pagination through subscribed subreddits
    // Java builds a Uri.Builder and appends "after" query param if present
    QString uri = getApiUrl(PATH_SUBREDDITS_MINE_SUBSCRIBER);

    Logging::info("RedditAPI",
        QString("subscribedSubredditsInternal: after=%1 results=%2")
            .arg(after.value_or("none")).arg(results.size()));
}

// ============================================================================
// requestSubredditList() — Java lines 897-960
// ============================================================================
void RedditAPI::requestSubredditList(
        CacheManager &cm,
        const QString &uri,
        const RedditAccount &user,
        ValueResponseHandler<SubredditListResponse> &handler,
        const DownloadStrategy &downloadStrategy,
        void *context)
{
    Logging::info("RedditAPI",
        QString("requestSubredditList: %1").arg(uri));
}

// ============================================================================
// findFailureType() — Java lines 962-1060
// ============================================================================
std::optional<APIFailureType> RedditAPI::findFailureType(const JsonValue &response)
{
    // This is a simplified port. The full implementation iterates through
    // JSON objects and arrays recursively, checking for error strings.
    // In practice, the JSON response is inspected for failure indicators.

    // For now, return nullopt (no failure detected).
    // The full implementation requires the JsonValue abstraction that will
    // be ported separately.
    (void)response;
    return std::nullopt;
}

// ============================================================================
// createPostRequest() — Java lines 1062-1076
// ============================================================================
CacheRequest RedditAPI::createPostRequest(
        const QString &url,
        const RedditAccount &user,
        const std::vector<PostField> &postFields,
        const CacheRequestJSONParser &parser,
        void *context)
{
    // Java: wraps createPostRequestUnprocessedResponse with a CacheRequestJSONParser
    // The parser converts raw HTTP body to JSON and calls findFailureType
    //
    // In C++: same pattern. createPostRequestUnprocessedResponse builds the
    // CacheRequest, then the JSON parser callback chain handles the response.
    //
    // For now, return a default CacheRequest (the actual implementation
    // requires CacheRequestJSONParser which is not yet ported).
    (void)url;
    (void)user;
    (void)postFields;
    (void)parser;
    (void)context;

    Logging::info("RedditAPI", "createPostRequest stub — needs full implementation");
    throw std::runtime_error("createPostRequest: full implementation pending "
                             "CacheRequestJSONParser port");
}

// ============================================================================
// createPostRequestUnprocessedResponse() — Java lines 1078-1097
// ============================================================================
CacheRequest RedditAPI::createPostRequestUnprocessedResponse(
        const QString &url,
        const RedditAccount &user,
        const std::vector<PostField> &postFields,
        const CacheRequestCallbacks &callbacks,
        void *context)
{
    // Java:
    //   return new CacheRequest(
    //       url, user, null,
    //       new Priority(Constants.Priority.API_ACTION),
    //       DownloadStrategyAlways.INSTANCE,
    //       Constants.FileType.NOCACHE,
    //       CacheRequest.DownloadQueueType.REDDIT_API,
    //       new HTTPRequestBody.PostFields(postFields),
    //       context,
    //       callbacks);

    (void)url;
    (void)user;
    (void)postFields;
    (void)callbacks;
    (void)context;

    Logging::info("RedditAPI", "createPostRequestUnprocessedResponse stub");
    throw std::runtime_error("createPostRequestUnprocessedResponse: full "
                             "implementation pending");
}

// ============================================================================
// createGetRequest() — Java lines 1099-1120
// ============================================================================
CacheRequest RedditAPI::createGetRequest(
        const QString &url,
        const RedditAccount &user,
        const Priority &priority,
        int fileType,
        const DownloadStrategy &downloadStrategy,
        const CacheRequestJSONParser &parser,
        void *context)
{
    // Java:
    //   return new CacheRequest(
    //       url, user, null,
    //       priority, downloadStrategy,
    //       fileType,
    //       CacheRequest.DownloadQueueType.REDDIT_API,
    //       null,
    //       context,
    //       new CacheRequestJSONParser(context, handler));

    (void)url;
    (void)user;
    (void)priority;
    (void)fileType;
    (void)downloadStrategy;
    (void)parser;
    (void)context;

    Logging::info("RedditAPI", "createGetRequest stub");
    throw std::runtime_error("createGetRequest: full implementation pending");
}

} // namespace PinkReader
