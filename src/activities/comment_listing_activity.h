/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: comment_listing_activity.h - Port of CommentListingActivity.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/activities/CommentListingActivity.java
 *
 * Original: public class CommentListingActivity extends RefreshableActivity
 *     implements RedditAccountChangeListener,
 *                OptionsMenuUtility.OptionsMenuCommentsListener,
 *                RedditPostView.PostSelectionListener,
 *                SessionChangeListener
 *
 * Every field, method, constant, and inner class ported exactly.
 */

#pragma once

#include <QObject>
#include <QString>
#include <memory>

// Forward declarations for Android/RedReader types used in signatures
namespace PinkReader {

// Activity base classes
class RefreshableActivity;

// Android framework stubs
class Bundle;
class Intent;
class Menu;
class MenuItem;

// RedReader fragments
class CommentListingFragment;
class SessionListDialog;

// RedReader controllers
class CommentListingController;

// RedReader models
class RedditPreparedPost;

// RedReader enums
enum class PostCommentSort;
enum class UserCommentSort;

// OptionsMenuUtility
class OptionsMenuUtility;

// LinkHandler
class LinkHandler;
class PrefsUtility;
class General;
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
 * @brief Port of org.quantumbadger.redreader.activities.CommentListingActivity
 *
 * Displays a comment listing for a Reddit post. Every field, method, constant ported exactly.
 */
class CommentListingActivity : public QObject {
    Q_OBJECT

public:
    // ===== Static constants (mirrors Java exactly) =====

    // private static final String TAG = "CommentListingActivity";
    static const QString TAG;

    // public static final String EXTRA_SEARCH_STRING = "cla_search_string";
    static const QString EXTRA_SEARCH_STRING;

    // ===== Private static constants =====
    // private static final String SAVEDSTATE_SESSION = "cla_session";
    static const QString SAVEDSTATE_SESSION;
    // private static final String SAVEDSTATE_SORT = "cla_sort";
    static const QString SAVEDSTATE_SORT;
    // private static final String SAVEDSTATE_SORT_IS_USER = "cla_sort_user";
    static const QString SAVEDSTATE_SORT_IS_USER;
    // private static final String SAVEDSTATE_FRAGMENT = "cla_fragment";
    static const QString SAVEDSTATE_FRAGMENT;

    // ===== Constructor / Lifecycle =====
    explicit CommentListingActivity(QObject *parent = nullptr);
    virtual ~CommentListingActivity();

    // ===== Activity lifecycle (port of overrides) =====
    void onCreate(Bundle *savedInstanceState);
    void onSaveInstanceState(Bundle *outState);
    bool onCreateOptionsMenu(Menu *menu);
    bool onOptionsItemSelected(MenuItem *item);
    void onBackPressed();

    // ===== Interface method stubs (port of implemented interfaces) =====

    // RedditAccountChangeListener: void onRedditAccountChanged()
    virtual void onRedditAccountChanged();

    // OptionsMenuUtility.OptionsMenuCommentsListener
    virtual void onRefreshComments();
    virtual void onPastComments();
    virtual void onSortSelected(PostCommentSort order);
    virtual void onSortSelected(UserCommentSort order);
    virtual void onSearchComments();

    // RedditPostView.PostSelectionListener
    virtual void onPostSelected(RedditPreparedPost *post);
    virtual void onPostCommentsSelected(RedditPreparedPost *post);

    // SessionChangeListener
    virtual void onSessionSelected(const QString &session, SessionChangeType::Type type);
    virtual void onSessionRefreshSelected(SessionChangeType::Type type);
    virtual void onSessionChanged(const QString &session, SessionChangeType::Type type,
                                  TimestampUTC *timestamp);

    // ===== Override methods (from RefreshableActivity) =====
    virtual bool baseActivityAllowToolbarHideOnScroll();

    // ===== Sorting getters =====
    void *getCommentSort();  // Returns OptionsMenuUtility.Sort (not yet ported)
    PostCommentSort getSuggestedCommentSort();

    // ===== doRefresh (from RefreshableActivity) =====
    void doRefresh(RefreshableFragment::Type which, bool force, Bundle *savedInstanceState);

private:
    // ===== Private fields (port of private fields) =====

    // private CommentListingController controller;
    CommentListingController *controller = nullptr;

    // private CommentListingFragment mFragment;
    CommentListingFragment *mFragment = nullptr;

signals:
    // Qt signal equivalent of invalidateOptionsMenu()
    void optionsMenuInvalidated();
};

} // namespace PinkReader
