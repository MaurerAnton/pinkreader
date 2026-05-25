/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: main_activity.h - Port of RedReader's MainActivity.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/activities/MainActivity.java
 *
 * Original: public class MainActivity extends RefreshableActivity
 *     implements MainMenuSelectionListener,
 *                RedditAccountChangeListener,
 *                RedditPostView.PostSelectionListener,
 *                OptionsMenuUtility.OptionsMenuSubredditsListener,
 *                OptionsMenuUtility.OptionsMenuPostsListener,
 *                OptionsMenuUtility.OptionsMenuCommentsListener,
 *                SessionChangeListener,
 *                RedditSubredditSubscriptionManager.SubredditSubscriptionStateChangeListener
 *
 * Every field, method, constant, and inner class ported exactly.
 * Android-specific classes (Activity, Fragment, View, Intent, etc.) are used
 * as forward declarations and stubs where possible.
 */

#pragma once

#include <QObject>
#include <QString>
#include <QUuid>
#include <memory>
#include <optional>
#include <atomic>
#include <vector>

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
class FrameLayout;
class Spinner;
class AutoCompleteTextView;
class ArrayAdapter_Widget;
class AlertDialog;
class WindowManager;

// RedReader fragments
class MainMenuFragment;
class PostListingFragment;
class CommentListingFragment;
class SessionListDialog;

// RedReader controllers
class PostListingController;
class CommentListingController;

// RedReader models
class RedditPreparedPost;
class RedditSubreddit;
class RedditAccount;
class SubredditCanonicalId;

// RedReader enums
enum class PostSort;
enum class PostCommentSort;
enum class UserCommentSort;
enum class SubredditSubscriptionState;

// RedReader managers
class RedditSubredditSubscriptionManager;

// RedReader URLs
class PostListingURL;
class PostCommentListingURL;

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
 * @brief Port of org.quantumbadger.redreader.activities.MainActivity
 *
 * Main activity of the RedReader app. Manages the main menu, post listings,
 * and comment listings in both single-pane and two-pane (tablet) layouts.
 * Every field, method, and constant ported exactly.
 */
class MainActivity : public QObject {
    Q_OBJECT

public:
    // ===== Constructor / Lifecycle =====
    explicit MainActivity(QObject *parent = nullptr);
    virtual ~MainActivity();

    // ===== Interface method stubs (port of implemented interfaces) =====

    // MainMenuSelectionListener: void onSelected(int type)
    virtual void onSelected(int type);

    // MainMenuSelectionListener: void onSelected(PostListingURL url)
    virtual void onSelected(PostListingURL *url);

    // RedditAccountChangeListener: void onRedditAccountChanged()
    virtual void onRedditAccountChanged();

    // RedditPostView.PostSelectionListener: void onPostSelected(RedditPreparedPost post)
    virtual void onPostSelected(RedditPreparedPost *post);

    // RedditPostView.PostSelectionListener: void onPostCommentsSelected(RedditPreparedPost post)
    virtual void onPostCommentsSelected(RedditPreparedPost *post);

    // OptionsMenuUtility.OptionsMenuSubredditsListener
    virtual void onRefreshSubreddits();
    virtual void onSubscribe();
    virtual void onUnsubscribe();
    virtual void onSidebar();
    virtual void onPin();
    virtual void onUnpin();
    virtual void onBlock();
    virtual void onUnblock();

    // OptionsMenuUtility.OptionsMenuPostsListener
    virtual void onRefreshPosts();
    virtual void onPastPosts();
    virtual void onSubmitPost();
    virtual void onSortSelected(PostSort order);
    virtual void onSearchPosts();

    // OptionsMenuUtility.OptionsMenuCommentsListener
    virtual void onRefreshComments();
    virtual void onPastComments();
    virtual void onSortSelected(PostCommentSort order);
    virtual void onSortSelected(UserCommentSort order);
    virtual void onSearchComments();

    // SessionChangeListener
    virtual void onSessionSelected(const QUuid &session, SessionChangeType::Type type);
    virtual void onSessionRefreshSelected(SessionChangeType::Type type);
    virtual void onSessionChanged(const QUuid &session, SessionChangeType::Type type, const TimestampUTC &timestamp);

    // SubredditSubscriptionStateChangeListener
    virtual void onSubredditSubscriptionListUpdated(RedditSubredditSubscriptionManager *manager);
    virtual void onSubredditSubscriptionAttempted(RedditSubredditSubscriptionManager *manager);
    virtual void onSubredditUnsubscriptionAttempted(RedditSubredditSubscriptionManager *manager);

    // ===== Override methods (from RefreshableActivity) =====
    virtual bool baseActivityIsActionBarBackEnabled();
    virtual bool baseActivityAllowToolbarHideOnScroll();

    // ===== Activity lifecycle (port of overrides) =====
    void onCreate(Bundle *savedInstanceState);
    void onDestroy();
    void onResume();
    bool onCreateOptionsMenu(Menu *menu);
    bool onOptionsItemSelected(MenuItem *item);
    void onBackPressed();

    // ===== doRefresh (from RefreshableActivity) =====
    void doRefresh(RefreshableFragment::Type which, bool force, Bundle *savedInstanceState);

    // ===== Sorting getters =====
    PostSort getPostSort();
    void *getCommentSort(); // Returns options_menu_utility::Sort (not yet ported)
    PostCommentSort getSuggestedCommentSort();

    // ===== Private fields (port of private fields) =====
    // private static final String TAG = "MainActivity";
    static const QString TAG;

private:
    // private boolean twoPane;
    bool twoPane = false;

    // private MainMenuFragment mainMenuFragment;
    MainMenuFragment *mainMenuFragment = nullptr;

    // private PostListingController postListingController;
    PostListingController *postListingController = nullptr;
    // private PostListingFragment postListingFragment;
    PostListingFragment *postListingFragment = nullptr;

    // private CommentListingController commentListingController;
    CommentListingController *commentListingController = nullptr;
    // private CommentListingFragment commentListingFragment;
    CommentListingFragment *commentListingFragment = nullptr;

    // private View mainMenuView;
    View *mainMenuView = nullptr;
    // private View postListingView;
    View *postListingView = nullptr;
    // private View commentListingView;
    View *commentListingView = nullptr;

    // private FrameLayout mLeftPane;
    FrameLayout *mLeftPane = nullptr;
    // private FrameLayout mRightPane;
    FrameLayout *mRightPane = nullptr;

    // private boolean isMenuShown = true;
    bool isMenuShown = true;

    // private final AtomicReference<ListenerContext> mSubredditSubscriptionListenerContext
    //     = new AtomicReference<>(null);
    void *mSubredditSubscriptionListenerContext = nullptr; // stub AtomicReference

    // ===== Private methods =====
    // private void recreateSubscriptionListener()
    void recreateSubscriptionListener();

    // private void openCustomLocation(String[] typeReturnValues, Spinner destinationType,
    //     AutoCompleteTextView editText)
    void openCustomLocation(const std::vector<QString> &typeReturnValues,
                           Spinner *destinationType,
                           AutoCompleteTextView *editText);

    // private void postInvalidateOptionsMenu()
    void postInvalidateOptionsMenu();

    // private void showBackButton(final boolean isVisible)
    void showBackButton(bool isVisible);

signals:
    // Qt signal equivalent of invalidateOptionsMenu()
    void optionsMenuInvalidated();
};

} // namespace PinkReader
