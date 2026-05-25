// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/imageview/BoundsHelper.java
#pragma once

namespace PinkReader {

// Forward declaration
class CoordinateHelper;

/**
 * Port of org.quantumbadger.redreader.views.imageview.BoundsHelper
 */
class BoundsHelper {
public:
	// Port of: public BoundsHelper(final int resolutionX, final int resolutionY,
	//         final int imageResolutionX, final int imageResolutionY,
	//         final CoordinateHelper coordinateHelper)
	BoundsHelper(
			int resolutionX, int resolutionY,
			int imageResolutionX, int imageResolutionY,
			CoordinateHelper& coordinateHelper);

	// Port of: public void applyMinScale()
	void applyMinScale();

	// Port of: public boolean isMinScale()
	bool isMinScale() const;

	// Port of: public void applyBounds()
	void applyBounds();

	// Port of: public float getMinScale()
	float getMinScale() const;

private:
	// Port of: private final int mResolutionX
	int mResolutionX;

	// Port of: private final int mResolutionY
	int mResolutionY;

	// Port of: private final int mImageResolutionX
	int mImageResolutionX;

	// Port of: private final int mImageResolutionY
	int mImageResolutionY;

	// Port of: private final CoordinateHelper mCoordinateHelper
	CoordinateHelper& mCoordinateHelper;

	// Port of: private final float mMinScale
	float mMinScale;
};

} // namespace PinkReader
