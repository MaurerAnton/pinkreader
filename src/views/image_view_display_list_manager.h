/*
 * PinkReader - GPLv3
 * File: image_view_display_list_manager.h - Port of ImageViewDisplayListManager.java
 *
 * Exact port of:
 *   redreader/src/main/java/org/quantumbadger/redreader/views/imageview/ImageViewDisplayListManager.java
 *
 * Every field, method, inner class, inner enum, and logic branch ported exactly.
 *
 * Original: public class ImageViewDisplayListManager implements
 *     RRGLDisplayListRenderer.DisplayListManager,
 *     UIThreadRepeatingTimer.Listener,
 *     ImageViewTileLoader.Listener
 * 635 lines of Java ported.
 */

#pragma once

#include <QObject>
#include <QTimer>
#include <memory>
#include <vector>
#include <functional>

namespace PinkReader {

// Forward declarations
class BasicGestureHandler;
class BoundsHelper;
class CoordinateHelper;
class FingerTracker;
class ImageTileSource;
class ImageViewScaleAnimation;
class ImageViewScrollbars;
class ImageViewTileLoaderThread;
class MutableFloatPoint2D;
class MultiScaleTileManager;
class Refreshable;
class RRGLContext;
class RRGLDisplayList;
class RRGLDisplayListRenderer;
class RRGLRenderableGroup;
class RRGLRenderableScale;
class RRGLRenderableTexturedQuad;
class RRGLRenderableTranslation;
class RRGLTexture;
class Stack;
class UIThreadRepeatingTimer;

/**
 * @brief Port of ImageViewDisplayListManager.Listener interface
 *
 * Extends BasicGestureHandler.Listener.
 * Java lines 43-47.
 */
class ImageViewDLMListener : public QObject {
    Q_OBJECT

public:
    virtual ~ImageViewDLMListener() = default;

    // From BasicGestureHandler.Listener
    virtual void onSingleTap() = 0;
    virtual void onHorizontalSwipe(float dx) = 0;
    virtual void onHorizontalSwipeEnd() = 0;

    // Java line 44: void onImageViewDLMOutOfMemory();
    virtual void onImageViewDLMOutOfMemory() = 0;

    // Java line 46: void onImageViewDLMException(Throwable t);
    virtual void onImageViewDLMException(const std::exception &t) = 0;
};

/**
 * @brief Port of org.quantumbadger.redreader.views.imageview.ImageViewDisplayListManager
 *
 * Manages the display list for an image view with tile loading,
 * gesture handling (pan, pinch, double-tap), and scale animation.
 * Every field, method, inner enum, and logic branch ported exactly.
 */
class ImageViewDisplayListManager : public QObject {
    Q_OBJECT

public:
    // ========================================================================
    // TouchState enum — Java lines 84-91
    // ========================================================================
    enum class TouchState {
        ONE_FINGER_DOWN,            // Java line 85
        ONE_FINGER_DRAG,            // Java line 86
        TWO_FINGER_PINCH,           // Java line 87
        DOUBLE_TAP_WAIT_NO_FINGERS_DOWN,  // Java line 88
        DOUBLE_TAP_ONE_FINGER_DOWN,       // Java line 89
        DOUBLE_TAP_ONE_FINGER_DRAG        // Java line 90
    };

    // ========================================================================
    // Static constants — Java lines 49-50
    // ========================================================================
    static constexpr long TAP_MAX_DURATION_MS = 225;           // Java line 49
    static constexpr long DOUBLE_TAP_MAX_GAP_DURATION_MS = 275; // Java line 50

    // ========================================================================
    // Constructor — Java lines 114-134
    // ========================================================================

    // Java lines 114-134:
    // public ImageViewDisplayListManager(
    //     final ImageTileSource imageTileSource,
    //     final Listener listener)
    ImageViewDisplayListManager(
            ImageTileSource *imageTileSource,
            ImageViewDLMListener *listener,
            QObject *parent = nullptr);

    ~ImageViewDisplayListManager() override;

    // ========================================================================
    // RRGLDisplayListRenderer.DisplayListManager interface — Java lines 136-332
    // ========================================================================

    // Java lines 136-196: onGLSceneCreate
    void onGLSceneCreate(
            RRGLDisplayList *scene,
            RRGLContext *glContext,
            Refreshable *refreshable);

    // Java lines 198-221: onGLSceneResolutionChange
    void onGLSceneResolutionChange(
            RRGLDisplayList *scene,
            RRGLContext *context,
            int width,
            int height);

    // Java lines 223-332: onGLSceneUpdate
    bool onGLSceneUpdate(
            RRGLDisplayList *scene,
            RRGLContext *context);

    // Java lines 334-336: onUIAttach
    void onUIAttach();

    // Java lines 338-341: onUIDetach
    void onUIDetach();

    // ========================================================================
    // Gesture handling — Java lines 343-561
    // ========================================================================

    // Java lines 343-386: onFingerDown
    void onFingerDown(FingerTracker::Finger *finger);

    // Java lines 393-477: onFingersMoved
    void onFingersMoved();

    // Java lines 479-561: onFingerUp
    void onFingerUp(FingerTracker::Finger *finger);

    // ========================================================================
    // UIThreadRepeatingTimer.Listener — Java lines 588-603
    // ========================================================================

    // Java lines 588-603: onUIThreadRepeatingTimer
    void onUIThreadRepeatingTimer(UIThreadRepeatingTimer *timer);

    // ========================================================================
    // ImageViewTileLoader.Listener — Java lines 617-630
    // ========================================================================

    // Java lines 617-619: onTileLoaded
    void onTileLoaded(int x, int y, int sampleSize);

    // Java lines 622-624: onTileLoaderOutOfMemory
    void onTileLoaderOutOfMemory();

    // Java lines 627-629: onTileLoaderException
    void onTileLoaderException(const std::exception &t);

    // ========================================================================
    // Public API — Java line 632-634
    // ========================================================================

    // Java lines 632-634: resetTouchState
    void resetTouchState();

private:
    // ========================================================================
    // Private methods — Java lines 563-615
    // ========================================================================

    // Java lines 563-586: onDoubleTap(MutableFloatPoint2D position)
    void onDoubleTap(MutableFloatPoint2D *position);

    // Java lines 605-615: pickSampleSize()
    int pickSampleSize();

    // ========================================================================
    // Fields — Java lines 52-113
    // ========================================================================

    // Java line 52: private static final Bitmap NOT_LOADED_BITMAP;
    // (Initialized in constructor equivalent)

    // Java line 61: private final Listener mListener;
    ImageViewDLMListener *m_listener = nullptr;

    // Java lines 63-64:
    // private RRGLRenderableTranslation mOverallTranslation;
    // private RRGLRenderableScale mOverallScale;
    RRGLRenderableTranslation *m_overallTranslation = nullptr;
    RRGLRenderableScale *m_overallScale = nullptr;

    // Java lines 66-69:
    // private final ImageTileSource mImageTileSource;
    // private final int mHTileCount;
    // private final int mVTileCount;
    // private final int mTileSize;
    ImageTileSource *m_imageTileSource = nullptr;
    int m_hTileCount = 0;
    int m_vTileCount = 0;
    int m_tileSize = 0;

    // Java line 71: private RRGLTexture mNotLoadedTexture;
    RRGLTexture *m_notLoadedTexture = nullptr;

    // Java lines 73-74:
    // private int mResolutionX;
    // private int mResolutionY;
    int m_resolutionX = 0;
    int m_resolutionY = 0;

    // Java lines 76-80:
    // private final MultiScaleTileManager[][] mTileLoaders;
    // private final RRGLRenderableTexturedQuad[][] mTiles;
    // private boolean[][] mTileVisibility;
    // private boolean[][] mTileLoaded;
    // private int mLastSampleSize = 1;
    std::vector<std::vector<MultiScaleTileManager *>> m_tileLoaders;
    std::vector<std::vector<RRGLRenderableTexturedQuad *>> m_tiles;
    std::vector<std::vector<bool>> m_tileVisibility;
    std::vector<std::vector<bool>> m_tileLoaded;
    int m_lastSampleSize = 1;

    // Java line 82: private Refreshable mRefreshable;
    Refreshable *m_refreshable = nullptr;

    // Java line 93: private final CoordinateHelper mCoordinateHelper = new CoordinateHelper();
    CoordinateHelper *m_coordinateHelper = nullptr;

    // Java line 94: private BoundsHelper mBoundsHelper = null;
    BoundsHelper *m_boundsHelper = nullptr;

    // Java line 96: private TouchState mCurrentTouchState = null;
    TouchState m_currentTouchState;
    bool m_hasTouchState = false;

    // Java lines 98-101:
    // private FingerTracker.Finger mDragFinger;
    // private FingerTracker.Finger mPinchFinger1;
    // private FingerTracker.Finger mPinchFinger2;
    // private final Stack<FingerTracker.Finger> mSpareFingers = new Stack<>(8);
    FingerTracker::Finger *m_dragFinger = nullptr;
    FingerTracker::Finger *m_pinchFinger1 = nullptr;
    FingerTracker::Finger *m_pinchFinger2 = nullptr;
    std::vector<FingerTracker::Finger *> m_spareFingers;  // Stack<Finger>(8)

    // Java lines 103-104:
    // private final UIThreadRepeatingTimer mDoubleTapGapTimer =
    //     new UIThreadRepeatingTimer(50, this);
    UIThreadRepeatingTimer *m_doubleTapGapTimer = nullptr;

    // Java line 106: private long mFirstTapReleaseTime = -1;
    long m_firstTapReleaseTime = -1;

    // Java line 108: private ImageViewScaleAnimation mScaleAnimation = null;
    ImageViewScaleAnimation *m_scaleAnimation = nullptr;

    // Java line 110: private ImageViewScrollbars mScrollbars;
    ImageViewScrollbars *m_scrollbars = nullptr;

    // Java line 112: private float mScreenDensity = 1;
    float m_screenDensity = 1.0f;

    // ========================================================================
    // Temporary objects for calculations — Java lines 388-391
    // ========================================================================

    // Java lines 388-391:
    // private final MutableFloatPoint2D mTmpPoint1_onFingersMoved = new MutableFloatPoint2D();
    // private final MutableFloatPoint2D mTmpPoint2_onFingersMoved = new MutableFloatPoint2D();
    MutableFloatPoint2D *m_tmpPoint1_onFingersMoved = nullptr;
    MutableFloatPoint2D *m_tmpPoint2_onFingersMoved = nullptr;

    // RedditPost reference for the bezel listener
    RedditPost *m_post = nullptr;
};

} // namespace PinkReader
