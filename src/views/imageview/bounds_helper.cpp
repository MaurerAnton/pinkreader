// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/imageview/BoundsHelper.java
#include "views/imageview/bounds_helper.h"
#include "views/imageview/coordinate_helper.h"

#include <algorithm>
#include <cmath>

namespace PinkReader {

// Port of: public BoundsHelper(final int resolutionX, final int resolutionY,
//         final int imageResolutionX, final int imageResolutionY,
//         final CoordinateHelper coordinateHelper)
BoundsHelper::BoundsHelper(
		int resolutionX, int resolutionY,
		int imageResolutionX, int imageResolutionY,
		CoordinateHelper& coordinateHelper)
	// Port of: mResolutionX = resolutionX;
	: mResolutionX(resolutionX)
	// Port of: mResolutionY = resolutionY;
	, mResolutionY(resolutionY)
	// Port of: mImageResolutionX = imageResolutionX;
	, mImageResolutionX(imageResolutionX)
	// Port of: mImageResolutionY = imageResolutionY;
	, mImageResolutionY(imageResolutionY)
	// Port of: mCoordinateHelper = coordinateHelper;
	, mCoordinateHelper(coordinateHelper)
	// Port of: mMinScale = Math.min(
	//             (float)mResolutionX / (float)mImageResolutionX,
	//             (float)mResolutionY / (float)mImageResolutionY);
	, mMinScale(std::min(
			static_cast<float>(mResolutionX) / static_cast<float>(mImageResolutionX),
			static_cast<float>(mResolutionY) / static_cast<float>(mImageResolutionY)))
{
}

// Port of: public void applyMinScale()
//   mCoordinateHelper.setScale(mMinScale);
void BoundsHelper::applyMinScale() {
	mCoordinateHelper.setScale(mMinScale);
}

// Port of: public boolean isMinScale()
//   return mCoordinateHelper.getScale() - 0.000_001f <= mMinScale;
bool BoundsHelper::isMinScale() const {
	return mCoordinateHelper.getScale() - 0.000001f <= mMinScale;
}

// Port of: public void applyBounds()
void BoundsHelper::applyBounds() {
	// Port of: if(mCoordinateHelper.getScale() < mMinScale) {
	//             applyMinScale();
	//         }
	if (mCoordinateHelper.getScale() < mMinScale) {
		applyMinScale();
	}

	// Port of: final float scale = mCoordinateHelper.getScale();
	float scale = mCoordinateHelper.getScale();

	// Port of: final MutableFloatPoint2D posOffset = mCoordinateHelper.getPositionOffset();
	MutableFloatPoint2D& posOffset = mCoordinateHelper.getPositionOffset();

	// Port of: final float scaledImageWidth = (float)mImageResolutionX * scale;
	float scaledImageWidth = static_cast<float>(mImageResolutionX) * scale;

	// Port of: final float scaledImageHeight = (float)mImageResolutionY * scale;
	float scaledImageHeight = static_cast<float>(mImageResolutionY) * scale;

	// Port of: if(scaledImageWidth <= mResolutionX) {
	if (scaledImageWidth <= static_cast<float>(mResolutionX)) {
		// Port of: posOffset.x = (mResolutionX - scaledImageWidth) / 2;
		posOffset.x = (static_cast<float>(mResolutionX) - scaledImageWidth) / 2.0f;

	// Port of: } else if(posOffset.x > 0) {
	} else if (posOffset.x > 0.0f) {
		// Port of: posOffset.x = 0;
		posOffset.x = 0.0f;

	// Port of: } else if(posOffset.x < mResolutionX - scaledImageWidth) {
	} else if (posOffset.x < static_cast<float>(mResolutionX) - scaledImageWidth) {
		// Port of: posOffset.x = mResolutionX - scaledImageWidth;
		posOffset.x = static_cast<float>(mResolutionX) - scaledImageWidth;
	}

	// Port of: if(scaledImageHeight <= mResolutionY) {
	if (scaledImageHeight <= static_cast<float>(mResolutionY)) {
		// Port of: posOffset.y = (mResolutionY - scaledImageHeight) / 2;
		posOffset.y = (static_cast<float>(mResolutionY) - scaledImageHeight) / 2.0f;

	// Port of: } else if(posOffset.y > 0) {
	} else if (posOffset.y > 0.0f) {
		// Port of: posOffset.y = 0;
		posOffset.y = 0.0f;

	// Port of: } else if(posOffset.y < mResolutionY - scaledImageHeight) {
	} else if (posOffset.y < static_cast<float>(mResolutionY) - scaledImageHeight) {
		// Port of: posOffset.y = mResolutionY - scaledImageHeight;
		posOffset.y = static_cast<float>(mResolutionY) - scaledImageHeight;
	}
}

// Port of: public float getMinScale()
//   return mMinScale;
float BoundsHelper::getMinScale() const {
	return mMinScale;
}

} // namespace PinkReader
