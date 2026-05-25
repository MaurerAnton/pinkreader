/*
 * PinkReader - Open source Reddit client
 * Copyright (C) 2024-2026 PinkReader Contributors - GPLv3
 * File: image_view_activity.h - Port of ImageViewActivity.java
 *
 * Line-by-line translation of:
 *   redreader/src/main/java/org/quantumbadger/redreader/activities/ImageViewActivity.java
 *
 * Original: public class ImageViewActivity extends ViewsBaseActivity
 *     implements RedditPostView.PostSelectionListener,
 *                ImageViewDisplayListManager.Listener
 *
 * Every field, method, constant, and inner class ported exactly.
 */

#pragma once

#include <QObject>
#include <QString>
#include <memory>
#include <atomic>
#include <vector>
#include <functional>

// Forward declarations for Android/RedReader types used in signatures
namespace PinkReader {

// Activity base classes
class ViewsBaseActivity;

// Android framework stubs
class Bundle;
class Intent;
class TextView;
class ImageView;
class ImageButton;
class FrameLayout;
class LinearLayout;
class RelativeLayout;
class ViewGroup;
class WindowManager;
class Configuration;
class View;

// RedReader views
class ExoPlayerWrapperView;
class HorizontalSwipeProgressOverlay;
class ImageViewDisplayListManager;
class GIFView;
class GifDecoderThread;

// RedReader models
class RedditPost;
class RedditPreparedPost;
class RedditParsedPost;
class ImageInfo;
class AlbumInfo;
class UriString;

// Preferences
class PrefsUtility;

// Managers
class CacheManager;

// Cache
class CacheRequest;
class CacheRequestCallbacks;
class DownloadStrategyIfNotCached;
class Priority;
class RRError;
class SeekableInputStream;
class GenericFactory;

// Link handler
class LinkHandler;

// Image
class GifDecoderThread;
class GetImageInfoListener;
class GetAlbumInfoListener;

// Common
class General;

// DonutProgress
class DonutProgress;

// RedditPostActions
class RedditPostActions;

// Bezel overlay
class BezelSwipeOverlay;
class SideToolbarOverlay;

// ErrorView
class ErrorView;

// Enums (nested in PrefsUtility)
namespace PrefsEnums {
    enum class AppearanceStatusBarMode;
    enum class VideoViewMode;
    enum class GifViewMode;
    enum class ImageViewMode;
}

// Timestamp
class TimestampUTC;

/**
 * @brief Port of org.quantumbadger.redreader.activities.ImageViewActivity
 *
 * Full-screen image/video viewer activity. Every field, method, constant ported exactly.
 */
class ImageViewActivity : public QObject {
    Q_OBJECT

public:
    // ===== Static constants (mirrors Java exactly) =====

    // private static final String TAG = "ImageViewActivity";
    static const QString TAG;

    // ===== Constructor / Lifecycle =====
    explicit ImageViewActivity(QObject *parent = nullptr);
    virtual ~ImageViewActivity();

    // ===== Activity lifecycle (port of overrides) =====
    void onCreate(Bundle *savedInstanceState);
    void onPause();
    void onResume();
    void onDestroy();
    void onBackPressed();
    void onConfigurationChanged(Configuration *newConfig);

    // ===== Override methods (from ViewsBaseActivity) =====
    virtual bool baseActivityIsToolbarActionBarEnabled();

    // ===== Interface method stubs (port of implemented interfaces) =====

    // RedditPostView.PostSelectionListener
    virtual void onPostSelected(RedditPreparedPost *post);
    virtual void onPostCommentsSelected(RedditPreparedPost *post);

    // ImageViewDisplayListManager.Listener
    virtual void onSingleTap();
    virtual void onHorizontalSwipe(float pixels);
    virtual void onHorizontalSwipeEnd();
    virtual void onImageViewDLMOutOfMemory();
    virtual void onImageViewDLMException(void *t); // Throwable

private:
    // ===== Private fields (port of private fields, Java lines 112-148) =====

    // private TextView mProgressText;
    TextView *mProgressText = nullptr;

    // private GLSurfaceView surfaceView;
    void *surfaceView = nullptr; // GLSurfaceView stub

    // private ImageView imageView;
    ImageView *imageView = nullptr;

    // private GifDecoderThread gifThread;
    GifDecoderThread *gifThread = nullptr;

    // private ExoPlayerWrapperView mVideoPlayerWrapper;
    ExoPlayerWrapperView *mVideoPlayerWrapper = nullptr;

    // private UriString mUrl;
    UriString *mUrl = nullptr; // or stored as value; stub

    // private boolean mIsPaused = true;
    bool mIsPaused = true;

    // private boolean mIsDestroyed = false;
    bool mIsDestroyed = false;

    // @NonNull private final ArrayList<Runnable> mActionsOnResume = new ArrayList<>();
    std::vector<std::function<void()>> mActionsOnResume;

    // @Nullable private CacheRequest mImageOrVideoRequest;
    CacheRequest *mImageOrVideoRequest = nullptr;

    // @Nullable private CacheRequest mAudioRequest;
    CacheRequest *mAudioRequest = nullptr;

    // private boolean mHaveReverted = false;
    bool mHaveReverted = false;

    // private ImageViewDisplayListManager mImageViewDisplayerManager;
    ImageViewDisplayListManager *mImageViewDisplayerManager = nullptr;

    // private HorizontalSwipeProgressOverlay mSwipeOverlay;
    HorizontalSwipeProgressOverlay *mSwipeOverlay = nullptr;

    // private boolean mSwipeCancelled;
    bool mSwipeCancelled = false;

    // private RedditPost mPost;
    RedditPost *mPost = nullptr;

    // private ImageInfo mImageInfo;
    ImageInfo *mImageInfo = nullptr;

    // private AlbumInfo mAlbumInfo;
    AlbumInfo *mAlbumInfo = nullptr;

    // private int mAlbumImageIndex;
    int mAlbumImageIndex = 0;

    // private FrameLayout mLayout;
    FrameLayout *mLayout = nullptr;

    // private int mGallerySwipeLengthPx;
    int mGallerySwipeLengthPx = 0;

    // @Nullable private LinearLayout mFloatingToolbar;
    LinearLayout *mFloatingToolbar = nullptr;

    // ===== Private methods (port exactly from Java) =====

    // private void setMainView(final View v)  -- Java line 421
    void setMainView(View *v);

    // private void onImageStreamReady(...)  -- Java line 432
    void onImageStreamReady(bool isNetwork,
                            GenericFactory *videoStream,
                            GenericFactory *audioStream,
                            const QString &mimetype,
                            void *videoStreamUri); // Uri

    // private void revertToWeb()  -- Java line 581
    void revertToWeb();

    // private void openInExternalBrowser()  -- Java line 611
    void openInExternalBrowser();

    // private void openImage(final DonutProgress progressBar,
    //     final UriString uri, @Nullable final UriString audioUri)  -- Java line 807
    void openImage(DonutProgress *progressBar,
                   UriString *uri,
                   UriString *audioUri);

    // private void manageAspectRatioIndicator(final DonutProgress progressBar)  -- Java line 868
    void manageAspectRatioIndicator(DonutProgress *progressBar);

    // private void makeCacheRequest(final DonutProgress progressBar,
    //     final UriString uri, @Nullable final UriString audioUri)  -- Java line 886
    void makeCacheRequest(DonutProgress *progressBar,
                          UriString *uri,
                          UriString *audioUri);

    // @Nullable private ImageButton addFloatingToolbarButton(...)  -- Java line 1047
    ImageButton *addFloatingToolbarButton(int drawable, int description,
                                          std::function<void(View*)> listener);

    // private void launchVlc(@NonNull final Uri uri)  -- Java line 1076
    void launchVlc(void *uri); // Uri

    // private void playWithExoplayer(...)  -- Java line 1097
    void playWithExoplayer(bool isNetwork,
                           GenericFactory *videoStream,
                           GenericFactory *audioStream);

    // private void playGIFWithMovie(...)  -- Java line 1204
    void playGIFWithMovie(GenericFactory *streamFactory);

    // private void playGIFWithLegacyDecoder(...)  -- Java line 1258
    void playGIFWithLegacyDecoder(GenericFactory *streamFactory);

    // private void showImageWithInternalViewer(...)  -- Java line 1318
    void showImageWithInternalViewer(GenericFactory *streamFactory);

    // private void cancelCacheRequests()  -- Java line 680
    void cancelCacheRequests();
};

} // namespace PinkReader
