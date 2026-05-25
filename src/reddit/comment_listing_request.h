// Origin: org/quantumbadger/redreader/reddit/CommentListingRequest.java
#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>

namespace PinkReader {

// Forward declarations
class Context;
class CommentListingFragment;
class BaseActivity;
class RedditURLParser_RedditURL;
class CacheManager;
class RedditAccount;
class DownloadStrategy;
class RRError;
class TimestampUTC;
class RedditPreparedPost;
class RedditCommentListItem;
class RedditThingResponse;
class CacheRequest;
class UriString;
class Priority;
template<typename T, typename E> class GenericFactory;
class SeekableInputStream;
template<typename T> class MaybeParseError;
class RedditThing;
class RedditMore;
class RedditComment;
class RedditFieldReplies;
class RedditListing;
class RedditPost;
class RedditParsedPost;
class RedditChangeDataManager;
class RedditMediaMetadata;
class UrlEncodedString;

class CommentListingRequest {
public:
	// Listener interface (was @UiThread inner interface in Java)
	class Listener {
	public:
		virtual ~Listener() = default;
		virtual void onCommentListingRequestDownloadNecessary() = 0;
		virtual void onCommentListingRequestFailure(const RRError& error) = 0;
		virtual void onCommentListingRequestCachedCopy(const TimestampUTC& timestamp) = 0;
		virtual void onCommentListingRequestParseStart() = 0;
		virtual void onCommentListingRequestPostDownloaded(RedditPreparedPost& post) = 0;
		virtual void onCommentListingRequestAllItemsDownloaded(std::vector<RedditCommentListItem*>& items) = 0;
	};

private:
	static const std::string TAG;

	Context* m_context;
	CommentListingFragment* m_fragment;
	BaseActivity* m_activity;
	RedditURLParser_RedditURL* m_commentListingURL;

	bool m_parsePostSelfText;
	CacheManager* m_cacheManager;
	RedditURLParser_RedditURL* m_url;
	RedditAccount* m_user;
	int m_session; // UUID stub as int
	DownloadStrategy* m_downloadStrategy;

	Listener* m_listener;

public:
	CommentListingRequest(
			Context* context,
			CommentListingFragment* fragment,
			BaseActivity* activity,
			RedditURLParser_RedditURL* commentListingURL,
			bool parsePostSelfText,
			RedditURLParser_RedditURL* url,
			RedditAccount* user,
			int session,
			DownloadStrategy* downloadStrategy,
			Listener* listener);

private:
	void onThingDownloaded(
			RedditThingResponse& thingResponse,
			int session,
			const TimestampUTC& timestamp,
			bool fromCache);

	CacheRequest* createCommentListingCacheRequest();

	void buildCommentTree(
			MaybeParseError<RedditThing>& maybeThing,
			RedditCommentListItem* parent,
			std::vector<RedditCommentListItem*>& output,
			const int* minimumCommentScore,
			const std::string* parentPostAuthor);
};

} // namespace PinkReader
