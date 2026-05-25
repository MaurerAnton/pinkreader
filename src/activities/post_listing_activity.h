/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: post_listing_activity.h - Port of PostListingActivity.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/activities/PostListingActivity.java
 *
 * Original: public class PostListingActivity extends RefreshableActivity
 *     implements RedditAccountChangeListener,
 *                RedditPostView.PostSelectionListener,
 *                OptionsMenuUtility.OptionsMenuPostsListener,
 *                SessionChangeListener,
 *                RedditSubredditSubscriptionManager.SubredditSubscriptionStateChangeListener
 *
 * Every field, method, constant, and inner class ported exactly.
 */

#pragma once

#include <QObject>
#include <QString>
#include <memory>
#include <atomic>
#include <functional>

// Forward declarations for Android/RedReader types used in signatures
namespace PinkReader {

// Activity base classes
class RefreshableActivity;
class AppCompatActivity;

// Android framework stubs
class Bundle;
class Intent;
class Menu;
class MenuItem;
class View;
class TypedArray;
class ColorDrawable;
class Window;

// RedReader fragments
class PostListingFragment;
class SessionListDialog;

// RedReader controllers
class PostListingController;

// RedReader models
class RedditAccount;
class RedditPreparedPost;
class SubredditCanonicalId;

// RedReader enums
enum class PostSort;

// Errors
class InvalidSubredditNameException;

// RedReader managers
class RedditSubredditSubscriptionManager;
class SubredditSubscriptionState;

// RedReader URLs
class PostListingURL;
class PostCommentListingURL;
class SearchPostListURL;
class RedditURLParser;

// RedReader other
class OptionsMenuUtility;
class LinkHandler;
class BugReportActivity;
class PrefsUtility;
class HtmlViewActivity;
class PostSubmitActivity;
class DialogUtils;

// Timestamp
class TimestampUTC;

// RefreshableFragment enum
namespace RefreshableFragment {
    enum Type {
        MAIN_RELAYOUT,
        ALL,
        MAIN,
        POSTS,
        COMMENTS
    };
}

// SessionChangeType enum
namespace SessionChangeType {
    enum Type {
        POSTS,
        COMMENTS
    };
}

/**
 * @brief Port of org.quantumbadger.redreader.activities.PostListingActivity
 *
 * Displays a listing of Reddit posts. Every field, method, constant ported exactly.
 */
class PostListingActivity : public QObject {
    Q_OBJECT

public:
    // ===== Static constants (mirrors Java exactly) =====
    // private static final String SAVEDSTATE_SESSION = "pla_session";
    static const QString SAVEDSTATE_SESSION;
    // private static final String SAVEDSTATE_SORT = "pla_sort";
    static const QString SAVEDSTATE_SORT;
    // private static final String SAVEDSTATE_FRAGMENT = "pla_fragment";
    static const QString SAVEDSTATE_FRAGMENT;

    // ===== Constructor / Lifecycle =====
    explicit PostListingActivity(QObject *parent = nullptr);
    virtual ~PostListingActivity();

    // ===== Activity lifecycle (port of overrides) =====
    void onCreate(Bundle *savedInstanceState);
    void onSaveInstanceState(Bundle *outState);
    void onDestroy();
    bool onCreateOptionsMenu(Menu *menu);
    void onBackPressed();

    // ===== Interface method stubs (port of implemented interfaces) =====

    // RedditAccountChangeListener: void onRedditAccountChanged()
    virtual void onRedditAccountChanged();

    // RedditPostView.PostSelectionListener: void onPostSelected(RedditPreparedPost post)
    virtual void onPostSelected(RedditPreparedPost *post);

    // RedditPostView.PostSelectionListener: void onPostCommentsSelected(RedditPreparedPost post)
    virtual void onPostCommentsSelected(RedditPreparedPost *post);

    // OptionsMenuUtility.OptionsMenuPostsListener
    virtual void onRefreshPosts();
    virtual void onPastPosts();
    virtual void onSubmitPost();
    virtual void onSortSelected(PostSort order);
    virtual void onSearchPosts();

    // OptionsMenuUtility.OptionsMenuPostsListener + OptionsMenuUtility.OptionsMenuSubredditsListener
    virtual void onSubscribe();
    virtual void onUnsubscribe();
    virtual void onSidebar();
    virtual void onPin();
    virtual void onUnpin();
    virtual void onBlock();
    virtual void onUnblock();

    // SessionChangeListener
    virtual void onSessionSelected(const QString &session, SessionChangeType::Type type);
    virtual void onSessionRefreshSelected(SessionChangeType::Type type);
    virtual void onSessionChanged(const QString &session, SessionChangeType::Type type,
                                  TimestampUTC *timestamp);

    // SubredditSubscriptionStateChangeListener
    virtual void onSubredditSubscriptionListUpdated(
        RedditSubredditSubscriptionManager *subredditSubscriptionManager);
    virtual void onSubredditSubscriptionAttempted(
        RedditSubredditSubscriptionManager *subredditSubscriptionManager);
    virtual void onSubredditUnsubscriptionAttempted(
        RedditSubredditSubscriptionManager *subredditSubscriptionManager);

    // ===== Override methods (from RefreshableActivity) =====
    virtual bool baseActivityAllowToolbarHideOnScroll();

    // ===== Sorting getters =====
    PostSort getPostSort();

    // ===== doRefresh (from RefreshableActivity) =====
    void doRefresh(RefreshableFragment::Type which, bool force, Bundle *savedInstanceState);

    // ===== Static method (port of public static void onSearchPosts) =====
    static void onSearchPosts(PostListingController *controller,
                              AppCompatActivity *activity);

private:
    // ===== Private fields (port of private fields) =====

    // private PostListingFragment fragment;
    PostListingFragment *fragment = nullptr;

    // private PostListingController controller;
    PostListingController *controller = nullptr;

    // private final AtomicReference<RedditSubredditSubscriptionManager.ListenerContext>
    //     mSubredditSubscriptionListenerContext = new AtomicReference<>(null);
    void *mSubredditSubscriptionListenerContext = nullptr; // stub AtomicReference

    // private long mDoubleTapBack_lastTapMs = -1;
    long long mDoubleTapBack_lastTapMs = -1;

    // ===== Private methods =====

    // private void recreateSubscriptionListener()
    void recreateSubscriptionListener();

    // private void postInvalidateOptionsMenu()
    void postInvalidateOptionsMenu();

signals:
    // Qt signal equivalent of invalidateOptionsMenu()
    void optionsMenuInvalidated();
};

} // namespace PinkReader
