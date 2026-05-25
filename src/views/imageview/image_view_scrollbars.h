// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/imageview/ImageViewScrollbars.java
#pragma once

#include <cstdint>

namespace PinkReader {

// Forward declarations for GL rendering infrastructure (to be implemented)
class RRGLContext;
class RRGLMatrixStack;

// Port of: RRGLRenderable base (placeholder)
class RRGLRenderable {
public:
	virtual ~RRGLRenderable() = default;
	virtual void onAdded() {}
	virtual void onRemoved() {}
	virtual bool isAnimating() { return false; }

protected:
	virtual void renderInternal(RRGLMatrixStack* stack, int64_t time) = 0;
};

class RRGLRenderableBlend;
class RRGLRenderableGroup;
class RRGLRenderableColouredQuad;
class RRGLRenderableScale;
class RRGLRenderableTranslation;

// Forward declarations from this package
class CoordinateHelper;

/**
 * Port of org.quantumbadger.redreader.views.imageview.ImageViewScrollbars
 * extends RRGLRenderable
 */
class ImageViewScrollbars : public RRGLRenderable {
public:
	// Port of: private static final float EPSILON = 0.0001f;
	static constexpr float EPSILON = 0.0001f;

	// Port of: private static final float ALPHA_STEP = 0.05f;
	static constexpr float ALPHA_STEP = 0.05f;

	// Port of: public ImageViewScrollbars(final RRGLContext glContext, final CoordinateHelper coordinateHelper,
	//         final int imageResX, final int imageResY)
	ImageViewScrollbars(
			RRGLContext* glContext,
			CoordinateHelper* coordinateHelper,
			int imageResX,
			int imageResY);

	~ImageViewScrollbars();

	// Port of: public void update()
	void update();

	// Port of: public synchronized void setResolution(final int x, final int y)
	void setResolution(int x, int y);

	// Port of: @Override public void onAdded()
	void onAdded() override;

	// Port of: @Override public void onRemoved()
	void onRemoved() override;

	// Port of: @Override public synchronized boolean isAnimating()
	bool isAnimating() override;

	// Port of: public synchronized void showBars()
	void showBars();

protected:
	// Port of: @Override protected synchronized void renderInternal(final RRGLMatrixStack stack, final long time)
	void renderInternal(RRGLMatrixStack* stack, int64_t time) override;

private:
	// Port of: private final RRGLRenderableBlend mRenderable
	RRGLRenderableBlend* mRenderable = nullptr;

	// Vertical scroll bar
	// Port of: private final RRGLRenderableGroup mVScroll
	RRGLRenderableGroup* mVScroll = nullptr;

	// Port of: private final RRGLRenderableTranslation mVScrollMarkerTranslation
	RRGLRenderableTranslation* mVScrollMarkerTranslation = nullptr;

	// Port of: private final RRGLRenderableScale mVScrollMarkerScale
	RRGLRenderableScale* mVScrollMarkerScale = nullptr;

	// Port of: private final RRGLRenderableTranslation mVScrollBarTranslation
	RRGLRenderableTranslation* mVScrollBarTranslation = nullptr;

	// Port of: private final RRGLRenderableScale mVScrollBarScale
	RRGLRenderableScale* mVScrollBarScale = nullptr;

	// Port of: private final RRGLRenderableTranslation mVScrollBorderTranslation
	RRGLRenderableTranslation* mVScrollBorderTranslation = nullptr;

	// Port of: private final RRGLRenderableScale mVScrollBorderScale
	RRGLRenderableScale* mVScrollBorderScale = nullptr;

	// Horizontal scroll bar
	// Port of: private final RRGLRenderableGroup mHScroll
	RRGLRenderableGroup* mHScroll = nullptr;

	// Port of: private final RRGLRenderableTranslation mHScrollMarkerTranslation
	RRGLRenderableTranslation* mHScrollMarkerTranslation = nullptr;

	// Port of: private final RRGLRenderableScale mHScrollMarkerScale
	RRGLRenderableScale* mHScrollMarkerScale = nullptr;

	// Port of: private final RRGLRenderableTranslation mHScrollBarTranslation
	RRGLRenderableTranslation* mHScrollBarTranslation = nullptr;

	// Port of: private final RRGLRenderableScale mHScrollBarScale
	RRGLRenderableScale* mHScrollBarScale = nullptr;

	// Port of: private final RRGLRenderableTranslation mHScrollBorderTranslation
	RRGLRenderableTranslation* mHScrollBorderTranslation = nullptr;

	// Port of: private final RRGLRenderableScale mHScrollBorderScale
	RRGLRenderableScale* mHScrollBorderScale = nullptr;

	// Port of: private final CoordinateHelper mCoordinateHelper
	CoordinateHelper* mCoordinateHelper;

	// Port of: private int mResX
	int mResX = 0;

	// Port of: private int mResY
	int mResY = 0;

	// Port of: private final int mImageResX
	int mImageResX;

	// Port of: private final int mImageResY
	int mImageResY;

	// Port of: private final int mDimMarginSides
	int mDimMarginSides = 0;

	// Port of: private final int mDimMarginEnds
	int mDimMarginEnds = 0;

	// Port of: private final int mDimBarWidth
	int mDimBarWidth = 0;

	// Port of: private final int mDimBorderWidth
	int mDimBorderWidth = 0;

	// Port of: private long mShowUntil = -1;
	int64_t mShowUntil = -1;

	// Port of: private float mCurrentAlpha = 1;
	float mCurrentAlpha = 1.0f;

	// Port of: private boolean mIsVisible = true;
	bool mIsVisible = true;
};

} // namespace PinkReader
