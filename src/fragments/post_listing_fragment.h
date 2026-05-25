/*
 * PinkReader - GPLv3
 * File: post_listing_fragment.h
 * Exact port of PostListingFragment.java (1143 lines)
 *
 * Ports every method, field, and logic branch. Android RecyclerView/View concepts
 * adapted to Qt widget equivalents. Post listing, loading more, precaching,
 * subscription management, session handling.
 */

#pragma once

#include <QObject>
#include <QWidget>
#include <QString>
#include <QSet>
#include <QUuid>
#include <memory>
#include <vector>
#include <optional>
#include <atomic>
#include <mutex>

namespace PinkReader {

// Forward declarations
class RedditAccount;
class RedditSubreddit;
class RedditPostListItem;
class RedditParsedPost;
class RedditPreparedPost;
class RedditPost;
class RedditThing;
class RedditListing;
class RedditIdAndType;
class SubredditCanonicalId;
class TimestampUTC;
class TimeDuration;
class UriString;
class CacheManager;
class CacheRequest;
class PostListingManager;
class ScrollbarRecyclerViewManager;
class PostListingHeader;
class SearchListingHeader;
class ErrorView;
class ImageInfo;
class RRError;
class PostListingURL;
class SubredditPostListURL;
class SearchPostListURL;
class PostCommentListingURL;
class CommentListingController;
template<typename T> class GenericFactory;
template<typename T> class MaybeParseError;
class SeekableInputStream;
class FailedRequestBody;

// Download strategy classes
class DownloadStrategy;
class DownloadStrategyAlways;
class DownloadStrategyIfNotCached;
class DownloadStrategyIfTimestampOutsideBounds;
class DownloadStrategyNever;

/**
 * @brief Port of PostListingFragment (Java lines 108-1143)
 *
 * Extends RRFragment, implements RedditPostView.PostSelectionListener.
 * Manages a scrollable post list with pull-to-refresh, load-more,
 * precaching of images/comments, subscription state, and subreddit headers.
 */
class PostListingFragment : public QObject {
    Q_OBJECT

public:
    // ========================================================================
    // PostSelectionListener interface — Java lines 109-110
    // ========================================================================
    class PostSelectionListener {
    public:
        virtual ~PostSelectionListener() = default;
        virtual void onPostSelected(RedditPreparedPost *post) = 0;
        virtual void onPostCommentsSelected(RedditPreparedPost *post) = 0;
    };

    // ========================================================================
    // Constructor — Java lines 143-387
    // ========================================================================

    // Java lines 143-148:
    // PostListingFragment(AppCompatActivity parent, Bundle savedInstanceState, Uri url,
    //     UUID session, boolean forceDownload)
    PostListingFragment(
            QWidget *parentWidget,
            const PostListingURL &postListingURL,
            const QUuid &session,
            bool forceDownload,
            PostSelectionListener *selectionListener = nullptr);

    ~PostListingFragment() override;

    // ========================================================================
    // Public API
    // ========================================================================

    // Java lines 389-392: getListingView() -> View
    QWidget *getListingView() const;

    // Java lines 394-406: onSaveInstanceState() -> Bundle
    // Returns first visible position for state restoration
    int savedFirstVisiblePosition() const;

    // Java lines 408-412: cancel()
    void cancel();

    // Java lines 414-418: restackRefreshCount()
    void restackRefreshCount();

    // Java lines 562-581: onSubscribe()
    void onSubscribe();

    // Java lines 583-599: onUnsubscribe()
    void onUnsubscribe();

    // Java lines 601-604: getPostListingURL()
    const PostListingURL &getPostListingURL() const;

    // Java lines 606-609: getSubreddit()
    RedditSubreddit *getSubreddit() const;

    // Java lines 611-629: onPostsAdded()
    void onPostsAdded();

    // ========================================================================
    // PostSelectionListener implementation — Java lines 481-504
    // ========================================================================

    void onPostSelected(RedditPreparedPost *post);
    void onPostCommentsSelected(RedditPreparedPost *post);

signals:
    void postsLoaded(int count);
    void loadError(const QString &message);
    void sessionChanged(const QUuid &session, const TimestampUTC &timestamp);

private:
    // ========================================================================
    // Private methods — Java lines 420-477, 506-560, 632-1142
    // ========================================================================

    // Java lines 420-446: onSubredditReceived()
    void onSubredditReceived();

    // Java lines 448-474: setHeader(String title, String subtitle, RedditSubreddit subreddit)
    void setHeader(const QString &title, const QString &subtitle, RedditSubreddit *subreddit);

    // Java lines 476-478: setHeader(View view)
    void setHeaderWidget(QWidget *view);

    // Java lines 506-560: onLoadMoreItemsCheck()
    void onLoadMoreItemsCheck();

    // Java lines 632-978: createPostListingRequest(...)
    CacheRequest *createPostListingRequest(
            const UriString &url,
            RedditAccount *user,
            const QUuid &requestSession,
            DownloadStrategy *downloadStrategy,
            bool firstDownload);

    // Java lines 980-1030: precacheComments(...)
    void precacheComments(RedditPreparedPost *preparedPost, int positionInList);

    // Java lines 1032-1100: precacheImage(...)
    void precacheImage(const ImageInfo &info, int positionInList);

    // Java lines 1102-1142: precacheImage(Activity, UriString, int)
    void precacheImage(const UriString &url, int positionInList);

    // ========================================================================
    // Fields — Java lines 111-141
    // ========================================================================

    // Java line 111: TAG
    static const QString TAG;

    // Java line 113: SAVEDSTATE_FIRST_VISIBLE_POS
    static const QString SAVEDSTATE_FIRST_VISIBLE_POS;

    // Java line 115: @NonNull private PostListingURL mPostListingURL;
    const PostListingURL &m_postListingURL;

    // Java line 117: @Nullable private RedditSubreddit mSubreddit;
    RedditSubreddit *m_subreddit = nullptr;

    // Java line 119: private UUID mSession;
    QUuid m_session;

    // Java line 120: private final int mPostCountLimit;
    int m_postCountLimit;

    // Java line 121: private TextView mLoadMoreView;
    QWidget *m_loadMoreView = nullptr;

    // Java line 123: private final PostListingManager mPostListingManager;
    PostListingManager *m_postListingManager;

    // Java line 124: private final RecyclerView mRecyclerView;
    QWidget *m_recyclerView = nullptr;

    // Java line 126: private final View mOuter;
    QWidget *m_outerView = nullptr;

    // Java lines 128-131:
    RedditIdAndType *m_after = nullptr;
    RedditIdAndType *m_lastAfter = nullptr;
    CacheRequest *m_request = nullptr;
    bool m_readyToDownloadMore = false;
    TimestampUTC *m_timestamp = nullptr;

    // Java lines 134-136:
    int m_postCount = 0;
    bool m_postsNotShown = false;
    std::atomic<int> m_postRefreshCount{0};

    // Java line 138: private final HashSet<String> mPostIds = new HashSet<>(200);
    QSet<QString> m_postIds;

    // Java line 140: private Integer mPreviousFirstVisibleItemPosition;
    std::optional<int> m_previousFirstVisibleItemPosition;

    // ScrollbarRecyclerViewManager
    ScrollbarRecyclerViewManager *m_recyclerViewManager = nullptr;

    // Listener
    PostSelectionListener *m_selectionListener = nullptr;

    // Parent widget
    QWidget *m_parentWidget = nullptr;

    // Download strategy
    DownloadStrategy *m_downloadStrategy = nullptr;
};

} // namespace PinkReader
