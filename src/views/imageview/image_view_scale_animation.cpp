// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/imageview/ImageViewScaleAnimation.java
#include "views/imageview/image_view_scale_animation.h"
#include "views/imageview/coordinate_helper.h"

#include <cmath>

namespace PinkReader {

// Port of: public ImageViewScaleAnimation(final float targetScale, final CoordinateHelper coordinateHelper,
//         final int stepCount, final MutableFloatPoint2D screenCoord)
ImageViewScaleAnimation::ImageViewScaleAnimation(
		float targetScale,
		CoordinateHelper& coordinateHelper,
		int stepCount,
		const MutableFloatPoint2D& screenCoord)
	// Port of: mTargetScale = targetScale;
	: mTargetScale(targetScale)
	// Port of: mCoordinateHelper = coordinateHelper;
	, mCoordinateHelper(coordinateHelper)
	// Port of: mStepSize = (float)Math.pow((targetScale / coordinateHelper.getScale()), (1.0 / (double)stepCount));
	, mStepSize(static_cast<float>(std::pow(
			(targetScale / coordinateHelper.getScale()),
			(1.0 / static_cast<double>(stepCount)))))
{
	// Port of: mScreenCoord.set(screenCoord);
	mScreenCoord.set(screenCoord);
}

// Port of: public boolean onStep()
bool ImageViewScaleAnimation::onStep() {
	// Port of: mCoordinateHelper.scaleAboutScreenPoint(mScreenCoord, mStepSize);
	mCoordinateHelper.scaleAboutScreenPoint(mScreenCoord, mStepSize);

	// Port of: if(mStepSize > 1) {
	if (mStepSize > 1.0f) {
		// Port of: if(mTargetScale <= mCoordinateHelper.getScale()) {
		//             mCoordinateHelper.setScaleAboutScreenPoint(mScreenCoord, mTargetScale);
		//             return false;
		//         }
		if (mTargetScale <= mCoordinateHelper.getScale()) {
			mCoordinateHelper.setScaleAboutScreenPoint(mScreenCoord, mTargetScale);
			return false;
		}

	// Port of: } else {
	} else {
		// Port of: if(mTargetScale >= mCoordinateHelper.getScale()) {
		//             mCoordinateHelper.setScaleAboutScreenPoint(mScreenCoord, mTargetScale);
		//             return false;
		//         }
		if (mTargetScale >= mCoordinateHelper.getScale()) {
			mCoordinateHelper.setScaleAboutScreenPoint(mScreenCoord, mTargetScale);
			return false;
		}
	}

	// Port of: return true;
	return true;
}

} // namespace PinkReader
