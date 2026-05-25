// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/imageview/CoordinateHelper.java
#pragma once

#include "common/mutable_float_point_2d.h"

namespace PinkReader {

/**
 * Port of org.quantumbadger.redreader.views.imageview.CoordinateHelper
 */
class CoordinateHelper {
public:
	// Port of: public void setScale(final float scale)
	void setScale(float scale);

	// Port of: public float getScale()
	float getScale() const;

	// Port of: public MutableFloatPoint2D getPositionOffset()
	MutableFloatPoint2D& getPositionOffset();

	// Port of: public void getPositionOffset(final MutableFloatPoint2D result)
	void getPositionOffset(MutableFloatPoint2D& result) const;

	// Port of: public void convertScreenToScene(final MutableFloatPoint2D screenPos, final MutableFloatPoint2D output)
	void convertScreenToScene(const MutableFloatPoint2D& screenPos, MutableFloatPoint2D& output) const;

	// Port of: public void convertSceneToScreen(final MutableFloatPoint2D scenePos, final MutableFloatPoint2D output)
	void convertSceneToScreen(const MutableFloatPoint2D& scenePos, MutableFloatPoint2D& output) const;

	// Port of: public void scaleAboutScreenPoint(final MutableFloatPoint2D screenPos, final float scaleFactor)
	void scaleAboutScreenPoint(const MutableFloatPoint2D& screenPos, float scaleFactor);

	// Port of: public void setScaleAboutScreenPoint(final MutableFloatPoint2D screenPos, final float scale)
	void setScaleAboutScreenPoint(const MutableFloatPoint2D& screenPos, float scale);

	// Port of: public void translateScreen(final MutableFloatPoint2D oldScreenPos, final MutableFloatPoint2D newScreenPos)
	void translateScreen(const MutableFloatPoint2D& oldScreenPos, const MutableFloatPoint2D& newScreenPos);

private:
	// Port of: private float mScale = 1.0f
	float mScale = 1.0f;

	// Port of: private final MutableFloatPoint2D mPositionOffset = new MutableFloatPoint2D()
	MutableFloatPoint2D mPositionOffset;
};

} // namespace PinkReader
