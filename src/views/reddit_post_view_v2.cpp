// Origin: org.quantumbadger.redreader.views.RedditPostView.java
// Ported from RedReader (GPLv3)

#include "views/reddit_post_view_v2.h"

#include <algorithm>
#include <string>
#include <atomic>

namespace PinkReader {

const std::string RedditPostView::PROMPT_PREF_KEY = "inline_image_prompt_accepted";
std::atomic<int> RedditPostView::s_inlinePreviewsShownThisSession{0};

// --- Constructor ---

RedditPostView::RedditPostView(
        void* context,
        PostListingFragment* fragmentParent,
        BaseActivity* activity,
        bool leftHandedMode)
    : m_activity(activity) {
    // Original constructor (~160 lines):
    //
    // 1. super(context);
    // 2. Create AccessibilityActionManager(this, context.getResources())
    // 3. Create thumbnailHandler (Handler with Looper.getMainLooper()):
    //    - handleMessage: if mUsageId matches msg.what, set thumbnail bitmap
    // 4. Inflate R.layout.reddit_post layout, find all views:
    //    - mOuterView (reddit_post_layout_outer)
    //    - mInnerView (reddit_post_layout_inner)
    //    - mPostErrors, mImagePreviewHolder, mImagePreviewImageView
    //    - mImagePreviewPlayOverlay, mImagePreviewOuter, mFooter
    //    - Create LoadingSpinnerView and add to mImagePreviewHolder
    //    - mThumbnailView, mOverlayIcon, title, subtitle
    // 5. Setup comments button preference and visibility
    // 6. Left-handed mode: reverse child views, fix focus IDs
    // 7. Set up click/listeners based on PrefsUtility.pref_behaviour_post_tap_action():
    //    - LINK: mOuterView -> onPostSelected, remove other click listeners
    //    - COMMENTS: mOuterView -> onPostCommentsSelected,
    //                mThumbnailView/mImagePreviewOuter -> onPostSelected
    //    - TITLE_COMMENTS: mOuterView -> onPostSelected, title -> onPostCommentsSelected
    // 8. mOuterView long click -> showActionMenu
    // 9. Comments button -> onPostCommentsSelected
    // 10. Apply font scaling to title and subtitle
    // 11. Read left/right fling preferences
    // 12. Read title color attributes (rrPostTitleCol, rrPostTitleReadCol)
    // 13. Compute mThumbnailSizePrefPixels from dp

    (void)context;
    (void)fragmentParent;
    (void)leftHandedMode;
}

// --- reset ---

void RedditPostView::reset(RedditPreparedPost* newPost) {
    if (newPost != m_post) {
        // Clear old state:
        // mThumbnailView.setImageBitmap(null)
        // mImagePreviewImageView.setImageBitmap(null)
        // mImagePreviewPlayOverlay.setVisibility(GONE)
        // mPostErrors.removeAllViews()
        // mFooter.removeAllViews()
        // mUsageId++

        // resetSwipeState()

        // title.setText(newPost.src.getTitle())
        // Update comments count text if enabled

        // Determine show/hide inline preview vs thumbnail
        // If showInlinePreview: downloadInlinePreview(newPost, mUsageId)
        // Else: hide loading spinner, hide image preview outer, setBottomMargin(false)

        // If showThumbnail: get thumbnail bitmap, set visibility, size
        // Else: hide thumbnail, set min height to 64dp

        // Unbind old post, bind new post
        if (m_post != nullptr) {
            // m_post.unbind(this);
        }
        // newPost.bind(this);
        m_post = newPost;

        updateAppearance();
    }
}

// --- updateAppearance ---

void RedditPostView::updateAppearance() {
    // In original:
    // 1. Set mOuterView background (rr_postlist_item_selector_main)
    // 2. Set comments button background
    // 3. Title color: read vs unread
    // 4. Update title and subtitle content descriptions
    // 5. Update overlay icon based on post state:
    //    - isSaved() -> star_dark
    //    - isHidden() -> ic_action_cross_dark
    //    - isUpvoted() -> arrow_up_bold_orangered
    //    - isDownvoted() -> arrow_down_bold_periwinkle
    //    - else: overlayVisible = false
    // 6. Show/hide overlay icon
    // 7. RedditPostActions.INSTANCE.setupAccessibilityActions()
}

// --- betterThumbnailAvailable ---

void RedditPostView::betterThumbnailAvailable(void* bitmap, int callbackUsageId) {
    // In original:
    // final Message msg = Message.obtain();
    // msg.obj = thumbnail;
    // msg.what = callbackUsageId;
    // thumbnailHandler.sendMessage(msg);
    (void)bitmap;
    (void)callbackUsageId;
}

// --- FlingableItemView overrides ---

std::string RedditPostView::getFlingLeftText() {
    // In original:
    // mLeftFlingAction = RedditPostActions.ActionDescriptionPair.from(mPost, mLeftFlingPref);
    // if(mLeftFlingAction != null) return mActivity.getString(mLeftFlingAction.getDescriptionRes());
    // else return "Disabled";
    return "Disabled";
}

std::string RedditPostView::getFlingRightText() {
    // mRightFlingAction = RedditPostActions.ActionDescriptionPair.from(mPost, mRightFlingPref);
    return "Disabled";
}

bool RedditPostView::allowFlingingLeft() {
    return m_leftFlingAction != nullptr;
}

bool RedditPostView::allowFlingingRight() {
    return m_rightFlingAction != nullptr;
}

void RedditPostView::onFlungLeft() {
    // RedditPostActions.INSTANCE.onActionMenuItemSelected(mPost, mActivity, mLeftFlingAction.getAction());
}

void RedditPostView::onFlungRight() {
    // RedditPostActions.INSTANCE.onActionMenuItemSelected(mPost, mActivity, mRightFlingAction.getAction());
}

void RedditPostView::onSetItemFlingPosition(float position) {
    // mOuterView.setTranslationX(position);
    (void)position;
}

// --- setBottomMargin ---

void RedditPostView::setBottomMargin(bool enabled) {
    // In original:
    // final MarginLayoutParams layoutParams = (MarginLayoutParams)mOuterView.getLayoutParams();
    // if(enabled) layoutParams.bottomMargin = General.dpToPixels(mActivity, 6);
    // else layoutParams.bottomMargin = 0;
    // mOuterView.setLayoutParams(layoutParams);
    (void)enabled;
}

// --- downloadInlinePreview ---

void RedditPostView::downloadInlinePreview(RedditPreparedPost* post, int usageId) {
    // In original (~130 lines):
    // 1. Get window visible display frame, compute screenWidth/Height
    // 2. Get ImagePreviewDetails from post.src.getPreview(screenWidth, 0)
    // 3. If null or tiny, hide preview
    // 4. Compute boundedImageHeight from aspect ratio
    // 5. Set constraint layout dimension ratio
    // 6. Show preview outer + loading spinner, setBottomMargin(true)
    // 7. Create CacheRequest with:
    //    - preview.url, anon account, INLINE_IMAGE_PREVIEW priority
    //    - DownloadStrategyIfNotCached, IMMEDIATE queue
    //    - CacheRequestCallbacks:
    //      - onDataStreamComplete: decode bitmap, check size < 50MB,
    //        check preview prompt every 8th showing, set image
    //      - onFailure: log error, hide preview, show ErrorView
    (void)post;
    (void)usageId;
}

// --- showPrefPrompt ---

void RedditPostView::showPrefPrompt() {
    // In original (~40 lines):
    // 1. Inflate inline_images_question_view into mFooter
    // 2. Find promptRoot, keepShowing button, turnOff button
    // 3. keepShowing: animate shrink, save accepted pref
    // 4. turnOff: animate, set preview pref to "never"
}

} // namespace PinkReader
