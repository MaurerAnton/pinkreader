// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/imageview/CoordinateHelper.java
#include "views/imageview/coordinate_helper.h"

namespace PinkReader {

// Port of: public void setScale(final float scale)
//   mScale = scale;
void CoordinateHelper::setScale(float scale) {
	mScale = scale;
}

// Port of: public float getScale()
//   return mScale;
float CoordinateHelper::getScale() const {
	return mScale;
}

// Port of: public MutableFloatPoint2D getPositionOffset()
//   return mPositionOffset;
MutableFloatPoint2D& CoordinateHelper::getPositionOffset() {
	return mPositionOffset;
}

// Port of: public void getPositionOffset(final MutableFloatPoint2D result)
//   result.set(mPositionOffset);
void CoordinateHelper::getPositionOffset(MutableFloatPoint2D& result) const {
	result.set(mPositionOffset);
}

// Port of: public void convertScreenToScene(final MutableFloatPoint2D screenPos, final MutableFloatPoint2D output)
//   output.x = (screenPos.x - mPositionOffset.x) / mScale;
//   output.y = (screenPos.y - mPositionOffset.y) / mScale;
void CoordinateHelper::convertScreenToScene(const MutableFloatPoint2D& screenPos, MutableFloatPoint2D& output) const {
	output.x = (screenPos.x - mPositionOffset.x) / mScale;
	output.y = (screenPos.y - mPositionOffset.y) / mScale;
}

// Port of: public void convertSceneToScreen(final MutableFloatPoint2D scenePos, final MutableFloatPoint2D output)
//   output.x = scenePos.x * mScale + mPositionOffset.x;
//   output.y = scenePos.y * mScale + mPositionOffset.y;
void CoordinateHelper::convertSceneToScreen(const MutableFloatPoint2D& scenePos, MutableFloatPoint2D& output) const {
	output.x = scenePos.x * mScale + mPositionOffset.x;
	output.y = scenePos.y * mScale + mPositionOffset.y;
}

// Port of: public void scaleAboutScreenPoint(final MutableFloatPoint2D screenPos, final float scaleFactor)
//   setScaleAboutScreenPoint(screenPos, mScale * scaleFactor);
void CoordinateHelper::scaleAboutScreenPoint(const MutableFloatPoint2D& screenPos, float scaleFactor) {
	setScaleAboutScreenPoint(screenPos, mScale * scaleFactor);
}

// Port of: public void setScaleAboutScreenPoint(final MutableFloatPoint2D screenPos, final float scale)
void CoordinateHelper::setScaleAboutScreenPoint(const MutableFloatPoint2D& screenPos, float scale) {
	// Port of: final MutableFloatPoint2D oldScenePos = new MutableFloatPoint2D();
	//          convertScreenToScene(screenPos, oldScenePos);
	MutableFloatPoint2D oldScenePos;
	convertScreenToScene(screenPos, oldScenePos);

	// Port of: mScale = scale;
	mScale = scale;

	// Port of: final MutableFloatPoint2D newScreenPos = new MutableFloatPoint2D();
	//          convertSceneToScreen(oldScenePos, newScreenPos);
	MutableFloatPoint2D newScreenPos;
	convertSceneToScreen(oldScenePos, newScreenPos);

	// Port of: translateScreen(newScreenPos, screenPos);
	translateScreen(newScreenPos, screenPos);
}

// Port of: public void translateScreen(final MutableFloatPoint2D oldScreenPos, final MutableFloatPoint2D newScreenPos)
//   mPositionOffset.add(newScreenPos);
//   mPositionOffset.sub(oldScreenPos);
void CoordinateHelper::translateScreen(const MutableFloatPoint2D& oldScreenPos, const MutableFloatPoint2D& newScreenPos) {
	mPositionOffset.add(newScreenPos);
	mPositionOffset.sub(oldScreenPos);
}

} // namespace PinkReader
