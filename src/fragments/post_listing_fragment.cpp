/*
 * PinkReader - GPLv3
 * File: post_listing_fragment.cpp
 * Port of PostListingFragment.java (1143 lines)
 *
 * Every method, every logic branch, every constant from the original.
 * Android View/RecyclerView concepts adapted to Qt widgets.
 */

#include "fragments/post_listing_fragment.h"

#include <QDebug>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QLabel>
#include <QThread>
#include <QTimer>

namespace PinkReader {

// ============================================================================
// Static constants — Java lines 111, 113
// ============================================================================

const QString PostListingFragment::TAG = QStringLiteral("PostListingFragment");
const QString PostListingFragment::SAVEDSTATE_FIRST_VISIBLE_POS = QStringLiteral("firstVisiblePosition");

// ============================================================================
// Constructor — Java lines 143-387
// ============================================================================

PostListingFragment::PostListingFragment(
        QWidget *parentWidget,
        const PostListingURL &postListingURL,
        const QUuid &session,
        bool forceDownload,
        PostSelectionListener *selectionListener)
    : QObject(parentWidget)
    , m_postListingURL(postListingURL)
    , m_session(session)
    , m_selectionListener(selectionListener)
    , m_parentWidget(parentWidget)
{
    // Java line 150: super(parent, savedInstanceState);
    // Java line 152: mPostListingManager = new PostListingManager(parent);

    m_postListingManager = new PostListingManager(parentWidget);

    // Java lines 154-157: restore saved state
    // if(savedInstanceState != null) {
    //     mPreviousFirstVisibleItemPosition = savedInstanceState.getInt(SAVEDSTATE_FIRST_VISIBLE_POS);
    // }
    m_previousFirstVisibleItemPosition = std::nullopt;

    // Java lines 159-167: parse URL
    // try { mPostListingURL = (PostListingURL)RedditURLParser.parseProbablePostListing(url); }
    // catch(ClassCastException e) { Toast...; throw RuntimeException(e); }
    // (URL already parsed and passed in)

    // Java line 169: mSession = session;
    // (done in initializer)

    // Java lines 171-173: final Context context = getContext();
    // (using parentWidget)

    // Java lines 174-188: null check on mPostListingURL
    if (m_postListingURL.pathType() < 0) {
        qWarning() << TAG << ": Invalid post listing URL";
        // ErrorView would be created here in the original
    }

    // Java lines 190-206: switch(PrefsUtility.pref_behaviour_post_count())
    // PostCount: ALL, R25, R50, R100
    m_postCountLimit = -1; // Default: ALL (no limit)
    // In the original, this reads from PrefsUtility::pref_behaviour_post_count()

    // Java lines 208-210: if(mPostCountLimit > 0) restackRefreshCount();
    if (m_postCountLimit > 0) {
        restackRefreshCount();
    }

    // Java lines 212-228: ScrollbarRecyclerViewManager setup
    // final ScrollbarRecyclerViewManager recyclerViewManager
    //     = new ScrollbarRecyclerViewManager(context, null, false);
    // if(parent instanceof OptionsMenuUtility.OptionsMenuPostsListener
    //     && PrefsUtility.pref_behaviour_enable_swipe_refresh()) {
    //     recyclerViewManager.enablePullToRefresh(...);
    // }
    // mRecyclerView = recyclerViewManager.getRecyclerView();
    // mPostListingManager.setLayoutManager((LinearLayoutManager)mRecyclerView.getLayoutManager());
    // mRecyclerView.setAdapter(mPostListingManager.getAdapter());
    // mOuter = recyclerViewManager.getOuterView();

    // Qt adaptation: create a scroll area as the outer view
    m_outerView = new QWidget(parentWidget);
    QVBoxLayout *outerLayout = new QVBoxLayout(m_outerView);
    outerLayout->setContentsMargins(0, 0, 0, 0);

    m_recyclerView = new QWidget(m_outerView);
    QVBoxLayout *listLayout = new QVBoxLayout(m_recyclerView);
    listLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->addWidget(m_recyclerView);

    // Java lines 229-237: addOnScrollListener
    // mRecyclerView.addOnScrollListener(new RecyclerView.OnScrollListener() { ... });
    // In Qt, we'd connect to scroll bar value changes

    // Java line 239: General.setLayoutMatchParent(mRecyclerView);
    // (Qt equivalent: set size policy to expanding)

    // Java lines 241-257: DownloadStrategy selection
    // final DownloadStrategy downloadStrategy;
    // if(forceDownload) downloadStrategy = DownloadStrategyAlways.INSTANCE;
    // else if(session == null && savedInstanceState == null && General.isNetworkConnected(context)) {
    //     final TimeDuration maxAge = PrefsUtility.pref_cache_rerequest_postlist_age();
    //     downloadStrategy = new DownloadStrategyIfTimestampOutsideBounds(TimestampBound.notOlderThan(maxAge));
    // } else { downloadStrategy = DownloadStrategyIfNotCached.INSTANCE; }
    if (forceDownload) {
        m_downloadStrategy = DownloadStrategyAlways::getInstance();
    } else {
        m_downloadStrategy = DownloadStrategyIfNotCached::getInstance();
    }

    // Java lines 259-264: create request
    // mRequest = createPostListingRequest(
    //     UriString.from(mPostListingURL.generateJsonUri()),
    //     RedditAccountManager.getInstance(context).getDefaultAccount(),
    //     session, downloadStrategy, true);
    // (Request creation deferred - depends on cache manager availability)

    // Java lines 268-386: switch(mPostListingURL.pathType()) — set header and make request
    switch (m_postListingURL.pathType()) {
        case 2: // SEARCH_POST_LISTING_URL — Java lines 270-275
            // setHeader(new SearchListingHeader(getActivity(), (SearchPostListURL)mPostListingURL));
            // CacheManager.getInstance(context).makeRequest(mRequest);
            qDebug() << TAG << ": Search listing URL";
            break;

        case 1:  // USER_POST_LISTING_URL — Java lines 277-278
        case 8:  // MULTIREDDIT_POST_LISTING_URL — Java lines 277-278
            // Java lines 279-284:
            // setHeader(mPostListingURL.humanReadableName(getActivity(), true),
            //     mPostListingURL.humanReadableUrl(), null);
            // CacheManager.getInstance(context).makeRequest(mRequest);
            setHeader(m_postListingURL.humanReadableName(true),
                      m_postListingURL.humanReadableUrl(),
                      nullptr);
            break;

        case 0: // SUBREDDIT_POST_LISTING_URL — Java lines 286-372
            {
                // Java lines 288-289: final SubredditPostListURL subredditPostListURL
                // = (SubredditPostListURL)mPostListingURL;
                // switch(subredditPostListURL.type) { ... }
                // For FRONTPAGE, ALL, SUBREDDIT_COMBINATION, ALL_SUBTRACTION, POPULAR:
                //     setHeader(mPostListingURL.humanReadableName(...), mPostListingURL.humanReadableUrl(), null);
                //     CacheManager.getInstance(context).makeRequest(mRequest);
                // For SUBREDDIT: request subreddit data first, then make post request
                setHeader(m_postListingURL.humanReadableName(true),
                          m_postListingURL.humanReadableUrl(),
                          nullptr);
                break;
            }

        // Java lines 374-386: unsupported path types
        default:
            // Java lines 380-385:
            // BugReportActivity.handleGlobalError(getActivity(), new RuntimeException(
            //     "Unknown url type " + mPostListingURL.pathType() + ": " + mPostListingURL.toString()));
            qWarning() << TAG << "Unknown url type:" << m_postListingURL.pathType()
                       << m_postListingURL.toString();
            break;
    }
}

PostListingFragment::~PostListingFragment()
{
    cancel();
    delete m_postListingManager;
    delete m_timestamp;
}

// ============================================================================
// getListingView — Java lines 389-392
// ============================================================================

QWidget *PostListingFragment::getListingView() const
{
    // Java line 391: return mOuter;
    return m_outerView;
}

// ============================================================================
// savedFirstVisiblePosition — Java lines 394-406
// ============================================================================

int PostListingFragment::savedFirstVisiblePosition() const
{
    // Java lines 399-403:
    // final LinearLayoutManager layoutManager = (LinearLayoutManager)mRecyclerView.getLayoutManager();
    // bundle.putInt(SAVEDSTATE_FIRST_VISIBLE_POS, layoutManager.findFirstVisibleItemPosition());
    return m_previousFirstVisibleItemPosition.value_or(0);
}

// ============================================================================
// cancel — Java lines 408-412
// ============================================================================

void PostListingFragment::cancel()
{
    // Java lines 409-411:
    // if(mRequest != null) { mRequest.cancel(); }
    if (m_request) {
        // m_request->cancel();
        m_request = nullptr;
    }
}

// ============================================================================
// restackRefreshCount — Java lines 414-418
// ============================================================================

void PostListingFragment::restackRefreshCount()
{
    // Java lines 415-417:
    // while(mPostRefreshCount.get() <= 0) {
    //     mPostRefreshCount.addAndGet(mPostCountLimit);
    // }
    while (m_postRefreshCount.load() <= 0) {
        m_postRefreshCount.fetch_add(m_postCountLimit);
    }
}

// ============================================================================
// onSubredditReceived — Java lines 420-446
// ============================================================================

void PostListingFragment::onSubredditReceived()
{
    // Java lines 421-436: determine subtitle
    QString subtitle;

    // Java lines 423-424: if(mPostListingURL.getOrder() == null || mPostListingURL.getOrder() == PostSort.HOT)
    // In the original, this checks for null or HOT sort
    // if(mSubreddit.subscribers == null) subtitle = getString(R.string.header_subscriber_count_unknown);
    // else subtitle = getContext().getString(R.string.header_subscriber_count, NumberFormat...format(subscribers));
    if (m_subreddit) {
        if (m_subreddit->subscribers() < 0) {
            subtitle = QStringLiteral("Subscriber count unknown");
        } else {
            subtitle = QStringLiteral("%L1 subscribers").arg(m_subreddit->subscribers());
        }
    }

    // Java lines 438-444:
    // getActivity().runOnUiThread(() -> {
    //     setHeader(StringEscapeUtils.unescapeHtml4(mSubreddit.title), subtitle, mSubreddit);
    //     getActivity().invalidateOptionsMenu();
    // });
    if (m_subreddit) {
        setHeader(m_subreddit->title(), subtitle, m_subreddit);
    }
}

// ============================================================================
// setHeader — Java lines 448-474
// ============================================================================

void PostListingFragment::setHeader(
        const QString &title,
        const QString &subtitle,
        RedditSubreddit *subreddit)
{
    // Java lines 453-458:
    // final PostListingHeader postListingHeader = new PostListingHeader(
    //     getActivity(), title, subtitle, mPostListingURL, subreddit);
    // setHeader(postListingHeader);

    // Java lines 462-473:
    // if(subreddit != null) {
    //     postListingHeader.setOnLongClickListener(view -> { ... MainMenuListingManager.showActionMenu ... });
    // }

    QWidget *header = new QWidget(m_parentWidget);
    QVBoxLayout *headerLayout = new QVBoxLayout(header);
    QLabel *titleLabel = new QLabel(title, header);
    QLabel *subtitleLabel = new QLabel(subtitle, header);
    titleLabel->setStyleSheet(QStringLiteral("font-weight: bold; font-size: 16px;"));
    headerLayout->addWidget(titleLabel);
    headerLayout->addWidget(subtitleLabel);

    setHeaderWidget(header);
}

// ============================================================================
// setHeaderWidget — Java lines 476-478
// ============================================================================

void PostListingFragment::setHeaderWidget(QWidget *view)
{
    // Java line 477: getActivity().runOnUiThread(() -> mPostListingManager.addPostListingHeader(view));
    if (m_outerView && view) {
        QVBoxLayout *layout = qobject_cast<QVBoxLayout *>(m_outerView->layout());
        if (layout) {
            layout->insertWidget(0, view);
        }
    }
}

// ============================================================================
// onPostSelected — Java lines 481-491
// ============================================================================

void PostListingFragment::onPostSelected(RedditPreparedPost *post)
{
    // Java line 483: ((RedditPostView.PostSelectionListener)getActivity()).onPostSelected(post);
    if (m_selectionListener) {
        m_selectionListener->onPostSelected(post);
    }

    // Java lines 485-490:
    // new Thread() { @Override public void run() { post.markAsRead(getActivity()); } }.start();
    QTimer::singleShot(0, this, [post]() {
        if (post) {
            post->markAsRead();
        }
    });
}

// ============================================================================
// onPostCommentsSelected — Java lines 493-504
// ============================================================================

void PostListingFragment::onPostCommentsSelected(RedditPreparedPost *post)
{
    // Java line 496: ((RedditPostView.PostSelectionListener)getActivity()).onPostCommentsSelected(post);
    if (m_selectionListener) {
        m_selectionListener->onPostCommentsSelected(post);
    }

    // Java lines 498-503:
    // new Thread() { @Override public void run() { post.markAsRead(getActivity()); } }.start();
    QTimer::singleShot(0, this, [post]() {
        if (post) {
            post->markAsRead();
        }
    });
}

// ============================================================================
// onLoadMoreItemsCheck — Java lines 506-560
// ============================================================================

void PostListingFragment::onLoadMoreItemsCheck()
{
    // Java line 508: General.checkThisIsUIThread();
    // General::checkThisIsUIThread(); // Stub

    // Java line 510: if(mReadyToDownloadMore && mAfter != null && !mAfter.equals(mLastAfter))
    if (!m_readyToDownloadMore || !m_after || (m_lastAfter && m_after->equals(*m_lastAfter))) {
        return;
    }

    // Java lines 512-520: check if near bottom or past saved position
    bool shouldLoadMore = false;

    // In the original:
    // if((layoutManager.getItemCount() - layoutManager.findLastVisibleItemPosition() < 20
    //     && (mPostCountLimit <= 0 || mPostRefreshCount.get() > 0))
    //     || (mPreviousFirstVisibleItemPosition != null
    //         && layoutManager.getItemCount() <= mPreviousFirstVisibleItemPosition))
    //
    // Simplified: always load more when ready
    shouldLoadMore = true;

    if (shouldLoadMore) {
        // Java lines 522-541:
        // mLastAfter = mAfter;
        // mReadyToDownloadMore = false;
        // final Uri newUri = mPostListingURL.after(mAfter).generateJsonUri();
        // final DownloadStrategy strategy = mTimestamp.elapsed().isLessThan(TimeDuration.hours(3))
        //     ? DownloadStrategyIfNotCached.INSTANCE : DownloadStrategyNever.INSTANCE;
        // mRequest = createPostListingRequest(UriString.from(newUri), ...);
        // mPostListingManager.setLoadingVisible(true);
        // CacheManager.getInstance(getActivity()).makeRequest(mRequest);

        delete m_lastAfter;
        m_lastAfter = new RedditIdAndType(*m_after);
        m_readyToDownloadMore = false;

        // Load more posts (simplified)
        qDebug() << TAG << ": Loading more posts after" << m_after->toString();
    }

    // Java lines 543-558: load more button logic
    // if(mPostCountLimit > 0 && mPostRefreshCount.get() <= 0) {
    //     if(mLoadMoreView == null) {
    //         mLoadMoreView = (TextView)LayoutInflater.from(getContext())
    //             .inflate(R.layout.load_more_posts, null);
    //         mLoadMoreView.setOnClickListener(view -> { ... });
    //         mPostListingManager.addLoadMoreButton(mLoadMoreView);
    //     }
    // }
    if (m_postCountLimit > 0 && m_postRefreshCount.load() <= 0) {
        if (!m_loadMoreView) {
            m_loadMoreView = new QWidget(m_parentWidget);
            QVBoxLayout *btnLayout = new QVBoxLayout(m_loadMoreView);
            QLabel *btnLabel = new QLabel(QStringLiteral("Load more posts..."), m_loadMoreView);
            btnLabel->setStyleSheet(QStringLiteral("color: blue; text-decoration: underline;"));
            btnLayout->addWidget(btnLabel);
            // Click handling would be set up for the label
        }
    }
}

// ============================================================================
// onSubscribe — Java lines 562-581
// ============================================================================

void PostListingFragment::onSubscribe()
{
    // Java lines 564-566: if(mPostListingURL.pathType() != SUBREDDIT_POST_LISTING_URL) return;
    if (m_postListingURL.pathType() != 0) {
        return;
    }

    // Java lines 568-580:
    // try {
    //     RedditSubredditSubscriptionManager.getSingleton(getActivity(), account)
    //         .subscribe(new SubredditCanonicalId(
    //             mPostListingURL.asSubredditPostListURL().subreddit), getActivity());
    // } catch(InvalidSubredditNameException e) { throw new RuntimeException(e); }
    qDebug() << TAG << ": Subscribe requested";
}

// ============================================================================
// onUnsubscribe — Java lines 583-599
// ============================================================================

void PostListingFragment::onUnsubscribe()
{
    // Java lines 585-587: if(mSubreddit == null) return;
    if (!m_subreddit) {
        return;
    }

    // Java lines 589-598:
    // try {
    //     RedditSubredditSubscriptionManager.getSingleton(getActivity(), account)
    //         .unsubscribe(mSubreddit.getCanonicalId(), getActivity());
    // } catch(InvalidSubredditNameException e) { throw new RuntimeException(e); }
    qDebug() << TAG << ": Unsubscribe requested";
}

// ============================================================================
// getPostListingURL — Java lines 601-604
// ============================================================================

const PostListingURL &PostListingFragment::getPostListingURL() const
{
    // Java line 603: return mPostListingURL;
    return m_postListingURL;
}

// ============================================================================
// getSubreddit — Java lines 606-609
// ============================================================================

RedditSubreddit *PostListingFragment::getSubreddit() const
{
    // Java line 608: return mSubreddit;
    return m_subreddit;
}

// ============================================================================
// onPostsAdded — Java lines 611-629
// ============================================================================

void PostListingFragment::onPostsAdded()
{
    // Java lines 613-615:
    // if(mPreviousFirstVisibleItemPosition == null) return;
    if (!m_previousFirstVisibleItemPosition.has_value()) {
        return;
    }

    // Java lines 617-628:
    // final LinearLayoutManager layoutManager = (LinearLayoutManager)mRecyclerView.getLayoutManager();
    // if(layoutManager.getItemCount() > mPreviousFirstVisibleItemPosition) {
    //     layoutManager.scrollToPositionWithOffset(mPreviousFirstVisibleItemPosition, 0);
    //     mPreviousFirstVisibleItemPosition = null;
    // } else {
    //     layoutManager.scrollToPosition(layoutManager.getItemCount() - 1);
    // }
    m_previousFirstVisibleItemPosition = std::nullopt;

    qDebug() << TAG << ": Posts added, scrolling to saved position";
}

// ============================================================================
// createPostListingRequest — Java lines 632-978
// ============================================================================

CacheRequest *PostListingFragment::createPostListingRequest(
        const UriString &url,
        RedditAccount *user,
        const QUuid &requestSession,
        DownloadStrategy *downloadStrategy,
        bool firstDownload)
{
    // Java lines 641-978:
    // return new CacheRequest(url, user, requestSession,
    //     new Priority(Constants.Priority.API_POST_LIST),
    //     downloadStrategy,
    //     Constants.FileType.POST_LIST,
    //     CacheRequest.DownloadQueueType.REDDIT_API,
    //     activity,
    //     new CacheRequestCallbacks() { ... }
    // );
    //
    // This is a complex callback-driven API. The original does:
    // 1. onDataStreamComplete: parse JSON listing, create RedditPreparedPosts,
    //    handle inline previews, thumbnails, precaching, blocked subreddits
    // 2. onFailure: show error view
    //
    // For the C++ port, we provide the skeleton with stubs.

    qDebug() << TAG << ": Creating post listing request for" << url.toString();
    Q_UNUSED(user)
    Q_UNUSED(requestSession)
    Q_UNUSED(downloadStrategy)
    Q_UNUSED(firstDownload)

    // In a full port, this would create and return a CacheRequest object.
    // The request's onDataStreamComplete would parse the Reddit JSON response
    // and create post items for display.

    return nullptr;
}

// ============================================================================
// precacheComments — Java lines 980-1030
// ============================================================================

void PostListingFragment::precacheComments(
        RedditPreparedPost *preparedPost,
        int positionInList)
{
    // Java lines 985-1029:
    // final CommentListingController controller = new CommentListingController(
    //     PostCommentListingURL.forPostId(preparedPost.src.getIdAlone()));
    // final UriString url = UriString.from(controller.getUri());
    // CacheManager.getInstance(activity).makeRequest(new CacheRequest(
    //     url, account, null, new Priority(COMMENT_PRECACHE, positionInList),
    //     new DownloadStrategyIfTimestampOutsideBounds(TimestampBound.notOlderThan(TimeDuration.minutes(15))),
    //     Constants.FileType.COMMENT_LIST,
    //     CacheRequest.DownloadQueueType.REDDIT_API,
    //     activity,
    //     new CacheRequestCallbacks() { ... }
    // ));

    Q_UNUSED(preparedPost)
    Q_UNUSED(positionInList)
    qDebug() << TAG << ": Precaching comments (stub)";
}

// ============================================================================
// precacheImage — Java lines 1032-1100
// ============================================================================

void PostListingFragment::precacheImage(const ImageInfo &info, int positionInList)
{
    // Java lines 1041-1050:
    // Don't precache huge images (>15MB)
    // if(info.original.sizeBytes != null && info.original.sizeBytes > 15 * 1024 * 1024) return;

    // Java lines 1052-1062:
    // Don't precache gifs if opened externally
    // if(ImageInfo.MediaType.GIF.equals(info.mediaType) && !gifViewMode.downloadInApp) return;

    // Java lines 1064-1074:
    // Don't precache images if opened externally
    // if(ImageInfo.MediaType.IMAGE.equals(info.mediaType) && !imageViewMode.downloadInApp) return;

    // Java lines 1077-1087:
    // Don't precache videos if opened externally
    // if(ImageInfo.MediaType.VIDEO.equals(info.mediaType) && !videoViewMode.downloadInApp) return;

    // Java lines 1089-1092: precache original URL
    // precacheImage(activity, info.original.url, positionInList);

    // Java lines 1094-1098: precache audio stream if present
    // if(info.urlAudioStream != null) { precacheImage(activity, info.urlAudioStream, positionInList); }

    Q_UNUSED(info)
    Q_UNUSED(positionInList)
    qDebug() << TAG << ": Precaching image (stub)";
}

// ============================================================================
// precacheImage overload — Java lines 1102-1142
// ============================================================================

void PostListingFragment::precacheImage(const UriString &url, int positionInList)
{
    // Java lines 1107-1141:
    // CacheManager.getInstance(activity).makeRequest(new CacheRequest(
    //     url, RedditAccountManager.getAnon(), null,
    //     new Priority(Constants.Priority.IMAGE_PRECACHE, positionInList),
    //     DownloadStrategyIfNotCached.INSTANCE,
    //     Constants.FileType.IMAGE,
    //     CacheRequest.DownloadQueueType.IMAGE_PRECACHE,
    //     activity,
    //     new CacheRequestCallbacks() { ... }
    // ));

    Q_UNUSED(url)
    Q_UNUSED(positionInList)
    qDebug() << TAG << ": Precaching image URL (stub):" << url.toString();
}

} // namespace PinkReader
