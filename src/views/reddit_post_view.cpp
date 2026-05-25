/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 *
 * Ported from RedReader: RedditPostView.java (lines 1-670)
 * File: reddit_post_view.cpp - Post list item view implementation
 */

#include "reddit_post_view.h"

// Stub includes for dependencies not yet fully ported
#include "../models/reddit_prepared_post.h"
#include "../models/reddit_parsed_post.h"
#include "../common/general.h"
#include "../common/constants.h"
#include "../common/prefs_utility.h"
#include "../common/display_utils.h"
#include "../cache/cache_manager.h"
#include "../cache/cache_request.h"
#include "../accounts/reddit_account_manager.h"
#include "../fragments/post_listing_fragment.h"
#include "../activities/base_activity.h"
#include "../views/loading_spinner_view.h"
#include "../views/error_view.h"
#include "../views/accessibility_action_manager.h"

#include <QDebug>
#include <QImage>
#include <QImageReader>
#include <QPainter>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QApplication>
#include <QScreen>
#include <QRect>
#include <QFont>
#include <QFontMetrics>
#include <cmath>

namespace PinkReader {

// --- Static member initialization ---
std::atomic<int> RedditPostView::sInlinePreviewsShownThisSession{0};

// --- Constructor ---
// Port of: public RedditPostView(Context context, PostListingFragment fragmentParent,
//     BaseActivity activity, boolean leftHandedMode)
RedditPostView::RedditPostView(
    QWidget *context,
    PostListingFragment *fragmentParent,
    BaseActivity *activity,
    bool leftHandedMode,
    QWidget *parent)
    : QWidget(parent)
    , m_activity(activity)
    , m_fragmentParent(fragmentParent)
    , m_leftHandedMode(leftHandedMode)
{
    Q_UNUSED(context);

    // mAccessibilityActionManager = new AccessibilityActionManager(this, context.getResources());
    m_accessibilityActionManager = nullptr; // TODO: create when AccessibilityActionManager is ported

    // thumbnailHandler = new Handler(Looper.getMainLooper()) { ... }
    // In Qt, we use a QTimer with a single-shot pattern and property storage
    // The handler receives thumbnail updates and sets them on the ImageView
    // if(mUsageId != msg.what) return; mThumbnailView.setImageBitmap((Bitmap)msg.obj);
    m_thumbnailHandler = new QTimer(this);
    m_thumbnailHandler->setSingleShot(true);

    // final float dpScale = context.getResources().getDisplayMetrics().density;
    const float dpScale = 1.0f; // TODO: get from device metrics

    // final float titleFontScale = PrefsUtility.appearance_fontscale_posts();
    const float titleFontScale = PrefsUtility::appearanceFontscalePosts();
    // final float subtitleFontScale = PrefsUtility.appearance_fontscale_post_subtitles();
    const float subtitleFontScale = PrefsUtility::appearanceFontscalePostSubtitles();

    // LayoutInflater.from(context).inflate(R.layout.reddit_post, this, true);
    setupLayout();

    // mCommentsButtonPref = PrefsUtility.appearance_post_show_comments_button();
    m_commentsButtonPref = PrefsUtility::appearancePostShowCommentsButton();

    // Set title/subtitle font sizes
    if (m_titleLabel) {
        QFont titleFont = m_titleLabel->font();
        titleFont.setPointSizeF(titleFont.pointSizeF() * titleFontScale);
        m_titleLabel->setFont(titleFont);
    }
    if (m_subtitleLabel) {
        QFont subFont = m_subtitleLabel->font();
        subFont.setPointSizeF(subFont.pointSizeF() * subtitleFontScale);
        m_subtitleLabel->setFont(subFont);
    }

    // Left-handed mode: reverse inner view children
    if (leftHandedMode) {
        applyLeftHandedMode();
    }

    // Set up click handlers
    setupClickHandlers();

    // Set up fling handlers
    setupFlingHandlers();

    // Theme color attributes (from TypedArray in Java)
    // r.attr.rrPostTitleCol, rrPostTitleReadCol
    m_rrPostTitleCol = 0xFF000000;     // default black
    m_rrPostTitleReadCol = 0xFF888888; // default gray

    // mThumbnailSizePrefPixels = (int)(dpScale * PrefsUtility.images_thumbnail_size_dp());
    m_thumbnailSizePrefPixels = static_cast<int>(dpScale * PrefsUtility::imagesThumbnailSizeDp());
}

RedditPostView::~RedditPostView() = default;

// --- setupLayout ---
// Port of: LayoutInflater and findViewById calls in constructor
void RedditPostView::setupLayout() {
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // mOuterView = rootView.findViewById(R.id.reddit_post_layout_outer);
    m_outerView = new QWidget(this);
    auto *outerLayout = new QVBoxLayout(m_outerView);
    outerLayout->setContentsMargins(8, 4, 8, 4);
    outerLayout->setSpacing(2);
    mainLayout->addWidget(m_outerView);

    // mImagePreviewOuter = rootView.findViewById(R.id.reddit_post_image_preview_outer);
    m_imagePreviewOuter = new QWidget(this);
    auto *previewOuterLayout = new QVBoxLayout(m_imagePreviewOuter);
    previewOuterLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(m_imagePreviewOuter);

    // mImagePreviewHolder = rootView.findViewById(R.id.reddit_post_image_preview_holder);
    m_imagePreviewHolder = new QWidget(m_imagePreviewOuter);
    auto *previewHolderLayout = new QVBoxLayout(m_imagePreviewHolder);
    previewHolderLayout->setContentsMargins(0, 0, 0, 0);
    previewOuterLayout->addWidget(m_imagePreviewHolder);

    // mImagePreviewImageView = rootView.findViewById(R.id.reddit_post_image_preview_imageview);
    m_imagePreviewImageView = new QLabel(m_imagePreviewHolder);
    m_imagePreviewImageView->setAlignment(Qt::AlignCenter);
    m_imagePreviewImageView->setScaledContents(true);
    previewHolderLayout->addWidget(m_imagePreviewImageView);

    // mImagePreviewPlayOverlay = rootView.findViewById(R.id.reddit_post_image_preview_play_overlay);
    m_imagePreviewPlayOverlay = new QWidget(m_imagePreviewHolder);
    m_imagePreviewPlayOverlay->setStyleSheet("background: rgba(0,0,0,128);");
    auto *playOverlayLayout = new QHBoxLayout(m_imagePreviewPlayOverlay);
    auto *playIcon = new QLabel(QStringLiteral("\u25B6"), m_imagePreviewPlayOverlay); // ▶
    playIcon->setStyleSheet("color: white; font-size: 32px;");
    playOverlayLayout->addStretch();
    playOverlayLayout->addWidget(playIcon);
    playOverlayLayout->addStretch();
    m_imagePreviewPlayOverlay->setVisible(false);

    // mImagePreviewLoadingSpinner = new LoadingSpinnerView(activity);
    // mImagePreviewHolder.addView(mImagePreviewLoadingSpinner);
    m_imagePreviewLoadingSpinner = nullptr; // TODO: create when LoadingSpinnerView is ported

    // mPostErrors = rootView.findViewById(R.id.reddit_post_errors);
    m_postErrors = new QWidget(this);
    auto *errorsLayout = new QVBoxLayout(m_postErrors);
    errorsLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(m_postErrors);

    // mInnerView = rootView.findViewById(R.id.reddit_post_layout_inner);
    m_innerView = new QWidget(m_outerView);
    auto *innerLayout = new QHBoxLayout(m_innerView);
    innerLayout->setContentsMargins(0, 0, 0, 0);
    innerLayout->setSpacing(8);
    outerLayout->addWidget(m_innerView);

    // mThumbnailView = rootView.findViewById(R.id.reddit_post_thumbnail_view);
    m_thumbnailView = new QLabel(m_innerView);
    m_thumbnailView->setFixedWidth(m_thumbnailSizePrefPixels);
    m_thumbnailView->setAlignment(Qt::AlignCenter);
    m_thumbnailView->setScaledContents(false);
    innerLayout->addWidget(m_thumbnailView);

    // Text column
    auto *textColumn = new QWidget(m_innerView);
    auto *textLayout = new QVBoxLayout(textColumn);
    textLayout->setContentsMargins(0, 0, 0, 0);
    textLayout->setSpacing(2);

    // title = rootView.findViewById(R.id.reddit_post_title);
    m_titleLabel = new QLabel(textColumn);
    m_titleLabel->setWordWrap(true);
    textLayout->addWidget(m_titleLabel);

    // subtitle = rootView.findViewById(R.id.reddit_post_subtitle);
    m_subtitleLabel = new QLabel(textColumn);
    m_subtitleLabel->setWordWrap(true);
    textLayout->addWidget(m_subtitleLabel);

    innerLayout->addWidget(textColumn, 1);

    // mOverlayIcon = rootView.findViewById(R.id.reddit_post_overlay_icon);
    m_overlayIcon = new QLabel(m_innerView);
    m_overlayIcon->setFixedSize(24, 24);
    m_overlayIcon->setAlignment(Qt::AlignCenter);
    innerLayout->addWidget(m_overlayIcon);

    // mCommentsButton = rootView.findViewById(R.id.reddit_post_comments_button);
    m_commentsButton = new QPushButton(m_outerView);
    outerLayout->addWidget(m_commentsButton);

    // mCommentsText = mCommentsButton.findViewById(R.id.reddit_post_comments_text);
    m_commentsText = new QLabel(m_commentsButton);
    auto *commentLayout = new QHBoxLayout(m_commentsButton);
    commentLayout->addWidget(m_commentsText);

    if (!m_commentsButtonPref) {
        m_commentsButton->setVisible(false);
    }

    // mFooter = rootView.findViewById(R.id.reddit_post_footer);
    m_footer = new QWidget(this);
    auto *footerLayout = new QVBoxLayout(m_footer);
    footerLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(m_footer);

    // Hide image preview initially
    m_imagePreviewOuter->setVisible(false);

    // Thumbnail initial state
    m_thumbnailView->setVisible(false);
}

// --- applyLeftHandedMode ---
// Port of: leftHandedMode block in constructor — reverses inner view child order
void RedditPostView::applyLeftHandedMode() {
    if (!m_innerView) return;

    auto *innerLayout = m_innerView->layout();
    if (!innerLayout) return;

    // Collect children in reverse order, remove them, re-add in original order
    QVector<QWidget *> innerViewElements;
    const int childCount = innerLayout->count();
    for (int i = childCount - 1; i >= 0; i--) {
        auto *item = innerLayout->itemAt(i);
        if (item && item->widget()) {
            innerViewElements.append(item->widget());
            innerLayout->removeWidget(item->widget());
        }
    }

    // Re-add in original order (which is now reversed)
    for (int i = 0; i < innerViewElements.size(); i++) {
        if (auto *boxLayout = qobject_cast<QBoxLayout *>(innerLayout)) {
            boxLayout->addWidget(innerViewElements[i]);
        }
    }

    // setNextFocusRightId(NO_ID), setNextFocusLeftId(mCommentsButton.getId())
    // In Qt, focus is handled via tab order
    if (m_innerView) {
        m_innerView->setFocusPolicy(Qt::NoFocus);
    }
}

// --- setupClickHandlers ---
// Port of: switch(PrefsUtility.pref_behaviour_post_tap_action()) block
void RedditPostView::setupClickHandlers() {
    if (!m_outerView || !m_fragmentParent) return;

    // Long click: show action menu
    // final OnLongClickListener longClickListener = v -> { RedditPostActions.showActionMenu(...); return true; };

    // Port of switch(PrefsUtility.pref_behaviour_post_tap_action())
    const int tapAction = PrefsUtility::prefBehaviourPostTapAction();

    switch (tapAction) {
    case 0: { // LINK
        // mOuterView.setOnClickListener(v -> fragmentParent.onPostSelected(mPost));
        // AndroidCommon.removeClickListeners(mThumbnailView, mImagePreviewOuter, title);
        break;
    }
    case 1: { // COMMENTS
        // mOuterView.setOnClickListener(v -> fragmentParent.onPostCommentsSelected(mPost));
        // mThumbnailView.setOnClickListener(v -> fragmentParent.onPostSelected(mPost));
        // mImagePreviewOuter.setOnClickListener(v -> fragmentParent.onPostSelected(mPost));
        break;
    }
    case 2: { // TITLE_COMMENTS
        // mOuterView.setOnClickListener(v -> fragmentParent.onPostSelected(mPost));
        // title.setOnClickListener(v -> fragmentParent.onPostCommentsSelected(mPost));
        break;
    }
    default:
        break;
    }

    // Comments button click
    if (m_commentsButtonPref && m_commentsButton) {
        // mCommentsButton.setOnClickListener(v -> fragmentParent.onPostCommentsSelected(mPost));
        QObject::connect(m_commentsButton, &QPushButton::clicked, this, [this]() {
            if (m_post && m_fragmentParent) {
                m_fragmentParent->onPostCommentsSelected(m_post);
            }
        });
    }
}

// --- setupFlingHandlers ---
void RedditPostView::setupFlingHandlers() {
    // mLeftFlingPref = PrefsUtility.pref_behaviour_fling_post_left();
    // mRightFlingPref = PrefsUtility.pref_behaviour_fling_post_right();
    const int leftVal = PrefsUtility::prefBehaviourFlingPostLeft();
    const int rightVal = PrefsUtility::prefBehaviourFlingPostRight();

    m_leftFlingPref = static_cast<PostFlingAction>(leftVal);
    m_rightFlingPref = static_cast<PostFlingAction>(rightVal);

    // In Qt, fling would be implemented via QGestureRecognizer or mouse events
    // This is a stub — full implementation depends on FlingableItemView port
}

// --- resizeEvent ---
void RedditPostView::resizeEvent(QResizeEvent *event) {
    QWidget::resizeEvent(event);
    // Update thumbnail size if needed
}

// --- reset ---
// Port of: public void reset(@NonNull RedditPreparedPost newPost)
void RedditPostView::reset(RedditPreparedPost *newPost) {
    if (newPost != m_post) {
        // mThumbnailView.setImageBitmap(null);
        m_thumbnailView->clear();
        // mImagePreviewImageView.setImageBitmap(null);
        m_imagePreviewImageView->clear();
        // mImagePreviewPlayOverlay.setVisibility(GONE);
        m_imagePreviewPlayOverlay->setVisible(false);
        // mPostErrors.removeAllViews();
        if (auto *layout = m_postErrors->layout()) {
            QLayoutItem *item;
            while ((item = layout->takeAt(0)) != nullptr) {
                delete item->widget();
                delete item;
            }
        }
        // mFooter.removeAllViews();
        if (auto *layout = m_footer->layout()) {
            QLayoutItem *item;
            while ((item = layout->takeAt(0)) != nullptr) {
                delete item->widget();
                delete item;
            }
        }

        // mUsageId++;
        m_usageId++;

        // resetSwipeState();
        onSetItemFlingPosition(0);

        // title.setText(newPost.src.getTitle());
        m_titleLabel->setText(newPost->src->title);
        // if(mCommentsButtonPref) mCommentsText.setText(String.valueOf(newPost.src.getSrc().getNum_comments()));
        if (m_commentsButtonPref) {
            m_commentsText->setText(QString::number(newPost->src->commentCount));
        }

        // final boolean showInlinePreview = newPost.shouldShowInlinePreview();
        const bool showInlinePreview = newPost->shouldShowInlinePreview();
        // final boolean showThumbnail = !showInlinePreview && newPost.hasThumbnail;
        const bool showThumbnail = !showInlinePreview && newPost->hasThumbnail;

        if (showInlinePreview) {
            // downloadInlinePreview(newPost, mUsageId);
            downloadInlinePreview(newPost, m_usageId);
        } else {
            // mImagePreviewLoadingSpinner.setVisibility(GONE);
            // mImagePreviewOuter.setVisibility(GONE);
            // setBottomMargin(false);
            if (m_imagePreviewLoadingSpinner) {
                m_imagePreviewLoadingSpinner->setVisible(false);
            }
            m_imagePreviewOuter->setVisible(false);
            setBottomMargin(false);
        }

        if (showThumbnail) {
            // Bitmap thumbnail = newPost.getThumbnail(this, mUsageId);
            // mThumbnailView.setImageBitmap(thumbnail);
            QImage thumbnail = newPost->getThumbnail(this, m_usageId);
            if (!thumbnail.isNull()) {
                m_thumbnailView->setPixmap(QPixmap::fromImage(thumbnail));
            }

            // mThumbnailView.setVisibility(VISIBLE);
            m_thumbnailView->setVisible(true);
            // mThumbnailView.setMinimumWidth(mThumbnailSizePrefPixels);
            m_thumbnailView->setMinimumWidth(m_thumbnailSizePrefPixels);

            // General.setLayoutWidthHeight(mThumbnailView, WRAP_CONTENT, MATCH_PARENT);
            m_thumbnailView->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);

            // mInnerView.setMinimumHeight(mThumbnailSizePrefPixels);
            m_innerView->setMinimumHeight(m_thumbnailSizePrefPixels);
        } else {
            // mThumbnailView.setMinimumWidth(0);
            m_thumbnailView->setMinimumWidth(0);
            // mThumbnailView.setVisibility(GONE);
            m_thumbnailView->setVisible(false);
            // mInnerView.setMinimumHeight(General.dpToPixels(mActivity, 64));
            const int minH = General::dpToPixels(m_activity, 64);
            m_innerView->setMinimumHeight(minH);
        }
    }

    if (m_post) {
        // mPost.unbind(this);
        m_post->unbind(this);
    }

    // newPost.bind(this);
    newPost->bind(this);

    // mPost = newPost;
    m_post = newPost;

    // updateAppearance();
    updateAppearance();
}

// --- updateAppearance ---
// Port of: public void updateAppearance()
void RedditPostView::updateAppearance() {
    if (!m_post) return;

    // mOuterView.setBackgroundResource(R.drawable.rr_postlist_item_selector_main);
    // In Qt: set stylesheet
    m_outerView->setStyleSheet(
        "QWidget { background: palette(base); border-bottom: 1px solid palette(midlight); }");

    // if(mCommentsButtonPref) mCommentsButton.setBackgroundResource(...)
    if (m_commentsButtonPref && m_commentsButton) {
        m_commentsButton->setStyleSheet(
            "QPushButton { background: palette(button); border: 1px solid palette(mid); }");
    }

    // Title color based on read status
    if (m_post->isRead()) {
        // title.setTextColor(rrPostTitleReadCol);
        QPalette pal = m_titleLabel->palette();
        pal.setColor(QPalette::WindowText, m_rrPostTitleReadCol);
        m_titleLabel->setPalette(pal);
    } else {
        // title.setTextColor(rrPostTitleCol);
        QPalette pal = m_titleLabel->palette();
        pal.setColor(QPalette::WindowText, m_rrPostTitleCol);
        m_titleLabel->setPalette(pal);
    }

    // title.setContentDescription(mPost.buildAccessibilityTitle(mActivity, false));
    m_titleLabel->setAccessibleName(m_post->buildAccessibilityTitle(nullptr, false));

    // subtitle.setText(mPost.buildSubtitle(mActivity, false));
    // subtitle.setContentDescription(mPost.buildAccessibilitySubtitle(mActivity, false));
    auto subtitleSs = m_post->buildSubtitle(nullptr, false);
    m_subtitleLabel->setText(QStringLiteral("[subtitle]")); // TODO: render BetterSSB
    m_subtitleLabel->setAccessibleName(m_post->buildAccessibilitySubtitle(nullptr, false));

    // Overlay icon: saved, hidden, upvoted, downvoted
    bool overlayVisible = true;

    if (m_post->isSaved()) {
        // mOverlayIcon.setImageResource(R.drawable.star_dark);
        m_overlayIcon->setText(QStringLiteral("\u2605")); // ★ star
        m_overlayIcon->setStyleSheet("color: #FFD700; font-size: 20px;");

    } else if (m_post->isHidden()) {
        // mOverlayIcon.setImageResource(R.drawable.ic_action_cross_dark);
        m_overlayIcon->setText(QStringLiteral("\u2715")); // ✕ cross
        m_overlayIcon->setStyleSheet("color: #888; font-size: 20px;");

    } else if (m_post->isUpvoted()) {
        // mOverlayIcon.setImageResource(R.drawable.arrow_up_bold_orangered);
        m_overlayIcon->setText(QStringLiteral("\u25B2")); // ▲ up arrow
        m_overlayIcon->setStyleSheet("color: #FF4500; font-size: 20px;");

    } else if (m_post->isDownvoted()) {
        // mOverlayIcon.setImageResource(R.drawable.arrow_down_bold_periwinkle);
        m_overlayIcon->setText(QStringLiteral("\u25BC")); // ▼ down arrow
        m_overlayIcon->setStyleSheet("color: #9494FF; font-size: 20px;");

    } else {
        overlayVisible = false;
    }

    // if(overlayVisible) mOverlayIcon.setVisibility(VISIBLE); else mOverlayIcon.setVisibility(GONE);
    m_overlayIcon->setVisible(overlayVisible);

    // RedditPostActions.INSTANCE.setupAccessibilityActions(mAccessibilityActionManager, mPost, mActivity, false);
    // TODO: integrate when RedditPostActions is ported
}

// --- betterThumbnailAvailable ---
// Port of: public void betterThumbnailAvailable(Bitmap thumbnail, int callbackUsageId)
void RedditPostView::betterThumbnailAvailable(const QImage &thumbnail, int callbackUsageId) {
    // final Message msg = Message.obtain();
    // msg.obj = thumbnail;
    // msg.what = callbackUsageId;
    // thumbnailHandler.sendMessage(msg);
    //
    // Cancel any pending timer, then set image after checking usage ID match
    m_thumbnailHandler->stop();

    // Store the thumbnail and usage ID for the handler
    // In Qt, we use a lambda with a single-shot timer
    QObject::connect(m_thumbnailHandler, &QTimer::timeout, this,
        [this, thumbnail, callbackUsageId]() {
            if (m_usageId != callbackUsageId) {
                return; // View has been recycled
            }
            if (!thumbnail.isNull()) {
                m_thumbnailView->setPixmap(QPixmap::fromImage(thumbnail));
            }
        },
        Qt::SingleShotConnection);

    m_thumbnailHandler->start(0); // Fire immediately (on next event loop iteration)
}

// --- FlingableItemView overrides ---

// onSetItemFlingPosition
void RedditPostView::onSetItemFlingPosition(float position) {
    // mOuterView.setTranslationX(position);
    if (m_outerView) {
        m_outerView->move(static_cast<int>(position), m_outerView->y());
    }
}

// getFlingLeftText
QString RedditPostView::getFlingLeftText() {
    // mLeftFlingAction = RedditPostActions.ActionDescriptionPair.from(mPost, mLeftFlingPref);
    // if(mLeftFlingAction != null) return mActivity.getString(mLeftFlingAction.getDescriptionRes());
    // else return "Disabled";
    if (m_post) {
        // TODO: resolve action description from RedditPostActions
        m_leftFlingActionValid = (m_leftFlingPref != PostFlingAction::DISABLED);
    } else {
        m_leftFlingActionValid = false;
    }
    return m_leftFlingActionValid ? QStringLiteral("Action") : QStringLiteral("Disabled");
}

// getFlingRightText
QString RedditPostView::getFlingRightText() {
    // mRightFlingAction = RedditPostActions.ActionDescriptionPair.from(mPost, mRightFlingPref);
    if (m_post) {
        m_rightFlingActionValid = (m_rightFlingPref != PostFlingAction::DISABLED);
    } else {
        m_rightFlingActionValid = false;
    }
    return m_rightFlingActionValid ? QStringLiteral("Action") : QStringLiteral("Disabled");
}

// allowFlingingLeft
bool RedditPostView::allowFlingingLeft() {
    return m_leftFlingActionValid;
}

// allowFlingingRight
bool RedditPostView::allowFlingingRight() {
    return m_rightFlingActionValid;
}

// onFlungLeft
void RedditPostView::onFlungLeft() {
    // RedditPostActions.INSTANCE.onActionMenuItemSelected(mPost, mActivity, mLeftFlingAction.getAction());
    emit flungLeft();
}

// onFlungRight
void RedditPostView::onFlungRight() {
    // RedditPostActions.INSTANCE.onActionMenuItemSelected(mPost, mActivity, mRightFlingAction.getAction());
    emit flungRight();
}

// --- setBottomMargin (private) ---
// Port of: private void setBottomMargin(boolean enabled)
void RedditPostView::setBottomMargin(bool enabled) {
    if (!m_outerView) return;

    // final MarginLayoutParams layoutParams = (MarginLayoutParams)mOuterView.getLayoutParams();
    // if(enabled) layoutParams.bottomMargin = General.dpToPixels(mActivity, 6);
    // else layoutParams.bottomMargin = 0;
    // mOuterView.setLayoutParams(layoutParams);

    const int margin = enabled ? General::dpToPixels(m_activity, 6) : 0;
    m_outerView->setContentsMargins(
        m_outerView->contentsMargins().left(),
        m_outerView->contentsMargins().top(),
        m_outerView->contentsMargins().right(),
        margin);
}

// --- downloadInlinePreview (private) ---
// Port of: private void downloadInlinePreview(@NonNull RedditPreparedPost post, int usageId)
void RedditPostView::downloadInlinePreview(RedditPreparedPost *post, int usageId) {
    // final Rect windowVisibleDisplayFrame = DisplayUtils.getWindowVisibleDisplayFrame(mActivity);
    // final int screenWidth = Math.min(1080, Math.max(720, windowVisibleDisplayFrame.width()));
    // final int screenHeight = Math.min(2000, Math.max(400, windowVisibleDisplayFrame.height()));

    // In Qt: use screen geometry
    const QRect screenGeo = QApplication::primaryScreen()->availableGeometry();
    const int screenWidth = std::min(1080, std::max(720, screenGeo.width()));
    const int screenHeight = std::min(2000, std::max(400, screenGeo.height()));

    // final RedditParsedPost.ImagePreviewDetails preview = post.src.getPreview(screenWidth, 0);
    // TODO: call getPreview when RedditParsedPost is fully ported
    const int previewWidth = screenWidth;
    const int previewHeight = screenHeight / 2;

    // if(preview == null || preview.width < 10 || preview.height < 10) { ... return; }
    if (previewWidth < 10 || previewHeight < 10) {
        m_imagePreviewOuter->setVisible(false);
        if (m_imagePreviewLoadingSpinner) {
            m_imagePreviewLoadingSpinner->setVisible(false);
        }
        setBottomMargin(false);
        return;
    }

    // final int boundedImageHeight = Math.min((screenHeight * 2) / 3,
    //     (int)(((long)preview.height * screenWidth) / preview.width));
    const int boundedImageHeight = std::min(
        (screenHeight * 2) / 3,
        static_cast<int>((static_cast<long>(previewHeight) * screenWidth) / previewWidth));

    // Layout params: dimensionRatio = screenWidth + ":" + boundedImageHeight
    // In Qt: set fixed aspect ratio
    if (m_imagePreviewHolder) {
        m_imagePreviewHolder->setFixedHeight(boundedImageHeight);
    }

    // mImagePreviewOuter.setVisibility(VISIBLE);
    m_imagePreviewOuter->setVisible(true);
    // mImagePreviewLoadingSpinner.setVisibility(VISIBLE);
    if (m_imagePreviewLoadingSpinner) {
        m_imagePreviewLoadingSpinner->setVisible(true);
    }
    // setBottomMargin(true);
    setBottomMargin(true);

    // CacheManager.getInstance(mActivity).makeRequest(new CacheRequest(
    //     preview.url, RedditAccountManager.getAnon(), null,
    //     new Priority(Constants.Priority.INLINE_IMAGE_PREVIEW),
    //     DownloadStrategyIfNotCached.INSTANCE,
    //     Constants.FileType.INLINE_IMAGE_PREVIEW,
    //     CacheRequest.DownloadQueueType.IMMEDIATE,
    //     mActivity, new CacheRequestCallbacks() { ... }));
    //
    // Callback onDataStreamComplete:
    // - Decode bitmap from stream
    // - Check byte count (reject > 50MB)
    // - Check alreadyAcceptedPrompt
    // - Increment sInlinePreviewsShownThisSession
    // - AndroidCommon.runOnUiThread(() -> { setImageBitmap, loading spinner GONE, play overlay if video })
    // - Show pref prompt every 8 previews starting at 2nd
    //
    // Callback onFailure:
    // - Log error
    // - Add ErrorView to mPostErrors
    //
    // TODO: implement when CacheManager/CacheRequest are fully ported
}

// --- showPrefPrompt (private) ---
// Port of: private void showPrefPrompt()
void RedditPostView::showPrefPrompt() {
    // final SharedPrefsWrapper sharedPrefs = General.getSharedPrefs(mActivity);
    // LayoutInflater.from(mActivity).inflate(R.layout.inline_images_question_view, mFooter, true);
    //
    // final FrameLayout promptView = mFooter.findViewById(R.id.inline_images_prompt_root);
    // final Button keepShowing = mFooter.findViewById(R.id.inline_preview_prompt_keep_showing_button);
    // final Button turnOff = mFooter.findViewById(R.id.inline_preview_prompt_turn_off_button);

    if (!m_footer) return;

    // In Qt, create a simple prompt widget
    auto *promptWidget = new QWidget(m_footer);
    auto *promptLayout = new QVBoxLayout(promptWidget);

    auto *promptLabel = new QLabel(QStringLiteral("Would you like to keep showing inline image previews?"), promptWidget);
    promptLabel->setWordWrap(true);
    promptLayout->addWidget(promptLabel);

    auto *buttonLayout = new QHBoxLayout();
    auto *keepShowingBtn = new QPushButton(QStringLiteral("Keep Showing"), promptWidget);
    auto *turnOffBtn = new QPushButton(QStringLiteral("Turn Off"), promptWidget);
    buttonLayout->addWidget(keepShowingBtn);
    buttonLayout->addWidget(turnOffBtn);
    promptLayout->addLayout(buttonLayout);

    // keepShowing onClickListener:
    // new RRAnimationShrinkHeight(promptView).start();
    // sharedPrefs.edit().putBoolean(PROMPT_PREF_KEY, true).apply();
    QObject::connect(keepShowingBtn, &QPushButton::clicked, this, [this, promptWidget]() {
        promptWidget->setVisible(false);
        promptWidget->deleteLater();
        // In Qt: use QSettings or app settings
        // TODO: persist PROMPT_PREF_KEY = true
    });

    // turnOff onClickListener:
    // sharedPrefs.edit()
    //     .putBoolean(PROMPT_PREF_KEY, true)
    //     .putString(prefPreview, "never")
    //     .apply();
    QObject::connect(turnOffBtn, &QPushButton::clicked, this, [promptWidget]() {
        promptWidget->setVisible(false);
        promptWidget->deleteLater();
        // TODO: persist settings
    });

    // Add to footer
    if (auto *layout = m_footer->layout()) {
        layout->addWidget(promptWidget);
    } else {
        auto *fl = new QVBoxLayout(m_footer);
        fl->addWidget(promptWidget);
    }
}

} // namespace PinkReader
