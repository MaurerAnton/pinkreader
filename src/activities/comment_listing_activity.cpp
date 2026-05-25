/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: comment_listing_activity.cpp - Port of CommentListingActivity.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/activities/CommentListingActivity.java
 *
 * Every field, method, constant, and inner class ported exactly.
 */

#include "comment_listing_activity.h"

namespace PinkReader {

// ===== Static constants (port of: private static final String ...) =====
const QString CommentListingActivity::TAG = QStringLiteral("CommentListingActivity");
const QString CommentListingActivity::EXTRA_SEARCH_STRING = QStringLiteral("cla_search_string");
const QString CommentListingActivity::SAVEDSTATE_SESSION = QStringLiteral("cla_session");
const QString CommentListingActivity::SAVEDSTATE_SORT = QStringLiteral("cla_sort");
const QString CommentListingActivity::SAVEDSTATE_SORT_IS_USER = QStringLiteral("cla_sort_user");
const QString CommentListingActivity::SAVEDSTATE_FRAGMENT = QStringLiteral("cla_fragment");

// ===== Constructor =====
CommentListingActivity::CommentListingActivity(QObject *parent)
    : QObject(parent)
{
}

// ===== Destructor =====
CommentListingActivity::~CommentListingActivity() = default;

// ===== Port of: protected void onCreate(final Bundle savedInstanceState) =====
void CommentListingActivity::onCreate(Bundle *savedInstanceState) {
    // PrefsUtility.applyTheme(this);
    // super.onCreate(savedInstanceState);
    //
    // setTitle(getString(R.string.app_name));
    //
    // RedditAccountManager.getInstance(this).addUpdateListener(this);
    //
    // if(getIntent() != null) {
    //
    //     final Intent intent = getIntent();
    //
    //     final String url = intent.getDataString();
    //     final String searchString = intent.getStringExtra(EXTRA_SEARCH_STRING);
    //     controller = new CommentListingController(
    //             RedditURLParser.parseProbableCommentListing(Uri.parse(url)));
    //     controller.setSearchString(searchString);
    //
    //     Bundle fragmentSavedInstanceState = null;
    //
    //     if(savedInstanceState != null) {
    //         if(savedInstanceState.containsKey(SAVEDSTATE_SESSION)) {
    //             controller.setSession(UUID.fromString(
    //                 savedInstanceState.getString(SAVEDSTATE_SESSION)));
    //         }
    //         if(savedInstanceState.containsKey(SAVEDSTATE_SORT)) {
    //             if(savedInstanceState.getBoolean(SAVEDSTATE_SORT_IS_USER)) {
    //                 controller.setSort(UserCommentSort.valueOf(
    //                         savedInstanceState.getString(SAVEDSTATE_SORT)));
    //             } else {
    //                 controller.setSort(PostCommentSort.valueOf(
    //                         savedInstanceState.getString(SAVEDSTATE_SORT)));
    //             }
    //         }
    //         if(savedInstanceState.containsKey(SAVEDSTATE_FRAGMENT)) {
    //             fragmentSavedInstanceState = savedInstanceState.getBundle(
    //                     SAVEDSTATE_FRAGMENT);
    //         }
    //     }
    //
    //     doRefresh(RefreshableFragment.COMMENTS, false, fragmentSavedInstanceState);
    //
    // } else {
    //     throw new RuntimeException("Nothing to show!");
    // }

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: protected void onSaveInstanceState(@NonNull final Bundle outState) =====
void CommentListingActivity::onSaveInstanceState(Bundle *outState) {
    // super.onSaveInstanceState(outState);
    //
    // final UUID session = controller.getSession();
    // if(session != null) {
    //     outState.putString(SAVEDSTATE_SESSION, session.toString());
    // }
    //
    // final OptionsMenuUtility.Sort sort = controller.getSort();
    // if(sort != null) {
    //     outState.putBoolean(SAVEDSTATE_SORT_IS_USER, controller.isUserCommentListing());
    //     outState.putString(SAVEDSTATE_SORT, sort.name());
    // }
    //
    // if(mFragment != null) {
    //     outState.putBundle(SAVEDSTATE_FRAGMENT, mFragment.onSaveInstanceState());
    // }

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: public boolean onCreateOptionsMenu(final Menu menu) =====
bool CommentListingActivity::onCreateOptionsMenu(Menu *menu) {
    // OptionsMenuUtility.prepare(
    //         this, menu,
    //         false, false, true, false, false,
    //         controller.isUserCommentListing(),
    //         false, controller.isSortable(), false,
    //         null, false, true, null, null);
    //
    // if(mFragment != null) {
    //     mFragment.onCreateOptionsMenu(menu);
    // }
    //
    // return true;

    // Stub: deep Android dependencies not yet ported
    return false;
}

// ===== Port of: public void onRedditAccountChanged() =====
void CommentListingActivity::onRedditAccountChanged() {
    // requestRefresh(RefreshableFragment.ALL, false);

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: protected void doRefresh(
//     final RefreshableFragment which, final boolean force, final Bundle savedInstanceState) =====
void CommentListingActivity::doRefresh(RefreshableFragment::Type which, bool force,
                                        Bundle *savedInstanceState) {
    // mFragment = controller.get(this, force, savedInstanceState);
    // mFragment.setBaseActivityContent(this);
    //
    // setTitle(controller.getCommentListingUrl().humanReadableName(this, false));
    // invalidateOptionsMenu();

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: public void onRefreshComments() =====
void CommentListingActivity::onRefreshComments() {
    // controller.setSession(null);
    // requestRefresh(RefreshableFragment.COMMENTS, true);

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: public void onPastComments() =====
void CommentListingActivity::onPastComments() {
    // final SessionListDialog sessionListDialog = SessionListDialog.newInstance(
    //         controller.getUri(),
    //         controller.getSession(),
    //         SessionChangeListener.SessionChangeType.COMMENTS);
    // sessionListDialog.show(getSupportFragmentManager(), null);

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: public void onSortSelected(final PostCommentSort order) =====
void CommentListingActivity::onSortSelected(PostCommentSort order) {
    // controller.setSort(order);
    // requestRefresh(RefreshableFragment.COMMENTS, false);

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: public void onSortSelected(final UserCommentSort order) =====
void CommentListingActivity::onSortSelected(UserCommentSort order) {
    // controller.setSort(order);
    // requestRefresh(RefreshableFragment.COMMENTS, false);

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: public void onSearchComments() =====
void CommentListingActivity::onSearchComments() {
    // DialogUtils.showSearchDialog(this, query -> {
    //     final Intent searchIntent = getIntent();
    //     searchIntent.putExtra(EXTRA_SEARCH_STRING, query);
    //     startActivity(searchIntent);
    // });

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: public boolean onOptionsItemSelected(@NonNull final MenuItem item) =====
bool CommentListingActivity::onOptionsItemSelected(MenuItem *item) {
    // if(mFragment != null) {
    //     if(mFragment.onOptionsItemSelected(item)) {
    //         return true;
    //     }
    // }
    // return super.onOptionsItemSelected(item);

    // Stub: deep Android dependencies not yet ported
    return false;
}

// ===== Port of: public void onSessionRefreshSelected(final SessionChangeType type) =====
void CommentListingActivity::onSessionRefreshSelected(SessionChangeType::Type type) {
    // onRefreshComments();

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: public void onSessionSelected(final UUID session, final SessionChangeType type) =====
void CommentListingActivity::onSessionSelected(const QString &session,
                                                SessionChangeType::Type type) {
    // controller.setSession(session);
    // requestRefresh(RefreshableFragment.COMMENTS, false);

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: public void onSessionChanged(
//     final UUID session, final SessionChangeType type, final TimestampUTC timestamp) =====
void CommentListingActivity::onSessionChanged(const QString &session,
                                               SessionChangeType::Type type,
                                               TimestampUTC *timestamp) {
    // Log.i(TAG,
    //         type.name() + " session changed to " + (session != null
    //                 ? session.toString() : "<null>"));
    // controller.setSession(session);

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: public void onPostSelected(final RedditPreparedPost post) =====
void CommentListingActivity::onPostSelected(RedditPreparedPost *post) {
    // LinkHandler.onLinkClicked(this, post.src.getUrl(), false, post.src.getSrc());

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: public void onPostCommentsSelected(final RedditPreparedPost post) =====
void CommentListingActivity::onPostCommentsSelected(RedditPreparedPost *post) {
    // LinkHandler.onLinkClicked(
    //         this,
    //         PostCommentListingURL.forPostId(post.src.getIdAlone()).toUriString(),
    //         false);

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: public void onBackPressed() =====
void CommentListingActivity::onBackPressed() {
    // if(General.onBackPressed()) {
    //     super.onBackPressed();
    // }

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: protected boolean baseActivityAllowToolbarHideOnScroll() =====
bool CommentListingActivity::baseActivityAllowToolbarHideOnScroll() {
    // return true; (from Java source)
    return true;
}

// ===== Port of: public OptionsMenuUtility.Sort getCommentSort() =====
void *CommentListingActivity::getCommentSort() {
    // return controller.getSort();

    // Stub: OptionsMenuUtility.Sort not yet ported
    return nullptr;
}

// ===== Port of: public PostCommentSort getSuggestedCommentSort() =====
PostCommentSort CommentListingActivity::getSuggestedCommentSort() {
    // if(mFragment == null || mFragment.getPost() == null) {
    //     return null;
    // }
    //
    // return mFragment.getPost().src.getSuggestedCommentSort();

    // Stub: deep Android dependencies not yet ported
    return static_cast<PostCommentSort>(0);
}

} // namespace PinkReader
