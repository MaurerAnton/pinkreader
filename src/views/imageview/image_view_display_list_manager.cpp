// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/imageview/ImageViewDisplayListManager.java
#include "views/imageview/image_view_display_list_manager.h"
#include "views/imageview/coordinate_helper.h"
#include "views/imageview/bounds_helper.h"
#include "views/imageview/finger_tracker.h"
#include "views/imageview/multi_scale_tile_manager.h"
#include "views/imageview/image_view_tile_loader.h"
#include "views/imageview/image_view_tile_loader_thread.h"
#include "views/imageview/image_view_scrollbars.h"
#include "views/imageview/image_view_scale_animation.h"
#include "common/mutable_float_point_2d.h"

#include <algorithm>
#include <cmath>
#include <chrono>
#include <cstring>

namespace PinkReader {

// ========================================================================
// Placeholder GL rendering classes (to be replaced by actual implementations)
// ========================================================================

class RRGLDisplayList {
public:
	void add(class RRGLRenderable*) {}
};

class RRGLContext {
public:
	float getScreenDensity() { return 1.0f; }
	int dpToPixels(int dp) { return dp; }
};

class RRGLTexture {
public:
	RRGLTexture(RRGLContext*, Bitmap*, bool) {}
	void releaseReference() {}
};

class RRGLRenderableTranslation : public RRGLRenderable {
public:
	explicit RRGLRenderableTranslation(RRGLRenderable*) {}
	void setPosition(float, float) {}
	void setPosition(const MutableFloatPoint2D& pos) { setPosition(pos.x, pos.y); }
protected:
	void renderInternal(RRGLMatrixStack*, int64_t) override {}
};

class RRGLRenderableScale : public RRGLRenderable {
public:
	explicit RRGLRenderableScale(RRGLRenderable*) {}
	void setScale(float, float) {}
protected:
	void renderInternal(RRGLMatrixStack*, int64_t) override {}
};

class RRGLRenderableGroup : public RRGLRenderable {
public:
	void add(RRGLRenderable*) {}
protected:
	void renderInternal(RRGLMatrixStack*, int64_t) override {}
};

class RRGLRenderableTexturedQuad : public RRGLRenderable {
public:
	RRGLRenderableTexturedQuad(RRGLContext*, RRGLTexture*) {}
	void setTexture(RRGLTexture*) {}
protected:
	void renderInternal(RRGLMatrixStack*, int64_t) override {}
};

class Refreshable {
public:
	virtual void refresh() = 0;
};

class Bitmap {
public:
	int width = 0;
	int height = 0;
	void recycle() {}
};

// ========================================================================
// ImageViewDisplayListManager implementation
// ========================================================================

// Helper for nullable TouchState
bool ImageViewDisplayListManager::hasCurrentTouchState() const {
	return mCurrentTouchStateSet;
}

// Port of: public ImageViewDisplayListManager(final ImageTileSource imageTileSource, final Listener listener)
ImageViewDisplayListManager::ImageViewDisplayListManager(
		ImageTileSource* imageTileSource, Listener* listener)
	// Port of: mImageTileSource = imageTileSource;
	: mImageTileSource(imageTileSource)
	// Port of: mListener = listener;
	, mListener(listener)
	// Port of: mHTileCount = mImageTileSource.getHTileCount();
	, mHTileCount(imageTileSource->getHTileCount())
	// Port of: mVTileCount = mImageTileSource.getVTileCount();
	, mVTileCount(imageTileSource->getVTileCount())
	// Port of: mTileSize = mImageTileSource.getTileSize();
	, mTileSize(imageTileSource->getTileSize())
	// Port of: mCoordinateHelper = new CoordinateHelper()
	, mCoordinateHelper(new CoordinateHelper())
	// Port of: mTmpPoint1_onFingersMoved = new MutableFloatPoint2D()
	, mTmpPoint1_onFingersMoved(new MutableFloatPoint2D())
	// Port of: mTmpPoint2_onFingersMoved = new MutableFloatPoint2D()
	, mTmpPoint2_onFingersMoved(new MutableFloatPoint2D())
{
	// Port of: mTiles = new RRGLRenderableTexturedQuad[mHTileCount][mVTileCount];
	mTiles = new RRGLRenderableTexturedQuad**[static_cast<size_t>(mHTileCount)];
	for (int x = 0; x < mHTileCount; x++) {
		mTiles[x] = new RRGLRenderableTexturedQuad*[static_cast<size_t>(mVTileCount)];
		for (int y = 0; y < mVTileCount; y++) {
			mTiles[x][y] = nullptr;
		}
	}

	// Port of: mTileLoaders = new MultiScaleTileManager[mHTileCount][mVTileCount];
	//          final ImageViewTileLoaderThread thread = new ImageViewTileLoaderThread();
	ImageViewTileLoaderThread* thread = new ImageViewTileLoaderThread();

	mTileLoaders = new MultiScaleTileManager**[static_cast<size_t>(mHTileCount)];
	for (int x = 0; x < mHTileCount; x++) {
		mTileLoaders[x] = new MultiScaleTileManager*[static_cast<size_t>(mVTileCount)];
		for (int y = 0; y < mVTileCount; y++) {
			// Port of: mTileLoaders[x][y] = new MultiScaleTileManager(imageTileSource, thread, x, y, this);
			mTileLoaders[x][y] = new MultiScaleTileManager(imageTileSource, thread, x, y, this);
		}
	}

	// Port of: mDoubleTapGapTimer = new UIThreadRepeatingTimer(50, this)
	// Simplified: nullptr since we don't have a scheduler set up
	mDoubleTapGapTimer = nullptr;
}

ImageViewDisplayListManager::~ImageViewDisplayListManager() {
	delete mCoordinateHelper;
	delete mTmpPoint1_onFingersMoved;
	delete mTmpPoint2_onFingersMoved;
	delete mScaleAnimation;

	for (int x = 0; x < mHTileCount; x++) {
		for (int y = 0; y < mVTileCount; y++) {
			delete mTileLoaders[x][y];
		}
		delete[] mTileLoaders[x];
		delete[] mTiles[x];
		if (mTileVisibility) delete[] mTileVisibility[x];
		if (mTileLoaded) delete[] mTileLoaded[x];
	}
	delete[] mTileLoaders;
	delete[] mTiles;
	if (mTileVisibility) delete[] mTileVisibility;
	if (mTileLoaded) delete[] mTileLoaded;
}

// Port of: @Override public synchronized void onGLSceneCreate(final RRGLDisplayList scene, final RRGLContext glContext, final Refreshable refreshable)
void ImageViewDisplayListManager::onGLSceneCreate(
		RRGLDisplayList* scene,
		RRGLContext* glContext,
		Refreshable* refreshable) {
	// Port of: mTileVisibility = new boolean[mHTileCount][mVTileCount];
	mTileVisibility = new bool*[static_cast<size_t>(mHTileCount)];
	for (int x = 0; x < mHTileCount; x++) {
		mTileVisibility[x] = new bool[static_cast<size_t>(mVTileCount)];
		for (int y = 0; y < mVTileCount; y++) {
			mTileVisibility[x][y] = false;
		}
	}

	// Port of: mTileLoaded = new boolean[mHTileCount][mVTileCount];
	mTileLoaded = new bool*[static_cast<size_t>(mHTileCount)];
	for (int x = 0; x < mHTileCount; x++) {
		mTileLoaded[x] = new bool[static_cast<size_t>(mVTileCount)];
		for (int y = 0; y < mVTileCount; y++) {
			mTileLoaded[x][y] = false;
		}
	}

	// Port of: mRefreshable = refreshable;
	mRefreshable = refreshable;

	// Port of: mScreenDensity = glContext.getScreenDensity();
	mScreenDensity = glContext->getScreenDensity();

	// Port of: mNotLoadedTexture = new RRGLTexture(glContext, NOT_LOADED_BITMAP, false);
	// NOT_LOADED_BITMAP is a static placeholder bitmap
	Bitmap* notLoadedBmp = new Bitmap();
	notLoadedBmp->width = 32;
	notLoadedBmp->height = 32;
	mNotLoadedTexture = new RRGLTexture(glContext, notLoadedBmp, false);
	delete notLoadedBmp;

	// Port of: final RRGLRenderableGroup group = new RRGLRenderableGroup();
	RRGLRenderableGroup* group = new RRGLRenderableGroup();

	// Port of: mOverallScale = new RRGLRenderableScale(group);
	mOverallScale = new RRGLRenderableScale(group);

	// Port of: mOverallTranslation = new RRGLRenderableTranslation(mOverallScale);
	mOverallTranslation = new RRGLRenderableTranslation(mOverallScale);

	// Port of: scene.add(mOverallTranslation);
	scene->add(mOverallTranslation);

	// Port of: for(int x = 0; x < mHTileCount; x++) {
	//             for(int y = 0; y < mVTileCount; y++) { ... }
	//         }
	for (int x = 0; x < mHTileCount; x++) {
		for (int y = 0; y < mVTileCount; y++) {
			// Port of: final RRGLRenderableTexturedQuad quad =
			//             new RRGLRenderableTexturedQuad(glContext, mNotLoadedTexture);
			//          mTiles[x][y] = quad;
			mTiles[x][y] = new RRGLRenderableTexturedQuad(glContext, mNotLoadedTexture);

			// Port of: final RRGLRenderableScale scale = new RRGLRenderableScale(quad);
			RRGLRenderableScale* scale = new RRGLRenderableScale(mTiles[x][y]);

			// Port of: int tileWidth = mTileSize;
			int tileWidth = mTileSize;

			// Port of: int tileHeight = mTileSize;
			int tileHeight = mTileSize;

			// Port of: final int imageWidth = mImageTileSource.getWidth();
			int imageWidth = mImageTileSource->getWidth();

			// Port of: final int imageHeight = mImageTileSource.getHeight();
			int imageHeight = mImageTileSource->getHeight();

			// Port of: if(x == mHTileCount - 1 && imageWidth % mTileSize != 0) {
			//             tileWidth = imageWidth % mTileSize;
			//         }
			if (x == mHTileCount - 1 && imageWidth % mTileSize != 0) {
				tileWidth = imageWidth % mTileSize;
			}

			// Port of: if(y == mVTileCount - 1 && imageHeight % mTileSize != 0) {
			//             tileHeight = imageHeight % mTileSize;
			//         }
			if (y == mVTileCount - 1 && imageHeight % mTileSize != 0) {
				tileHeight = imageHeight % mTileSize;
			}

			// Port of: scale.setScale(tileWidth, tileHeight);
			scale->setScale(static_cast<float>(tileWidth), static_cast<float>(tileHeight));

			// Port of: final RRGLRenderableTranslation translation =
			//             new RRGLRenderableTranslation(scale);
			//          translation.setPosition(x * mTileSize, y * mTileSize);
			RRGLRenderableTranslation* translation = new RRGLRenderableTranslation(scale);
			translation->setPosition(
					static_cast<float>(x * mTileSize),
					static_cast<float>(y * mTileSize));

			// Port of: group.add(translation);
			group->add(translation);
		}
	}

	// Port of: mScrollbars = new ImageViewScrollbars(glContext, mCoordinateHelper,
	//             mImageTileSource.getWidth(), mImageTileSource.getHeight());
	mScrollbars = new ImageViewScrollbars(
			glContext,
			mCoordinateHelper,
			mImageTileSource->getWidth(),
			mImageTileSource->getHeight());

	// Port of: scene.add(mScrollbars);
	scene->add(mScrollbars);
}

// Port of: @Override public synchronized void onGLSceneResolutionChange(final RRGLDisplayList scene,
//         final RRGLContext context, final int width, final int height)
void ImageViewDisplayListManager::onGLSceneResolutionChange(
		RRGLDisplayList* /*scene*/,
		RRGLContext* /*context*/,
		int width,
		int height) {
	// Port of: mResolutionX = width;
	mResolutionX = width;

	// Port of: mResolutionY = height;
	mResolutionY = height;

	// Port of: final boolean setInitialScale = (mBoundsHelper == null);
	bool setInitialScale = (mBoundsHelper == nullptr);

	// Port of: mBoundsHelper = new BoundsHelper(width, height,
	//             mImageTileSource.getWidth(), mImageTileSource.getHeight(), mCoordinateHelper);
	mBoundsHelper = new BoundsHelper(
			width, height,
			mImageTileSource->getWidth(), mImageTileSource->getHeight(),
			*mCoordinateHelper);

	// Port of: if(setInitialScale) { mBoundsHelper.applyMinScale(); }
	if (setInitialScale) {
		mBoundsHelper->applyMinScale();
	}

	// Port of: mScrollbars.setResolution(width, height);
	mScrollbars->setResolution(width, height);

	// Port of: mScrollbars.showBars();
	mScrollbars->showBars();
}

// Port of: @Override public synchronized boolean onGLSceneUpdate(final RRGLDisplayList scene, final RRGLContext context)
bool ImageViewDisplayListManager::onGLSceneUpdate(RRGLDisplayList* /*scene*/, RRGLContext* context) {
	// Port of: if(mScaleAnimation != null) {
	//             if(!mScaleAnimation.onStep()) { mScaleAnimation = null; }
	//         }
	if (mScaleAnimation != nullptr) {
		if (!mScaleAnimation->onStep()) {
			delete mScaleAnimation;
			mScaleAnimation = nullptr;
		}
	}

	// Port of: if(mBoundsHelper != null) { mBoundsHelper.applyBounds(); }
	if (mBoundsHelper != nullptr) {
		mBoundsHelper->applyBounds();
	}

	// Port of: final MutableFloatPoint2D positionOffset = mCoordinateHelper.getPositionOffset();
	MutableFloatPoint2D& positionOffset = mCoordinateHelper->getPositionOffset();

	// Port of: final float scale = mCoordinateHelper.getScale();
	float scale = mCoordinateHelper->getScale();

	// Port of: mOverallTranslation.setPosition(positionOffset);
	mOverallTranslation->setPosition(positionOffset);

	// Port of: mOverallScale.setScale(scale, scale);
	mOverallScale->setScale(scale, scale);

	// Port of: mScrollbars.update();
	mScrollbars->update();

	// Port of: final int sampleSize = pickSampleSize();
	int sampleSize = pickSampleSize();

	// Port of: if(mLastSampleSize != sampleSize) {
	if (mLastSampleSize != sampleSize) {
		// Port of: for(final boolean[] arr : mTileLoaded) { Arrays.fill(arr, false); }
		for (int x = 0; x < mHTileCount; x++) {
			for (int y = 0; y < mVTileCount; y++) {
				mTileLoaded[x][y] = false;
			}
		}

		// Port of: mLastSampleSize = sampleSize;
		mLastSampleSize = sampleSize;
	}

	// Port of: final float firstVisiblePixelX = -positionOffset.x / scale;
	float firstVisiblePixelX = -positionOffset.x / scale;

	// Port of: final float firstVisiblePixelY = -positionOffset.y / scale;
	float firstVisiblePixelY = -positionOffset.y / scale;

	// Port of: final int firstVisibleTileX = (int)Math.floor(firstVisiblePixelX / mTileSize);
	int firstVisibleTileX = static_cast<int>(std::floor(firstVisiblePixelX / static_cast<float>(mTileSize)));

	// Port of: final int firstVisibleTileY = (int)Math.floor(firstVisiblePixelY / mTileSize);
	int firstVisibleTileY = static_cast<int>(std::floor(firstVisiblePixelY / static_cast<float>(mTileSize)));

	// Port of: final float lastVisiblePixelX = firstVisiblePixelX + (float)mResolutionX / scale;
	float lastVisiblePixelX = firstVisiblePixelX + static_cast<float>(mResolutionX) / scale;

	// Port of: final float lastVisiblePixelY = firstVisiblePixelY + (float)mResolutionY / scale;
	float lastVisiblePixelY = firstVisiblePixelY + static_cast<float>(mResolutionY) / scale;

	// Port of: final int lastVisibleTileX = (int)Math.ceil(lastVisiblePixelX / mTileSize);
	int lastVisibleTileX = static_cast<int>(std::ceil(lastVisiblePixelX / static_cast<float>(mTileSize)));

	// Port of: final int lastVisibleTileY = (int)Math.ceil(lastVisiblePixelY / mTileSize);
	int lastVisibleTileY = static_cast<int>(std::ceil(lastVisiblePixelY / static_cast<float>(mTileSize)));

	// Port of: final int desiredScaleIndex = MultiScaleTileManager.sampleSizeToScaleIndex(sampleSize);
	int desiredScaleIndex = MultiScaleTileManager::sampleSizeToScaleIndex(sampleSize);

	// Port of: for(int x = 0; x < mHTileCount; x++) {
	//             for(int y = 0; y < mVTileCount; y++) { ... }
	//         }
	for (int x = 0; x < mHTileCount; x++) {
		for (int y = 0; y < mVTileCount; y++) {
			// Port of: final boolean isTileVisible =
			//             x >= firstVisibleTileX && y >= firstVisibleTileY
			//               && x <= lastVisibleTileX && y <= lastVisibleTileY;
			bool isTileVisible =
					x >= firstVisibleTileX
					&& y >= firstVisibleTileY
					&& x <= lastVisibleTileX
					&& y <= lastVisibleTileY;

			// Port of: final boolean isTileWanted =
			//             x >= firstVisibleTileX - 1 && y >= firstVisibleTileY - 1
			//               && x <= lastVisibleTileX + 1 && y <= lastVisibleTileY + 1;
			bool isTileWanted =
					x >= firstVisibleTileX - 1
					&& y >= firstVisibleTileY - 1
					&& x <= lastVisibleTileX + 1
					&& y <= lastVisibleTileY + 1;

			// Port of: if(isTileWanted && !mTileLoaded[x][y]) {
			//             mTileLoaders[x][y].markAsWanted(desiredScaleIndex);
			//         } else {
			//             mTileLoaders[x][y].markAsUnwanted();
			//         }
			if (isTileWanted && !mTileLoaded[x][y]) {
				mTileLoaders[x][y]->markAsWanted(desiredScaleIndex);
			} else {
				mTileLoaders[x][y]->markAsUnwanted();
			}

			// Port of: if(isTileVisible != mTileVisibility[x][y] || !mTileLoaded[x][y]) {
			if (isTileVisible != mTileVisibility[x][y] || !mTileLoaded[x][y]) {
				// Port of: if(isTileVisible && !mTileLoaded[x][y]) {
				if (isTileVisible && !mTileLoaded[x][y]) {
					// Port of: final Bitmap tile = mTileLoaders[x][y].getAtDesiredScale();
					Bitmap* tile = mTileLoaders[x][y]->getAtDesiredScale();

					// Port of: if(tile != null) {
					if (tile != nullptr) {
						// Port of: try {
						//             final RRGLTexture texture = new RRGLTexture(context, tile, true);
						//             mTiles[x][y].setTexture(texture);
						//             texture.releaseReference();
						//             mTileLoaded[x][y] = true;
						//             tile.recycle();
						//         } catch(final Exception e) {
						//             Log.e("ImageViewDisplayListMan", "Exception when creating texture", e);
						//         }
						try {
							RRGLTexture* texture = new RRGLTexture(context, tile, true);
							mTiles[x][y]->setTexture(texture);
							texture->releaseReference();
							mTileLoaded[x][y] = true;
							tile->recycle();
						} catch (...) {
							// Exception creating texture -- silently handled per original
						}
					}

				// Port of: } else if(!isTileWanted) {
				} else if (!isTileWanted) {
					// Port of: mTiles[x][y].setTexture(mNotLoadedTexture);
					mTiles[x][y]->setTexture(mNotLoadedTexture);
				}

				// Port of: mTileVisibility[x][y] = isTileVisible;
				mTileVisibility[x][y] = isTileVisible;
			}
		}
	}

	// Port of: if(mScaleAnimation != null) { mScrollbars.showBars(); }
	if (mScaleAnimation != nullptr) {
		mScrollbars->showBars();
	}

	// Port of: return mScaleAnimation != null;
	return mScaleAnimation != nullptr;
}

// Port of: @Override public void onUIAttach()
void ImageViewDisplayListManager::onUIAttach() {
	// Empty in original
}

// Port of: @Override public void onUIDetach()
void ImageViewDisplayListManager::onUIDetach() {
	// Port of: mImageTileSource.dispose();
	mImageTileSource->dispose();
}

// Port of: @Override public synchronized void onFingerDown(final FingerTracker.Finger finger)
void ImageViewDisplayListManager::onFingerDown(FingerTracker::Finger& finger) {
	// Port of: if(mScrollbars == null) { return; }
	if (mScrollbars == nullptr) {
		return;
	}

	// Port of: mScaleAnimation = null;
	delete mScaleAnimation;
	mScaleAnimation = nullptr;

	// Port of: mScrollbars.showBars();
	mScrollbars->showBars();

	// Port of: if(mCurrentTouchState == null) {
	if (!hasCurrentTouchState()) {
		// Port of: mCurrentTouchState = TouchState.ONE_FINGER_DOWN;
		mCurrentTouchStateVal = TouchState::ONE_FINGER_DOWN;
		mCurrentTouchStateSet = true;

		// Port of: mDragFinger = finger;
		mDragFinger = &finger;

	// Port of: } else {
	} else {
		// Port of: switch(mCurrentTouchState) {
		switch (mCurrentTouchStateVal) {
			// Port of: case DOUBLE_TAP_WAIT_NO_FINGERS_DOWN:
			case TouchState::DOUBLE_TAP_WAIT_NO_FINGERS_DOWN:
				// Port of: mCurrentTouchState = TouchState.DOUBLE_TAP_ONE_FINGER_DOWN;
				mCurrentTouchStateVal = TouchState::DOUBLE_TAP_ONE_FINGER_DOWN;

				// Port of: mDragFinger = finger;
				mDragFinger = &finger;

				// Port of: mDoubleTapGapTimer.stopTimer();
				if (mDoubleTapGapTimer) {
					mDoubleTapGapTimer->stopTimer();
				}
				break;

			// Port of: case ONE_FINGER_DRAG:
			case TouchState::ONE_FINGER_DRAG:
				// Port of: mListener.onHorizontalSwipeEnd();
				mListener->onHorizontalSwipeEnd();
				// Deliberate fallthrough

			// Port of: case ONE_FINGER_DOWN:
			//          case DOUBLE_TAP_ONE_FINGER_DOWN:
			//          case DOUBLE_TAP_ONE_FINGER_DRAG:
			case TouchState::ONE_FINGER_DOWN:
			case TouchState::DOUBLE_TAP_ONE_FINGER_DOWN:
			case TouchState::DOUBLE_TAP_ONE_FINGER_DRAG:
				// Port of: mCurrentTouchState = TouchState.TWO_FINGER_PINCH;
				mCurrentTouchStateVal = TouchState::TWO_FINGER_PINCH;

				// Port of: mPinchFinger1 = mDragFinger;
				mPinchFinger1 = mDragFinger;

				// Port of: mPinchFinger2 = finger;
				mPinchFinger2 = &finger;

				// Port of: mDragFinger = null;
				mDragFinger = nullptr;
				break;

			// Port of: default:
			default:
				// Port of: mSpareFingers.push(finger);
				mSpareFingers.push(&finger);
				break;
		}
	}
}

// Port of: @Override public synchronized void onFingersMoved()
void ImageViewDisplayListManager::onFingersMoved() {
	// Port of: if(mCurrentTouchState == null) { return; }
	if (!hasCurrentTouchState()) {
		return;
	}

	// Port of: if(mScrollbars == null) { return; }
	if (mScrollbars == nullptr) {
		return;
	}

	// Port of: mScaleAnimation = null;
	delete mScaleAnimation;
	mScaleAnimation = nullptr;

	// Port of: mScrollbars.showBars();
	mScrollbars->showBars();

	// Port of: switch(mCurrentTouchState) {
	switch (mCurrentTouchStateVal) {
		// Port of: case DOUBLE_TAP_ONE_FINGER_DOWN: {
		case TouchState::DOUBLE_TAP_ONE_FINGER_DOWN: {
			// Port of: if(mDragFinger.mTotalPosDifference.distanceSquared()
			//             >= 400f * mScreenDensity * mScreenDensity) {
			if (mDragFinger->mTotalPosDifference.distanceSquared()
					>= 400.0f * mScreenDensity * mScreenDensity) {
				// Port of: mCurrentTouchState = TouchState.DOUBLE_TAP_ONE_FINGER_DRAG;
				mCurrentTouchStateVal = TouchState::DOUBLE_TAP_ONE_FINGER_DRAG;
			}
			break;
		}

		// Port of: case DOUBLE_TAP_ONE_FINGER_DRAG: {
		case TouchState::DOUBLE_TAP_ONE_FINGER_DRAG: {
			// Port of: final MutableFloatPoint2D screenCentre = mTmpPoint1_onFingersMoved;
			//          screenCentre.set(mResolutionX / 2, mResolutionY / 2);
			mTmpPoint1_onFingersMoved->set(
					static_cast<float>(mResolutionX) / 2.0f,
					static_cast<float>(mResolutionY) / 2.0f);

			// Port of: mCoordinateHelper.scaleAboutScreenPoint(
			//             screenCentre,
			//             (float)Math.pow(1.01, mDragFinger.mPosDifference.y / mScreenDensity));
			mCoordinateHelper->scaleAboutScreenPoint(
					*mTmpPoint1_onFingersMoved,
					static_cast<float>(std::pow(
							1.01,
							static_cast<double>(mDragFinger->mPosDifference.y / mScreenDensity))));
			break;
		}

		// Port of: case ONE_FINGER_DOWN: {
		case TouchState::ONE_FINGER_DOWN: {
			// Port of: if(mDragFinger.mTotalPosDifference.distanceSquared()
			//             >= 100f * mScreenDensity * mScreenDensity) {
			if (mDragFinger->mTotalPosDifference.distanceSquared()
					>= 100.0f * mScreenDensity * mScreenDensity) {
				// Port of: mCurrentTouchState = TouchState.ONE_FINGER_DRAG;
				mCurrentTouchStateVal = TouchState::ONE_FINGER_DRAG;
			}
			// Deliberate fall-through
		}

		// Port of: case ONE_FINGER_DRAG:
		case TouchState::ONE_FINGER_DRAG:
			// Port of: if(mBoundsHelper.isMinScale()) {
			if (mBoundsHelper->isMinScale()) {
				// Port of: mListener.onHorizontalSwipe(mDragFinger.mTotalPosDifference.x);
				mListener->onHorizontalSwipe(mDragFinger->mTotalPosDifference.x);

			// Port of: } else {
			} else {
				// Port of: mCoordinateHelper.translateScreen(mDragFinger.mLastPos, mDragFinger.mCurrentPos);
				mCoordinateHelper->translateScreen(mDragFinger->mLastPos, mDragFinger->mCurrentPos);
			}
			break;

		// Port of: case TWO_FINGER_PINCH: {
		case TouchState::TWO_FINGER_PINCH: {
			// Port of: final double oldDistance = mPinchFinger1.mLastPos.euclideanDistanceTo(mPinchFinger2.mLastPos);
			double oldDistance = mPinchFinger1->mLastPos.euclideanDistanceTo(mPinchFinger2->mLastPos);

			// Port of: final double newDistance = mPinchFinger1.mCurrentPos.euclideanDistanceTo(mPinchFinger2.mCurrentPos);
			double newDistance = mPinchFinger1->mCurrentPos.euclideanDistanceTo(mPinchFinger2->mCurrentPos);

			// Port of: final MutableFloatPoint2D oldCentre = mTmpPoint1_onFingersMoved;
			//          mPinchFinger1.mLastPos.add(mPinchFinger2.mLastPos, oldCentre);
			//          oldCentre.scale(0.5);
			mPinchFinger1->mLastPos.add(mPinchFinger2->mLastPos, *mTmpPoint1_onFingersMoved);
			mTmpPoint1_onFingersMoved->scale(0.5);

			// Port of: final MutableFloatPoint2D newCentre = mTmpPoint2_onFingersMoved;
			//          mPinchFinger1.mCurrentPos.add(mPinchFinger2.mCurrentPos, newCentre);
			//          newCentre.scale(0.5);
			mPinchFinger1->mCurrentPos.add(mPinchFinger2->mCurrentPos, *mTmpPoint2_onFingersMoved);
			mTmpPoint2_onFingersMoved->scale(0.5);

			// Port of: final float scaleDifference = (float)(newDistance / oldDistance);
			float scaleDifference = static_cast<float>(newDistance / oldDistance);

			// Port of: mCoordinateHelper.scaleAboutScreenPoint(newCentre, scaleDifference);
			mCoordinateHelper->scaleAboutScreenPoint(*mTmpPoint2_onFingersMoved, scaleDifference);

			// Port of: mCoordinateHelper.translateScreen(oldCentre, newCentre);
			mCoordinateHelper->translateScreen(*mTmpPoint1_onFingersMoved, *mTmpPoint2_onFingersMoved);
			break;
		}

		default:
			break;
	}
}

// Port of: @Override public synchronized void onFingerUp(final FingerTracker.Finger finger)
void ImageViewDisplayListManager::onFingerUp(FingerTracker::Finger& finger) {
	// Port of: if(mScrollbars == null) { return; }
	if (mScrollbars == nullptr) {
		return;
	}

	// Port of: mScaleAnimation = null;
	delete mScaleAnimation;
	mScaleAnimation = nullptr;

	// Port of: mScrollbars.showBars();
	mScrollbars->showBars();

	// Port of: if(mSpareFingers.remove(finger)) { return; }
	if (mSpareFingers.remove(&finger)) {
		return;
	}

	// Port of: if(mCurrentTouchState == null) { return; }
	if (!hasCurrentTouchState()) {
		return;
	}

	// Port of: switch(mCurrentTouchState) {
	switch (mCurrentTouchStateVal) {
		// Port of: case DOUBLE_TAP_ONE_FINGER_DOWN:
		case TouchState::DOUBLE_TAP_ONE_FINGER_DOWN:
			// Port of: if(finger.mDownDuration < TAP_MAX_DURATION_MS) {
			if (finger.mDownDuration < TAP_MAX_DURATION_MS) {
				// Port of: onDoubleTap(finger.mCurrentPos);
				onDoubleTap(finger.mCurrentPos);
			}

			// Port of: mCurrentTouchState = null;
			mCurrentTouchStateSet = false;

			// Port of: mDragFinger = null;
			mDragFinger = nullptr;
			break;

		// Port of: case ONE_FINGER_DOWN:
		case TouchState::ONE_FINGER_DOWN:
			// Port of: if(finger.mDownDuration < TAP_MAX_DURATION_MS) {
			if (finger.mDownDuration < TAP_MAX_DURATION_MS) {
				// Port of: // Maybe a single tap
				//          mDoubleTapGapTimer.startTimer();
				if (mDoubleTapGapTimer) {
					mDoubleTapGapTimer->startTimer();
				}

				// Port of: mCurrentTouchState = TouchState.DOUBLE_TAP_WAIT_NO_FINGERS_DOWN;
				mCurrentTouchStateVal = TouchState::DOUBLE_TAP_WAIT_NO_FINGERS_DOWN;

				// Port of: mFirstTapReleaseTime = System.currentTimeMillis();
				auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
						std::chrono::system_clock::now().time_since_epoch()).count();
				mFirstTapReleaseTime = static_cast<int64_t>(now);

			// Port of: } else {
			} else {
				// Port of: mCurrentTouchState = null;
				mCurrentTouchStateSet = false;
			}

			// Port of: mDragFinger = null;
			mDragFinger = nullptr;
			break;

		// Port of: case ONE_FINGER_DRAG:
		case TouchState::ONE_FINGER_DRAG:
			// Port of: mListener.onHorizontalSwipeEnd();
			mListener->onHorizontalSwipeEnd();
			// Deliberate fallthrough

		// Port of: case DOUBLE_TAP_ONE_FINGER_DRAG:
		case TouchState::DOUBLE_TAP_ONE_FINGER_DRAG:
			// Port of: if(mSpareFingers.isEmpty()) {
			if (mSpareFingers.isEmpty()) {
				// Port of: mCurrentTouchState = null;
				mCurrentTouchStateSet = false;

				// Port of: mDragFinger = null;
				mDragFinger = nullptr;

			// Port of: } else {
			} else {
				// Port of: mDragFinger = mSpareFingers.pop();
				mDragFinger = mSpareFingers.pop();
			}
			break;

		// Port of: case TWO_FINGER_PINCH:
		case TouchState::TWO_FINGER_PINCH:
			// Port of: if(mSpareFingers.isEmpty()) {
			if (mSpareFingers.isEmpty()) {
				// Port of: mCurrentTouchState = TouchState.ONE_FINGER_DRAG;
				mCurrentTouchStateVal = TouchState::ONE_FINGER_DRAG;

				// Port of: mDragFinger = (mPinchFinger1 == finger) ? mPinchFinger2 : mPinchFinger1;
				mDragFinger = (mPinchFinger1 == &finger) ? mPinchFinger2 : mPinchFinger1;

				// Port of: mPinchFinger1 = null;
				mPinchFinger1 = nullptr;

				// Port of: mPinchFinger2 = null;
				mPinchFinger2 = nullptr;

			// Port of: } else {
			} else {
				// Port of: if(mPinchFinger1 == finger) {
				if (mPinchFinger1 == &finger) {
					// Port of: mPinchFinger1 = mSpareFingers.pop();
					mPinchFinger1 = mSpareFingers.pop();

				// Port of: } else {
				} else {
					// Port of: mPinchFinger2 = mSpareFingers.pop();
					mPinchFinger2 = mSpareFingers.pop();
				}
			}
			break;

		default:
			break;
	}
}

// Port of: private void onDoubleTap(final MutableFloatPoint2D position)
void ImageViewDisplayListManager::onDoubleTap(const MutableFloatPoint2D& position) {
	// Port of: final float minScale = mBoundsHelper.getMinScale();
	float minScale = mBoundsHelper->getMinScale();

	// Port of: final float currentScale = mCoordinateHelper.getScale();
	float currentScale = mCoordinateHelper->getScale();

	// Port of: float targetScale;
	float targetScale;

	// Port of: if(currentScale > minScale * 1.01) {
	if (currentScale > minScale * 1.01f) {
		// Port of: targetScale = minScale;
		targetScale = minScale;

	// Port of: } else {
	} else {
		// Port of: targetScale = Math.max(
		//             (float)mResolutionX / (float)mImageTileSource.getWidth(),
		//             (float)mResolutionY / (float)mImageTileSource.getHeight());
		targetScale = std::max(
				static_cast<float>(mResolutionX) / static_cast<float>(mImageTileSource->getWidth()),
				static_cast<float>(mResolutionY) / static_cast<float>(mImageTileSource->getHeight()));

		// Port of: if(Math.abs((targetScale / currentScale) - 1.0) < 0.05) {
		if (std::abs((targetScale / currentScale) - 1.0f) < 0.05f) {
			// Port of: targetScale = currentScale * 3;
			targetScale = currentScale * 3.0f;
		}
	}

	// Port of: mScaleAnimation = new ImageViewScaleAnimation(targetScale, mCoordinateHelper, 15, position);
	delete mScaleAnimation;
	mScaleAnimation = new ImageViewScaleAnimation(targetScale, *mCoordinateHelper, 15, position);
}

// Port of: @Override public void onUIThreadRepeatingTimer(final UIThreadRepeatingTimer timer)
void ImageViewDisplayListManager::onUIThreadRepeatingTimer(UIThreadRepeatingTimer* /*timer*/) {
	// Port of: if(mCurrentTouchState == TouchState.DOUBLE_TAP_WAIT_NO_FINGERS_DOWN) {
	if (hasCurrentTouchState() && mCurrentTouchStateVal == TouchState::DOUBLE_TAP_WAIT_NO_FINGERS_DOWN) {
		// Port of: if(System.currentTimeMillis() - mFirstTapReleaseTime > DOUBLE_TAP_MAX_GAP_DURATION_MS) {
		auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
				std::chrono::system_clock::now().time_since_epoch()).count();
		int64_t currentMs = static_cast<int64_t>(now);

		if (currentMs - mFirstTapReleaseTime > DOUBLE_TAP_MAX_GAP_DURATION_MS) {
			// Port of: mListener.onSingleTap();
			mListener->onSingleTap();

			// Port of: mCurrentTouchState = null;
			mCurrentTouchStateSet = false;

			// Port of: mDoubleTapGapTimer.stopTimer();
			if (mDoubleTapGapTimer) {
				mDoubleTapGapTimer->stopTimer();
			}
		}

	// Port of: } else {
	} else {
		// Port of: mDoubleTapGapTimer.stopTimer();
		if (mDoubleTapGapTimer) {
			mDoubleTapGapTimer->stopTimer();
		}
	}
}

// Port of: private int pickSampleSize()
int ImageViewDisplayListManager::pickSampleSize() {
	// Port of: int result = 1;
	int result = 1;

	// Port of: while(result <= MultiScaleTileManager.MAX_SAMPLE_SIZE
	//             && (1.0 / (result * 2)) > mCoordinateHelper.getScale()) {
	//             result *= 2;
	//         }
	while (result <= MultiScaleTileManager::MAX_SAMPLE_SIZE
			&& (1.0f / static_cast<float>(result * 2)) > mCoordinateHelper->getScale()) {
		result *= 2;
	}

	// Port of: return result;
	return result;
}

// Port of: @Override public void onTileLoaded(final int x, final int y, final int sampleSize)
void ImageViewDisplayListManager::onTileLoaded(int /*x*/, int /*y*/, int /*sampleSize*/) {
	// Port of: mRefreshable.refresh();
	if (mRefreshable) {
		mRefreshable->refresh();
	}
}

// Port of: @Override public void onTileLoaderOutOfMemory()
void ImageViewDisplayListManager::onTileLoaderOutOfMemory() {
	// Port of: mListener.onImageViewDLMOutOfMemory();
	mListener->onImageViewDLMOutOfMemory();
}

// Port of: @Override public void onTileLoaderException(const Throwable t)
void ImageViewDisplayListManager::onTileLoaderException(const char* error) {
	// Port of: mListener.onImageViewDLMException(t);
	mListener->onImageViewDLMException(error);
}

// Port of: public void resetTouchState()
void ImageViewDisplayListManager::resetTouchState() {
	// Port of: mCurrentTouchState = null;
	mCurrentTouchStateSet = false;
}

// Port of: BasicGestureHandler.Listener methods
void ImageViewDisplayListManager::onSingleTap() {
	mListener->onSingleTap();
}

void ImageViewDisplayListManager::onHorizontalSwipe(float pixels) {
	mListener->onHorizontalSwipe(pixels);
}

void ImageViewDisplayListManager::onHorizontalSwipeEnd() {
	mListener->onHorizontalSwipeEnd();
}

} // namespace PinkReader
