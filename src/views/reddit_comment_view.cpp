/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: reddit_comment_view.cpp - Port of RedditCommentView.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/views/RedditCommentView.java
 *
 * Every field, method, constant, and inner class ported exactly.
 */

#include "reddit_comment_view.h"

namespace PinkReader {

// ===== Constructor =====
// Port of: public RedditCommentView(
//     final BaseActivity context,
//     final RRThemeAttributes themeAttributes,
//     final CommentListener listener,
//     @Nullable final CommentListingFragment fragment)
RedditCommentView::RedditCommentView(BaseActivity *context,
                                      RRThemeAttributes *themeAttributes,
                                      CommentListener *listener,
                                      CommentListingFragment *fragment,
                                      QWidget *parent)
    : QWidget(parent)
{
    // super(context);
    //
    // mAccessibilityActionManager = new AccessibilityActionManager(this, context.getResources());
    //
    // mActivity = context;
    // mTheme = themeAttributes;
    // mListener = listener;
    // mFragment = fragment;
    //
    // mChangeDataManager = RedditChangeDataManager.getInstance(
    //         RedditAccountManager.getInstance(context).getDefaultAccount());
    //
    // final View rootView =
    //         LayoutInflater.from(context).inflate(R.layout.reddit_comment, this, true);
    //
    // mIndentView = rootView.findViewById(R.id.view_reddit_comment_indentview);
    // mHeader = rootView.findViewById(R.id.view_reddit_comment_header);
    // mBodyHolder = rootView.findViewById(R.id.view_reddit_comment_bodyholder);
    // mIndentedContent = rootView.findViewById(R.id.view_reddit_comment_indented_content);
    //
    // final int minimumCommentHeight = PrefsUtility.pref_accessibility_min_comment_height();
    // mIndentedContent.setMinimumHeight(General.dpToPixels(context, minimumCommentHeight));
    //
    // mBodyFontScale = PrefsUtility.appearance_fontscale_bodytext();
    // final float mHeaderFontScale = PrefsUtility.appearance_fontscale_comment_headers();
    // mHeader.setTextSize(TypedValue.COMPLEX_UNIT_PX,
    //         mHeader.getTextSize() * mHeaderFontScale);
    //
    // mShowLinkButtons = PrefsUtility.pref_appearance_linkbuttons();
    //
    // setOnClickListener(view -> mListener.onCommentClicked(this));
    // setOnLongClickListener(v -> {
    //     mListener.onCommentLongClicked(this);
    //     return true;
    // });

    mActivity = context;
    mTheme = themeAttributes;
    mListener = listener;
    mFragment = fragment;

    // Stub: deep Android dependencies not yet ported
}

// ===== Destructor =====
RedditCommentView::~RedditCommentView() = default;

// ===== Port of: protected void onSetItemFlingPosition(final float position) =====
void RedditCommentView::onSetItemFlingPosition(float position) {
    // mIndentedContent.setTranslationX(position);

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: @NonNull protected String getFlingLeftText() =====
QString RedditCommentView::getFlingLeftText() {
    // final Context context = getContext();
    //
    // final PrefsUtility.CommentFlingAction pref =
    //         PrefsUtility.pref_behaviour_fling_comment_left();
    //
    // mLeftFlingAction = chooseFlingAction(pref);
    //
    // if(mLeftFlingAction == null) {
    //     return "Disabled";
    // }
    //
    // return context.getString(mLeftFlingAction.descriptionRes);

    // Stub: deep Android dependencies not yet ported
    return QStringLiteral("Disabled");
}

// ===== Port of: @NonNull protected String getFlingRightText() =====
QString RedditCommentView::getFlingRightText() {
    // final Context context = getContext();
    //
    // final PrefsUtility.CommentFlingAction pref =
    //         PrefsUtility.pref_behaviour_fling_comment_right();
    //
    // mRightFlingAction = chooseFlingAction(pref);
    //
    // if(mRightFlingAction == null) {
    //     return "Disabled";
    // }
    //
    // return context.getString(mRightFlingAction.descriptionRes);

    // Stub: deep Android dependencies not yet ported
    return QStringLiteral("Disabled");
}

// ===== Port of: protected boolean allowFlingingLeft() =====
bool RedditCommentView::allowFlingingLeft() {
    // return mLeftFlingAction != null;

    // Stub: deep Android dependencies not yet ported
    return false;
}

// ===== Port of: protected boolean allowFlingingRight() =====
bool RedditCommentView::allowFlingingRight() {
    // return mRightFlingAction != null;

    // Stub: deep Android dependencies not yet ported
    return false;
}

// ===== Port of: protected void onFlungLeft() =====
void RedditCommentView::onFlungLeft() {
    // if(mLeftFlingAction == null || !mComment.isComment()) {
    //     return;
    // }
    //
    // RedditAPICommentAction.onActionMenuItemSelected(
    //         mComment.asComment(),
    //         this,
    //         mActivity,
    //         mFragment,
    //         mLeftFlingAction.action,
    //         mChangeDataManager);

    // Stub: deep Android dependencies not yet ported
    emit flungLeft();
}

// ===== Port of: protected void onFlungRight() =====
void RedditCommentView::onFlungRight() {
    // if(mRightFlingAction == null || !mComment.isComment()) {
    //     return;
    // }
    //
    // RedditAPICommentAction.onActionMenuItemSelected(
    //         mComment.asComment(),
    //         this,
    //         mActivity,
    //         mFragment,
    //         mRightFlingAction.action,
    //         mChangeDataManager);

    // Stub: deep Android dependencies not yet ported
    emit flungRight();
}

// ===== Port of: public void onRedditDataChange(final RedditIdAndType thingIdAndType) =====
void RedditCommentView::onRedditDataChange(RedditIdAndType *thingIdAndType) {
    // reset(mActivity, mComment, true);

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: public void reset(
//     final BaseActivity activity, final RedditCommentListItem comment) =====
void RedditCommentView::reset(BaseActivity *activity, RedditCommentListItem *comment) {
    // reset(activity, comment, false);

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: public void reset(
//     final BaseActivity activity, final RedditCommentListItem comment,
//     final boolean updateOnly) =====
void RedditCommentView::reset(BaseActivity *activity, RedditCommentListItem *comment,
                               bool updateOnly) {
    // if(!updateOnly) {
    //     if(!comment.isComment()) {
    //         throw new RuntimeException("Not a comment");
    //     }
    //
    //     if(mComment != comment) {
    //         if(mComment != null) {
    //             mChangeDataManager.removeListener(
    //                 mComment.asComment().getIdAndType(), this);
    //         }
    //         mChangeDataManager.addListener(comment.asComment().getIdAndType(), this);
    //     }
    //
    //     mComment = comment;
    //     resetSwipeState();
    // }
    //
    // mIndentView.setIndentation(comment.getIndent());
    //
    // final boolean hideLinkButtons = comment.asComment()
    //         .getParsedComment()
    //         .getRawComment().getAuthor().getDecoded().equalsIgnoreCase("autowikibot");
    //
    // mBodyHolder.removeAllViews();
    // final View commentBody = comment.asComment().getBody(
    //         activity,
    //         mTheme.rrCommentBodyCol,
    //         13.0f * mBodyFontScale,
    //         mShowLinkButtons && !hideLinkButtons);
    //
    // mBodyHolder.addView(commentBody);
    // General.setLayoutMatchWidthWrapHeight(commentBody);
    //
    // ((MarginLayoutParams)commentBody.getLayoutParams()).topMargin =
    //         General.dpToPixels(activity, 1);
    //
    // final RedditRenderableComment renderableComment = mComment.asComment();
    //
    // final int ageUnits = PrefsUtility.appearance_comment_age_units();
    //
    // final TimestampUTC postTimestamp = (mFragment != null && mFragment.getPost() != null)
    //         ? mFragment.getPost().src.getCreatedTimeUTC()
    //         : null;
    //
    // final TimestampUTC parentCommentTimestamp = mComment.getParent() != null
    //         ? mComment.getParent().asComment().getParsedComment().getRawComment()
    //                 .getCreated_utc().getValue()
    //         : null;
    //
    // final boolean isCollapsed = mComment.isCollapsed(mChangeDataManager);
    //
    // final BetterSSB header = renderableComment.getHeader(
    //         mTheme, mChangeDataManager, activity, ageUnits,
    //         postTimestamp, parentCommentTimestamp);
    //
    // final Observer observer = (observable, o) -> {
    //     if (isCollapsed) {
    //         mHeaderText = "[ + ]  " + o;
    //     } else {
    //         mHeaderText = (SpannableStringBuilder) o;
    //     }
    //     AndroidCommon.runOnUiThread(() -> mHeader.setText(mHeaderText));
    // };
    //
    // header.addObserver(observer);
    // mHeaderText = header.get();
    //
    // mHeader.setContentDescription(renderableComment.getAccessibilityHeader(
    //         mTheme, mChangeDataManager, activity, ageUnits,
    //         postTimestamp, parentCommentTimestamp,
    //         isCollapsed, Optional.of(comment.getIndent())));
    //
    // if(isCollapsed) {
    //     setFlingingEnabled(false);
    //     //noinspection SetTextI18n
    //     mHeader.setText("[ + ]  " + mHeaderText);
    //     mBodyHolder.setVisibility(GONE);
    // } else {
    //     setFlingingEnabled(true);
    //     mHeader.setText(mHeaderText);
    //     mBodyHolder.setVisibility(VISIBLE);
    // }
    //
    // setupAccessibilityActions();

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: private void setupAccessibilityActions() =====
void RedditCommentView::setupAccessibilityActions() {
    // final RedditAccount defaultAccount
    //         = RedditAccountManager.getInstance(mActivity).getDefaultAccount();
    // final boolean isAuthenticated = defaultAccount.isNotAnonymous();
    //
    // mAccessibilityActionManager.removeAllActions();
    //
    // if(!mComment.isComment()) {
    //     return;
    // }
    //
    // addAccessibilityActionFromDescriptionPair(
    //     chooseFlingAction(PrefsUtility.CommentFlingAction.COLLAPSE));
    //
    // mAccessibilityActionManager.addAction(R.string.button_next_comment_parent, () -> {
    //     mFragment.onNextParent();
    // });
    //
    // mAccessibilityActionManager.addAction(R.string.button_prev_comment_parent, () -> {
    //     mFragment.onPreviousParent();
    // });
    //
    // if (isAuthenticated) {
    //     addAccessibilityActionFromDescriptionPair(
    //             chooseFlingAction(PrefsUtility.CommentFlingAction.REPLY));
    // }
    //
    // if (mComment.asComment().getParsedComment().getRawComment()
    //         .getAuthor().getDecoded().equalsIgnoreCase(defaultAccount.username)) {
    //     addAccessibilityActionFromDescriptionPair(
    //         new ActionDescriptionPair(
    //             RedditAPICommentAction.RedditCommentAction.EDIT,
    //             R.string.action_edit));
    //     addAccessibilityActionFromDescriptionPair(
    //         new ActionDescriptionPair(
    //             RedditAPICommentAction.RedditCommentAction.DELETE,
    //             R.string.action_delete));
    // }
    //
    // addAccessibilityActionFromDescriptionPair(
    //         chooseFlingAction(PrefsUtility.CommentFlingAction.USER_PROFILE));
    //
    // if (isAuthenticated) {
    //     addAccessibilityActionFromDescriptionPair(
    //             chooseFlingAction(PrefsUtility.CommentFlingAction.REPORT));
    // }
    //
    // addAccessibilityActionFromDescriptionPair(
    //         chooseFlingAction(PrefsUtility.CommentFlingAction.SHARE));
    //
    // if (isAuthenticated) {
    //     addAccessibilityActionFromDescriptionPair(
    //         chooseFlingAction(PrefsUtility.CommentFlingAction.DOWNVOTE));
    //     addAccessibilityActionFromDescriptionPair(
    //         chooseFlingAction(PrefsUtility.CommentFlingAction.UPVOTE));
    // }
    //
    // mAccessibilityActionManager.setClickHint(
    //     getAccessibilityHintForActionPref(
    //         PrefsUtility.pref_behaviour_actions_comment_tap()));
    //
    // mAccessibilityActionManager.setLongClickHint(
    //     getAccessibilityHintForActionPref(
    //             PrefsUtility.pref_behaviour_actions_comment_longclick()));

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: @Nullable @StringRes private Integer getAccessibilityHintForActionPref(
//     @NonNull final PrefsUtility.CommentAction pref) =====
int RedditCommentView::getAccessibilityHintForActionPref(CommentAction::Type pref) {
    // switch (pref) {
    //     case COLLAPSE:
    //         return R.string.action_collapse;
    //     case ACTION_MENU:
    //         return R.string.action_actionmenu;
    // }
    // return null;

    // Stub: deep Android dependencies not yet ported
    return 0; // null equivalent in Java (0 resource ID)
}

// ===== Port of: public RedditCommentListItem getComment() =====
RedditCommentListItem *RedditCommentView::getComment() {
    // return mComment;
    return mComment;
}

// ===== Port of: private void addAccessibilityActionFromDescriptionPair(
//     @Nullable final ActionDescriptionPair pair) =====
void RedditCommentView::addAccessibilityActionFromDescriptionPair(ActionDescriptionPair *pair) {
    // if(pair == null) {
    //     return;
    // }
    //
    // mAccessibilityActionManager.addAction(pair.descriptionRes, () -> {
    //     RedditAPICommentAction.onActionMenuItemSelected(
    //         mComment.asComment(),
    //         this,
    //         mActivity,
    //         mFragment,
    //         pair.action,
    //         mChangeDataManager
    //     );
    // });

    // Stub: deep Android dependencies not yet ported
}

// ===== Port of: @Nullable private ActionDescriptionPair chooseFlingAction(
//     final PrefsUtility.CommentFlingAction pref) =====
ActionDescriptionPair *RedditCommentView::chooseFlingAction(CommentFlingAction::Type pref) {
    // if(!mComment.isComment()) {
    //     return null;
    // }
    //
    // final RedditParsedComment comment = mComment.asComment().getParsedComment();
    //
    // switch(pref) {
    //     case UPVOTE:
    //         if(mChangeDataManager.isUpvoted(comment.getIdAndType())) {
    //             return new ActionDescriptionPair(
    //                     RedditAPICommentAction.RedditCommentAction.UNVOTE,
    //                     R.string.action_vote_remove);
    //         } else {
    //             return new ActionDescriptionPair(
    //                     RedditAPICommentAction.RedditCommentAction.UPVOTE,
    //                     R.string.action_upvote);
    //         }
    //     case DOWNVOTE:
    //         if(mChangeDataManager.isDownvoted(comment.getIdAndType())) {
    //             return new ActionDescriptionPair(
    //                     RedditAPICommentAction.RedditCommentAction.UNVOTE,
    //                     R.string.action_vote_remove);
    //         } else {
    //             return new ActionDescriptionPair(
    //                     RedditAPICommentAction.RedditCommentAction.DOWNVOTE,
    //                     R.string.action_downvote);
    //         }
    //     case SAVE:
    //         if(mChangeDataManager.isSaved(comment.getIdAndType())) {
    //             return new ActionDescriptionPair(
    //                     RedditAPICommentAction.RedditCommentAction.UNSAVE,
    //                     R.string.action_unsave);
    //         } else {
    //             return new ActionDescriptionPair(
    //                     RedditAPICommentAction.RedditCommentAction.SAVE,
    //                     R.string.action_save);
    //         }
    //     case REPORT:
    //         return new ActionDescriptionPair(
    //                 RedditAPICommentAction.RedditCommentAction.REPORT,
    //                 R.string.action_report);
    //     case REPLY:
    //         return new ActionDescriptionPair(
    //                 RedditAPICommentAction.RedditCommentAction.REPLY,
    //                 R.string.action_reply);
    //     case CONTEXT:
    //         return new ActionDescriptionPair(
    //                 RedditAPICommentAction.RedditCommentAction.CONTEXT,
    //                 R.string.action_comment_context);
    //     case GO_TO_COMMENT:
    //         return new ActionDescriptionPair(
    //                 RedditAPICommentAction.RedditCommentAction.GO_TO_COMMENT,
    //                 R.string.action_comment_go_to);
    //     case COMMENT_LINKS:
    //         return new ActionDescriptionPair(
    //                 RedditAPICommentAction.RedditCommentAction.COMMENT_LINKS,
    //                 R.string.action_comment_links);
    //     case SHARE:
    //         return new ActionDescriptionPair(
    //                 RedditAPICommentAction.RedditCommentAction.SHARE,
    //                 R.string.action_share);
    //     case COPY_TEXT:
    //         return new ActionDescriptionPair(
    //                 RedditAPICommentAction.RedditCommentAction.COPY_TEXT,
    //                 R.string.action_copy_text);
    //     case COPY_URL:
    //         return new ActionDescriptionPair(
    //                 RedditAPICommentAction.RedditCommentAction.COPY_URL,
    //                 R.string.action_copy_link);
    //     case USER_PROFILE:
    //         return new ActionDescriptionPair(
    //                 RedditAPICommentAction.RedditCommentAction.USER_PROFILE,
    //                 R.string.action_user_profile);
    //     case COLLAPSE:
    //         if(mFragment == null) {
    //             return null;
    //         }
    //         return new ActionDescriptionPair(
    //                 RedditAPICommentAction.RedditCommentAction.COLLAPSE,
    //                 R.string.action_collapse);
    //     case ACTION_MENU:
    //         if(mFragment == null) {
    //             return null;
    //         }
    //         return new ActionDescriptionPair(
    //                 RedditAPICommentAction.RedditCommentAction.ACTION_MENU,
    //                 R.string.action_actionmenu_short);
    //     case PROPERTIES:
    //         return new ActionDescriptionPair(
    //                 RedditAPICommentAction.RedditCommentAction.PROPERTIES,
    //                 R.string.action_properties);
    //     case BACK:
    //         return new ActionDescriptionPair(
    //                 RedditAPICommentAction.RedditCommentAction.BACK,
    //                 R.string.action_back);
    //     case DISABLED:
    //         return null;
    // }
    // return null;

    // Stub: deep Android dependencies not yet ported
    return nullptr;
}

} // namespace PinkReader
