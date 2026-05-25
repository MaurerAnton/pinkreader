// Origin: org/quantumbadger/redreader/reddit/RedditAPI.java
#include "reddit_api.h"
#include <stdexcept>

namespace PinkReader {

// ============================================================================
// Inner class: GenericResponseHandler (implements CacheRequestJSONParser.Listener)
// ============================================================================
// NOTE: Mapped as private inner class in Java, exposed here as nested class
// with stubbed implementation.

// ============================================================================
// Inner class: SubmitJSONListener (implements CacheRequestJSONParser.Listener)
// ============================================================================
// NOTE: Original Java inner class - stubbed here.

// ============================================================================
// flairSelectorForNewLink
// ============================================================================
void RedditAPI::flairSelectorForNewLink(
		Context& context,
		CacheManager& cm,
		RedditAccount& user,
		const SubredditCanonicalId& subreddit,
		FlairSelectorResponseHandler& responseHandler) {

	(void)context;
	(void)cm;
	(void)user;
	(void)subreddit;
	(void)responseHandler;

	// Original Java:
	// final LinkedList<PostField> postFields = new LinkedList<>();
	// postFields.add(new PostField("is_newlink", "true"));
	// final UriString apiUrl = Constants.Reddit.getUri(subreddit + "/api/flairselector");
	// cm.makeRequest(createPostRequest(apiUrl, user, postFields, context,
	//     new CacheRequestJSONParser.Listener() {
	//         @Override public void onJsonParsed(JsonValue result, ...) {
	//             if(result.asObject() != null && result.asObject().isEmpty()) {
	//                 responseHandler.onSuccess(Collections.emptyList()); return;
	//             }
	//             if(result.asString() != null && result.asString().equals("{}")) {
	//                 responseHandler.onSuccess(Collections.emptyList()); return;
	//             }
	//             final Optional<JsonArray> array = result.getArrayAtPath("choices");
	//             if(array.isEmpty()) {
	//                 final APIFailureType failureType = findFailureType(result);
	//                 responseHandler.onFailure(General.getGeneralErrorForFailure(context,
	//                     General.nullAlternative(failureType, APIFailureType.UNKNOWN),
	//                     "flairselector", Optional.of(new FailedRequestBody(result))));
	//                 return;
	//             }
	//             final Optional<List<RedditFlairChoice>> choices = RedditFlairChoice.fromJsonList(array.get());
	//             if(choices.isEmpty()) {
	//                 responseHandler.onFailure(General.getGeneralErrorForFailure(context,
	//                     CacheRequest.RequestFailureType.PARSE, new RuntimeException(),
	//                     null, apiUrl, Optional.of(new FailedRequestBody(result))));
	//                 return;
	//             }
	//             responseHandler.onSuccess(choices.get());
	//         }
	//         @Override public void onFailure(RRError error) {
	//             if(error.httpStatus != null && error.httpStatus == 404)
	//                 responseHandler.onSubredditDoesNotExist();
	//             else if(error.httpStatus != null && error.httpStatus == 403)
	//                 responseHandler.onSubredditPermissionDenied();
	//             else responseHandler.onFailure(error);
	//         }
	//     }));
}

// ============================================================================
// submit
// ============================================================================
void RedditAPI::submit(
		CacheManager& cm,
		SubmitResponseHandler& responseHandler,
		RedditAccount& user,
		bool isSelfPost,
		const std::string& subreddit,
		const std::string& title,
		const std::string& body,
		bool sendRepliesToInbox,
		bool markAsNsfw,
		bool markAsSpoiler,
		const std::string* flairId,
		Context& context) {

	(void)cm;
	(void)responseHandler;
	(void)user;
	(void)isSelfPost;
	(void)subreddit;
	(void)title;
	(void)body;
	(void)sendRepliesToInbox;
	(void)markAsNsfw;
	(void)markAsSpoiler;
	(void)flairId;
	(void)context;

	// Original Java:
	// final LinkedList<PostField> postFields = new LinkedList<>();
	// postFields.add(new PostField("api_type", "json"));
	// postFields.add(new PostField("kind", isSelfPost ? "self" : "link"));
	// postFields.add(new PostField("sendreplies", sendRepliesToInbox ? "true" : "false"));
	// postFields.add(new PostField("nsfw", markAsNsfw ? "true" : "false"));
	// postFields.add(new PostField("spoiler", markAsSpoiler ? "true" : "false"));
	// postFields.add(new PostField("sr", subreddit));
	// postFields.add(new PostField("title", title));
	// if(flairId != null) postFields.add(new PostField("flair_id", flairId));
	// if(isSelfPost) postFields.add(new PostField("text", body));
	// else postFields.add(new PostField("url", body));
	// cm.makeRequest(createPostRequest(Constants.Reddit.getUri("/api/submit"), user, postFields, context,
	//     new SubmitJSONListener(responseHandler)));
}

// ============================================================================
// compose
// ============================================================================
void RedditAPI::compose(
		CacheManager& cm,
		ActionResponseHandler& responseHandler,
		RedditAccount& user,
		const std::string& recipient,
		const std::string& subject,
		const std::string& body,
		Context& context) {

	(void)cm;
	(void)responseHandler;
	(void)user;
	(void)recipient;
	(void)subject;
	(void)body;
	(void)context;

	// Original Java:
	// final LinkedList<PostField> postFields = new LinkedList<>();
	// postFields.add(new PostField("api_type", "json"));
	// postFields.add(new PostField("subject", subject));
	// postFields.add(new PostField("to", recipient));
	// postFields.add(new PostField("text", body));
	// cm.makeRequest(createPostRequest(Constants.Reddit.getUri("/api/compose"), user, postFields, context,
	//     new GenericResponseHandler(responseHandler)));
}

// ============================================================================
// comment
// ============================================================================
void RedditAPI::comment(
		CacheManager& cm,
		SubmitResponseHandler& responseHandler,
		ActionResponseHandler& inboxResponseHandler,
		RedditAccount& user,
		const RedditIdAndType& parentIdAndType,
		const std::string& markdown,
		bool sendRepliesToInbox,
		AppCompatActivity& context) {

	(void)cm;
	(void)responseHandler;
	(void)inboxResponseHandler;
	(void)user;
	(void)parentIdAndType;
	(void)markdown;
	(void)sendRepliesToInbox;
	(void)context;

	// Original Java:
	// final LinkedList<PostField> postFields = new LinkedList<>();
	// postFields.add(new PostField("api_type", "json"));
	// postFields.add(new PostField("thing_id", parentIdAndType.getValue()));
	// postFields.add(new PostField("text", markdown));
	// cm.makeRequest(createPostRequest(Constants.Reddit.getUri("/api/comment"), user, postFields, context,
	//     new SubmitJSONListener(new SubmitResponseHandler(context) {
	//         @Override public void onSubmitErrors(ArrayList<String> errors) {
	//             responseHandler.onSubmitErrors(errors);
	//         }
	//         @Override public void onSuccess(Optional<String> redirectUrl, Optional<String> thingId) {
	//             if(!sendRepliesToInbox) {
	//                 thingId.ifPresent(commentFullname -> sendReplies(cm, inboxResponseHandler,
	//                     user, commentFullname, false, context));
	//             }
	//             responseHandler.onSuccess(redirectUrl, thingId);
	//         }
	//         @Override protected void onCallbackException(Throwable t) {
	//             responseHandler.onCallbackException(t);
	//         }
	//         @Override protected void onFailure(RRError error) {
	//             responseHandler.notifyFailure(error);
	//         }
	//     })));
}

// ============================================================================
// markAllAsRead
// ============================================================================
void RedditAPI::markAllAsRead(
		CacheManager& cm,
		ActionResponseHandler& responseHandler,
		RedditAccount& user,
		Context& context) {

	(void)cm;
	(void)responseHandler;
	(void)user;
	(void)context;

	// Original Java:
	// final LinkedList<PostField> postFields = new LinkedList<>();
	// cm.makeRequest(createPostRequestUnprocessedResponse(
	//     Constants.Reddit.getUri("/api/read_all_messages"), user, postFields, context,
	//     new CacheRequestCallbacks() {
	//         @Override public void onFailure(RRError error) { responseHandler.notifyFailure(error); }
	//         @Override public void onDataStreamComplete(...) { responseHandler.notifySuccess(); }
	//     }));
}

// ============================================================================
// editComment
// ============================================================================
void RedditAPI::editComment(
		CacheManager& cm,
		ActionResponseHandler& responseHandler,
		RedditAccount& user,
		const RedditIdAndType& commentIdAndType,
		const std::string& markdown,
		Context& context) {

	(void)cm;
	(void)responseHandler;
	(void)user;
	(void)commentIdAndType;
	(void)markdown;
	(void)context;

	// Original Java:
	// final LinkedList<PostField> postFields = new LinkedList<>();
	// postFields.add(new PostField("thing_id", commentIdAndType.getValue()));
	// postFields.add(new PostField("text", markdown));
	// cm.makeRequest(createPostRequest(Constants.Reddit.getUri("/api/editusertext"), user, postFields, context,
	//     new GenericResponseHandler(responseHandler)));
}

// ============================================================================
// action
// ============================================================================
void RedditAPI::action(
		CacheManager& cm,
		ActionResponseHandler& responseHandler,
		RedditAccount& user,
		const RedditIdAndType& idAndType,
		int action,
		Context& context) {

	(void)cm;
	(void)responseHandler;
	(void)user;
	(void)idAndType;
	(void)action;
	(void)context;

	// Original Java:
	// final LinkedList<PostField> postFields = new LinkedList<>();
	// postFields.add(new PostField("id", idAndType.getValue()));
	// final UriString url = prepareActionUri(action, postFields);
	// cm.makeRequest(createPostRequest(url, user, postFields, context, new GenericResponseHandler(responseHandler)));
}

// ============================================================================
// prepareActionUri
// ============================================================================
UriString RedditAPI::prepareActionUri(int action, std::vector<PostField>& postFields) {
	switch (action) {
		case ACTION_DOWNVOTE:
			// postFields.add(new PostField("dir", "-1"));
			// return Constants.Reddit.getUri(Constants.Reddit.PATH_VOTE);
			(void)postFields;
			return UriString();
		case ACTION_UNVOTE:
			// postFields.add(new PostField("dir", "0"));
			// return Constants.Reddit.getUri(Constants.Reddit.PATH_VOTE);
			(void)postFields;
			return UriString();
		case ACTION_UPVOTE:
			// postFields.add(new PostField("dir", "1"));
			// return Constants.Reddit.getUri(Constants.Reddit.PATH_VOTE);
			(void)postFields;
			return UriString();
		case ACTION_SAVE:
			// return Constants.Reddit.getUri(Constants.Reddit.PATH_SAVE);
			(void)postFields;
			return UriString();
		case ACTION_HIDE:
			// return Constants.Reddit.getUri(Constants.Reddit.PATH_HIDE);
			(void)postFields;
			return UriString();
		case ACTION_UNSAVE:
			// return Constants.Reddit.getUri(Constants.Reddit.PATH_UNSAVE);
			(void)postFields;
			return UriString();
		case ACTION_UNHIDE:
			// return Constants.Reddit.getUri(Constants.Reddit.PATH_UNHIDE);
			(void)postFields;
			return UriString();
		case ACTION_REPORT:
			// return Constants.Reddit.getUri(Constants.Reddit.PATH_REPORT);
			(void)postFields;
			return UriString();
		case ACTION_DELETE:
			// return Constants.Reddit.getUri(Constants.Reddit.PATH_DELETE);
			(void)postFields;
			return UriString();
		default:
			throw std::runtime_error("Unknown post/comment action");
	}
}

// ============================================================================
// subscriptionAction
// ============================================================================
void RedditAPI::subscriptionAction(
		CacheManager& cm,
		ActionResponseHandler& responseHandler,
		RedditAccount& user,
		const SubredditCanonicalId& subredditId,
		int action,
		Context& context) {

	(void)cm;
	(void)responseHandler;
	(void)user;
	(void)subredditId;
	(void)action;
	(void)context;

	// Original Java:
	// RedditSubredditManager.getInstance(context, user).getSubreddit(subredditId, TimestampBound.ANY,
	//     new RequestResponseHandler<RedditSubreddit, RRError>() {
	//         @Override public void onRequestFailed(RRError failureReason) {
	//             responseHandler.notifyFailure(failureReason);
	//         }
	//         @Override public void onRequestSuccess(RedditSubreddit subreddit, TimestampUTC timeCached) {
	//             final LinkedList<PostField> postFields = new LinkedList<>();
	//             postFields.add(new PostField("sr", subreddit.name));
	//             final UriString url = subscriptionPrepareActionUri(action, postFields);
	//             cm.makeRequest(createPostRequest(url, user, postFields, context,
	//                 new GenericResponseHandler(responseHandler)));
	//         }
	//     }, null);
}

// ============================================================================
// subscriptionPrepareActionUri
// ============================================================================
UriString RedditAPI::subscriptionPrepareActionUri(int action, std::vector<PostField>& postFields) {
	switch (action) {
		case SUBSCRIPTION_ACTION_SUBSCRIBE:
			// postFields.add(new PostField("action", "sub"));
			// return Constants.Reddit.getUri(Constants.Reddit.PATH_SUBSCRIBE);
			(void)postFields;
			return UriString();
		case SUBSCRIPTION_ACTION_UNSUBSCRIBE:
			// postFields.add(new PostField("action", "unsub"));
			// return Constants.Reddit.getUri(Constants.Reddit.PATH_SUBSCRIBE);
			(void)postFields;
			return UriString();
		default:
			throw std::runtime_error("Unknown subreddit action");
	}
}

// ============================================================================
// getUser
// ============================================================================
void RedditAPI::getUser(
		CacheManager& cm,
		const std::string& usernameToGet,
		UserResponseHandler& responseHandler,
		RedditAccount& user,
		DownloadStrategy& downloadStrategy,
		Context& context) {

	(void)cm;
	(void)usernameToGet;
	(void)responseHandler;
	(void)user;
	(void)downloadStrategy;
	(void)context;

	// Original Java:
	// final UriString uri = Constants.Reddit.getUri("/user/" + usernameToGet + "/about.json");
	// cm.makeRequest(createGetRequest(uri, user,
	//     new Priority(Constants.Priority.API_USER_ABOUT),
	//     Constants.FileType.USER_ABOUT, downloadStrategy, context,
	//     new CacheRequestJSONParser.Listener() {
	//         @Override public void onJsonParsed(JsonValue result, ...) {
	//             try {
	//                 final RedditThing userThing = result.asObject(RedditThing.class);
	//                 final RedditUser userResult = userThing.asUser();
	//                 responseHandler.notifySuccess(userResult, timestamp);
	//             } catch(Throwable t) {
	//                 responseHandler.notifyFailure(General.getGeneralErrorForFailure(
	//                     context, CacheRequest.RequestFailureType.PARSE, t, null, uri,
	//                     Optional.of(new FailedRequestBody(result))));
	//             }
	//         }
	//         @Override public void onFailure(RRError error) { responseHandler.notifyFailure(error); }
	//     }));
}

// ============================================================================
// unblockUser
// ============================================================================
void RedditAPI::unblockUser(
		CacheManager& cm,
		const std::string& usernameToUnblock,
		const std::string& currentUserFullname,
		ActionResponseHandler& responseHandler,
		RedditAccount& user,
		Context& context) {

	(void)cm;
	(void)usernameToUnblock;
	(void)currentUserFullname;
	(void)responseHandler;
	(void)user;
	(void)context;

	// Original Java:
	// final LinkedList<PostField> postFields = new LinkedList<>();
	// postFields.add(new PostField("name", usernameToUnblock));
	// postFields.add(new PostField("container", currentUserFullname));
	// postFields.add(new PostField("type", "enemy"));
	// cm.makeRequest(createPostRequest(Constants.Reddit.getUri("/api/unfriend"), user, postFields, context,
	//     new GenericResponseHandler(responseHandler)));
}

// ============================================================================
// blockUser
// ============================================================================
void RedditAPI::blockUser(
		CacheManager& cm,
		const std::string& usernameToBlock,
		BlockUserResponseHandler& responseHandler,
		RedditAccount& user,
		Context& context) {

	(void)cm;
	(void)usernameToBlock;
	(void)responseHandler;
	(void)user;
	(void)context;

	// Original Java:
	// final LinkedList<PostField> postFields = new LinkedList<>();
	// postFields.add(new PostField("name", usernameToBlock));
	// postFields.add(new PostField("api_type", "json"));
	// cm.makeRequest(createPostRequestUnprocessedResponse(
	//     Constants.Reddit.getUri("/api/block_user"), user, postFields, context,
	//     new CacheRequestCallbacks() {
	//         @Override public void onFailure(RRError error) {
	//             if (error.httpStatus != null && error.httpStatus == 403)
	//                 responseHandler.onBlockUserPermissionDenied();
	//             else responseHandler.onFailure(error);
	//         }
	//         @Override public void onDataStreamComplete(...) { responseHandler.onSuccess(); }
	//     }));
}

// ============================================================================
// sendReplies
// ============================================================================
void RedditAPI::sendReplies(
		CacheManager& cm,
		ActionResponseHandler& responseHandler,
		RedditAccount& user,
		const std::string& fullname,
		bool state,
		Context& context) {

	(void)cm;
	(void)responseHandler;
	(void)user;
	(void)fullname;
	(void)state;
	(void)context;

	// Original Java:
	// final LinkedList<PostField> postFields = new LinkedList<>();
	// postFields.add(new PostField("id", fullname));
	// postFields.add(new PostField("state", String.valueOf(state)));
	// cm.makeRequest(createPostRequest(Constants.Reddit.getUri("/api/sendreplies"), user, postFields, context,
	//     new GenericResponseHandler(responseHandler)));
}

// ============================================================================
// popularSubreddits
// ============================================================================
void RedditAPI::popularSubreddits(
		CacheManager& cm,
		RedditAccount& user,
		Context& context,
		ValueResponseHandler<SubredditListResponse>& handler,
		const std::optional<std::string>& after) {

	(void)cm;
	(void)user;
	(void)context;
	(void)handler;
	(void)after;

	// Original Java:
	// final TimeDuration maxCacheAgeMs = TimeDuration.hours(1);
	// final Uri.Builder builder = Constants.Reddit.getUriBuilder(Constants.Reddit.PATH_SUBREDDITS_POPULAR);
	// builder.appendQueryParameter("limit", "100");
	// after.apply(value -> builder.appendQueryParameter("after", value));
	// final UriString uri = UriString.from(builder.build());
	// requestSubredditList(cm, uri, user, context, handler,
	//     new DownloadStrategyIfTimestampOutsideBounds(TimestampBound.notOlderThan(maxCacheAgeMs)));
}

// ============================================================================
// searchSubreddits
// ============================================================================
void RedditAPI::searchSubreddits(
		CacheManager& cm,
		RedditAccount& user,
		const std::string& queryString,
		Context& context,
		ValueResponseHandler<SubredditListResponse>& handler,
		const std::optional<std::string>& after) {

	(void)cm;
	(void)user;
	(void)queryString;
	(void)context;
	(void)handler;
	(void)after;

	// Original Java:
	// final TimeDuration maxCacheAgeMs = TimeDuration.minutes(1);
	// final Uri.Builder builder = Constants.Reddit.getUriBuilder("/subreddits/search.json");
	// builder.appendQueryParameter("q", queryString);
	// builder.appendQueryParameter("limit", "100");
	// if(PrefsUtility.pref_behaviour_nsfw()) builder.appendQueryParameter("include_over_18", "on");
	// after.apply(value -> builder.appendQueryParameter("after", value));
	// final UriString uri = UriString.from(builder.build());
	// requestSubredditList(cm, uri, user, context, handler,
	//     new DownloadStrategyIfTimestampOutsideBounds(TimestampBound.notOlderThan(maxCacheAgeMs)));
}

// ============================================================================
// subscribedSubreddits
// ============================================================================
void RedditAPI::subscribedSubreddits(
		CacheManager& cm,
		RedditAccount& user,
		AppCompatActivity& context,
		ValueResponseHandler<std::vector<RedditSubreddit>>& handler) {

	(void)context;
	std::vector<RedditSubreddit> results;
	results.reserve(128);
	subscribedSubredditsInternal(cm, user, context, handler, std::nullopt, results);
}

// ============================================================================
// subscribedSubredditsInternal
// ============================================================================
void RedditAPI::subscribedSubredditsInternal(
		CacheManager& cm,
		RedditAccount& user,
		AppCompatActivity& context,
		ValueResponseHandler<std::vector<RedditSubreddit>>& handler,
		const std::optional<std::string>& after,
		std::vector<RedditSubreddit>& results) {

	(void)cm;
	(void)user;
	(void)context;
	(void)handler;
	(void)after;
	(void)results;

	// Original Java:
	// final Uri.Builder builder = Constants.Reddit.getUriBuilder(
	//     Constants.Reddit.PATH_SUBREDDITS_MINE_SUBSCRIBER);
	// after.apply(value -> builder.appendQueryParameter("after", value));
	// final UriString uri = UriString.from(builder.build());
	// requestSubredditList(cm, uri, user, context,
	//     new ValueResponseHandler<SubredditListResponse>(context) {
	//         @Override protected void onSuccess(SubredditListResponse value) {
	//             results.addAll(value.subreddits);
	//             if(value.after.isEmpty()) handler.onSuccess(results);
	//             else subscribedSubredditsInternal(cm, user, context, handler, value.after, results);
	//         }
	//         @Override protected void onCallbackException(Throwable t) { handler.onCallbackException(t); }
	//         @Override protected void onFailure(RRError error) { handler.onFailure(error); }
	//     },
	//     DownloadStrategyAlways.INSTANCE);
}

// ============================================================================
// requestSubredditList
// ============================================================================
void RedditAPI::requestSubredditList(
		CacheManager& cm,
		const UriString& uri,
		RedditAccount& user,
		Context& context,
		ValueResponseHandler<SubredditListResponse>& handler,
		DownloadStrategy& downloadStrategy) {

	(void)cm;
	(void)uri;
	(void)user;
	(void)context;
	(void)handler;
	(void)downloadStrategy;

	// Original Java:
	// cm.makeRequest(createGetRequest(uri, user,
	//     new Priority(Constants.Priority.API_SUBREDDIT_LIST),
	//     Constants.FileType.SUBREDDIT_LIST, downloadStrategy, context,
	//     new CacheRequestJSONParser.Listener() {
	//         @Override public void onJsonParsed(JsonValue result, ...) {
	//             try {
	//                 final Optional<JsonArray> subreddits = result.getArrayAtPath("data", "children");
	//                 final Optional<String> after = result.getStringAtPath("data", "after");
	//                 if(subreddits.isEmpty()) throw new IOException("Subreddit data not found");
	//                 final ArrayList<RedditSubreddit> output = new ArrayList<>();
	//                 for(final JsonValue value : subreddits.get()) {
	//                     final RedditThing redditThing = value.asObject(RedditThing.class);
	//                     final RedditSubreddit subreddit = redditThing.asSubreddit();
	//                     output.add(subreddit);
	//                 }
	//                 handler.notifySuccess(new SubredditListResponse(output, after));
	//             } catch(Exception e) {
	//                 onFailure(General.getGeneralErrorForFailure(context,
	//                     CacheRequest.RequestFailureType.PARSE, e, null, uri,
	//                     Optional.of(new FailedRequestBody(result))));
	//             }
	//         }
	//         @Override public void onFailure(RRError error) { handler.notifyFailure(error); }
	//     }));
}

// ============================================================================
// findFailureType
// ============================================================================
int RedditAPI::findFailureType(const JsonValue& response) {
	(void)response;

	// Original Java logic (returns APIFailureType or null / UNKNOWN):
	// if(response == null) return null;
	// boolean unknownError = false;
	// if(response.asObject() != null) {
	//     for(final Map.Entry<String, JsonValue> v : response.asObject()) {
	//         if("success".equals(v.getKey()) && Boolean.FALSE.equals(v.getValue().asBoolean()))
	//             unknownError = true;
	//         final APIFailureType failureType = findFailureType(v.getValue());
	//         if(failureType == APIFailureType.UNKNOWN) unknownError = true;
	//         else if(failureType != null) return failureType;
	//     }
	//     final Optional<JsonArray> errors = response.getArrayAtPath("json", "errors");
	//     if(errors.isPresent() && errors.get().size() > 0) unknownError = true;
	// } else if(response.asArray() != null) {
	//     for(final JsonValue v : response.asArray()) {
	//         final APIFailureType failureType = findFailureType(v);
	//         if(failureType == APIFailureType.UNKNOWN) unknownError = true;
	//         else if(failureType != null) return failureType;
	//     }
	// } else if(response instanceof JsonString) {
	//     final String responseAsString = response.asString();
	//     if(Constants.Reddit.isApiErrorUser(responseAsString))
	//         return APIFailureType.INVALID_USER;
	//     if(Constants.Reddit.isApiErrorCaptcha(responseAsString))
	//         return APIFailureType.BAD_CAPTCHA;
	//     if(Constants.Reddit.isApiErrorNotAllowed(responseAsString))
	//         return APIFailureType.NOTALLOWED;
	//     if(Constants.Reddit.isApiErrorSubredditRequired(responseAsString))
	//         return APIFailureType.SUBREDDIT_REQUIRED;
	//     if(Constants.Reddit.isApiErrorURLRequired(responseAsString))
	//         return APIFailureType.URL_REQUIRED;
	//     if(Constants.Reddit.isApiTooFast(responseAsString))
	//         return APIFailureType.TOO_FAST;
	//     if(Constants.Reddit.isApiTooLong(responseAsString))
	//         return APIFailureType.TOO_LONG;
	//     if(Constants.Reddit.isApiAlreadySubmitted(responseAsString))
	//         return APIFailureType.ALREADY_SUBMITTED;
	//     if(Constants.Reddit.isPostFlairRequired(responseAsString))
	//         return APIFailureType.POST_FLAIR_REQUIRED;
	//     if(Constants.Reddit.isApiError(responseAsString))
	//         unknownError = true;
	// }
	// return unknownError ? APIFailureType.UNKNOWN : null;

	return -1; // null equivalent
}

// ============================================================================
// createPostRequest
// ============================================================================
CacheRequest* RedditAPI::createPostRequest(
		const UriString& url,
		RedditAccount& user,
		const std::vector<PostField>& postFields,
		Context& context,
		CacheRequestJSONParser::Listener& handler) {

	(void)url;
	(void)user;
	(void)postFields;
	(void)context;
	(void)handler;

	// Original Java:
	// return createPostRequestUnprocessedResponse(url, user, postFields, context,
	//     new CacheRequestJSONParser(context, handler));
	return nullptr;
}

// ============================================================================
// createPostRequestUnprocessedResponse
// ============================================================================
CacheRequest* RedditAPI::createPostRequestUnprocessedResponse(
		const UriString& url,
		RedditAccount& user,
		const std::vector<PostField>& postFields,
		Context& context,
		CacheRequestCallbacks& callbacks) {

	(void)url;
	(void)user;
	(void)postFields;
	(void)context;
	(void)callbacks;

	// Original Java:
	// return new CacheRequest(url, user, null,
	//     new Priority(Constants.Priority.API_ACTION),
	//     DownloadStrategyAlways.INSTANCE,
	//     Constants.FileType.NOCACHE,
	//     CacheRequest.DownloadQueueType.REDDIT_API,
	//     new HTTPRequestBody.PostFields(postFields),
	//     context, callbacks);
	return nullptr;
}

// ============================================================================
// createGetRequest
// ============================================================================
CacheRequest* RedditAPI::createGetRequest(
		const UriString& url,
		RedditAccount& user,
		const Priority& priority,
		int fileType,
		DownloadStrategy& downloadStrategy,
		Context& context,
		CacheRequestJSONParser::Listener& handler) {

	(void)url;
	(void)user;
	(void)priority;
	(void)fileType;
	(void)downloadStrategy;
	(void)context;
	(void)handler;

	// Original Java:
	// return new CacheRequest(url, user, null, priority, downloadStrategy, fileType,
	//     CacheRequest.DownloadQueueType.REDDIT_API, null, context,
	//     new CacheRequestJSONParser(context, handler));
	return nullptr;
}

} // namespace PinkReader
