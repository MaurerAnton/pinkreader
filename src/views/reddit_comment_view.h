/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: reddit_comment_view.h - Port of RedditCommentView.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/views/RedditCommentView.java
 *
 * Original: public class RedditCommentView extends FlingableItemView
 *     implements RedditChangeDataManager.Listener
 *
 * Every field, method, constant, and inner class ported exactly.
 */

#pragma once

#include <QWidget>
#include <QString>
#include <memory>
#include <functional>

// Forward declarations for Android/RedReader types used in signatures
namespace PinkReader {

// View base classes
class FlingableItemView; // parent - ported as QWidget convention

// Android framework stubs
class TextView;
class FrameLayout;
class LinearLayout;
class View;
class Resources;
class Context;

// RedReader types
class BaseActivity;
class RedditChangeDataManager;
class RRThemeAttributes;
class Accessible;
class AccessibilityActionManager;
class RedditCommentListItem;
class RedditParsedComment;
class RedditRenderableComment;
class RedditIdAndType;
class BetterSSB;
class Optional_t; // generic Optional

// RedReader API
class RedditAPICommentAction;
class RedditAccount;
class RedditAccountManager;

// Fragments
class CommentListingFragment;

// Preferences
class PrefsUtility;
class General;

// Timestamp
class TimestampUTC;

/**
 * @brief Enums mirroring nested enums in PrefsUtility (CommentFlingAction, CommentAction)
 * Ported from Java: PrefsUtility.CommentFlingAction enum and PrefsUtility.CommentAction
 */
namespace CommentFlingAction {
    enum Type {
        UPVOTE,
        DOWNVOTE,
        SAVE,
        REPORT,
        REPLY,
        CONTEXT,
        GO_TO_COMMENT,
        COMMENT_LINKS,
        SHARE,
        COPY_TEXT,
        COPY_URL,
        USER_PROFILE,
        COLLAPSE,
        ACTION_MENU,
        PROPERTIES,
        BACK,
        DISABLED
    };
}

namespace CommentAction {
    enum Type {
        COLLAPSE,
        ACTION_MENU,
        REPLY,
        USER_PROFILE,
        COLLAPSE_ALL,
        NEXT_PARENT,
        PREV_PARENT
    };
}

/**
 * @brief Comment selection listener interface.
 * Port of RedditCommentView.CommentListener (Java lines 314-318)
 */
class CommentListener {
public:
    virtual ~CommentListener() = default;
    virtual void onCommentClicked(void *view) = 0; // RedditCommentView
    virtual void onCommentLongClicked(void *view) = 0; // RedditCommentView
};

/**
 * @brief Action-description pair.
 * Port of: private static final class ActionDescriptionPair (Java lines 91-101)
 */
class ActionDescriptionPair {
public:
    // public final RedditAPICommentAction.RedditCommentAction action;
    void *action = nullptr; // RedditAPICommentAction.RedditCommentAction (stub enum)
    // public final int descriptionRes;
    int descriptionRes = 0;

    ActionDescriptionPair() = default;

    // private ActionDescriptionPair(
    //     final RedditAPICommentAction.RedditCommentAction action, final int descriptionRes)
    ActionDescriptionPair(void *action, int descriptionRes)
        : action(action), descriptionRes(descriptionRes) {}
};

/**
 * @brief Port of org.quantumbadger.redreader.views.RedditCommentView
 *
 * A single comment view with fling/swipe actions, header, body, indent, etc.
 * Every field, method, constant, and inner class ported exactly.
 */
class RedditCommentView : public QWidget {
    Q_OBJECT

public:
    // ===== Nested types (ported out to namespace level above) =====
    // CommentListener interface - see: class CommentListener
    // ActionDescriptionPair inner class - see: class ActionDescriptionPair

    // ===== Constructor =====
    // public RedditCommentView(
    //     final BaseActivity context,
    //     final RRThemeAttributes themeAttributes,
    //     final CommentListener listener,
    //     @Nullable final CommentListingFragment fragment)
    explicit RedditCommentView(BaseActivity *context,
                               RRThemeAttributes *themeAttributes,
                               CommentListener *listener,
                               CommentListingFragment *fragment,
                               QWidget *parent = nullptr);

    ~RedditCommentView() override;

    // ===== Interface method stubs (port of implemented interfaces) =====

    // RedditChangeDataManager.Listener: void onRedditDataChange(RedditIdAndType thingIdAndType)
    virtual void onRedditDataChange(RedditIdAndType *thingIdAndType);

    // ===== Public methods (mirrors Java exactly) =====

    // public void reset(final BaseActivity activity, final RedditCommentListItem comment)
    void reset(BaseActivity *activity, RedditCommentListItem *comment);

    // public void reset(final BaseActivity activity, final RedditCommentListItem comment,
    //     final boolean updateOnly)
    void reset(BaseActivity *activity, RedditCommentListItem *comment, bool updateOnly);

    // public RedditCommentListItem getComment()
    RedditCommentListItem *getComment();

    // ===== FlingableItemView overrides (ported as methods) =====

    // protected void onSetItemFlingPosition(final float position)
    void onSetItemFlingPosition(float position);

    // @NonNull protected String getFlingLeftText()
    QString getFlingLeftText();

    // @NonNull protected String getFlingRightText()
    QString getFlingRightText();

    // protected boolean allowFlingingLeft()
    bool allowFlingingLeft();

    // protected boolean allowFlingingRight()
    bool allowFlingingRight();

    // protected void onFlungLeft()
    void onFlungLeft();

    // protected void onFlungRight()
    void onFlungRight();

signals:
    // Qt signals for fling/click events
    void flungLeft();
    void flungRight();
    void commentClicked();
    void commentLongClicked();

private:
    // ===== Private inner class (struc/class on namespace level) =====
    // ActionDescriptionPair - see above

    // ===== Private fields (mirrors Java exactly, lines 58-85) =====

    // private final AccessibilityActionManager mAccessibilityActionManager;
    AccessibilityActionManager *mAccessibilityActionManager = nullptr;

    // private RedditCommentListItem mComment;
    RedditCommentListItem *mComment = nullptr;

    // private final BaseActivity mActivity;
    BaseActivity *mActivity = nullptr;

    // private final RedditChangeDataManager mChangeDataManager;
    RedditChangeDataManager *mChangeDataManager = nullptr;

    // private final RRThemeAttributes mTheme;
    RRThemeAttributes *mTheme = nullptr;

    // private final TextView mHeader;
    TextView *mHeader = nullptr; // stub

    // private final FrameLayout mBodyHolder;
    FrameLayout *mBodyHolder = nullptr; // stub

    // private final IndentView mIndentView;
    void *mIndentView = nullptr; // IndentView stub

    // private final LinearLayout mIndentedContent;
    LinearLayout *mIndentedContent = nullptr; // stub

    // private final float mBodyFontScale;
    float mBodyFontScale = 1.0f;

    // private final boolean mShowLinkButtons;
    bool mShowLinkButtons = false;

    // private CharSequence mHeaderText;
    void *mHeaderText = nullptr; // CharSequence stub

    // private final CommentListener mListener;
    CommentListener *mListener = nullptr;

    // @Nullable private final CommentListingFragment mFragment;
    CommentListingFragment *mFragment = nullptr;

    // @Nullable private ActionDescriptionPair mLeftFlingAction;
    ActionDescriptionPair *mLeftFlingAction = nullptr;

    // @Nullable private ActionDescriptionPair mRightFlingAction;
    ActionDescriptionPair *mRightFlingAction = nullptr;

    // ===== Private methods (mirrors Java exactly) =====

    // @Nullable private ActionDescriptionPair chooseFlingAction(
    //     final PrefsUtility.CommentFlingAction pref)  -- Java line 103
    ActionDescriptionPair *chooseFlingAction(CommentFlingAction::Type pref);

    // private void setupAccessibilityActions()  -- Java line 486
    void setupAccessibilityActions();

    // @Nullable @StringRes private Integer getAccessibilityHintForActionPref(
    //     @NonNull final PrefsUtility.CommentAction pref)  -- Java line 561
    int getAccessibilityHintForActionPref(CommentAction::Type pref);

    // private void addAccessibilityActionFromDescriptionPair(
    //     @Nullable final ActionDescriptionPair pair)  -- Java line 578
    void addAccessibilityActionFromDescriptionPair(ActionDescriptionPair *pair);
};

} // namespace PinkReader
