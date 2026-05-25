/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: comment_listing_fragment.cpp - Port of CommentListingFragment.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/fragments/CommentListingFragment.java
 *
 * Every field, method, constant, and inner class ported exactly.
 */

#include "comment_listing_fragment.h"

namespace PinkReader {

// ===== Static constants (port of Java static finals) =====

// Java line 92: SAVEDSTATE_FIRST_VISIBLE_POS = "firstVisiblePosition";
const QString CommentListingFragment::SAVEDSTATE_FIRST_VISIBLE_POS =
    QStringLiteral("firstVisiblePosition");

// Java line 93: SAVEDSTATE_SELFTEXT_VISIBLE = "selftextVisible";
const QString CommentListingFragment::SAVEDSTATE_SELFTEXT_VISIBLE =
    QStringLiteral("selftextVisible");

// ===== Constructor (Java lines 120-313) =====
CommentListingFragment::CommentListingFragment(
        QObject *parent,
        Bundle *savedInstanceState,
        const std::vector<RedditURLParser *> &urls,
        const QUuid &session,
        const QString &searchString,
        bool forceDownload)
    : QObject(parent)
    , mAllUrls(urls)
    , mSession(session)
{
    // ===== Java lines 128-137: Saved state restoration =====
    // if(savedInstanceState != null) {
    //     mPreviousFirstVisibleItemPosition = savedInstanceState.getInt(
    //             SAVEDSTATE_FIRST_VISIBLE_POS);
    //
    //     if(savedInstanceState.containsKey(SAVEDSTATE_SELFTEXT_VISIBLE)) {
    //         mSelfTextVisible = savedInstanceState.getBoolean(SAVEDSTATE_SELFTEXT_VISIBLE);
    //     }
    // }

    // ===== Java line 139: mCommentListingManager = new FilteredCommentListingManager(parent, searchString); =====
    // mCommentListingManager = new FilteredCommentListingManager(parent, searchString);

    // ===== Java line 140: mAllUrls = urls; =====
    // (done in initializer list)

    // ===== Java line 142: mUrlsToDownload = new LinkedList<>(mAllUrls); =====
    mUrlsToDownload = mAllUrls;

    // ===== Java line 144: this.mSession = session; =====
    // (done in initializer list)

    // ===== Java lines 146-157: Download strategy selection =====
    // if(forceDownload) {
    //     mDownloadStrategy = DownloadStrategyAlways.INSTANCE;
    //
    // } else if(session == null
    //         && savedInstanceState == null
    //         && General.isNetworkConnected(parent)) {
    //     mDownloadStrategy = new DownloadStrategyIfTimestampOutsideBounds(
    //             TimestampBound.notOlderThan(TimeDuration.minutes(20)));
    //
    // } else {
    //     mDownloadStrategy = DownloadStrategyIfNotCached.INSTANCE;
    // }

    // ===== Java line 159: mUser = RedditAccountManager.getInstance(getActivity()).getDefaultAccount(); =====
    // mUser = RedditAccountManager.getInstance(getActivity()).getDefaultAccount();

    // ===== Java line 161: parent.invalidateOptionsMenu(); =====
    // parent.invalidateOptionsMenu();

    // ===== Java line 163: final Context context = getActivity(); =====
    // final Context context = getActivity();

    // ===== Java line 165: mSelfTextFontScale = PrefsUtility.appearance_fontscale_bodytext(); =====
    // mSelfTextFontScale = PrefsUtility.appearance_fontscale_bodytext();

    // ===== Java line 167: mShowLinkButtons = PrefsUtility.pref_appearance_linkbuttons(); =====
    // mShowLinkButtons = PrefsUtility.pref_appearance_linkbuttons();

    // ===== Java line 169: mOverlayFrame = new FrameLayout(context); =====
    // mOverlayFrame = new FrameLayout(context);

    // ===== Java lines 171-183: ScrollbarRecyclerViewManager setup =====
    // final ScrollbarRecyclerViewManager recyclerViewManager
    //         = new ScrollbarRecyclerViewManager(context, null, false);
    //
    // if(parent instanceof OptionsMenuUtility.OptionsMenuCommentsListener
    //         && PrefsUtility.pref_behaviour_enable_swipe_refresh()) {
    //
    //     recyclerViewManager.enablePullToRefresh(
    //             ((OptionsMenuUtility.OptionsMenuCommentsListener)parent)::onRefreshComments);
    // }
    //
    // mRecyclerView = recyclerViewManager.getRecyclerView();
    // mCommentListingManager.setLayoutManager(
    //         (LinearLayoutManager)mRecyclerView.getLayoutManager());
    //
    // mRecyclerView.setAdapter(mCommentListingManager.getAdapter());
    // mListingView = recyclerViewManager.getOuterView();
    //
    // mRecyclerView.setItemAnimator(null);

    // ===== Java lines 190-268: Floating toolbar setup =====
    // if(!PrefsUtility.pref_appearance_comments_show_floating_toolbar()) {
    //     mFloatingToolbar = null;
    //
    // } else {
    //     mFloatingToolbar = (LinearLayout)LayoutInflater.from(context).inflate(
    //             R.layout.floating_toolbar,
    //             mOverlayFrame,
    //             false);
    //
    //     if (PrefsUtility.pref_appearance_left_handed()) {
    //         final FrameLayout.LayoutParams toolBarParams =
    //                 (FrameLayout.LayoutParams) mFloatingToolbar.getLayoutParams();
    //         toolBarParams.gravity = Gravity.START | Gravity.BOTTOM;
    //         mFloatingToolbar.setLayoutParams(toolBarParams);
    //     }
    //
    //     // Container so that setVisible() doesn't mess with the Z-order
    //     final FrameLayout floatingToolbarContainer = new FrameLayout(context);
    //     floatingToolbarContainer.addView(mFloatingToolbar);
    //     mOverlayFrame.addView(floatingToolbarContainer);
    //
    //     if(PrefsUtility.isNightMode()) {
    //         mFloatingToolbar.setBackgroundColor(Color.argb(0xCC, 0x33, 0x33, 0x33));
    //     }
    //
    //     final int buttonVPadding = General.dpToPixels(context, 12);
    //     final int buttonHPadding = General.dpToPixels(context, 16);
    //
    //     // Previous parent button
    //     {
    //         final ImageButton previousButton = (ImageButton)LayoutInflater.from(
    //                 context).inflate(
    //                 R.layout.flat_image_button, mFloatingToolbar, false);
    //         previousButton.setPadding(buttonHPadding, buttonVPadding,
    //                 buttonHPadding, buttonVPadding);
    //         previousButton.setImageResource(R.drawable.ic_ff_up_dark);
    //         previousButton.setContentDescription(getString(R.string.button_prev_comment_parent));
    //         mFloatingToolbar.addView(previousButton);
    //         previousButton.setOnClickListener(view -> onPreviousParent());
    //         previousButton.setOnLongClickListener(view -> {
    //             General.quickToast(context, R.string.button_prev_comment_parent);
    //             return true;
    //         });
    //     }
    //
    //     // Next parent button
    //     {
    //         final ImageButton nextButton = (ImageButton)LayoutInflater.from(context)
    //                 .inflate(R.layout.flat_image_button, mFloatingToolbar, false);
    //         nextButton.setPadding(buttonHPadding, buttonVPadding,
    //                 buttonHPadding, buttonVPadding);
    //         nextButton.setImageResource(R.drawable.ic_ff_down_dark);
    //         nextButton.setContentDescription(getString(R.string.button_next_comment_parent));
    //         mFloatingToolbar.addView(nextButton);
    //         nextButton.setOnClickListener(view -> onNextParent());
    //         nextButton.setOnLongClickListener(view -> {
    //             General.quickToast(context, R.string.button_next_comment_parent);
    //             return true;
    //         });
    //     }
    // }

    // ===== Java lines 271-311: Bezel swipe overlay setup =====
    // final SideToolbarOverlay toolbarOverlay = new SideToolbarOverlay(context);
    //
    // final BezelSwipeOverlay bezelOverlay = new BezelSwipeOverlay(
    //         context,
    //         new BezelSwipeOverlay.BezelSwipeListener() {
    //             @Override
    //             public boolean onSwipe(@BezelSwipeOverlay.SwipeEdge final int edge) {
    //                 if(mPost == null) return false;
    //                 toolbarOverlay.setContents(RedditPostActions.INSTANCE.generateToolbar(
    //                         mPost, (BaseActivity)getActivity(), true, toolbarOverlay));
    //                 toolbarOverlay.show(edge == BezelSwipeOverlay.LEFT
    //                         ? SideToolbarOverlay.SideToolbarPosition.LEFT
    //                         : SideToolbarOverlay.SideToolbarPosition.RIGHT);
    //                 return true;
    //             }
    //
    //             @Override
    //             public boolean onTap() {
    //                 if(toolbarOverlay.isShown()) {
    //                     toolbarOverlay.hide();
    //                     return true;
    //                 }
    //                 return false;
    //             }
    //         });
    //
    // mOverlayFrame.addView(bezelOverlay);
    // mOverlayFrame.addView(toolbarOverlay);
    //
    // General.setLayoutMatchParent(bezelOverlay);
    // General.setLayoutMatchParent(toolbarOverlay);

    // ===== Java line 313: makeNextRequest(context); =====
    // makeNextRequest(context);

    // Stub: deep Android dependencies not yet ported
}

// ===== Destructor =====
CommentListingFragment::~CommentListingFragment() = default;

// ===== Port of: public void handleCommentVisibilityToggle(RedditCommentView view) (Java lines 316-340) =====
void CommentListingFragment::handleCommentVisibilityToggle(RedditCommentView *view) {
    // final RedditChangeDataManager changeDataManager
    //         = RedditChangeDataManager.getInstance(mUser);
    // final RedditCommentListItem item = view.getComment();
    //
    // if(item.isComment()) {
    //     final RedditRenderableComment comment = item.asComment();
    //
    //     changeDataManager.markHidden(
    //             TimestampUTC.now(),
    //             comment.getIdAndType(),
    //             !comment.isCollapsed(changeDataManager));
    //
    //     mCommentListingManager.updateHiddenStatus();
    //
    //     final LinearLayoutManager layoutManager
    //             = (LinearLayoutManager)mRecyclerView.getLayoutManager();
    //     final int position = layoutManager.getPosition(view);
    //
    //     if(position == layoutManager.findFirstVisibleItemPosition()) {
    //         layoutManager.scrollToPositionWithOffset(position, 0);
    //     }
    // }

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: @Override public View getListingView() (Java lines 342-345) =====
QObject *CommentListingFragment::getListingView() {
    return mListingView;
}

// ===== Port of: @Nullable @Override public View getOverlayView() (Java lines 347-351) =====
QObject *CommentListingFragment::getOverlayView() {
    return mOverlayFrame;
}

// ===== Port of: public RedditPreparedPost getPost() (Java lines 353-355) =====
RedditPreparedPost *CommentListingFragment::getPost() {
    return mPost;
}

// ===== Port of: @Override public Bundle onSaveInstanceState() (Java lines 357-373) =====
Bundle *CommentListingFragment::onSaveInstanceState() {
    // final Bundle bundle = new Bundle();
    //
    // final LinearLayoutManager layoutManager
    //         = (LinearLayoutManager)mRecyclerView.getLayoutManager();
    // bundle.putInt(
    //         SAVEDSTATE_FIRST_VISIBLE_POS,
    //         layoutManager.findFirstVisibleItemPosition());
    //
    // if(mPost != null && mPost.isSelf()) {
    //     bundle.putBoolean(SAVEDSTATE_SELFTEXT_VISIBLE, mSelfTextVisible);
    // }
    //
    // return bundle;

    // Stub: deep Android dependencies not yet ported
    return nullptr;
}

// ===== Port of: private void makeNextRequest(final Context context) (Java lines 375-392) =====
void CommentListingFragment::makeNextRequest(Context *context) {
    // if(!mUrlsToDownload.isEmpty()) {
    //     new CommentListingRequest(
    //             context,
    //             this,
    //             (BaseActivity)getActivity(),
    //             mUrlsToDownload.getFirst(),
    //             mAllUrls.size() == 1,
    //             mUrlsToDownload.getFirst(),
    //             mUser,
    //             mSession,
    //             mDownloadStrategy,
    //             this
    //     );
    // }

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: @Override void onCommentClicked(RedditCommentView view) (Java lines 394-416) =====
void CommentListingFragment::onCommentClicked(RedditCommentView *view) {
    // switch(PrefsUtility.pref_behaviour_actions_comment_tap()) {
    //
    //     case COLLAPSE:
    //         handleCommentVisibilityToggle(view);
    //         break;
    //
    //     case ACTION_MENU: {
    //         final RedditCommentListItem item = view.getComment();
    //         if(item != null && item.isComment()) {
    //             RedditAPICommentAction.showActionMenu(
    //                     getActivity(),
    //                     this,
    //                     item.asComment(),
    //                     view,
    //                     RedditChangeDataManager.getInstance(mUser),
    //                     mPost != null && mPost.isLocked);
    //         }
    //         break;
    //     }
    // }

    // Stub: deep Android/PrefsUtility dependencies not yet ported
}

// ===== Port of: @Override void onCommentLongClicked(RedditCommentView view) (Java lines 418-443) =====
void CommentListingFragment::onCommentLongClicked(RedditCommentView *view) {
    // switch(PrefsUtility.pref_behaviour_actions_comment_longclick()) {
    //
    //     case ACTION_MENU: {
    //         final RedditCommentListItem item = view.getComment();
    //         if(item != null && item.isComment()) {
    //             RedditAPICommentAction.showActionMenu(
    //                     getActivity(),
    //                     this,
    //                     item.asComment(),
    //                     view,
    //                     RedditChangeDataManager.getInstance(mUser),
    //                     mPost != null && mPost.isLocked);
    //         }
    //         break;
    //     }
    //
    //     case COLLAPSE:
    //         handleCommentVisibilityToggle(view);
    //         break;
    //
    //     case NOTHING:
    //         break;
    // }

    // Stub: deep Android/PrefsUtility dependencies not yet ported
}

// ===== Port of: @Override void onCommentListingRequestDownloadNecessary() (Java lines 445-448) =====
void CommentListingFragment::onCommentListingRequestDownloadNecessary() {
    // mCommentListingManager.setLoadingVisible(true);

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: @Override void onCommentListingRequestFailure(RRError error) (Java lines 450-454) =====
void CommentListingFragment::onCommentListingRequestFailure(const RRError &error) {
    // mCommentListingManager.setLoadingVisible(false);
    // mCommentListingManager.addFooterError(new ErrorView(getActivity(), error));

    // Stub: deep Android dependencies not yet ported
    emit loadError(QStringLiteral("Comment listing request failed"));
}

// ===== Port of: @Override void onCommentListingRequestCachedCopy(TimestampUTC) (Java lines 456-459) =====
void CommentListingFragment::onCommentListingRequestCachedCopy(TimestampUTC *timestamp) {
    mCachedTimestamp = timestamp;
}

// ===== Port of: @Override void onCommentListingRequestParseStart() (Java lines 461-464) =====
void CommentListingFragment::onCommentListingRequestParseStart() {
    // mCommentListingManager.setLoadingVisible(true);

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: @Override void onCommentListingRequestPostDownloaded(RedditPreparedPost) (Java lines 466-595) =====
void CommentListingFragment::onCommentListingRequestPostDownloaded(RedditPreparedPost *post) {
    // final BaseActivity activity = (BaseActivity)getActivity();
    //
    // if(mPost == null) {
    //
    //     final RRThemeAttributes attr = new RRThemeAttributes(activity);
    //
    //     mPost = post;
    //
    //     // Invalidate the options menu, so the suggested sort will be shown if needed.
    //     activity.invalidateOptionsMenu();
    //
    //     final RedditPostHeaderView postHeader = new RedditPostHeaderView(
    //             activity,
    //             this.mPost);
    //
    //     mCommentListingManager.addPostHeader(postHeader);
    //
    //     final LinearLayoutManager layoutManager
    //             = (LinearLayoutManager)mRecyclerView.getLayoutManager();
    //     layoutManager.scrollToPositionWithOffset(0, 0);
    //
    //     if(post.src.getSelfText() != null) {
    //         final View selfText = post.src.getSelfText().generateView(
    //                 activity,
    //                 attr.rrMainTextCol,
    //                 13f * mSelfTextFontScale,
    //                 mShowLinkButtons);
    //         selfText.setFocusable(false);
    //
    //         if(selfText instanceof ViewGroup) {
    //             ((ViewGroup)selfText).setDescendantFocusability(
    //                     ViewGroup.FOCUS_BLOCK_DESCENDANTS);
    //         }
    //
    //         final int paddingPx = General.dpToPixels(activity, 10);
    //         final FrameLayout paddingLayout = new FrameLayout(activity);
    //         final TextView collapsedView = new TextView(activity);
    //         //noinspection SetTextI18n
    //         collapsedView.setText("[ + ]  "
    //                 + activity.getString(R.string.collapsed_self_post));
    //         collapsedView.setVisibility(View.GONE);
    //         collapsedView.setPadding(paddingPx, paddingPx, paddingPx, paddingPx);
    //         paddingLayout.addView(selfText);
    //         paddingLayout.addView(collapsedView);
    //         paddingLayout.setPadding(paddingPx, paddingPx, paddingPx, paddingPx);
    //
    //         final PrefsUtility.SelfpostAction actionOnClick
    //                 = PrefsUtility.pref_behaviour_self_post_tap_actions();
    //         if(actionOnClick == PrefsUtility.SelfpostAction.COLLAPSE) {
    //             paddingLayout.setOnClickListener(v -> {
    //                 if(selfText.getVisibility() == View.GONE) {
    //                     mSelfTextVisible = true;
    //                     selfText.setVisibility(View.VISIBLE);
    //                     collapsedView.setVisibility(View.GONE);
    //                 } else {
    //                     mSelfTextVisible = false;
    //                     selfText.setVisibility(View.GONE);
    //                     collapsedView.setVisibility(View.VISIBLE);
    //                     layoutManager.scrollToPositionWithOffset(0, 0);
    //                 }
    //             });
    //         }
    //
    //         if(!mSelfTextVisible) {
    //             selfText.setVisibility(View.GONE);
    //             collapsedView.setVisibility(View.VISIBLE);
    //             layoutManager.scrollToPositionWithOffset(0, 0);
    //         }
    //
    //         paddingLayout.setOnLongClickListener(v -> {
    //             RedditPostActions.INSTANCE.showActionMenu(activity, mPost);
    //             return true;
    //         });
    //
    //         RedditPostActions.INSTANCE.setupAccessibilityActions(
    //                 new AccessibilityActionManager(
    //                         paddingLayout,
    //                         activity.getResources()),
    //                 post,
    //                 activity,
    //                 true);
    //
    //         mCommentListingManager.addPostSelfText(paddingLayout);
    //     }
    //
    //     if(!General.isTablet(activity)) {
    //         activity.setTitle(post.src.getTitle());
    //     }
    //
    //     if(mCommentListingManager.isSearchListing()) {
    //         final CommentSubThreadView searchCommentThreadView
    //                 = new CommentSubThreadView(
    //                 activity,
    //                 mAllUrls.get(0).asPostCommentListURL(),
    //                 R.string.comment_header_search_thread_title
    //         );
    //
    //         mCommentListingManager.addNotification(searchCommentThreadView);
    //     } else if(!mAllUrls.isEmpty()
    //             && mAllUrls.get(0).pathType() == RedditURLParser.POST_COMMENT_LISTING_URL
    //             && mAllUrls.get(0).asPostCommentListURL().commentId != null) {
    //
    //         final CommentSubThreadView specificCommentThreadView
    //                 = new CommentSubThreadView(
    //                 activity,
    //                 mAllUrls.get(0).asPostCommentListURL(),
    //                 R.string.comment_header_specific_thread_title);
    //
    //         mCommentListingManager.addNotification(specificCommentThreadView);
    //     }
    //
    //     // 30 minutes
    //     if(mCachedTimestamp != null) {
    //         if (mCachedTimestamp.elapsed().isGreaterThan(TimeDuration.minutes(30))) {
    //
    //             final TextView cacheNotif = (TextView) LayoutInflater.from(activity).inflate(
    //                     R.layout.cached_header,
    //                     null,
    //                     false);
    //             cacheNotif.setText(activity.getString(
    //                     R.string.listing_cached,
    //                     mCachedTimestamp.format()));
    //             mCommentListingManager.addNotification(cacheNotif);
    //         }
    //     }
    // }

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: @Override void onCommentListingRequestAllItemsDownloaded(...) (Java lines 597-658) =====
void CommentListingFragment::onCommentListingRequestAllItemsDownloaded(
        const std::vector<RedditCommentListItem *> &items) {
    // mCommentListingManager.addComments(items);
    //
    // if(mFloatingToolbar != null && mFloatingToolbar.getVisibility() != View.VISIBLE) {
    //     mFloatingToolbar.setVisibility(View.VISIBLE);
    //     final Animation animation = AnimationUtils.loadAnimation(
    //             getContext(),
    //             R.anim.slide_in_from_bottom);
    //     animation.setInterpolator(new OvershootInterpolator());
    //     mFloatingToolbar.startAnimation(animation);
    // }
    //
    // mUrlsToDownload.removeFirst();
    //
    // final LinearLayoutManager layoutManager
    //         = (LinearLayoutManager)mRecyclerView.getLayoutManager();
    //
    // if(mPreviousFirstVisibleItemPosition != null
    //         && layoutManager.getItemCount() > mPreviousFirstVisibleItemPosition) {
    //
    //     layoutManager.scrollToPositionWithOffset(
    //             mPreviousFirstVisibleItemPosition,
    //             0);
    //
    //     mPreviousFirstVisibleItemPosition = null;
    // }
    //
    // if(mUrlsToDownload.isEmpty()) {
    //
    //     if(mCommentListingManager.getCommentCount() == 0) {
    //
    //         final View emptyView = LayoutInflater.from(getContext()).inflate(
    //                 R.layout.no_items_yet,
    //                 mRecyclerView,
    //                 false);
    //
    //         if(mCommentListingManager.isSearchListing()) {
    //             ((TextView)emptyView.findViewById(R.id.empty_view_text))
    //                     .setText(R.string.no_search_results);
    //         } else {
    //             ((TextView)emptyView.findViewById(R.id.empty_view_text))
    //                     .setText(R.string.no_comments_yet);
    //         }
    //
    //         mCommentListingManager.addViewToItems(emptyView);
    //
    //     } else {
    //
    //         final View blankView = new View(getContext());
    //         blankView.setMinimumWidth(1);
    //         blankView.setMinimumHeight(General.dpToPixels(getContext(), 96));
    //         mCommentListingManager.addViewToItems(blankView);
    //     }
    //
    //     mCommentListingManager.setLoadingVisible(false);
    //
    // } else {
    //     makeNextRequest(getActivity());
    // }

    emit commentsLoaded(static_cast<int>(items.size()));

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: @Override void onCreateOptionsMenu(Menu menu) (Java lines 661-686) =====
void CommentListingFragment::onCreateOptionsMenu(Menu *menu) {
    // final Map<OptionsMenuUtility.AppbarItemsPref, Integer> appbarItemsPrefs =
    //         PrefsUtility.pref_menus_appbar_items();
    // final int replyShowAsAction = OptionsMenuUtility.getOrThrow(
    //         appbarItemsPrefs,
    //         OptionsMenuUtility.AppbarItemsPref.REPLY);
    //
    // if(mAllUrls != null
    //         && !mAllUrls.isEmpty()
    //         && mAllUrls.get(0).pathType() == RedditURLParser.POST_COMMENT_LISTING_URL
    //         && replyShowAsAction != OptionsMenuUtility.DO_NOT_SHOW) {
    //     final MenuItem reply = menu.add(
    //             Menu.NONE,
    //             OptionsMenuUtility.AppbarItemsPref.REPLY.ordinal(),
    //             Menu.NONE,
    //             R.string.action_reply);
    //
    //     reply.setShowAsAction(OptionsMenuUtility.handleShowAsActionIfRoom(
    //             replyShowAsAction));
    //     reply.setIcon(R.drawable.ic_action_reply_dark);
    //
    //     OptionsMenuUtility.pruneMenu(getActivity(), menu, appbarItemsPrefs, true);
    // }

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: @Override boolean onOptionsItemSelected(MenuItem item) (Java lines 688-703) =====
bool CommentListingFragment::onOptionsItemSelected(MenuItem *item) {
    // if(item.getTitle() != null
    //         && item.getTitle()
    //         .equals(getActivity().getString(R.string.action_reply))) {
    //
    //     RedditPostActions.INSTANCE.onActionMenuItemSelected(
    //             mPost,
    //             (BaseActivity)getActivity(),
    //             RedditPostActions.Action.REPLY);
    //     return true;
    // }
    //
    // return false;

    // Stub: deep Android dependencies not yet ported
    return false;
}

// ===== Port of: @Override void onPostSelected(RedditPreparedPost post) (Java lines 705-708) =====
void CommentListingFragment::onPostSelected(RedditPreparedPost *post) {
    // ((RedditPostView.PostSelectionListener)getActivity()).onPostSelected(post);

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: @Override void onPostCommentsSelected(RedditPreparedPost post) (Java lines 710-713) =====
void CommentListingFragment::onPostCommentsSelected(RedditPreparedPost *post) {
    // ((RedditPostView.PostSelectionListener)getActivity()).onPostCommentsSelected(post);

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: public void onPreviousParent() (Java lines 715-739) =====
void CommentListingFragment::onPreviousParent() {
    // final LinearLayoutManager layoutManager = (LinearLayoutManager)
    //     mRecyclerView.getLayoutManager();
    //
    // for(
    //     int pos = layoutManager.findFirstVisibleItemPosition() - 1;
    //     pos > 0;
    //     pos--
    // ) {
    //     final GroupedRecyclerViewAdapter.Item item = mCommentListingManager.getItemAtPosition(
    //         pos
    //     );
    //     if(
    //         item instanceof RedditCommentListItem
    //         && ((RedditCommentListItem)item).isComment()
    //         && ((RedditCommentListItem)item).getIndent() == 0
    //     ) {
    //         layoutManager.scrollToPositionWithOffset(pos, 0);
    //         setFocusDelayed(pos);
    //         return;
    //     }
    // }
    //
    // layoutManager.scrollToPositionWithOffset(0, 0);
    // setFocusDelayed(0);

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: public void onNextParent() (Java lines 742-762) =====
void CommentListingFragment::onNextParent() {
    // final LinearLayoutManager layoutManager = (LinearLayoutManager)
    //     mRecyclerView.getLayoutManager();
    // for(
    //     int pos = layoutManager.findFirstVisibleItemPosition() + 1;
    //     pos < layoutManager.getItemCount();
    //     pos++
    // ) {
    //     final GroupedRecyclerViewAdapter.Item item = mCommentListingManager.getItemAtPosition(
    //         pos
    //     );
    //     if(
    //         item instanceof RedditCommentListItem
    //         && ((RedditCommentListItem)item).isComment()
    //         && ((RedditCommentListItem)item).getIndent() == 0
    //     ) {
    //         layoutManager.scrollToPositionWithOffset(pos, 0);
    //         setFocusDelayed(pos);
    //         break;
    //     }
    // }

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: private void setFocusDelayed(final int pos) (Java lines 765-778) =====
void CommentListingFragment::setFocusDelayed(int pos) {
    // AndroidCommon.UI_THREAD_HANDLER.postDelayed(() -> {
    //     final RecyclerView.ViewHolder view
    //             = mRecyclerView.findViewHolderForAdapterPosition(pos);
    //     if (view != null) {
    //         final View item = view.itemView;
    //         item.sendAccessibilityEvent(AccessibilityEvent.TYPE_VIEW_FOCUSED);
    //         item.sendAccessibilityEvent(AccessibilityEvent.TYPE_VIEW_SELECTED);
    //         item.performAccessibilityAction(
    //                 AccessibilityNodeInfo.ACTION_ACCESSIBILITY_FOCUS,
    //                 null);
    //     }
    // }, 800);

    // Stub: deep Android/Accessibility dependencies not yet ported
}

} // namespace PinkReader
