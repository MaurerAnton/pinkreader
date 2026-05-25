// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/imageview/ImageViewDisplayListManager.java
#pragma once

#include "views/imageview/basic_gesture_handler.h"
#include "views/imageview/image_tile_source.h"
#include "common/collections/stack.h"
#include "common/ui_thread_repeating_timer.h"

#include <cstdint>
#include <memory>

namespace PinkReader {

// Forward declarations
class CoordinateHelper;
class BoundsHelper;
class FingerTracker;
class MultiScaleTileManager;
class ImageViewTileLoader;
class ImageViewTileLoaderThread;
class ImageViewScrollbars;
class ImageViewScaleAnimation;
class MutableFloatPoint2D;

// GL rendering forward declarations (to be implemented)
class RRGLDisplayList;
class RRGLContext;
class RRGLTexture;
class RRGLRenderableTranslation;
class RRGLRenderableScale;
class RRGLRenderableGroup;
class RRGLRenderableTexturedQuad;
class Refreshable;
class RRGLDisplayListRenderer;
class Bitmap;

/**
 * Port of org.quantumbadger.redreader.views.imageview.ImageViewDisplayListManager
 * implements RRGLDisplayListRenderer.DisplayListManager,
 *            UIThreadRepeatingTimer.Listener,
 *            ImageViewTileLoader.Listener
 */
class ImageViewDisplayListManager
		: public BasicGestureHandler::Listener
		, public UIThreadRepeatingTimer::Listener
		, public ImageViewTileLoader::Listener {
public:
	// Port of: public interface Listener extends BasicGestureHandler.Listener
	class Listener : public BasicGestureHandler::Listener {
	public:
		~Listener() override = default;

		// Port of: void onImageViewDLMOutOfMemory()
		virtual void onImageViewDLMOutOfMemory() = 0;

		// Port of: void onImageViewDLMException(Throwable t)
		virtual void onImageViewDLMException(const char* error) = 0;
	};

	// Port of: private static final long TAP_MAX_DURATION_MS = 225;
	static constexpr int64_t TAP_MAX_DURATION_MS = 225;

	// Port of: private static final long DOUBLE_TAP_MAX_GAP_DURATION_MS = 275;
	static constexpr int64_t DOUBLE_TAP_MAX_GAP_DURATION_MS = 275;

	// Port of: private enum TouchState
	enum class TouchState {
		ONE_FINGER_DOWN,
		ONE_FINGER_DRAG,
		TWO_FINGER_PINCH,
		DOUBLE_TAP_WAIT_NO_FINGERS_DOWN,
		DOUBLE_TAP_ONE_FINGER_DOWN,
		DOUBLE_TAP_ONE_FINGER_DRAG
	};

	// Port of: public ImageViewDisplayListManager(final ImageTileSource imageTileSource, final Listener listener)
	ImageViewDisplayListManager(ImageTileSource* imageTileSource, Listener* listener);

	~ImageViewDisplayListManager();

	// Port of: @Override public synchronized void onGLSceneCreate(final RRGLDisplayList scene, final RRGLContext glContext, final Refreshable refreshable)
	void onGLSceneCreate(RRGLDisplayList* scene, RRGLContext* glContext, Refreshable* refreshable);

	// Port of: @Override public synchronized void onGLSceneResolutionChange(final RRGLDisplayList scene, final RRGLContext context, final int width, final int height)
	void onGLSceneResolutionChange(RRGLDisplayList* scene, RRGLContext* context, int width, int height);

	// Port of: @Override public synchronized boolean onGLSceneUpdate(final RRGLDisplayList scene, final RRGLContext context)
	bool onGLSceneUpdate(RRGLDisplayList* scene, RRGLContext* context);

	// Port of: @Override public void onUIAttach()
	void onUIAttach();

	// Port of: @Override public void onUIDetach()
	void onUIDetach();

	// Port of: @Override public synchronized void onFingerDown(final FingerTracker.Finger finger)
	void onFingerDown(FingerTracker::Finger& finger);

	// Port of: @Override public synchronized void onFingersMoved()
	void onFingersMoved();

	// Port of: @Override public synchronized void onFingerUp(final FingerTracker.Finger finger)
	void onFingerUp(FingerTracker::Finger& finger);

	// Port of: @Override public void onUIThreadRepeatingTimer(final UIThreadRepeatingTimer timer)
	void onUIThreadRepeatingTimer(UIThreadRepeatingTimer* timer) override;

	// Port of: @Override public void onTileLoaded(final int x, final int y, final int sampleSize)
	void onTileLoaded(int x, int y, int sampleSize) override;

	// Port of: @Override public void onTileLoaderOutOfMemory()
	void onTileLoaderOutOfMemory() override;

	// Port of: @Override public void onTileLoaderException(const Throwable t)
	void onTileLoaderException(const char* error) override;

	// Port of: public void resetTouchState()
	void resetTouchState();

	// Port of: BasicGestureHandler.Listener methods
	void onSingleTap() override;
	void onHorizontalSwipe(float pixels) override;
	void onHorizontalSwipeEnd() override;

private:
	// Port of: private void onDoubleTap(final MutableFloatPoint2D position)
	void onDoubleTap(const MutableFloatPoint2D& position);

	// Port of: private int pickSampleSize()
	int pickSampleSize();

	// Port of: private final Listener mListener
	Listener* mListener;

	// Port of: private RRGLRenderableTranslation mOverallTranslation
	RRGLRenderableTranslation* mOverallTranslation = nullptr;

	// Port of: private RRGLRenderableScale mOverallScale
	RRGLRenderableScale* mOverallScale = nullptr;

	// Port of: private final ImageTileSource mImageTileSource
	ImageTileSource* mImageTileSource;

	// Port of: private final int mHTileCount
	int mHTileCount;

	// Port of: private final int mVTileCount
	int mVTileCount;

	// Port of: private final int mTileSize
	int mTileSize;

	// Port of: private RRGLTexture mNotLoadedTexture
	RRGLTexture* mNotLoadedTexture = nullptr;

	// Port of: private int mResolutionX
	int mResolutionX = 0;

	// Port of: private int mResolutionY
	int mResolutionY = 0;

	// Port of: private final MultiScaleTileManager[][] mTileLoaders
	MultiScaleTileManager*** mTileLoaders = nullptr;

	// Port of: private final RRGLRenderableTexturedQuad[][] mTiles
	RRGLRenderableTexturedQuad*** mTiles = nullptr;

	// Port of: private boolean[][] mTileVisibility
	bool** mTileVisibility = nullptr;

	// Port of: private boolean[][] mTileLoaded
	bool** mTileLoaded = nullptr;

	// Port of: private int mLastSampleSize = 1
	int mLastSampleSize = 1;

	// Port of: private Refreshable mRefreshable
	Refreshable* mRefreshable = nullptr;

	// Port of: private final CoordinateHelper mCoordinateHelper = new CoordinateHelper()
	CoordinateHelper* mCoordinateHelper = nullptr;

	// Port of: private BoundsHelper mBoundsHelper = null
	BoundsHelper* mBoundsHelper = nullptr;

	// Port of: private TouchState mCurrentTouchState = null
	TouchState mCurrentTouchStateSet = false;
	TouchState mCurrentTouchStateVal;
	bool hasCurrentTouchState() const;

	// Port of: private FingerTracker.Finger mDragFinger
	FingerTracker::Finger* mDragFinger = nullptr;

	// Port of: private FingerTracker.Finger mPinchFinger1
	FingerTracker::Finger* mPinchFinger1 = nullptr;

	// Port of: private FingerTracker.Finger mPinchFinger2
	FingerTracker::Finger* mPinchFinger2 = nullptr;

	// Port of: private final Stack<FingerTracker.Finger> mSpareFingers = new Stack<>(8)
	Stack<FingerTracker::Finger*> mSpareFingers{8};

	// Port of: private final UIThreadRepeatingTimer mDoubleTapGapTimer = new UIThreadRepeatingTimer(50, this)
	// Simplified: uses direct scheduling
	std::shared_ptr<UIThreadRepeatingTimer> mDoubleTapGapTimer;

	// Port of: private long mFirstTapReleaseTime = -1
	int64_t mFirstTapReleaseTime = -1;

	// Port of: private ImageViewScaleAnimation mScaleAnimation = null
	ImageViewScaleAnimation* mScaleAnimation = nullptr;

	// Port of: private ImageViewScrollbars mScrollbars
	ImageViewScrollbars* mScrollbars = nullptr;

	// Port of: private float mScreenDensity = 1
	float mScreenDensity = 1.0f;

	// Port of: private final MutableFloatPoint2D mTmpPoint1_onFingersMoved = new MutableFloatPoint2D()
	MutableFloatPoint2D* mTmpPoint1_onFingersMoved = nullptr;

	// Port of: private final MutableFloatPoint2D mTmpPoint2_onFingersMoved = new MutableFloatPoint2D()
	MutableFloatPoint2D* mTmpPoint2_onFingersMoved = nullptr;
};

} // namespace PinkReader
