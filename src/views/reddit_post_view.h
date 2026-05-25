/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 *
 * Ported from RedReader: RedditPostView.java (lines 1-670)
 * File: reddit_post_view.h - Post list item view with fling, thumbnails, inline previews
 */

#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QImage>
#include <QSharedPointer>
#include <QTimer>
#include <atomic>
#include <functional>

// Forward declarations for types not yet fully ported
namespace PinkReader {

class RedditPreparedPost;
class RedditParsedPost;
class BaseActivity;
class PostListingFragment;
class LoadingSpinnerView;
class AccessibilityActionManager;
class ErrorView;
class RedditPostActions;

/** Enum mirrors PrefsUtility.PostFlingAction */
enum class PostFlingAction {
    UPVOTE,
    DOWNVOTE,
    SAVE,
    HIDE,
    COMMENTS,
    LINK,
    DISABLED
};

/**
 * @brief Interface for post selection callbacks.
 * Mirrors: RedditPostView.PostSelectionListener
 */
class PostSelectionListener {
public:
    virtual ~PostSelectionListener() = default;
    virtual void onPostSelected(RedditPreparedPost *post) = 0;
    virtual void onPostCommentsSelected(RedditPreparedPost *post) = 0;
};

/**
 * @brief Port of RedditPostView.java (first 500 lines)
 *
 * A post list item view with:
 * - Fling/swipe actions (left/right)
 * - Thumbnail display with async loading
 * - Inline image/video previews
 * - Comment count button
 * - Accessibility actions
 * - Read/unread appearance
 * - Vote/save/hide overlay icons
 *
 * In the Java original: extends FlingableItemView implements RedditPreparedPost.ThumbnailLoadedCallback
 * In Qt: extends QWidget
 */
class RedditPostView : public QWidget {
    Q_OBJECT

public:
    // --- Static constants (mirrors Java exactly) ---
    static constexpr const char *TAG = "RedditPostView";
    static constexpr const char *PROMPT_PREF_KEY = "inline_image_prompt_accepted";
    static std::atomic<int> sInlinePreviewsShownThisSession;

    // --- PostSelectionListener (nested interface, mirrored above as separate class) ---

    /**
     * @brief Constructor — mirrors Java constructor exactly.
     *
     * Port of:
     * public RedditPostView(Context context, PostListingFragment fragmentParent,
     *     BaseActivity activity, boolean leftHandedMode)
     */
    explicit RedditPostView(
        QWidget *context,       // Context
        PostListingFragment *fragmentParent,
        BaseActivity *activity,
        bool leftHandedMode,
        QWidget *parent = nullptr);

    ~RedditPostView() override;

    /** Reset view to display a new post (mirrors: public void reset(RedditPreparedPost)) */
    void reset(RedditPreparedPost *newPost);

    /** Update visual appearance based on read/vote/save state (mirrors: updateAppearance) */
    void updateAppearance();

    /** Callback when a better thumbnail is available (mirrors: betterThumbnailAvailable) */
    void betterThumbnailAvailable(const QImage &thumbnail, int callbackUsageId);

    // --- FlingableItemView overrides (ported as methods) ---
    void onSetItemFlingPosition(float position);
    QString getFlingLeftText();
    QString getFlingRightText();
    bool allowFlingingLeft();
    bool allowFlingingRight();
    void onFlungLeft();
    void onFlungRight();

signals:
    /** Emitted when the post view is scrolled/flung */
    void flungLeft();
    void flungRight();

protected:
    /** Override QWidget resize for layout adjustments */
    void resizeEvent(QResizeEvent *event) override;

private:
    // --- Private fields (mirrors Java exactly) ---

    /** Accessibility action manager */
    AccessibilityActionManager *m_accessibilityActionManager = nullptr;

    /** Current displayed post */
    RedditPreparedPost *m_post = nullptr;

    // --- UI widgets ---
    QLabel *m_titleLabel = nullptr;
    QLabel *m_subtitleLabel = nullptr;

    /** Thumbnail image view */
    QLabel *m_thumbnailView = nullptr;

    /** Overlay icon (star, cross, arrows) */
    QLabel *m_overlayIcon = nullptr;

    /** Outer layout */
    QWidget *m_outerView = nullptr;

    /** Inner content layout */
    QWidget *m_innerView = nullptr;

    /** Comments button */
    QPushButton *m_commentsButton = nullptr;

    /** Comments text label */
    QLabel *m_commentsText = nullptr;

    /** Error views container */
    QWidget *m_postErrors = nullptr;

    /** Image preview holder */
    QWidget *m_imagePreviewHolder = nullptr;

    /** Image preview image view */
    QLabel *m_imagePreviewImageView = nullptr;

    /** Play overlay for video previews */
    QWidget *m_imagePreviewPlayOverlay = nullptr;

    /** Image preview outer container */
    QWidget *m_imagePreviewOuter = nullptr;

    /** Loading spinner for image preview */
    LoadingSpinnerView *m_imagePreviewLoadingSpinner = nullptr;

    /** Footer for prompts */
    QWidget *m_footer = nullptr;

    /** Usage ID for tracking view recycling */
    int m_usageId = 0;

    /** Thumbnail update handler (mirrors Java: thumbnailHandler) */
    QTimer *m_thumbnailHandler = nullptr;

    /** Parent activity */
    BaseActivity *m_activity = nullptr;

    /** Post listing fragment */
    PostListingFragment *m_fragmentParent = nullptr;

    /** Fling preferences */
    PostFlingAction m_leftFlingPref = PostFlingAction::DISABLED;
    PostFlingAction m_rightFlingPref = PostFlingAction::DISABLED;

    /** Current fling action descriptions */
    // RedditPostActions.ActionDescriptionPair mLeftFlingAction, mRightFlingAction;
    int m_leftFlingActionDescRes = 0;
    int m_rightFlingActionDescRes = 0;
    int m_leftFlingActionType = -1;
    int m_rightFlingActionType = -1;
    bool m_leftFlingActionValid = false;
    bool m_rightFlingActionValid = false;

    /** Comments button preference */
    bool m_commentsButtonPref = true;

    /** Title colors from theme */
    int m_rrPostTitleReadCol = 0xFF888888;   // gray for read
    int m_rrPostTitleCol = 0xFF000000;       // black for unread

    /** Thumbnail size in pixels */
    int m_thumbnailSizePrefPixels = 72;

    /** Left-handed mode flag */
    bool m_leftHandedMode = false;

    // --- Private methods (mirrors Java exactly) ---

    /** Set bottom margin on outer view */
    void setBottomMargin(bool enabled);

    /** Download inline image/video preview */
    void downloadInlinePreview(RedditPreparedPost *post, int usageId);

    /** Show the inline image preference prompt */
    void showPrefPrompt();

    /** Setup layout structure */
    void setupLayout();

    /** Setup left-handed mode (reverse child order) */
    void applyLeftHandedMode();

    /** Setup click/long-click handlers */
    void setupClickHandlers();

    /** Setup fling/swipe gesture handlers */
    void setupFlingHandlers();
};

} // namespace PinkReader
