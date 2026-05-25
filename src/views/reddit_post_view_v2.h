// Origin: org.quantumbadger.redreader.views.RedditPostView.java
// Ported from RedReader (GPLv3)
#pragma once

#include <atomic>
#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace PinkReader {

// Forward declarations
class RedditPreparedPost;
class RedditParsedPost;
class PostListingFragment;
class BaseActivity;
class AccessibilityActionManager;
class LoadingSpinnerView;
class ErrorView;
class SeekableInputStream;
struct Rect;

// GenericFactory equivalent
template<typename T>
using GenericFactory = std::function<std::unique_ptr<T>()>;

class RedditPostView {
public:
    // --- PostSelectionListener interface ---
    struct PostSelectionListener {
        virtual ~PostSelectionListener() = default;
        virtual void onPostSelected(RedditPreparedPost* post) = 0;
        virtual void onPostCommentsSelected(RedditPreparedPost* post) = 0;
    };

    // --- ThumbnailLoadedCallback (from RedditPreparedPost) ---
    class ThumbnailLoadedCallback {
    public:
        virtual ~ThumbnailLoadedCallback() = default;
        virtual void betterThumbnailAvailable(void* bitmap, int callbackUsageId) = 0;
    };

    // --- Constructor ---
    RedditPostView(
            void* context,                        // Context*
            PostListingFragment* fragmentParent,
            BaseActivity* activity,
            bool leftHandedMode);

    // --- Public API ---
    void reset(RedditPreparedPost* newPost);
    void updateAppearance();
    void betterThumbnailAvailable(void* bitmap, int callbackUsageId);

    // --- FlingableItemView overrides ---
    std::string getFlingLeftText();
    std::string getFlingRightText();
    bool allowFlingingLeft();
    bool allowFlingingRight();
    void onFlungLeft();
    void onFlungRight();
    void onSetItemFlingPosition(float position);

private:
    // --- Constants ---
    static const std::string PROMPT_PREF_KEY;  // "inline_image_prompt_accepted"
    static std::atomic<int> s_inlinePreviewsShownThisSession;

    // --- Fields ---
    RedditPreparedPost* m_post = nullptr;
    void* m_title = nullptr;                     // TextView*
    void* m_subtitle = nullptr;                  // TextView*
    void* m_thumbnailView = nullptr;             // ImageView*
    void* m_overlayIcon = nullptr;               // ImageView*
    void* m_outerView = nullptr;                 // LinearLayout*
    void* m_innerView = nullptr;                 // LinearLayout*
    void* m_commentsButton = nullptr;            // LinearLayout*
    void* m_commentsText = nullptr;              // TextView*
    void* m_postErrors = nullptr;                // LinearLayout*
    void* m_imagePreviewHolder = nullptr;        // FrameLayout*
    void* m_imagePreviewImageView = nullptr;     // ImageView*
    void* m_imagePreviewPlayOverlay = nullptr;   // ConstraintLayout*
    void* m_imagePreviewOuter = nullptr;         // LinearLayout*
    LoadingSpinnerView* m_imagePreviewLoadingSpinner = nullptr;
    void* m_footer = nullptr;                    // LinearLayout*
    AccessibilityActionManager* m_accessibilityActionManager = nullptr;

    int m_usageId = 0;

    void* m_thumbnailHandler = nullptr;          // Handler (Android message passing)

    BaseActivity* m_activity;

    int m_leftFlingPref;                         // PrefsUtility.PostFlingAction
    int m_rightFlingPref;                        // PrefsUtility.PostFlingAction
    void* m_leftFlingAction = nullptr;           // RedditPostActions.ActionDescriptionPair
    void* m_rightFlingAction = nullptr;

    bool m_commentsButtonPref;

    int m_rrPostTitleReadCol;
    int m_rrPostTitleCol;
    int m_thumbnailSizePrefPixels;

    // --- Private methods ---
    void setBottomMargin(bool enabled);
    void downloadInlinePreview(RedditPreparedPost* post, int usageId);
    void showPrefPrompt();
};

} // namespace PinkReader
