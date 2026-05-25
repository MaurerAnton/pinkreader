/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: comment_listing_fragment.h - Port of CommentListingFragment.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/fragments/CommentListingFragment.java
 *
 * Original: public class CommentListingFragment extends RRFragment
 *     implements RedditPostView.PostSelectionListener,
 *                RedditCommentView.CommentListener,
 *                CommentListingRequest.Listener
 *
 * Every field, method, constant, and inner class ported exactly.
 */

#pragma once

#include <QObject>
#include <QString>
#include <QUuid>
#include <memory>
#include <vector>
#include <optional>

// Forward declarations for Android/RedReader types used in signatures
namespace PinkReader {

// Activity base classes
class AppCompatActivity;
class BaseActivity;
class RRFragment;

// Android framework stubs
class Bundle;
class Context;
class View;
class ViewGroup;
class FrameLayout;
class LinearLayout;
class ImageButton;
class TextView;
class LayoutInflater;
class Menu;
class MenuItem;
class RecyclerView;
class Animation;
class LinearLayoutManager;
class AnimationUtils;
class OvershootInterpolator;
class AccessibilityEvent;
class AccessibilityNodeInfo;

// RedReader adapters
class GroupedRecyclerViewAdapter;

// RedReader models
class RedditAccount;
class RedditAccountManager;
class RedditPreparedPost;
class RedditCommentListItem;
class RedditRenderableComment;
class RedditChangeDataManager;
class RRThemeAttributes;
class RedditPost;

// RedReader cache/download
class DownloadStrategy;
class DownloadStrategyAlways;
class DownloadStrategyIfNotCached;
class DownloadStrategyIfTimestampOutsideBounds;

// RedReader common
class General;
class PrefsUtility;
class RRError;
class TimestampBound;
class TimeDuration;
class TimestampUTC;
class AndroidCommon;

// RedReader reddit
class CommentListingRequest;
class RedditAPICommentAction;
class RedditPostActions;
class RedditURLParser;

// RedReader views
class RedditCommentView;
class RedditPostView;
class RedditPostHeaderView;
class ScrollbarRecyclerViewManager;
class BezelSwipeOverlay;
class SideToolbarOverlay;
class CommentSubThreadView;
class ErrorView;
class AccessibilityActionManager;

// RedReader other
class OptionsMenuUtility;
class FilteredCommentListingManager;

/**
 * @brief Port of org.quantumbadger.redreader.fragments.CommentListingFragment
 *
 * Displays comment threads for Reddit posts. Implements
 * RedditPostView.PostSelectionListener, RedditCommentView.CommentListener,
 * and CommentListingRequest.Listener. Every field, method, constant, and
 * inner class ported exactly.
 */
class CommentListingFragment : public QObject {
    Q_OBJECT

public:
    // ===== Nested interfaces =====

    /**
     * @brief PostSelectionListener interface (from RedditPostView)
     */
    class PostSelectionListener {
    public:
        virtual ~PostSelectionListener() = default;
        virtual void onPostSelected(RedditPreparedPost *post) = 0;
        virtual void onPostCommentsSelected(RedditPreparedPost *post) = 0;
    };

    /**
     * @brief CommentListener interface (from RedditCommentView)
     */
    class CommentListener {
    public:
        virtual ~CommentListener() = default;
        virtual void onCommentClicked(RedditCommentView *view) = 0;
        virtual void onCommentLongClicked(RedditCommentView *view) = 0;
    };

    /**
     * @brief Listener interface (from CommentListingRequest)
     */
    class CommentListingRequestListener {
    public:
        virtual ~CommentListingRequestListener() = default;
        virtual void onCommentListingRequestDownloadNecessary() = 0;
        virtual void onCommentListingRequestFailure(const RRError &error) = 0;
        virtual void onCommentListingRequestCachedCopy(TimestampUTC *timestamp) = 0;
        virtual void onCommentListingRequestParseStart() = 0;
        virtual void onCommentListingRequestPostDownloaded(RedditPreparedPost *post) = 0;
        virtual void onCommentListingRequestAllItemsDownloaded(
            const std::vector<RedditCommentListItem *> &items) = 0;
    };

    // ===== Static constants (mirrors Java exactly) =====

    // Java line 92: private static final String SAVEDSTATE_FIRST_VISIBLE_POS = "firstVisiblePosition";
    static const QString SAVEDSTATE_FIRST_VISIBLE_POS;

    // Java line 93: private static final String SAVEDSTATE_SELFTEXT_VISIBLE = "selftextVisible";
    static const QString SAVEDSTATE_SELFTEXT_VISIBLE;

    // ===== Constructor / Lifecycle =====

    // Java line 120-313: CommentListingFragment(AppCompatActivity parent, Bundle
    //     savedInstanceState, ArrayList<RedditURLParser.RedditURL> urls, UUID session,
    //     String searchString, boolean forceDownload)
    CommentListingFragment(
        QObject *parent,
        Bundle *savedInstanceState,
        const std::vector<RedditURLParser *> &urls,
        const QUuid &session,
        const QString &searchString,
        bool forceDownload);

    virtual ~CommentListingFragment();

    // ===== Public API (port of public methods) =====

    // Java lines 316-340: public void handleCommentVisibilityToggle(final RedditCommentView view)
    void handleCommentVisibilityToggle(RedditCommentView *view);

    // Java lines 342-345: @Override public View getListingView()
    QObject *getListingView();

    // Java lines 347-351: @Nullable @Override public View getOverlayView()
    QObject *getOverlayView();

    // Java lines 353-355: public RedditPreparedPost getPost()
    RedditPreparedPost *getPost();

    // Java lines 357-373: @Override public Bundle onSaveInstanceState()
    Bundle *onSaveInstanceState();

    // Java lines 715-739: public void onPreviousParent()
    void onPreviousParent();

    // Java lines 742-762: public void onNextParent()
    void onNextParent();

    // ===== interface implementation stubs =====

    // Java lines 394-416: @Override void onCommentClicked(RedditCommentView view)
    void onCommentClicked(RedditCommentView *view);

    // Java lines 418-443: @Override void onCommentLongClicked(RedditCommentView view)
    void onCommentLongClicked(RedditCommentView *view);

    // Java lines 445-448: @Override void onCommentListingRequestDownloadNecessary()
    void onCommentListingRequestDownloadNecessary();

    // Java lines 450-454: @Override void onCommentListingRequestFailure(final RRError error)
    void onCommentListingRequestFailure(const RRError &error);

    // Java lines 456-459: @Override void onCommentListingRequestCachedCopy(TimestampUTC timestamp)
    void onCommentListingRequestCachedCopy(TimestampUTC *timestamp);

    // Java lines 461-464: @Override void onCommentListingRequestParseStart()
    void onCommentListingRequestParseStart();

    // Java lines 466-595: @Override void onCommentListingRequestPostDownloaded(
    //     final RedditPreparedPost post)
    void onCommentListingRequestPostDownloaded(RedditPreparedPost *post);

    // Java lines 597-658: @Override void onCommentListingRequestAllItemsDownloaded(
    //     final ArrayList<RedditCommentListItem> items)
    void onCommentListingRequestAllItemsDownloaded(
        const std::vector<RedditCommentListItem *> &items);

    // Java lines 661-686: @Override void onCreateOptionsMenu(final Menu menu)
    void onCreateOptionsMenu(Menu *menu);

    // Java lines 688-703: @Override boolean onOptionsItemSelected(final MenuItem item)
    bool onOptionsItemSelected(MenuItem *item);

    // Java lines 705-708: @Override void onPostSelected(final RedditPreparedPost post)
    void onPostSelected(RedditPreparedPost *post);

    // Java lines 710-713: @Override void onPostCommentsSelected(final RedditPreparedPost post)
    void onPostCommentsSelected(RedditPreparedPost *post);

private:
    // ===== Private fields (Java lines 95-118) =====

    // Java line 95: private final RedditAccount mUser;
    RedditAccount *mUser = nullptr;

    // Java line 96: private final ArrayList<RedditURLParser.RedditURL> mAllUrls;
    std::vector<RedditURLParser *> mAllUrls;

    // Java line 97: private final LinkedList<RedditURLParser.RedditURL> mUrlsToDownload;
    std::vector<RedditURLParser *> mUrlsToDownload;

    // Java line 98: private final UUID mSession;
    QUuid mSession;

    // Java line 99: private final DownloadStrategy mDownloadStrategy;
    DownloadStrategy *mDownloadStrategy = nullptr;

    // Java line 101: private RedditPreparedPost mPost = null;
    RedditPreparedPost *mPost = nullptr;

    // Java line 103: private boolean mSelfTextVisible = true;
    bool mSelfTextVisible = true;

    // Java line 105: private final FilteredCommentListingManager mCommentListingManager;
    FilteredCommentListingManager *mCommentListingManager = nullptr;

    // Java line 107: private final RecyclerView mRecyclerView;
    QObject *mRecyclerView = nullptr;

    // Java line 109: private final View mListingView;
    QObject *mListingView = nullptr;

    // Java line 110: private final FrameLayout mOverlayFrame;
    QObject *mOverlayFrame = nullptr;

    // Java line 111: private final @Nullable LinearLayout mFloatingToolbar;
    QObject *mFloatingToolbar = nullptr;

    // Java line 113: private final float mSelfTextFontScale;
    float mSelfTextFontScale = 1.0f;

    // Java line 114: private final boolean mShowLinkButtons;
    bool mShowLinkButtons = false;

    // Java line 116: private TimestampUTC mCachedTimestamp = null;
    TimestampUTC *mCachedTimestamp = nullptr;

    // Java line 118: private Integer mPreviousFirstVisibleItemPosition;
    std::optional<int> mPreviousFirstVisibleItemPosition;

    // ===== Private methods =====

    // Java lines 375-392: private void makeNextRequest(final Context context)
    void makeNextRequest(Context *context);

    // Java lines 765-778: private void setFocusDelayed(final int pos)
    void setFocusDelayed(int pos);

signals:
    // Qt signal equivalents
    void commentsLoaded(int count);
    void loadError(const QString &message);
    void optionsMenuInvalidated();
};

} // namespace PinkReader
