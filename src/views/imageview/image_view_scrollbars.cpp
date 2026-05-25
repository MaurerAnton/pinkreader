// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/imageview/ImageViewScrollbars.java
#include "views/imageview/image_view_scrollbars.h"
#include "views/imageview/coordinate_helper.h"

#include <algorithm>
#include <cstdint>
#include <chrono>

namespace PinkReader {

// ========================================================================
// Placeholder GL rendering classes (to be replaced by actual implementations)
// ========================================================================

// Port of: RRGLRenderableColouredQuad
class RRGLRenderableColouredQuad : public RRGLRenderable {
public:
	explicit RRGLRenderableColouredQuad(RRGLContext*) {}
	void setColour(float, float, float, float) {}
protected:
	void renderInternal(RRGLMatrixStack*, int64_t) override {}
};

// Port of: RRGLRenderableScale
class RRGLRenderableScale : public RRGLRenderable {
public:
	explicit RRGLRenderableScale(RRGLRenderable*) {}
	void setScale(float, float) {}
protected:
	void renderInternal(RRGLMatrixStack*, int64_t) override {}
};

// Port of: RRGLRenderableTranslation
class RRGLRenderableTranslation : public RRGLRenderable {
public:
	explicit RRGLRenderableTranslation(RRGLRenderable*) {}
	void setPosition(float, float) {}
protected:
	void renderInternal(RRGLMatrixStack*, int64_t) override {}
};

// Port of: RRGLRenderableGroup
class RRGLRenderableGroup : public RRGLRenderable {
public:
	void add(RRGLRenderable*) {}
	void show() { mHidden = false; }
	void hide() { mHidden = true; }
	bool isHidden() const { return mHidden; }
protected:
	void renderInternal(RRGLMatrixStack*, int64_t) override {}
private:
	bool mHidden = false;
};

// Port of: RRGLRenderableBlend
class RRGLRenderableBlend : public RRGLRenderable {
public:
	explicit RRGLRenderableBlend(RRGLRenderable*) {}
	void setOverallAlpha(float) {}
	void startRender(RRGLMatrixStack*, int64_t) {}
protected:
	void renderInternal(RRGLMatrixStack*, int64_t) override {}
};

// ========================================================================
// ImageViewScrollbars implementation
// ========================================================================

// Port of: public ImageViewScrollbars(final RRGLContext glContext, final CoordinateHelper coordinateHelper,
//         final int imageResX, final int imageResY)
ImageViewScrollbars::ImageViewScrollbars(
		RRGLContext* glContext,
		CoordinateHelper* coordinateHelper,
		int imageResX,
		int imageResY)
	// Port of: mCoordinateHelper = coordinateHelper;
	: mCoordinateHelper(coordinateHelper)
	// Port of: mImageResX = imageResX;
	, mImageResX(imageResX)
	// Port of: mImageResY = imageResY;
	, mImageResY(imageResY)
{
	// Port of: final RRGLRenderableGroup group = new RRGLRenderableGroup();
	//          mRenderable = new RRGLRenderableBlend(group);
	RRGLRenderableGroup* group = new RRGLRenderableGroup();
	mRenderable = new RRGLRenderableBlend(group);

	// Port of: mDimMarginSides = glContext.dpToPixels(10);
	mDimMarginSides = 10; // placeholder: glContext.dpToPixels(10)

	// Port of: mDimMarginEnds = glContext.dpToPixels(20);
	mDimMarginEnds = 20;  // placeholder

	// Port of: mDimBarWidth = glContext.dpToPixels(6);
	mDimBarWidth = 6;     // placeholder

	// Port of: mDimBorderWidth = glContext.dpToPixels(1);
	mDimBorderWidth = 1;  // placeholder

	// Port of: // Vertical scroll bar
	{
		// Port of: mVScroll = new RRGLRenderableGroup();
		mVScroll = new RRGLRenderableGroup();

		// Port of: group.add(mVScroll);
		group->add(mVScroll);

		// Port of: final RRGLRenderableColouredQuad vScrollMarker = new RRGLRenderableColouredQuad(glContext);
		RRGLRenderableColouredQuad* vScrollMarker = new RRGLRenderableColouredQuad(glContext);

		// Port of: final RRGLRenderableColouredQuad vScrollBar = new RRGLRenderableColouredQuad(glContext);
		RRGLRenderableColouredQuad* vScrollBar = new RRGLRenderableColouredQuad(glContext);

		// Port of: final RRGLRenderableColouredQuad vScrollBorder = new RRGLRenderableColouredQuad(glContext);
		RRGLRenderableColouredQuad* vScrollBorder = new RRGLRenderableColouredQuad(glContext);

		// Port of: vScrollMarker.setColour(1, 1, 1, 0.8f);
		vScrollMarker->setColour(1, 1, 1, 0.8f);

		// Port of: vScrollBar.setColour(0, 0, 0, 0.5f);
		vScrollBar->setColour(0, 0, 0, 0.5f);

		// Port of: vScrollBorder.setColour(1, 1, 1, 0.5f);
		vScrollBorder->setColour(1, 1, 1, 0.5f);

		// Port of: mVScrollMarkerScale = new RRGLRenderableScale(vScrollMarker);
		mVScrollMarkerScale = new RRGLRenderableScale(vScrollMarker);

		// Port of: mVScrollBarScale = new RRGLRenderableScale(vScrollBar);
		mVScrollBarScale = new RRGLRenderableScale(vScrollBar);

		// Port of: mVScrollBorderScale = new RRGLRenderableScale(vScrollBorder);
		mVScrollBorderScale = new RRGLRenderableScale(vScrollBorder);

		// Port of: mVScrollMarkerTranslation = new RRGLRenderableTranslation(mVScrollMarkerScale);
		mVScrollMarkerTranslation = new RRGLRenderableTranslation(mVScrollMarkerScale);

		// Port of: mVScrollBarTranslation = new RRGLRenderableTranslation(mVScrollBarScale);
		mVScrollBarTranslation = new RRGLRenderableTranslation(mVScrollBarScale);

		// Port of: mVScrollBorderTranslation = new RRGLRenderableTranslation(mVScrollBorderScale);
		mVScrollBorderTranslation = new RRGLRenderableTranslation(mVScrollBorderScale);

		// Port of: mVScroll.add(mVScrollBorderTranslation);
		mVScroll->add(mVScrollBorderTranslation);

		// Port of: mVScroll.add(mVScrollBarTranslation);
		mVScroll->add(mVScrollBarTranslation);

		// Port of: mVScroll.add(mVScrollMarkerTranslation);
		mVScroll->add(mVScrollMarkerTranslation);
	}

	// Port of: // Horizontal scroll bar
	{
		// Port of: mHScroll = new RRGLRenderableGroup();
		mHScroll = new RRGLRenderableGroup();

		// Port of: group.add(mHScroll);
		group->add(mHScroll);

		// Port of: final RRGLRenderableColouredQuad hScrollMarker = new RRGLRenderableColouredQuad(glContext);
		RRGLRenderableColouredQuad* hScrollMarker = new RRGLRenderableColouredQuad(glContext);

		// Port of: final RRGLRenderableColouredQuad hScrollBar = new RRGLRenderableColouredQuad(glContext);
		RRGLRenderableColouredQuad* hScrollBar = new RRGLRenderableColouredQuad(glContext);

		// Port of: final RRGLRenderableColouredQuad hScrollBorder = new RRGLRenderableColouredQuad(glContext);
		RRGLRenderableColouredQuad* hScrollBorder = new RRGLRenderableColouredQuad(glContext);

		// Port of: hScrollMarker.setColour(1, 1, 1, 0.8f);
		hScrollMarker->setColour(1, 1, 1, 0.8f);

		// Port of: hScrollBar.setColour(0, 0, 0, 0.5f);
		hScrollBar->setColour(0, 0, 0, 0.5f);

		// Port of: hScrollBorder.setColour(1, 1, 1, 0.5f);
		hScrollBorder->setColour(1, 1, 1, 0.5f);

		// Port of: mHScrollMarkerScale = new RRGLRenderableScale(hScrollMarker);
		mHScrollMarkerScale = new RRGLRenderableScale(hScrollMarker);

		// Port of: mHScrollBarScale = new RRGLRenderableScale(hScrollBar);
		mHScrollBarScale = new RRGLRenderableScale(hScrollBar);

		// Port of: mHScrollBorderScale = new RRGLRenderableScale(hScrollBorder);
		mHScrollBorderScale = new RRGLRenderableScale(hScrollBorder);

		// Port of: mHScrollMarkerTranslation = new RRGLRenderableTranslation(mHScrollMarkerScale);
		mHScrollMarkerTranslation = new RRGLRenderableTranslation(mHScrollMarkerScale);

		// Port of: mHScrollBarTranslation = new RRGLRenderableTranslation(mHScrollBarScale);
		mHScrollBarTranslation = new RRGLRenderableTranslation(mHScrollBarScale);

		// Port of: mHScrollBorderTranslation = new RRGLRenderableTranslation(mHScrollBorderScale);
		mHScrollBorderTranslation = new RRGLRenderableTranslation(mHScrollBorderScale);

		// Port of: mHScroll.add(mHScrollBorderTranslation);
		mHScroll->add(mHScrollBorderTranslation);

		// Port of: mHScroll.add(mHScrollBarTranslation);
		mHScroll->add(mHScrollBarTranslation);

		// Port of: mHScroll.add(mHScrollMarkerTranslation);
		mHScroll->add(mHScrollMarkerTranslation);
	}
}

ImageViewScrollbars::~ImageViewScrollbars() {
	delete mRenderable;
	// Note: child renderables are owned by their parent groups in the real GL system
}

// Port of: public void update()
void ImageViewScrollbars::update() {
	// Port of: // TODO avoid GC
	//          final MutableFloatPoint2D tmp1 = new MutableFloatPoint2D();
	//          final MutableFloatPoint2D tmp2 = new MutableFloatPoint2D();
	MutableFloatPoint2D tmp1;
	MutableFloatPoint2D tmp2;

	// Port of: mCoordinateHelper.convertScreenToScene(tmp1, tmp2);
	mCoordinateHelper->convertScreenToScene(tmp1, tmp2);

	// Port of: final float xStart = tmp2.x / (float)mImageResX;
	float xStart = tmp2.x / static_cast<float>(mImageResX);

	// Port of: final float yStart = tmp2.y / (float)mImageResY;
	float yStart = tmp2.y / static_cast<float>(mImageResY);

	// Port of: tmp1.set(mResX, mResY);
	tmp1.set(static_cast<float>(mResX), static_cast<float>(mResY));

	// Port of: mCoordinateHelper.convertScreenToScene(tmp1, tmp2);
	mCoordinateHelper->convertScreenToScene(tmp1, tmp2);

	// Port of: final float xEnd = tmp2.x / (float)mImageResX;
	float xEnd = tmp2.x / static_cast<float>(mImageResX);

	// Port of: final float yEnd = tmp2.y / (float)mImageResY;
	float yEnd = tmp2.y / static_cast<float>(mImageResY);

	// Port of: // Vertical scroll bar
	//          if(yStart < EPSILON && yEnd > 1 - EPSILON) {
	//              mVScroll.hide();
	//          } else {
	if (yStart < EPSILON && yEnd > 1.0f - EPSILON) {
		mVScroll->hide();
	} else {
		// Port of: mVScroll.show();
		mVScroll->show();

		// Port of: final float vScrollTotalHeight = mResY - 2 * mDimMarginEnds;
		float vScrollTotalHeight = static_cast<float>(mResY - 2 * mDimMarginEnds);

		// Port of: final float vScrollHeight = (yEnd - yStart) * vScrollTotalHeight;
		float vScrollHeight = (yEnd - yStart) * vScrollTotalHeight;

		// Port of: final float vScrollTop = yStart * vScrollTotalHeight + mDimMarginEnds;
		float vScrollTop = yStart * vScrollTotalHeight + static_cast<float>(mDimMarginEnds);

		// Port of: final float vScrollLeft = mResX - mDimBarWidth - mDimMarginSides;
		float vScrollLeft = static_cast<float>(mResX - mDimBarWidth - mDimMarginSides);

		// Port of: mVScrollBorderTranslation.setPosition(vScrollLeft - mDimBorderWidth, mDimMarginEnds - mDimBorderWidth);
		mVScrollBorderTranslation->setPosition(
				vScrollLeft - static_cast<float>(mDimBorderWidth),
				static_cast<float>(mDimMarginEnds - mDimBorderWidth));

		// Port of: mVScrollBorderScale.setScale(mDimBarWidth + 2 * mDimBorderWidth, vScrollTotalHeight + 2 * mDimBorderWidth);
		mVScrollBorderScale->setScale(
				static_cast<float>(mDimBarWidth + 2 * mDimBorderWidth),
				vScrollTotalHeight + static_cast<float>(2 * mDimBorderWidth));

		// Port of: mVScrollBarTranslation.setPosition(vScrollLeft, mDimMarginEnds);
		mVScrollBarTranslation->setPosition(vScrollLeft, static_cast<float>(mDimMarginEnds));

		// Port of: mVScrollBarScale.setScale(mDimBarWidth, vScrollTotalHeight);
		mVScrollBarScale->setScale(static_cast<float>(mDimBarWidth), vScrollTotalHeight);

		// Port of: mVScrollMarkerTranslation.setPosition(vScrollLeft, vScrollTop);
		mVScrollMarkerTranslation->setPosition(vScrollLeft, vScrollTop);

		// Port of: mVScrollMarkerScale.setScale(mDimBarWidth, vScrollHeight);
		mVScrollMarkerScale->setScale(static_cast<float>(mDimBarWidth), vScrollHeight);
	}

	// Port of: // Horizontal scroll bar
	//          if(xStart < EPSILON && xEnd > 1 - EPSILON) {
	//              mHScroll.hide();
	//          } else {
	if (xStart < EPSILON && xEnd > 1.0f - EPSILON) {
		mHScroll->hide();
	} else {
		// Port of: mHScroll.show();
		mHScroll->show();

		// Port of: final float hScrollTotalWidth = mResX - 2 * mDimMarginEnds;
		float hScrollTotalWidth = static_cast<float>(mResX - 2 * mDimMarginEnds);

		// Port of: final float hScrollWidth = (xEnd - xStart) * hScrollTotalWidth;
		float hScrollWidth = (xEnd - xStart) * hScrollTotalWidth;

		// Port of: final float hScrollLeft = xStart * hScrollTotalWidth + mDimMarginEnds;
		float hScrollLeft = xStart * hScrollTotalWidth + static_cast<float>(mDimMarginEnds);

		// Port of: final float hScrollTop = mResY - mDimBarWidth - mDimMarginSides;
		float hScrollTop = static_cast<float>(mResY - mDimBarWidth - mDimMarginSides);

		// Port of: mHScrollBorderTranslation.setPosition(mDimMarginEnds - mDimBorderWidth, hScrollTop - mDimBorderWidth);
		mHScrollBorderTranslation->setPosition(
				static_cast<float>(mDimMarginEnds - mDimBorderWidth),
				hScrollTop - static_cast<float>(mDimBorderWidth));

		// Port of: mHScrollBorderScale.setScale(hScrollTotalWidth + 2 * mDimBorderWidth, mDimBarWidth + mDimBorderWidth * 2);
		mHScrollBorderScale->setScale(
				hScrollTotalWidth + static_cast<float>(2 * mDimBorderWidth),
				static_cast<float>(mDimBarWidth + mDimBorderWidth * 2));

		// Port of: mHScrollBarTranslation.setPosition(mDimMarginEnds, hScrollTop);
		mHScrollBarTranslation->setPosition(static_cast<float>(mDimMarginEnds), hScrollTop);

		// Port of: mHScrollBarScale.setScale(hScrollTotalWidth, mDimBarWidth);
		mHScrollBarScale->setScale(hScrollTotalWidth, static_cast<float>(mDimBarWidth));

		// Port of: mHScrollMarkerTranslation.setPosition(hScrollLeft, hScrollTop);
		mHScrollMarkerTranslation->setPosition(hScrollLeft, hScrollTop);

		// Port of: mHScrollMarkerScale.setScale(hScrollWidth, mDimBarWidth);
		mHScrollMarkerScale->setScale(hScrollWidth, static_cast<float>(mDimBarWidth));
	}
}

// Port of: public synchronized void setResolution(final int x, final int y)
//   mResX = x;
//   mResY = y;
void ImageViewScrollbars::setResolution(int x, int y) {
	mResX = x;
	mResY = y;
}

// Port of: @Override public void onAdded()
//   super.onAdded();
//   mRenderable.onAdded();
void ImageViewScrollbars::onAdded() {
	RRGLRenderable::onAdded();
	mRenderable->onAdded();
}

// Port of: @Override public void onRemoved()
//   mRenderable.onRemoved();
//   super.onRemoved();
void ImageViewScrollbars::onRemoved() {
	mRenderable->onRemoved();
	RRGLRenderable::onRemoved();
}

// Port of: @Override public synchronized boolean isAnimating()
//   return mIsVisible;
bool ImageViewScrollbars::isAnimating() {
	return mIsVisible;
}

// Port of: public synchronized void showBars()
//   mShowUntil = System.currentTimeMillis() + 600;
//   mIsVisible = true;
//   mCurrentAlpha = 1;
void ImageViewScrollbars::showBars() {
	// Port of: mShowUntil = System.currentTimeMillis() + 600;
	auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch()).count();
	mShowUntil = static_cast<int64_t>(now) + 600;

	// Port of: mIsVisible = true;
	mIsVisible = true;

	// Port of: mCurrentAlpha = 1;
	mCurrentAlpha = 1.0f;
}

// Port of: @Override protected synchronized void renderInternal(final RRGLMatrixStack stack, final long time)
void ImageViewScrollbars::renderInternal(RRGLMatrixStack* stack, int64_t time) {
	// Port of: if(mIsVisible && time > mShowUntil) {
	if (mIsVisible && time > mShowUntil) {
		// Port of: mCurrentAlpha -= ALPHA_STEP;
		mCurrentAlpha -= ALPHA_STEP;

		// Port of: if(mCurrentAlpha < 0) {
		if (mCurrentAlpha < 0.0f) {
			// Port of: mIsVisible = false;
			mIsVisible = false;

			// Port of: mCurrentAlpha = 0;
			mCurrentAlpha = 0.0f;
		}
	}

	// Port of: mRenderable.setOverallAlpha(mCurrentAlpha);
	mRenderable->setOverallAlpha(mCurrentAlpha);

	// Port of: mRenderable.startRender(stack, time);
	mRenderable->startRender(stack, time);
}

} // namespace PinkReader
