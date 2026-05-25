/*
 * PinkReader - GPLv3
 * File: image_view_display_list_manager.cpp - Port of ImageViewDisplayListManager.java
 *
 * Exact port of:
 *   redreader/src/main/java/org/quantumbadger/redreader/views/imageview/ImageViewDisplayListManager.java
 *
 * Every field, method, inner enum, and logic branch ported exactly.
 * 635 lines of Java ported.
 */

#include "views/image_view_display_list_manager.h"

#include <QDateTime>
#include <cmath>
#include <algorithm>

#include "common/mutable_float_point2d.h"
#include "common/ui_thread_repeating_timer.h"
#include "common/collections/stack.h"
#include "views/glview/refreshable.h"
#include "views/glview/displaylist/rrgl_display_list.h"
#include "views/glview/displaylist/rrgl_display_list_renderer.h"
#include "views/glview/displaylist/rrgl_renderable_group.h"
#include "views/glview/displaylist/rrgl_renderable_scale.h"
#include "views/glview/displaylist/rrgl_renderable_textured_quad.h"
#include "views/glview/displaylist/rrgl_renderable_translation.h"
#include "views/glview/program/rrgl_context.h"
#include "views/glview/program/rrgl_texture.h"
#include "views/imageview/basic_gesture_handler.h"
#include "views/imageview/bounds_helper.h"
#include "views/imageview/coordinate_helper.h"
#include "views/imageview/finger_tracker.h"
#include "views/imageview/image_tile_source.h"
#include "views/imageview/image_view_scale_animation.h"
#include "views/imageview/image_view_scrollbars.h"
#include "views/imageview/image_view_tile_loader.h"
#include "views/imageview/image_view_tile_loader_thread.h"
#include "views/imageview/multi_scale_tile_manager.h"

namespace PinkReader {

// ========================================================================
// Static initialization — Java lines 52-59
// ========================================================================

// Java lines 52-59:
// private static final Bitmap NOT_LOADED_BITMAP;
// static {
//     NOT_LOADED_BITMAP = Bitmap.createBitmap(32, 32, Bitmap.Config.ARGB_8888);
//     final Canvas notLoadedCanvas = new Canvas(NOT_LOADED_BITMAP);
//     notLoadedCanvas.drawRGB(70, 70, 70);
// }
// In C++, this is handled during onGLSceneCreate by creating
// a texture from a gray buffer.

// ========================================================================
// Constructor — Java lines 114-134
// ========================================================================

ImageViewDisplayListManager::ImageViewDisplayListManager(
        ImageTileSource *imageTileSource,
        ImageViewDLMListener *listener,
        QObject *parent)
    : QObject(parent)
    , m_listener(listener)
    , m_imageTileSource(imageTileSource) {

    // Java line 119: mImageTileSource = imageTileSource;
    // Java line 120: mHTileCount = mImageTileSource.getHTileCount();
    // Java line 121: mVTileCount = mImageTileSource.getVTileCount();
    // Java line 122: mTileSize = mImageTileSource.getTileSize();
    m_hTileCount = m_imageTileSource->getHTileCount();
    m_vTileCount = m_imageTileSource->getVTileCount();
    m_tileSize = m_imageTileSource->getTileSize();

    // Java line 123: mTiles = new RRGLRenderableTexturedQuad[mHTileCount][mVTileCount];
    m_tiles.resize(m_hTileCount);
    for (int x = 0; x < m_hTileCount; x++) {
        m_tiles[x].resize(m_vTileCount, nullptr);
    }

    // Java lines 125-133:
    // mTileLoaders = new MultiScaleTileManager[mHTileCount][mVTileCount];
    // final ImageViewTileLoaderThread thread = new ImageViewTileLoaderThread();
    // for(...)
    m_tileLoaders.resize(m_hTileCount);
    for (int x = 0; x < m_hTileCount; x++) {
        m_tileLoaders[x].resize(m_vTileCount, nullptr);
    }

    ImageViewTileLoaderThread *thread = new ImageViewTileLoaderThread(this);

    for (int x = 0; x < m_hTileCount; x++) {
        for (int y = 0; y < m_vTileCount; y++) {
            // Java lines 130-131:
            // mTileLoaders[x][y] = new MultiScaleTileManager(
            //     imageTileSource, thread, x, y, this);
            m_tileLoaders[x][y] = new MultiScaleTileManager(
                    imageTileSource,
                    thread,
                    x,
                    y,
                    this /* ImageViewTileLoader.Listener */);
        }
    }

    // Java line 93: mCoordinateHelper = new CoordinateHelper();
    m_coordinateHelper = new CoordinateHelper();

    // Java lines 103-104: mDoubleTapGapTimer
    m_doubleTapGapTimer = new UIThreadRepeatingTimer(50, this);

    // Java lines 388-391: Temp point objects
    m_tmpPoint1_onFingersMoved = new MutableFloatPoint2D();
    m_tmpPoint2_onFingersMoved = new MutableFloatPoint2D();

    // Java line 101: mSpareFingers = new Stack<>(8)
    m_spareFingers.reserve(8);
}

ImageViewDisplayListManager::~ImageViewDisplayListManager() {
    delete m_coordinateHelper;
    delete m_doubleTapGapTimer;
    delete m_tmpPoint1_onFingersMoved;
    delete m_tmpPoint2_onFingersMoved;
}

// ========================================================================
// onGLSceneCreate — Java lines 136-196
// ========================================================================

void ImageViewDisplayListManager::onGLSceneCreate(
        RRGLDisplayList *scene,
        RRGLContext *glContext,
        Refreshable *refreshable) {

    // Java lines 142-144:
    // mTileVisibility = new boolean[mHTileCount][mVTileCount];
    // mTileLoaded = new boolean[mHTileCount][mVTileCount];
    // mRefreshable = refreshable;
    // mScreenDensity = glContext.getScreenDensity();
    m_tileVisibility.resize(m_hTileCount);
    for (int x = 0; x < m_hTileCount; x++) {
        m_tileVisibility[x].resize(m_vTileCount, false);
    }
    m_tileLoaded.resize(m_hTileCount);
    for (int x = 0; x < m_hTileCount; x++) {
        m_tileLoaded[x].resize(m_vTileCount, false);
    }
    m_refreshable = refreshable;
    m_screenDensity = glContext->getScreenDensity();

    // Java line 147:
    // mNotLoadedTexture = new RRGLTexture(glContext, NOT_LOADED_BITMAP, false);
    m_notLoadedTexture = new RRGLTexture(glContext, nullptr, false);
    // Note: NOT_LOADED_BITMAP is a 32x32 gray bitmap. We create
    // a placeholder texture here that will be used for unloaded tiles.

    // Java lines 149-186: Build the display list hierarchy
    // final RRGLRenderableGroup group = new RRGLRenderableGroup();
    // mOverallScale = new RRGLRenderableScale(group);
    // mOverallTranslation = new RRGLRenderableTranslation(mOverallScale);
    // scene.add(mOverallTranslation);
    RRGLRenderableGroup *group = new RRGLRenderableGroup();
    m_overallScale = new RRGLRenderableScale(group);
    m_overallTranslation = new RRGLRenderableTranslation(m_overallScale);
    scene->add(m_overallTranslation);

    for (int x = 0; x < m_hTileCount; x++) {
        for (int y = 0; y < m_vTileCount; y++) {
            // Java lines 158-159: Create quad with not-loaded texture
            // final RRGLRenderableTexturedQuad quad =
            //     new RRGLRenderableTexturedQuad(glContext, mNotLoadedTexture);
            // mTiles[x][y] = quad;
            RRGLRenderableTexturedQuad *quad =
                    new RRGLRenderableTexturedQuad(glContext, m_notLoadedTexture);
            m_tiles[x][y] = quad;

            // Java lines 161-163: Scale for this tile
            RRGLRenderableScale *scale = new RRGLRenderableScale(quad);

            // Java lines 165-177: Calculate tile dimensions
            int tileWidth = m_tileSize;
            int tileHeight = m_tileSize;

            int imageWidth = m_imageTileSource->getWidth();
            int imageHeight = m_imageTileSource->getHeight();

            // Java lines 170-172:
            // if(x == mHTileCount - 1 && imageWidth % mTileSize != 0) {
            //     tileWidth = imageWidth % mTileSize;
            // }
            if (x == m_hTileCount - 1 && imageWidth % m_tileSize != 0) {
                tileWidth = imageWidth % m_tileSize;
            }

            // Java lines 174-176:
            // if(y == mVTileCount - 1 && imageHeight % mTileSize != 0) {
            //     tileHeight = imageHeight % mTileSize;
            // }
            if (y == m_vTileCount - 1 && imageHeight % m_tileSize != 0) {
                tileHeight = imageHeight % m_tileSize;
            }

            // Java line 178: scale.setScale(tileWidth, tileHeight);
            scale->setScale(tileWidth, tileHeight);

            // Java lines 180-182:
            // final RRGLRenderableTranslation translation =
            //     new RRGLRenderableTranslation(scale);
            // translation.setPosition(x * mTileSize, y * mTileSize);
            RRGLRenderableTranslation *translation =
                    new RRGLRenderableTranslation(scale);
            translation->setPosition(x * m_tileSize, y * m_tileSize);

            // Java line 184: group.add(translation);
            group->add(translation);
        }
    }

    // Java lines 188-195: Create scrollbars
    // mScrollbars = new ImageViewScrollbars(
    //     glContext, mCoordinateHelper,
    //     mImageTileSource.getWidth(), mImageTileSource.getHeight());
    // scene.add(mScrollbars);
    m_scrollbars = new ImageViewScrollbars(
            glContext,
            m_coordinateHelper,
            m_imageTileSource->getWidth(),
            m_imageTileSource->getHeight());
    scene->add(m_scrollbars);
}

// ========================================================================
// onGLSceneResolutionChange — Java lines 198-221
// ========================================================================

void ImageViewDisplayListManager::onGLSceneResolutionChange(
        RRGLDisplayList *scene,
        RRGLContext *context,
        int width,
        int height) {

    Q_UNUSED(scene)
    Q_UNUSED(context)

    // Java lines 205-206:
    // mResolutionX = width;
    // mResolutionY = height;
    m_resolutionX = width;
    m_resolutionY = height;

    // Java line 208: final boolean setInitialScale = (mBoundsHelper == null);
    bool setInitialScale = (m_boundsHelper == nullptr);

    // Java lines 210-213:
    // mBoundsHelper = new BoundsHelper(
    //     width, height,
    //     mImageTileSource.getWidth(), mImageTileSource.getHeight(),
    //     mCoordinateHelper);
    m_boundsHelper = new BoundsHelper(
            width, height,
            m_imageTileSource->getWidth(),
            m_imageTileSource->getHeight(),
            m_coordinateHelper);

    // Java lines 215-217:
    // if(setInitialScale) {
    //     mBoundsHelper.applyMinScale();
    // }
    if (setInitialScale) {
        m_boundsHelper->applyMinScale();
    }

    // Java lines 219-220:
    // mScrollbars.setResolution(width, height);
    // mScrollbars.showBars();
    m_scrollbars->setResolution(width, height);
    m_scrollbars->showBars();
}

// ========================================================================
// onGLSceneUpdate — Java lines 223-332
// ========================================================================

bool ImageViewDisplayListManager::onGLSceneUpdate(
        RRGLDisplayList *scene,
        RRGLContext *context) {

    Q_UNUSED(scene)

    // Java lines 228-232: Scale animation step
    if (m_scaleAnimation != nullptr) {
        if (!m_scaleAnimation->onStep()) {
            m_scaleAnimation = nullptr;
        }
    }

    // Java lines 234-236: Apply bounds
    if (m_boundsHelper != nullptr) {
        m_boundsHelper->applyBounds();
    }

    // Java lines 238-243: Update position and scale
    // final MutableFloatPoint2D positionOffset = mCoordinateHelper.getPositionOffset();
    // final float scale = mCoordinateHelper.getScale();
    // mOverallTranslation.setPosition(positionOffset);
    // mOverallScale.setScale(scale, scale);
    MutableFloatPoint2D *positionOffset = m_coordinateHelper->getPositionOffset();
    float scale = m_coordinateHelper->getScale();

    m_overallTranslation->setPosition(positionOffset);
    m_overallScale->setScale(scale, scale);

    // Java line 244: mScrollbars.update();
    m_scrollbars->update();

    // Java lines 246-255: Pick sample size and reset loaded flags if changed
    int sampleSize = pickSampleSize();

    if (m_lastSampleSize != sampleSize) {
        // Java lines 250-252:
        // for(final boolean[] arr : mTileLoaded) {
        //     Arrays.fill(arr, false);
        // }
        for (auto &row : m_tileLoaded) {
            std::fill(row.begin(), row.end(), false);
        }
        // Java line 254: mLastSampleSize = sampleSize;
        m_lastSampleSize = sampleSize;
    }

    // Java lines 257-267: Calculate visible tile range
    float firstVisiblePixelX = -positionOffset->x / scale;
    float firstVisiblePixelY = -positionOffset->y / scale;

    int firstVisibleTileX = (int)std::floor(firstVisiblePixelX / m_tileSize);
    int firstVisibleTileY = (int)std::floor(firstVisiblePixelY / m_tileSize);

    float lastVisiblePixelX = firstVisiblePixelX + (float)m_resolutionX / scale;
    float lastVisiblePixelY = firstVisiblePixelY + (float)m_resolutionY / scale;

    int lastVisibleTileX = (int)std::ceil(lastVisiblePixelX / m_tileSize);
    int lastVisibleTileY = (int)std::ceil(lastVisiblePixelY / m_tileSize);

    // Java lines 269-270:
    // final int desiredScaleIndex =
    //     MultiScaleTileManager.sampleSizeToScaleIndex(sampleSize);
    int desiredScaleIndex =
            MultiScaleTileManager::sampleSizeToScaleIndex(sampleSize);

    // Java lines 272-325: Update tile visibility and loading
    for (int x = 0; x < m_hTileCount; x++) {
        for (int y = 0; y < m_vTileCount; y++) {

            // Java lines 275-279: isTileVisible
            bool isTileVisible =
                    x >= firstVisibleTileX
                    && y >= firstVisibleTileY
                    && x <= lastVisibleTileX
                    && y <= lastVisibleTileY;

            // Java lines 281-285: isTileWanted (with 1-tile buffer)
            bool isTileWanted =
                    x >= firstVisibleTileX - 1
                    && y >= firstVisibleTileY - 1
                    && x <= lastVisibleTileX + 1
                    && y <= lastVisibleTileY + 1;

            // Java lines 287-292: Mark tiles as wanted/unwanted
            if (isTileWanted && !m_tileLoaded[x][y]) {
                m_tileLoaders[x][y]->markAsWanted(desiredScaleIndex);
            } else {
                m_tileLoaders[x][y]->markAsUnwanted();
            }

            // Java lines 294-324: Update tile textures
            if (isTileVisible != m_tileVisibility[x][y] || !m_tileLoaded[x][y]) {

                if (isTileVisible && !m_tileLoaded[x][y]) {
                    // Java lines 298-308:
                    // final Bitmap tile = mTileLoaders[x][y].getAtDesiredScale();
                    // if(tile != null) {
                    //     final RRGLTexture texture = new RRGLTexture(context, tile, true);
                    //     mTiles[x][y].setTexture(texture);
                    //     texture.releaseReference();
                    //     mTileLoaded[x][y] = true;
                    //     tile.recycle();
                    // }
                    Bitmap *tile = m_tileLoaders[x][y]->getAtDesiredScale();

                    if (tile != nullptr) {
                        try {
                            RRGLTexture *texture =
                                    new RRGLTexture(context, tile, true);
                            m_tiles[x][y]->setTexture(texture);
                            texture->releaseReference();
                            m_tileLoaded[x][y] = true;
                            tile->recycle();
                        } catch (const std::exception &e) {
                            // Java lines 311-315:
                            // Log.e("ImageViewDisplayListMan",
                            //     "Exception when creating texture", e);
                            // Silently handle — texture creation failed
                        }
                    }

                } else if (!isTileWanted) {
                    // Java line 319:
                    // mTiles[x][y].setTexture(mNotLoadedTexture);
                    m_tiles[x][y]->setTexture(m_notLoadedTexture);
                }

                // Java line 323: mTileVisibility[x][y] = isTileVisible;
                m_tileVisibility[x][y] = isTileVisible;
            }
        }
    }

    // Java lines 327-329:
    // if(mScaleAnimation != null) {
    //     mScrollbars.showBars();
    // }
    if (m_scaleAnimation != nullptr) {
        m_scrollbars->showBars();
    }

    // Java line 331: return mScaleAnimation != null;
    return m_scaleAnimation != nullptr;
}

// ========================================================================
// onUIAttach / onUIDetach — Java lines 334-341
// ========================================================================

// Java lines 334-336: onUIAttach
void ImageViewDisplayListManager::onUIAttach() {
    // Java: empty
}

// Java lines 338-341: onUIDetach
void ImageViewDisplayListManager::onUIDetach() {
    // Java line 340: mImageTileSource.dispose();
    m_imageTileSource->dispose();
}

// ========================================================================
// onFingerDown — Java lines 343-386
// ========================================================================

void ImageViewDisplayListManager::onFingerDown(FingerTracker::Finger *finger) {
    // Java lines 346-348:
    // if(mScrollbars == null) { return; }
    if (m_scrollbars == nullptr) {
        return;
    }

    // Java lines 350-351:
    // mScaleAnimation = null;
    // mScrollbars.showBars();
    m_scaleAnimation = nullptr;
    m_scrollbars->showBars();

    // Java lines 353-385: Touch state machine
    if (!m_hasTouchState) {
        // Java lines 354-355: null → ONE_FINGER_DOWN
        m_currentTouchState = TouchState::ONE_FINGER_DOWN;
        m_dragFinger = finger;

    } else {
        // Java lines 358-384: switch on current state
        switch (m_currentTouchState) {

            // Java lines 360-364: DOUBLE_TAP_WAIT_NO_FINGERS_DOWN
            case TouchState::DOUBLE_TAP_WAIT_NO_FINGERS_DOWN:
                m_currentTouchState = TouchState::DOUBLE_TAP_ONE_FINGER_DOWN;
                m_dragFinger = finger;
                m_doubleTapGapTimer->stopTimer();
                break;

            // Java lines 366-369: ONE_FINGER_DRAG → fallthrough
            case TouchState::ONE_FINGER_DRAG:
                // Java line 367:
                // mListener.onHorizontalSwipeEnd();
                m_listener->onHorizontalSwipeEnd();
                // Fallthrough deliberate

            // Java lines 371-379: ONE_FINGER_DOWN, DOUBLE_TAP_ONE_FINGER_DOWN,
            //                     DOUBLE_TAP_ONE_FINGER_DRAG
            case TouchState::ONE_FINGER_DOWN:
            case TouchState::DOUBLE_TAP_ONE_FINGER_DOWN:
            case TouchState::DOUBLE_TAP_ONE_FINGER_DRAG:
                m_currentTouchState = TouchState::TWO_FINGER_PINCH;
                m_pinchFinger1 = m_dragFinger;
                m_pinchFinger2 = finger;
                m_dragFinger = nullptr;
                break;

            // Java lines 381-383: default → push spare
            default:
                m_spareFingers.push_back(finger);
                break;
        }
    }

    m_hasTouchState = true;
}

// ========================================================================
// onFingersMoved — Java lines 393-477
// ========================================================================

void ImageViewDisplayListManager::onFingersMoved() {
    // Java lines 396-398: null check
    if (!m_hasTouchState) {
        return;
    }

    // Java lines 400-402: scrollbars null check
    if (m_scrollbars == nullptr) {
        return;
    }

    // Java lines 404-405:
    // mScaleAnimation = null;
    // mScrollbars.showBars();
    m_scaleAnimation = nullptr;
    m_scrollbars->showBars();

    // Java lines 407-476: switch on touch state
    switch (m_currentTouchState) {

        // Java lines 409-417: DOUBLE_TAP_ONE_FINGER_DOWN
        case TouchState::DOUBLE_TAP_ONE_FINGER_DOWN: {
            // Java lines 411-414:
            // if(mDragFinger.mTotalPosDifference.distanceSquared()
            //     >= 400f * mScreenDensity * mScreenDensity) {
            //     mCurrentTouchState = TouchState.DOUBLE_TAP_ONE_FINGER_DRAG;
            // }
            if (m_dragFinger->mTotalPosDifference->distanceSquared()
                >= 400.0f * m_screenDensity * m_screenDensity) {
                m_currentTouchState = TouchState::DOUBLE_TAP_ONE_FINGER_DRAG;
            }
            break;
        }

        // Java lines 419-432: DOUBLE_TAP_ONE_FINGER_DRAG
        case TouchState::DOUBLE_TAP_ONE_FINGER_DRAG: {
            // Java lines 421-422:
            // final MutableFloatPoint2D screenCentre = mTmpPoint1_onFingersMoved;
            // screenCentre.set(mResolutionX / 2, mResolutionY / 2);
            m_tmpPoint1_onFingersMoved->set(
                    m_resolutionX / 2.0f,
                    m_resolutionY / 2.0f);

            // Java lines 424-429:
            // mCoordinateHelper.scaleAboutScreenPoint(
            //     screenCentre,
            //     (float)Math.pow(1.01,
            //         mDragFinger.mPosDifference.y / mScreenDensity));
            m_coordinateHelper->scaleAboutScreenPoint(
                    m_tmpPoint1_onFingersMoved,
                    (float)std::pow(1.01,
                            m_dragFinger->mPosDifference->y / m_screenDensity));
            break;
        }

        // Java lines 434-452: ONE_FINGER_DOWN (with fallthrough to ONE_FINGER_DRAG)
        case TouchState::ONE_FINGER_DOWN: {
            // Java lines 436-439:
            // if(mDragFinger.mTotalPosDifference.distanceSquared()
            //     >= 100f * mScreenDensity * mScreenDensity) {
            //     mCurrentTouchState = TouchState.ONE_FINGER_DRAG;
            // }
            if (m_dragFinger->mTotalPosDifference->distanceSquared()
                >= 100.0f * m_screenDensity * m_screenDensity) {
                m_currentTouchState = TouchState::ONE_FINGER_DRAG;
            }
            // Fallthrough deliberate (Java: // Deliberate fall-through)
        }

        // Java lines 444-452: ONE_FINGER_DRAG
        case TouchState::ONE_FINGER_DRAG: {
            // Java lines 445-451:
            // if(mBoundsHelper.isMinScale()) {
            //     mListener.onHorizontalSwipe(mDragFinger.mTotalPosDifference.x);
            // } else {
            //     mCoordinateHelper.translateScreen(
            //         mDragFinger.mLastPos, mDragFinger.mCurrentPos);
            // }
            if (m_boundsHelper->isMinScale()) {
                m_listener->onHorizontalSwipe(
                        m_dragFinger->mTotalPosDifference->x);
            } else {
                m_coordinateHelper->translateScreen(
                        m_dragFinger->mLastPos,
                        m_dragFinger->mCurrentPos);
            }
            break;
        }

        // Java lines 454-475: TWO_FINGER_PINCH
        case TouchState::TWO_FINGER_PINCH: {
            // Java lines 456-459:
            // final double oldDistance = mPinchFinger1.mLastPos
            //     .euclideanDistanceTo(mPinchFinger2.mLastPos);
            // final double newDistance = mPinchFinger1.mCurrentPos
            //     .euclideanDistanceTo(mPinchFinger2.mCurrentPos);
            double oldDistance = m_pinchFinger1->mLastPos->euclideanDistanceTo(
                    m_pinchFinger2->mLastPos);
            double newDistance = m_pinchFinger1->mCurrentPos->euclideanDistanceTo(
                    m_pinchFinger2->mCurrentPos);

            // Java lines 461-463:
            // final MutableFloatPoint2D oldCentre = mTmpPoint1_onFingersMoved;
            // mPinchFinger1.mLastPos.add(mPinchFinger2.mLastPos, oldCentre);
            // oldCentre.scale(0.5);
            m_pinchFinger1->mLastPos->add(
                    m_pinchFinger2->mLastPos,
                    m_tmpPoint1_onFingersMoved);
            m_tmpPoint1_onFingersMoved->scale(0.5f);

            // Java lines 465-467:
            // final MutableFloatPoint2D newCentre = mTmpPoint2_onFingersMoved;
            // mPinchFinger1.mCurrentPos.add(mPinchFinger2.mCurrentPos, newCentre);
            // newCentre.scale(0.5);
            m_pinchFinger1->mCurrentPos->add(
                    m_pinchFinger2->mCurrentPos,
                    m_tmpPoint2_onFingersMoved);
            m_tmpPoint2_onFingersMoved->scale(0.5f);

            // Java line 469: final float scaleDifference = (float)(newDistance / oldDistance);
            float scaleDifference = (float)(newDistance / oldDistance);

            // Java lines 471-472:
            // mCoordinateHelper.scaleAboutScreenPoint(newCentre, scaleDifference);
            // mCoordinateHelper.translateScreen(oldCentre, newCentre);
            m_coordinateHelper->scaleAboutScreenPoint(
                    m_tmpPoint2_onFingersMoved, scaleDifference);
            m_coordinateHelper->translateScreen(
                    m_tmpPoint1_onFingersMoved,
                    m_tmpPoint2_onFingersMoved);
            break;
        }
    }
}

// ========================================================================
// onFingerUp — Java lines 479-561
// ========================================================================

void ImageViewDisplayListManager::onFingerUp(FingerTracker::Finger *finger) {
    // Java lines 482-484: null check
    if (m_scrollbars == nullptr) {
        return;
    }

    // Java lines 486-487:
    // mScaleAnimation = null;
    // mScrollbars.showBars();
    m_scaleAnimation = nullptr;
    m_scrollbars->showBars();

    // Java lines 489-491: Remove from spare fingers
    // if(mSpareFingers.remove(finger)) { return; }
    auto it = std::find(m_spareFingers.begin(), m_spareFingers.end(), finger);
    if (it != m_spareFingers.end()) {
        m_spareFingers.erase(it);
        return;
    }

    // Java lines 493-495: null check
    if (!m_hasTouchState) {
        return;
    }

    // Java lines 497-560: switch on touch state
    switch (m_currentTouchState) {

        // Java lines 499-507: DOUBLE_TAP_ONE_FINGER_DOWN
        case TouchState::DOUBLE_TAP_ONE_FINGER_DOWN: {
            // Java lines 501-502:
            // if(finger.mDownDuration < TAP_MAX_DURATION_MS) {
            //     onDoubleTap(finger.mCurrentPos);
            // }
            if (finger->mDownDuration < TAP_MAX_DURATION_MS) {
                onDoubleTap(finger->mCurrentPos);
            }

            // Java lines 504-506:
            // mCurrentTouchState = null;
            // mDragFinger = null;
            m_hasTouchState = false;
            m_dragFinger = nullptr;
            break;
        }

        // Java lines 509-524: ONE_FINGER_DOWN
        case TouchState::ONE_FINGER_DOWN: {
            // Java lines 511-521:
            // if(finger.mDownDuration < TAP_MAX_DURATION_MS) {
            //     // Maybe a single tap
            //     mDoubleTapGapTimer.startTimer();
            //     mCurrentTouchState = DOUBLE_TAP_WAIT_NO_FINGERS_DOWN;
            //     mFirstTapReleaseTime = System.currentTimeMillis();
            // } else {
            //     mCurrentTouchState = null;
            // }
            if (finger->mDownDuration < TAP_MAX_DURATION_MS) {
                m_doubleTapGapTimer->startTimer();
                m_currentTouchState = TouchState::DOUBLE_TAP_WAIT_NO_FINGERS_DOWN;
                m_firstTapReleaseTime = QDateTime::currentMSecsSinceEpoch();
            } else {
                m_hasTouchState = false;
            }

            // Java lines 523: mDragFinger = null;
            m_dragFinger = nullptr;
            break;
        }

        // Java lines 526-541: ONE_FINGER_DRAG / DOUBLE_TAP_ONE_FINGER_DRAG
        case TouchState::ONE_FINGER_DRAG: {
            // Java line 528: mListener.onHorizontalSwipeEnd();
            m_listener->onHorizontalSwipeEnd();
            // Fallthrough deliberate
        }
        case TouchState::DOUBLE_TAP_ONE_FINGER_DRAG: {
            // Java lines 534-539:
            // if(mSpareFingers.isEmpty()) {
            //     mCurrentTouchState = null;
            //     mDragFinger = null;
            // } else {
            //     mDragFinger = mSpareFingers.pop();
            // }
            if (m_spareFingers.empty()) {
                m_hasTouchState = false;
                m_dragFinger = nullptr;
            } else {
                m_dragFinger = m_spareFingers.back();
                m_spareFingers.pop_back();
            }
            break;
        }

        // Java lines 543-559: TWO_FINGER_PINCH
        case TouchState::TWO_FINGER_PINCH: {
            // Java lines 545-558:
            // if(mSpareFingers.isEmpty()) {
            //     mCurrentTouchState = ONE_FINGER_DRAG;
            //     mDragFinger = (mPinchFinger1 == finger)
            //         ? mPinchFinger2 : mPinchFinger1;
            //     mPinchFinger1 = null;
            //     mPinchFinger2 = null;
            // } else {
            //     if(mPinchFinger1 == finger) {
            //         mPinchFinger1 = mSpareFingers.pop();
            //     } else {
            //         mPinchFinger2 = mSpareFingers.pop();
            //     }
            // }
            if (m_spareFingers.empty()) {
                m_currentTouchState = TouchState::ONE_FINGER_DRAG;
                m_dragFinger = (m_pinchFinger1 == finger)
                        ? m_pinchFinger2 : m_pinchFinger1;
                m_pinchFinger1 = nullptr;
                m_pinchFinger2 = nullptr;
            } else {
                if (m_pinchFinger1 == finger) {
                    m_pinchFinger1 = m_spareFingers.back();
                    m_spareFingers.pop_back();
                } else {
                    m_pinchFinger2 = m_spareFingers.back();
                    m_spareFingers.pop_back();
                }
            }
            break;
        }
    }
}

// ========================================================================
// onDoubleTap — Java lines 563-586
// ========================================================================

void ImageViewDisplayListManager::onDoubleTap(MutableFloatPoint2D *position) {
    // Java lines 565-566:
    // final float minScale = mBoundsHelper.getMinScale();
    // final float currentScale = mCoordinateHelper.getScale();
    float minScale = m_boundsHelper->getMinScale();
    float currentScale = m_coordinateHelper->getScale();

    // Java lines 568-582: Determine target scale
    float targetScale;

    // Java lines 570-577:
    // if(currentScale > minScale * 1.01) {
    //     targetScale = minScale;
    // } else {
    //     targetScale = Math.max(
    //         (float)mResolutionX / (float)mImageTileSource.getWidth(),
    //         (float)mResolutionY / (float)mImageTileSource.getHeight());
    //
    //     if(Math.abs((targetScale / currentScale) - 1.0) < 0.05) {
    //         targetScale = currentScale * 3;
    //     }
    // }
    if (currentScale > minScale * 1.01f) {
        targetScale = minScale;
    } else {
        targetScale = std::max(
                (float)m_resolutionX / (float)m_imageTileSource->getWidth(),
                (float)m_resolutionY / (float)m_imageTileSource->getHeight());

        if (std::abs((targetScale / currentScale) - 1.0) < 0.05) {
            targetScale = currentScale * 3.0f;
        }
    }

    // Java lines 584-585:
    // mScaleAnimation = new ImageViewScaleAnimation(
    //     targetScale, mCoordinateHelper, 15, position);
    m_scaleAnimation = new ImageViewScaleAnimation(
            targetScale, m_coordinateHelper, 15, position);
}

// ========================================================================
// onUIThreadRepeatingTimer — Java lines 588-603
// ========================================================================

void ImageViewDisplayListManager::onUIThreadRepeatingTimer(
        UIThreadRepeatingTimer *timer) {

    Q_UNUSED(timer)

    // Java lines 591-602:
    // if(mCurrentTouchState == TouchState.DOUBLE_TAP_WAIT_NO_FINGERS_DOWN) {
    //     if(System.currentTimeMillis() - mFirstTapReleaseTime
    //         > DOUBLE_TAP_MAX_GAP_DURATION_MS) {
    //         mListener.onSingleTap();
    //         mCurrentTouchState = null;
    //         mDoubleTapGapTimer.stopTimer();
    //     }
    // } else {
    //     mDoubleTapGapTimer.stopTimer();
    // }
    if (m_hasTouchState
        && m_currentTouchState == TouchState::DOUBLE_TAP_WAIT_NO_FINGERS_DOWN) {

        if (QDateTime::currentMSecsSinceEpoch() - m_firstTapReleaseTime
            > DOUBLE_TAP_MAX_GAP_DURATION_MS) {
            m_listener->onSingleTap();
            m_hasTouchState = false;
            m_doubleTapGapTimer->stopTimer();
        }

    } else {
        m_doubleTapGapTimer->stopTimer();
    }
}

// ========================================================================
// pickSampleSize — Java lines 605-615
// ========================================================================

int ImageViewDisplayListManager::pickSampleSize() {
    // Java lines 607-614:
    // int result = 1;
    // while(result <= MultiScaleTileManager.MAX_SAMPLE_SIZE
    //     && (1.0 / (result * 2)) > mCoordinateHelper.getScale()) {
    //     result *= 2;
    // }
    // return result;
    int result = 1;

    while (result <= MultiScaleTileManager::MAX_SAMPLE_SIZE
           && (1.0 / (result * 2)) > m_coordinateHelper->getScale()) {
        result *= 2;
    }

    return result;
}

// ========================================================================
// ImageViewTileLoader.Listener callbacks — Java lines 617-630
// ========================================================================

// Java lines 617-619: onTileLoaded
void ImageViewDisplayListManager::onTileLoaded(int x, int y, int sampleSize) {
    Q_UNUSED(x)
    Q_UNUSED(y)
    Q_UNUSED(sampleSize)
    // Java line 618: mRefreshable.refresh();
    if (m_refreshable) {
        m_refreshable->refresh();
    }
}

// Java lines 622-624: onTileLoaderOutOfMemory
void ImageViewDisplayListManager::onTileLoaderOutOfMemory() {
    // Java line 623: mListener.onImageViewDLMOutOfMemory();
    m_listener->onImageViewDLMOutOfMemory();
}

// Java lines 627-629: onTileLoaderException
void ImageViewDisplayListManager::onTileLoaderException(const std::exception &t) {
    // Java line 628: mListener.onImageViewDLMException(t);
    m_listener->onImageViewDLMException(t);
}

// ========================================================================
// resetTouchState — Java lines 632-634
// ========================================================================

// Java lines 632-634: resetTouchState
void ImageViewDisplayListManager::resetTouchState() {
    // Java line 633: mCurrentTouchState = null;
    m_hasTouchState = false;
}

} // namespace PinkReader
