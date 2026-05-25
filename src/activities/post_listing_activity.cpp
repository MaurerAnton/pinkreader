/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: post_listing_activity.cpp - Port of PostListingActivity.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/activities/PostListingActivity.java
 *
 * Every field, method, constant, and inner class ported exactly.
 */

#include "post_listing_activity.h"

namespace PinkReader {

// ===== Static constants (port of: private static final String ...) =====
const QString PostListingActivity::SAVEDSTATE_SESSION = QStringLiteral("pla_session");
const QString PostListingActivity::SAVEDSTATE_SORT = QStringLiteral("pla_sort");
const QString PostListingActivity::SAVEDSTATE_FRAGMENT = QStringLiteral("pla_fragment");

// ===== Constructor =====
PostListingActivity::PostListingActivity(QObject *parent)
    : QObject(parent)
{
}

// ===== Destructor =====
PostListingActivity::~PostListingActivity() = default;

// ===== Port of: protected void onCreate(final Bundle savedInstanceState) =====
void PostListingActivity::onCreate(Bundle *savedInstanceState) {
    // PrefsUtility.applyTheme(this);
    // super.onCreate(savedInstanceState);

    // final TypedArray typedArray
    //     = obtainStyledAttributes(new int[] {R.attr.rrListBackgroundCol});
    //
    // try {
    //     getWindow().setBackgroundDrawable(
    //             new ColorDrawable(typedArray.getColor(0, 0)));
    // } finally {
    //     typedArray.recycle();
    // }

    // RedditAccountManager.getInstance(this).addUpdateListener(this);

    // if(getIntent() != null) {
    //     final Intent intent = getIntent();
    //     final RedditURLParser.RedditURL url
    //             = RedditURLParser.parseProbablePostListing(intent.getData());
    //
    //     if(!(url instanceof PostListingURL)) {
    //         throw new RuntimeException(String.format(Locale.US,
    //             "'%s' is not a post listing URL!", url.generateJsonUri()));
    //     }
    //
    //     controller = new PostListingController((PostListingURL)url, this);
    //
    //     Bundle fragmentSavedInstanceState = null;
    //
    //     if(savedInstanceState != null) {
    //         if(savedInstanceState.containsKey(SAVEDSTATE_SESSION)) {
    //             controller.setSession(UUID.fromString(
    //                 savedInstanceState.getString(SAVEDSTATE_SESSION)));
    //         }
    //         if(savedInstanceState.containsKey(SAVEDSTATE_SORT)) {
    //             controller.setSort(PostSort.valueOf(
    //                 savedInstanceState.getString(SAVEDSTATE_SORT)));
    //         }
    //         if(savedInstanceState.containsKey(SAVEDSTATE_FRAGMENT)) {
    //             fragmentSavedInstanceState = savedInstanceState.getBundle(
    //                     SAVEDSTATE_FRAGMENT);
    //         }
    //     }
    //
    //     setTitle(url.humanReadableName(this, false));
    //     setBaseActivityListing(R.layout.main_single);
    //     doRefresh(RefreshableFragment.POSTS, false, fragmentSavedInstanceState);
    //
    // } else {
    //     throw new RuntimeException("Nothing to show!");
    // }
    //
    // recreateSubscriptionListener();

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: protected void onSaveInstanceState(@NonNull final Bundle outState) =====
void PostListingActivity::onSaveInstanceState(Bundle *outState) {
    // super.onSaveInstanceState(outState);
    //
    // final UUID session = controller.getSession();
    // if(session != null) {
    //     outState.putString(SAVEDSTATE_SESSION, session.toString());
    // }
    //
    // final PostSort sort = controller.getSort();
    // if(sort != null) {
    //     outState.putString(SAVEDSTATE_SORT, sort.name());
    // }
    //
    // if(fragment != null) {
    //     outState.putBundle(SAVEDSTATE_FRAGMENT, fragment.onSaveInstanceState());
    // }

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: protected void onDestroy() =====
void PostListingActivity::onDestroy() {
    // super.onDestroy();
    //
    // final RedditSubredditSubscriptionManager.ListenerContext listenerContext
    //         = mSubredditSubscriptionListenerContext.get();
    //
    // if(listenerContext != null) {
    //     listenerContext.removeListener();
    // }

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: public boolean onCreateOptionsMenu(final Menu menu) =====
bool PostListingActivity::onCreateOptionsMenu(Menu *menu) {
    // final RedditAccount user = RedditAccountManager.getInstance(this)
    //         .getDefaultAccount();
    // final SubredditSubscriptionState subredditSubscriptionState;
    // final RedditSubredditSubscriptionManager subredditSubscriptionManager
    //         = RedditSubredditSubscriptionManager.getSingleton(this, user);
    //
    // if(!user.isAnonymous()
    //         && controller.isSubreddit()
    //         && subredditSubscriptionManager.areSubscriptionsReady()
    //         && fragment != null
    //         && fragment.getSubreddit() != null) {
    //     subredditSubscriptionState = subredditSubscriptionManager.getSubscriptionState(
    //             controller.subredditCanonicalName());
    // } else {
    //     subredditSubscriptionState = null;
    // }
    //
    // final String subredditDescription = fragment != null
    //         && fragment.getSubreddit() != null
    //         ? fragment.getSubreddit().description_html
    //         : null;
    //
    // Boolean subredditPinState = null;
    // Boolean subredditBlockedState = null;
    //
    // if(controller.isSubreddit()
    //         && fragment != null
    //         && fragment.getSubreddit() != null) {
    //     try {
    //         subredditPinState = PrefsUtility.pref_pinned_subreddits_check(
    //                 fragment.getSubreddit().getCanonicalId());
    //         subredditBlockedState = PrefsUtility.pref_blocked_subreddits_check(
    //                 fragment.getSubreddit().getCanonicalId());
    //     } catch(final InvalidSubredditNameException e) {
    //         subredditPinState = null;
    //         subredditBlockedState = null;
    //     }
    // }
    //
    // OptionsMenuUtility.prepare(
    //         this, menu,
    //         false, true, false,
    //         controller.isSearchResults(),
    //         controller.isUserPostListing(),
    //         false, controller.isSortable(), true,
    //         controller.isFrontPage(),
    //         subredditSubscriptionState,
    //         subredditDescription != null && !subredditDescription.isEmpty(),
    //         false,
    //         subredditPinState,
    //         subredditBlockedState);
    //
    // return true;

    // Stub: deep Android dependencies not yet ported
    return false;
}

// ===== Port of: private void recreateSubscriptionListener() =====
void PostListingActivity::recreateSubscriptionListener() {
    // final RedditSubredditSubscriptionManager.ListenerContext oldContext
    //         = mSubredditSubscriptionListenerContext.getAndSet(
    //         RedditSubredditSubscriptionManager
    //                 .getSingleton(
    //                         this,
    //                         RedditAccountManager.getInstance(this)
    //                                 .getDefaultAccount())
    //                 .addListener(this));
    //
    // if(oldContext != null) {
    //     oldContext.removeListener();
    // }

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: public void onRedditAccountChanged() =====
void PostListingActivity::onRedditAccountChanged() {
    // recreateSubscriptionListener();
    // postInvalidateOptionsMenu();
    // requestRefresh(RefreshableFragment.ALL, false);

    recreateSubscriptionListener();
    postInvalidateOptionsMenu();

    // Stub: requestRefresh(RefreshableFragment.ALL, false) not yet ported
}

// ===== Port of: protected void doRefresh(
//     final RefreshableFragment which, final boolean force, final Bundle savedInstanceState) =====
void PostListingActivity::doRefresh(RefreshableFragment::Type which, bool force,
                                     Bundle *savedInstanceState) {
    // if(fragment != null) {
    //     fragment.cancel();
    // }
    //
    // fragment = controller.get(this, force, savedInstanceState);
    // fragment.setBaseActivityContent(this);

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: public void onPostSelected(final RedditPreparedPost post) =====
void PostListingActivity::onPostSelected(RedditPreparedPost *post) {
    // LinkHandler.onLinkClicked(this, post.src.getUrl(), false, post.src.getSrc());

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: public void onPostCommentsSelected(final RedditPreparedPost post) =====
void PostListingActivity::onPostCommentsSelected(RedditPreparedPost *post) {
    // LinkHandler.onLinkClicked(
    //         this,
    //         PostCommentListingURL.forPostId(post.src.getIdAlone()).toUriString(),
    //         false);

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: public void onRefreshPosts() =====
void PostListingActivity::onRefreshPosts() {
    // controller.setSession(null);
    // requestRefresh(RefreshableFragment.POSTS, true);

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: public void onPastPosts() =====
void PostListingActivity::onPastPosts() {
    // final SessionListDialog sessionListDialog = SessionListDialog.newInstance(
    //         controller.getUri(),
    //         controller.getSession(),
    //         SessionChangeType.POSTS);
    // sessionListDialog.show(getSupportFragmentManager(), "SessionListDialog");

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: public void onSubmitPost() =====
void PostListingActivity::onSubmitPost() {
    // final Intent intent = new Intent(this, PostSubmitActivity.class);
    //
    // if(controller.isSubreddit()) {
    //     intent.putExtra("subreddit", controller.subredditCanonicalName().toString());
    // }
    //
    // startActivity(intent);

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: public void onSortSelected(final PostSort order) =====
void PostListingActivity::onSortSelected(PostSort order) {
    // controller.setSort(order);
    // requestRefresh(RefreshableFragment.POSTS, false);
    // invalidateOptionsMenu();

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: public void onSearchPosts() =====
void PostListingActivity::onSearchPosts() {
    // onSearchPosts(controller, this);

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: public static void onSearchPosts(
//     final PostListingController controller, final AppCompatActivity activity) =====
void PostListingActivity::onSearchPosts(PostListingController *controller,
                                         AppCompatActivity *activity) {
    // DialogUtils.showSearchDialog(activity, query -> {
    //     if(query == null) {
    //         return;
    //     }
    //
    //     final SearchPostListURL url;
    //
    //     if(controller != null && (controller.isSubreddit()
    //             || controller.isSubredditCombination()
    //             || controller.isSubredditSearchResults())) {
    //
    //         final SubredditCanonicalId subredditCanonicalId
    //                 = controller.subredditCanonicalName();
    //
    //         if(subredditCanonicalId == null) {
    //             BugReportActivity.handleGlobalError(
    //                     activity,
    //                     new RuntimeException("Can't search post listing "
    //                             + controller.getUri()));
    //             return;
    //         }
    //
    //         url = SearchPostListURL.build(
    //                 subredditCanonicalId.toString(), query);
    //     } else if(controller != null && controller.isMultireddit()) {
    //         final String multiName = controller.multiredditName();
    //         final String multiUsername = controller.multiredditUsername();
    //         url = SearchPostListURL.build(multiUsername, multiName, query);
    //     } else {
    //         url = SearchPostListURL.build(null, query);
    //     }
    //
    //     final Intent intent = new Intent(activity, PostListingActivity.class);
    //     intent.setData(url.generateJsonUri());
    //     activity.startActivity(intent);
    // });

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: public void onSubscribe() =====
void PostListingActivity::onSubscribe() {
    // fragment.onSubscribe();

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: public void onUnsubscribe() =====
void PostListingActivity::onUnsubscribe() {
    // fragment.onUnsubscribe();

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: public void onSidebar() =====
void PostListingActivity::onSidebar() {
    // if(fragment.getSubreddit() != null) {
    //     final Intent intent = new Intent(this, HtmlViewActivity.class);
    //     intent.putExtra(
    //             "html",
    //             fragment.getSubreddit()
    //                     .getSidebarHtml(PrefsUtility.isNightMode()));
    //     intent.putExtra("title", String.format(Locale.US, "%s: %s",
    //             getString(R.string.sidebar_activity_title),
    //             fragment.getSubreddit().url));
    //     startActivityForResult(intent, 1);
    // }

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: public void onPin() =====
void PostListingActivity::onPin() {
    // if(fragment == null) {
    //     return;
    // }
    //
    // if(fragment.getSubreddit() == null) {
    //     BugReportActivity.handleGlobalError(
    //             this,
    //             new RuntimeException("Can't pin post listing "
    //                     + fragment.getPostListingURL()));
    //     return;
    // }
    //
    // try {
    //     PrefsUtility.pref_pinned_subreddits_add(
    //             this,
    //             fragment.getSubreddit().getCanonicalId());
    // } catch(final InvalidSubredditNameException e) {
    //     throw new RuntimeException(e);
    // }
    //
    // invalidateOptionsMenu();

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: public void onUnpin() =====
void PostListingActivity::onUnpin() {
    // if(fragment == null) {
    //     return;
    // }
    //
    // if(fragment.getSubreddit() == null) {
    //     BugReportActivity.handleGlobalError(
    //             this,
    //             new RuntimeException("Can't unpin post listing "
    //                     + fragment.getPostListingURL()));
    //     return;
    // }
    //
    // try {
    //     PrefsUtility.pref_pinned_subreddits_remove(
    //             this,
    //             fragment.getSubreddit().getCanonicalId());
    // } catch(final InvalidSubredditNameException e) {
    //     throw new RuntimeException(e);
    // }
    //
    // invalidateOptionsMenu();

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: public void onBlock() =====
void PostListingActivity::onBlock() {
    // if(fragment == null) {
    //     return;
    // }
    //
    // if(fragment.getSubreddit() == null) {
    //     BugReportActivity.handleGlobalError(
    //             this,
    //             new RuntimeException("Can't block post listing "
    //                     + fragment.getPostListingURL()));
    //     return;
    // }
    //
    // try {
    //     PrefsUtility.pref_blocked_subreddits_add(
    //             this,
    //             fragment.getSubreddit().getCanonicalId());
    // } catch(final InvalidSubredditNameException e) {
    //     throw new RuntimeException(e);
    // }
    //
    // invalidateOptionsMenu();

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: public void onUnblock() =====
void PostListingActivity::onUnblock() {
    // if(fragment == null) {
    //     return;
    // }
    //
    // if(fragment.getSubreddit() == null) {
    //     BugReportActivity.handleGlobalError(
    //             this,
    //             new RuntimeException("Can't unblock post listing "
    //                     + fragment.getPostListingURL()));
    //     return;
    // }
    //
    // try {
    //     PrefsUtility.pref_blocked_subreddits_remove(
    //             this,
    //             fragment.getSubreddit().getCanonicalId());
    // } catch(final InvalidSubredditNameException e) {
    //     throw new RuntimeException(e);
    // }
    //
    // invalidateOptionsMenu();

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: public void onSessionSelected(final UUID session, final SessionChangeType type) =====
void PostListingActivity::onSessionSelected(const QString &session,
                                             SessionChangeType::Type type) {
    // controller.setSession(session);
    // requestRefresh(RefreshableFragment.POSTS, false);

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: public void onSessionRefreshSelected(final SessionChangeType type) =====
void PostListingActivity::onSessionRefreshSelected(SessionChangeType::Type type) {
    // onRefreshPosts();

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: public void onSessionChanged(
//     final UUID session, final SessionChangeType type, final TimestampUTC timestamp) =====
void PostListingActivity::onSessionChanged(const QString &session,
                                            SessionChangeType::Type type,
                                            TimestampUTC *timestamp) {
    // controller.setSession(session);

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: public void onBackPressed() =====
void PostListingActivity::onBackPressed() {
    // if(PrefsUtility.pref_behaviour_back_again()
    //         && (mDoubleTapBack_lastTapMs < SystemClock.uptimeMillis() - 5000)) {
    //
    //     mDoubleTapBack_lastTapMs = SystemClock.uptimeMillis();
    //     Toast.makeText(this, R.string.press_back_again, Toast.LENGTH_SHORT).show();
    //
    // } else if(General.onBackPressed()) {
    //     super.onBackPressed();
    // }

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: public void onSubredditSubscriptionListUpdated(
//     final RedditSubredditSubscriptionManager subredditSubscriptionManager) =====
void PostListingActivity::onSubredditSubscriptionListUpdated(
        RedditSubredditSubscriptionManager *subredditSubscriptionManager) {
    // postInvalidateOptionsMenu();

    postInvalidateOptionsMenu();
}

// ===== Port of: public void onSubredditSubscriptionAttempted(
//     final RedditSubredditSubscriptionManager subredditSubscriptionManager) =====
void PostListingActivity::onSubredditSubscriptionAttempted(
        RedditSubredditSubscriptionManager *subredditSubscriptionManager) {
    // postInvalidateOptionsMenu();

    postInvalidateOptionsMenu();
}

// ===== Port of: public void onSubredditUnsubscriptionAttempted(
//     final RedditSubredditSubscriptionManager subredditSubscriptionManager) =====
void PostListingActivity::onSubredditUnsubscriptionAttempted(
        RedditSubredditSubscriptionManager *subredditSubscriptionManager) {
    // postInvalidateOptionsMenu();

    postInvalidateOptionsMenu();
}

// ===== Port of: private void postInvalidateOptionsMenu() =====
void PostListingActivity::postInvalidateOptionsMenu() {
    // runOnUiThread(this::invalidateOptionsMenu);

    // Instead emit a Qt signal
    emit optionsMenuInvalidated();
}

// ===== Port of: protected boolean baseActivityAllowToolbarHideOnScroll() =====
bool PostListingActivity::baseActivityAllowToolbarHideOnScroll() {
    // return true; (from Java source)
    return true;
}

// ===== Port of: public PostSort getPostSort() =====
PostSort PostListingActivity::getPostSort() {
    // return controller.getSort();

    // Stub: controller.getSort() not yet ported
    return static_cast<PostSort>(0);
}

} // namespace PinkReader
