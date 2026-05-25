// Origin: org/quantumbadger/redreader/reddit/CommentListingRequest.java
#include "comment_listing_request.h"
#include <stdexcept>

namespace PinkReader {

const std::string CommentListingRequest::TAG = "CommentListingRequest";

CommentListingRequest::CommentListingRequest(
		Context* context,
		CommentListingFragment* fragment,
		BaseActivity* activity,
		RedditURLParser_RedditURL* commentListingURL,
		bool parsePostSelfText,
		RedditURLParser_RedditURL* url,
		RedditAccount* user,
		int session,
		DownloadStrategy* downloadStrategy,
		Listener* listener)
	: m_context(context)
	, m_fragment(fragment)
	, m_activity(activity)
	, m_commentListingURL(commentListingURL)
	, m_parsePostSelfText(parsePostSelfText)
	, m_cacheManager(nullptr) // CacheManager.getInstance(context)
	, m_url(url)
	, m_user(user)
	, m_session(session)
	, m_downloadStrategy(downloadStrategy)
	, m_listener(listener)
{
	// m_cacheManager = CacheManager.getInstance(context);
	// m_cacheManager.makeRequest(createCommentListingCacheRequest());
}

void CommentListingRequest::onThingDownloaded(
		RedditThingResponse& thingResponse,
		int session,
		const TimestampUTC& timestamp,
		bool fromCache) {
	(void)session;

	// String parentPostAuthor = null;
	std::string* parentPostAuthor = nullptr;

	// if(mActivity instanceof SessionChangeListener) {
	//     ((SessionChangeListener)mActivity).onSessionChanged(
	//         session, SessionChangeListener.SessionChangeType.COMMENTS, timestamp);
	// }
	(void)m_activity;

	// final Integer minimumCommentScore = PrefsUtility.pref_behaviour_comment_min();
	// NOTE: using nullptr as "no minimum" for stub
	const int* minimumCommentScore = nullptr;

	if (fromCache) {
		// AndroidCommon.runOnUiThread(() -> mListener.onCommentListingRequestCachedCopy(timestamp));
		m_listener->onCommentListingRequestCachedCopy(timestamp);
	}

	// AndroidCommon.runOnUiThread(mListener::onCommentListingRequestParseStart);
	m_listener->onCommentListingRequestParseStart();

	// RedditListing commentListing;
	// if(thingResponse instanceof RedditThingResponse.Single) { ... } else { ... }
	// NOTE: RedditThingResponse type dispatch requires the actual types.
	// This logic is preserved as comments for now since the Java instanceof/pattern
	// matching maps to dynamic_cast in C++ once the type hierarchy exists.

	// Download comments:
	// final ArrayList<MaybeParseError<RedditThing>> topLevelComments = commentListing.getChildren();
	// final ArrayList<RedditCommentListItem> items = new ArrayList<>(200);
	// for(final MaybeParseError<RedditThing> commentThingValue : topLevelComments) {
	//     buildCommentTree(commentThingValue, null, items, minimumCommentScore, parentPostAuthor);
	// }

	// final RedditChangeDataManager changeDataManager = RedditChangeDataManager.getInstance(mUser);
	// for(final RedditCommentListItem item : items) {
	//     if(item.isComment()) {
	//         changeDataManager.update(timestamp, item.asComment().getParsedComment().getRawComment());
	//     }
	// }

	// AndroidCommon.runOnUiThread(() -> mListener.onCommentListingRequestAllItemsDownloaded(items));
	(void)thingResponse;
	(void)timestamp;
	(void)fromCache;
	(void)minimumCommentScore;
}

CacheRequest* CommentListingRequest::createCommentListingCacheRequest() {
	// Original Java:
	// final UriString url = UriString.from(mUrl.generateJsonUri());
	// return new CacheRequest(
	//     url, mUser, mSession,
	//     new Priority(Constants.Priority.API_COMMENT_LIST),
	//     mDownloadStrategy,
	//     Constants.FileType.COMMENT_LIST,
	//     CacheRequest.DownloadQueueType.REDDIT_API,
	//     mContext,
	//     new CacheRequestCallbacks() {
	//         @Override public void onFailure(@NonNull final RRError error) {
	//             AndroidCommon.runOnUiThread(() -> mListener.onCommentListingRequestFailure(error));
	//         }
	//         @Override public void onDownloadNecessary() {
	//             AndroidCommon.runOnUiThread(mListener::onCommentListingRequestDownloadNecessary);
	//         }
	//         @Override public void onDataStreamAvailable(
	//             @NonNull final GenericFactory<SeekableInputStream, IOException> streamFactory,
	//             final TimestampUTC timestamp,
	//             @NonNull final UUID session,
	//             final boolean fromCache,
	//             @Nullable final String mimetype) {
	//             new Thread(null, () -> {
	//                 try {
	//                     final RedditThingResponse thingResponse
	//                         = JsonUtils.INSTANCE.decodeRedditThingResponseFromStream(streamFactory.create());
	//                     onThingDownloaded(thingResponse, session, timestamp, fromCache);
	//                 } catch(final Exception e) {
	//                     onFailure(General.getGeneralErrorForFailure(
	//                         mContext, CacheRequest.RequestFailureType.PARSE, e, null, url,
	//                         FailedRequestBody.from(streamFactory)));
	//                 }
	//             }, "Comment parsing", 1_000_000).start();
	//         }
	//     });
	//
	return nullptr; // Stub
}

void CommentListingRequest::buildCommentTree(
		MaybeParseError<RedditThing>& maybeThing,
		RedditCommentListItem* parent,
		std::vector<RedditCommentListItem*>& output,
		const int* minimumCommentScore,
		const std::string* parentPostAuthor) {

	(void)maybeThing;
	(void)parent;
	(void)output;
	(void)minimumCommentScore;
	(void)parentPostAuthor;

	// Original Java logic preserved as comments:
	// TODO handle gracefully by showing error message
	// final RedditThing thing = maybeThing.ok();
	//
	// if(thing instanceof RedditThing.More
	//     && mUrl.pathType() == RedditURLParser.POST_COMMENT_LISTING_URL) {
	//     output.add(new RedditCommentListItem(
	//         ((RedditThing.More)thing).getData(), parent, mFragment, mActivity, mCommentListingURL));
	//
	// } else if(thing instanceof RedditThing.Comment) {
	//     RedditComment comment = ((RedditThing.Comment) thing).getData();
	//
	//     // Emote processing logic for media_metadata
	//     if (comment.getMedia_metadata() != null && comment.getBody_html() != null) {
	//         // Process emotes in media_metadata
	//     }
	//
	//     // Build RedditCommentListItem with RedditRenderableComment
	//     // ...
	//
	//     // Handle blocked users
	//     if (comment.isBlockedByUser() && PrefsUtility.pref_appearance_hide_comments_from_blocked_users()) {
	//         return;
	//     }
	//     output.add(item);
	//
	//     // Recurse into replies
	//     if(comment.getReplies() instanceof RedditFieldReplies.Some) {
	//         final RedditListing listing = ((RedditThing.Listing)(
	//             (RedditFieldReplies.Some)comment.getReplies()).getValue()).getData();
	//         for(final MaybeParseError<RedditThing> reply : listing.getChildren()) {
	//             buildCommentTree(reply, item, output, minimumCommentScore, parentPostAuthor);
	//         }
	//     }
	// }
}

} // namespace PinkReader
