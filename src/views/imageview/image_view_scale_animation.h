// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/imageview/ImageViewScaleAnimation.java
#pragma once

#include "common/mutable_float_point_2d.h"

namespace PinkReader {

// Forward declaration
class CoordinateHelper;

/**
 * Port of org.quantumbadger.redreader.views.imageview.ImageViewScaleAnimation
 */
class ImageViewScaleAnimation {
public:
	// Port of: public ImageViewScaleAnimation(final float targetScale, final CoordinateHelper coordinateHelper,
	//         final int stepCount, final MutableFloatPoint2D screenCoord)
	ImageViewScaleAnimation(
			float targetScale,
			CoordinateHelper& coordinateHelper,
			int stepCount,
			const MutableFloatPoint2D& screenCoord);

	// Port of: public boolean onStep()
	// Returns true if animation should continue
	bool onStep();

private:
	// Port of: private final float mStepSize
	float mStepSize;

	// Port of: private final float mTargetScale
	float mTargetScale;

	// Port of: private final CoordinateHelper mCoordinateHelper
	CoordinateHelper& mCoordinateHelper;

	// Port of: private final MutableFloatPoint2D mScreenCoord = new MutableFloatPoint2D()
	MutableFloatPoint2D mScreenCoord;
};

} // namespace PinkReader
