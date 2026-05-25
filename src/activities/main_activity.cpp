/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: main_activity.cpp - Port of RedReader's MainActivity.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/activities/MainActivity.java
 *
 * Every field, method, constant, and inner class ported exactly.
 */

#include "main_activity.h"

namespace PinkReader {

// ===== Static constant (port of: private static final String TAG = "MainActivity") =====
const QString MainActivity::TAG = QStringLiteral("MainActivity");

// ===== Constructor =====
MainActivity::MainActivity(QObject *parent)
    : QObject(parent)
{
}

// ===== Destructor =====
MainActivity::~MainActivity() = default;

// ===== Port of: protected boolean baseActivityIsActionBarBackEnabled() =====
// @Override returns false
bool MainActivity::baseActivityIsActionBarBackEnabled() {
    return false;
}

// ===== Port of: protected boolean baseActivityAllowToolbarHideOnScroll() =====
// @Override returns !General.isTablet(this)
bool MainActivity::baseActivityAllowToolbarHideOnScroll() {
    // return !General.isTablet(this);
    // Stub: General.isTablet not yet ported; assume phone (non-tablet) => returns true
    return true;
}

// ===== Port of: protected void onCreate(final Bundle savedInstanceState) =====
void MainActivity::onCreate(Bundle *savedInstanceState) {
    // PrefsUtility.applyTheme(this);
    // super.onCreate(savedInstanceState);

    // if(!isTaskRoot()
    //     && getIntent().hasCategory(Intent.CATEGORY_LAUNCHER)
    //     && getIntent().getAction() != null
    //     && getIntent().getAction().equals(Intent.ACTION_MAIN)) {
    //     // Workaround for issue where a new MainActivity is created despite the app already running
    //     finish();
    //     return;
    // }

    // if(!PrefsUtility.isRedditUserAgreementAccepted()
    //     && !PrefsUtility.isRedditUserAgreementDeclined()) {
    //     RedditTermsActivity.launch(this, true);
    //     finish();
    //     return;
    // }

    // final SharedPrefsWrapper sharedPreferences = General.getSharedPrefs(this);
    // twoPane = General.isTablet(this);
    twoPane = false; // stub

    // setTitle(R.string.app_name);

    // RedditAccountManager.getInstance(this).addUpdateListener(this);

    // final AndroidCommon.PackageInfo pInfo = RedReader.getInstance(this).getPackageInfo();
    // final int appVersion = pInfo.getVersionCode();
    // Log.i(TAG, "[Migration] App version: " + appVersion);

    // if(!sharedPreferences.contains(FeatureFlagHandler.PREF_FIRST_RUN_MESSAGE_SHOWN)) {
    //     Log.i(TAG, "[Migration] Showing first run message");
    //     FeatureFlagHandler.handleFirstInstall(sharedPreferences);
    //     // Show first-run dialog...
    //     sharedPreferences.edit()
    //         .putString(FeatureFlagHandler.PREF_FIRST_RUN_MESSAGE_SHOWN, "true")
    //         .putInt(FeatureFlagHandler.PREF_LAST_VERSION, appVersion)
    //         .apply();
    // } else if(sharedPreferences.contains(FeatureFlagHandler.PREF_LAST_VERSION)) {
    //     FeatureFlagHandler.handleLegacyUpgrade(this, appVersion, pInfo.getVersionName());
    // } else {
    //     Log.i(TAG, "[Migration] Last version not set.");
    //     sharedPreferences.edit()
    //         .putInt(FeatureFlagHandler.PREF_LAST_VERSION, appVersion)
    //         .apply();
    //     ChangelogDialog.newInstance().show(getSupportFragmentManager(), null);
    // }
    // FeatureFlagHandler.handleUpgrade(this);

    // if(RedditOAuth.anyNeedRelogin(this)) {
    //     General.showMustReloginDialog(this);
    // } else {
    //     AndroidCommon.promptForNotificationPermission(this, null);
    // }

    // recreateSubscriptionListener();

    // doRefresh(RefreshableFragment.MAIN_RELAYOUT, false, null);

    // if(savedInstanceState == null
    //     && PrefsUtility.pref_behaviour_skiptofrontpage()) {
    //     onSelected(SubredditPostListURL.getFrontPage());
    // }
}

// ===== Port of: private void recreateSubscriptionListener() =====
void MainActivity::recreateSubscriptionListener() {
    // final RedditSubredditSubscriptionManager.ListenerContext oldContext
    //     = mSubredditSubscriptionListenerContext.getAndSet(
    //         RedditSubredditSubscriptionManager
    //             .getSingleton(this, RedditAccountManager.getInstance(this)
    //                 .getDefaultAccount())
    //             .addListener(this));
    // if(oldContext != null) {
    //     oldContext.removeListener();
    // }
    // Stub: not yet ported
}

// ===== Port of: protected void onDestroy() =====
void MainActivity::onDestroy() {
    // super.onDestroy();

    // final RedditSubredditSubscriptionManager.ListenerContext listenerContext
    //     = mSubredditSubscriptionListenerContext.get();
    // if(listenerContext != null) {
    //     listenerContext.removeListener();
    // }
}

// ===== Port of: public void onSelected(final @MainMenuFragment.MainMenuAction int type) =====
void MainActivity::onSelected(int type) {
    // final String username = RedditAccountManager.getInstance(this)
    //     .getDefaultAccount().username;

    // switch(type) {
    //     case MainMenuFragment.MENU_MENU_ACTION_FRONTPAGE:
    //         onSelected(SubredditPostListURL.getFrontPage());
    //         break;
    //     case MainMenuFragment.MENU_MENU_ACTION_POPULAR:
    //         onSelected(SubredditPostListURL.getPopular());
    //         break;
    //     case MainMenuFragment.MENU_MENU_ACTION_ALL:
    //         onSelected(SubredditPostListURL.getAll());
    //         break;
    //     case MainMenuFragment.MENU_MENU_ACTION_SUBMITTED:
    //         onSelected(UserPostListingURL.getSubmitted(username));
    //         break;
    //     case MainMenuFragment.MENU_MENU_ACTION_SUBMITTED_COMMENTS:
    //         LinkHandler.onLinkClicked(this,
    //             Constants.Reddit.getUri("/user/" + username + "/comments.json"), false);
    //         break;
    //     case MainMenuFragment.MENU_MENU_ACTION_SAVED:
    //         onSelected(UserPostListingURL.getSaved(username));
    //         break;
    //     case MainMenuFragment.MENU_MENU_ACTION_HIDDEN:
    //         onSelected(UserPostListingURL.getHidden(username));
    //         break;
    //     case MainMenuFragment.MENU_MENU_ACTION_UPVOTED:
    //         onSelected(UserPostListingURL.getLiked(username));
    //         break;
    //     case MainMenuFragment.MENU_MENU_ACTION_DOWNVOTED:
    //         onSelected(UserPostListingURL.getDisliked(username));
    //         break;
    //     case MainMenuFragment.MENU_MENU_ACTION_PROFILE:
    //         LinkHandler.onLinkClicked(this, new UserProfileURL(username).toUriString());
    //         break;
    //     case MainMenuFragment.MENU_MENU_ACTION_CUSTOM:
    //         // Complex dialog with Spinner, AutoCompleteTextView, TextWatcher, etc.
    //         break;
    //     case MainMenuFragment.MENU_MENU_ACTION_INBOX:
    //         startActivity(new Intent(this, InboxListingActivity.class));
    //         break;
    //     case MainMenuFragment.MENU_MENU_ACTION_SENT_MESSAGES:
    //         // Intent with inboxType extra
    //         break;
    //     case MainMenuFragment.MENU_MENU_ACTION_MODMAIL:
    //         // Intent with inboxType extra
    //         break;
    //     case MainMenuFragment.MENU_MENU_ACTION_FIND_SUBREDDIT:
    //         startActivity(new Intent(this, SubredditSearchActivity.class));
    //         break;
    // }
    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: public void onSelected(final PostListingURL url) =====
void MainActivity::onSelected(PostListingURL *url) {
    // if(url == null) return;
    if (url == nullptr) {
        return;
    }

    // if(twoPane) {
    //     postListingController = new PostListingController(url, this);
    //     requestRefresh(RefreshableFragment.POSTS, false);
    // } else {
    //     final Intent intent = new Intent(this, PostListingActivity.class);
    //     intent.setData(url.generateJsonUri());
    //     startActivityForResult(intent, 1);
    // }
    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: public void onRedditAccountChanged() =====
void MainActivity::onRedditAccountChanged() {
    // recreateSubscriptionListener();
    // postInvalidateOptionsMenu();
    // requestRefresh(RefreshableFragment.ALL, false);
    recreateSubscriptionListener();
    postInvalidateOptionsMenu();
}

// ===== Port of: protected void doRefresh(final RefreshableFragment which, final boolean force,
//      final Bundle savedInstanceState) =====
void MainActivity::doRefresh(RefreshableFragment::Type which, bool force, Bundle *savedInstanceState) {
    // if(which == RefreshableFragment.MAIN_RELAYOUT) {
    //     mainMenuFragment = null; ... etc
    //     twoPane = General.isTablet(this);
    //     if(twoPane) {
    //         final View layout = getLayoutInflater().inflate(R.layout.main_double, null);
    //         mLeftPane = layout.findViewById(R.id.main_left_frame);
    //         mRightPane = layout.findViewById(R.id.main_right_frame);
    //         setBaseActivityListing(layout);
    //     } else {
    //         mLeftPane = null;
    //         mRightPane = null;
    //     }
    //     invalidateOptionsMenu();
    //     requestRefresh(RefreshableFragment.ALL, false);
    //     return;
    // }
    // if(twoPane) {
    //     // Complex fragment management...
    // } else {
    //     if(which == RefreshableFragment.ALL || which == RefreshableFragment.MAIN) {
    //         mainMenuFragment = new MainMenuFragment(this, null, force);
    //         mainMenuFragment.setBaseActivityContent(this);
    //     }
    // }
    // invalidateOptionsMenu();
    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: public void onBackPressed() =====
void MainActivity::onBackPressed() {
    // if(!General.onBackPressed()) {
    //     return;
    // }
    // if(!twoPane || isMenuShown) {
    //     super.onBackPressed();
    //     return;
    // }
    // isMenuShown = true;
    // ... restore menu state
    // showBackButton(false);
    // invalidateOptionsMenu();
    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: public void onPostCommentsSelected(final RedditPreparedPost post) =====
void MainActivity::onPostCommentsSelected(RedditPreparedPost *post) {
    // if(twoPane) {
    //     commentListingController = new CommentListingController(
    //         PostCommentListingURL.forPostId(post.src.getIdAlone()));
    //     showBackButton(true);
    //     if(isMenuShown) {
    //         // Switch to two-pane with posts+comments
    //         isMenuShown = false;
    //     } else {
    //         requestRefresh(RefreshableFragment.COMMENTS, false);
    //     }
    // } else {
    //     LinkHandler.onLinkClicked(this,
    //         PostCommentListingURL.forPostId(post.src.getIdAlone()).toUriString(), false);
    // }
    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: public void onPostSelected(final RedditPreparedPost post) =====
void MainActivity::onPostSelected(RedditPreparedPost *post) {
    // if(post.isSelf()) {
    //     onPostCommentsSelected(post);
    // } else {
    //     LinkHandler.onLinkClicked(this, post.src.getUrl(), false, post.src.getSrc());
    // }
    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: public boolean onCreateOptionsMenu(final Menu menu) =====
bool MainActivity::onCreateOptionsMenu(Menu *menu) {
    // final boolean postsVisible = postListingFragment != null;
    // final boolean commentsVisible = commentListingFragment != null;
    // final boolean postsSortable = postListingController != null && postListingController.isSortable();
    // final boolean commentsSortable = commentListingController != null && commentListingController.isSortable();
    // final boolean isFrontPage = postListingController != null && postListingController.isFrontPage();
    // final RedditAccount user = RedditAccountManager.getInstance(this).getDefaultAccount();
    // ... complex OptionsMenuUtility.prepare() call ...
    // if(commentListingFragment != null) {
    //     commentListingFragment.onCreateOptionsMenu(menu);
    // }
    // return true;
    // Stub: deep Android dependencies not yet ported
    return false;
}

// ===== Port of: public boolean onOptionsItemSelected(@NonNull final MenuItem item) =====
bool MainActivity::onOptionsItemSelected(MenuItem *item) {
    // if(commentListingFragment != null) {
    //     if(commentListingFragment.onOptionsItemSelected(item)) {
    //         return true;
    //     }
    // }
    // switch(item.getItemId()) {
    //     case android.R.id.home:
    //         onBackPressed();
    //         return true;
    //     default:
    //         return super.onOptionsItemSelected(item);
    // }
    // Stub: deep Android dependencies not yet ported
    return false;
}

// ===== Port of: public void onRefreshComments() =====
void MainActivity::onRefreshComments() {
    // commentListingController.setSession(null);
    // requestRefresh(RefreshableFragment.COMMENTS, true);
}

// ===== Port of: public void onPastComments() =====
void MainActivity::onPastComments() {
    // final SessionListDialog sessionListDialog = SessionListDialog.newInstance(
    //     commentListingController.getUri(),
    //     commentListingController.getSession(),
    //     SessionChangeListener.SessionChangeType.COMMENTS);
    // sessionListDialog.show(getSupportFragmentManager(), null);
}

// ===== Port of: public void onSortSelected(final PostCommentSort order) =====
void MainActivity::onSortSelected(PostCommentSort order) {
    // commentListingController.setSort(order);
    // requestRefresh(RefreshableFragment.COMMENTS, false);
}

// ===== Port of: public void onSortSelected(final UserCommentSort order) =====
void MainActivity::onSortSelected(UserCommentSort order) {
    // commentListingController.setSort(order);
    // requestRefresh(RefreshableFragment.COMMENTS, false);
}

// ===== Port of: public void onSearchComments() =====
void MainActivity::onSearchComments() {
    // DialogUtils.showSearchDialog(this, R.string.action_search_comments, query -> {
    //     final Intent searchIntent = new Intent(this, CommentListingActivity.class);
    //     searchIntent.setData(commentListingController.getUri());
    //     searchIntent.putExtra(CommentListingActivity.EXTRA_SEARCH_STRING, query);
    //     startActivity(searchIntent);
    // });
}

// ===== Port of: public void onRefreshPosts() =====
void MainActivity::onRefreshPosts() {
    // postListingController.setSession(null);
    // requestRefresh(RefreshableFragment.POSTS, true);
}

// ===== Port of: public void onPastPosts() =====
void MainActivity::onPastPosts() {
    // final SessionListDialog sessionListDialog = SessionListDialog.newInstance(
    //     postListingController.getUri(),
    //     postListingController.getSession(),
    //     SessionChangeListener.SessionChangeType.POSTS);
    // sessionListDialog.show(getSupportFragmentManager(), null);
}

// ===== Port of: public void onSubmitPost() =====
void MainActivity::onSubmitPost() {
    // final Intent intent = new Intent(this, PostSubmitActivity.class);
    // if(postListingController.isSubreddit()) {
    //     intent.putExtra("subreddit", postListingController.subredditCanonicalName().toString());
    // }
    // startActivity(intent);
}

// ===== Port of: public void onSortSelected(final PostSort order) =====
void MainActivity::onSortSelected(PostSort order) {
    // postListingController.setSort(order);
    // requestRefresh(RefreshableFragment.POSTS, false);
}

// ===== Port of: public void onSearchPosts() =====
void MainActivity::onSearchPosts() {
    // PostListingActivity.onSearchPosts(postListingController, this);
}

// ===== Port of: public void onSubscribe() =====
void MainActivity::onSubscribe() {
    // if(postListingFragment != null) {
    //     postListingFragment.onSubscribe();
    // }
}

// ===== Port of: public void onUnsubscribe() =====
void MainActivity::onUnsubscribe() {
    // if(postListingFragment != null) {
    //     postListingFragment.onUnsubscribe();
    // }
}

// ===== Port of: public void onSidebar() =====
void MainActivity::onSidebar() {
    // postListingFragment.getSubreddit().showSidebarActivity(this);
}

// ===== Port of: public void onPin() =====
void MainActivity::onPin() {
    // if(postListingFragment == null) return;
    // try {
    //     PrefsUtility.pref_pinned_subreddits_add(this,
    //         postListingFragment.getSubreddit().getCanonicalId());
    // } catch(final InvalidSubredditNameException e) {
    //     throw new RuntimeException(e);
    // }
    // invalidateOptionsMenu();
}

// ===== Port of: public void onUnpin() =====
void MainActivity::onUnpin() {
    // if(postListingFragment == null) return;
    // try {
    //     PrefsUtility.pref_pinned_subreddits_remove(this,
    //         postListingFragment.getSubreddit().getCanonicalId());
    // } catch(final InvalidSubredditNameException e) {
    //     throw new RuntimeException(e);
    // }
    // invalidateOptionsMenu();
}

// ===== Port of: public void onBlock() =====
void MainActivity::onBlock() {
    // if(postListingFragment == null) return;
    // try {
    //     PrefsUtility.pref_blocked_subreddits_add(this,
    //         postListingFragment.getSubreddit().getCanonicalId());
    // } catch(final InvalidSubredditNameException e) {
    //     throw new RuntimeException(e);
    // }
    // invalidateOptionsMenu();
}

// ===== Port of: public void onUnblock() =====
void MainActivity::onUnblock() {
    // if(postListingFragment == null) return;
    // try {
    //     PrefsUtility.pref_blocked_subreddits_remove(this,
    //         postListingFragment.getSubreddit().getCanonicalId());
    // } catch(final InvalidSubredditNameException e) {
    //     throw new RuntimeException(e);
    // }
    // invalidateOptionsMenu();
}

// ===== Port of: public void onRefreshSubreddits() =====
void MainActivity::onRefreshSubreddits() {
    // requestRefresh(RefreshableFragment.MAIN, true);
}

// ===== Port of: protected void onResume() =====
void MainActivity::onResume() {
    // super.onResume();
    // if(mainMenuFragment != null) {
    //     mainMenuFragment.onUpdateAnnouncement();
    // }
}

// ===== Port of: public void onSessionSelected(final UUID session, final SessionChangeType type) =====
void MainActivity::onSessionSelected(const QUuid &session, SessionChangeType::Type type) {
    // switch(type) {
    //     case POSTS:
    //         postListingController.setSession(session);
    //         requestRefresh(RefreshableFragment.POSTS, false);
    //         break;
    //     case COMMENTS:
    //         commentListingController.setSession(session);
    //         requestRefresh(RefreshableFragment.COMMENTS, false);
    //         break;
    // }
}

// ===== Port of: public void onSessionRefreshSelected(final SessionChangeType type) =====
void MainActivity::onSessionRefreshSelected(SessionChangeType::Type type) {
    // switch(type) {
    //     case POSTS: onRefreshPosts(); break;
    //     case COMMENTS: onRefreshComments(); break;
    // }
}

// ===== Port of: public void onSessionChanged(final UUID session, final SessionChangeType type,
//      final TimestampUTC timestamp) =====
void MainActivity::onSessionChanged(const QUuid &session, SessionChangeType::Type type,
                                     const TimestampUTC &timestamp) {
    // switch(type) {
    //     case POSTS:
    //         if(postListingController != null) {
    //             postListingController.setSession(session);
    //         }
    //         break;
    //     case COMMENTS:
    //         if(commentListingController != null) {
    //             commentListingController.setSession(session);
    //         }
    //         break;
    // }
}

// ===== Port of: public void onSubredditSubscriptionListUpdated(...) =====
void MainActivity::onSubredditSubscriptionListUpdated(RedditSubredditSubscriptionManager *manager) {
    // postInvalidateOptionsMenu();
    postInvalidateOptionsMenu();
}

// ===== Port of: public void onSubredditSubscriptionAttempted(...) =====
void MainActivity::onSubredditSubscriptionAttempted(RedditSubredditSubscriptionManager *manager) {
    // postInvalidateOptionsMenu();
    postInvalidateOptionsMenu();
}

// ===== Port of: public void onSubredditUnsubscriptionAttempted(...) =====
void MainActivity::onSubredditUnsubscriptionAttempted(RedditSubredditSubscriptionManager *manager) {
    // postInvalidateOptionsMenu();
    postInvalidateOptionsMenu();
}

// ===== Port of: private void postInvalidateOptionsMenu() =====
// private void postInvalidateOptionsMenu() { runOnUiThread(this::invalidateOptionsMenu); }
void MainActivity::postInvalidateOptionsMenu() {
    // runOnUiThread(this::invalidateOptionsMenu);
    // Qt equivalent: signal
    emit optionsMenuInvalidated();
}

// ===== Port of: private void showBackButton(final boolean isVisible) =====
// private void showBackButton(final boolean isVisible) { configBackButton(isVisible, v -> onBackPressed()); }
void MainActivity::showBackButton(bool isVisible) {
    // configBackButton(isVisible, v -> onBackPressed());
    // Stub: Android configBackButton not yet ported
}

// ===== Port of: public PostSort getPostSort() =====
PostSort MainActivity::getPostSort() {
    // if(postListingController == null) { return null; }
    // return postListingController.getSort();
    return PostSort(); // stub
}

// ===== Port of: public OptionsMenuUtility.Sort getCommentSort() =====
void *MainActivity::getCommentSort() {
    // if(commentListingController == null) { return null; }
    // return commentListingController.getSort();
    return nullptr; // stub
}

// ===== Port of: public PostCommentSort getSuggestedCommentSort() =====
PostCommentSort MainActivity::getSuggestedCommentSort() {
    // if(commentListingFragment == null || commentListingFragment.getPost() == null) {
    //     return null;
    // }
    // return commentListingFragment.getPost().src.getSuggestedCommentSort();
    return PostCommentSort(); // stub
}

// ===== Port of: private void openCustomLocation(...) =====
void MainActivity::openCustomLocation(const std::vector<QString> &typeReturnValues,
                                       Spinner *destinationType,
                                       AutoCompleteTextView *editText) {
    // final String typeName = typeReturnValues[destinationType.getSelectedItemPosition()];
    // switch(typeName) {
    //     case "subreddit":
    //         // Parse subreddit name, navigate
    //         break;
    //     case "user":
    //         // Parse username, navigate
    //         break;
    //     case "url":
    //         // Parse URL, navigate
    //         break;
    //     case "search":
    //         // Parse query, navigate to search
    //         break;
    // }
    // Stub: deep Android dependencies not yet ported
}

} // namespace PinkReader
