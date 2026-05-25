// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/MutableFloatRectangle2D.java
#pragma once

#include "mutable_float_point_2d.h"

namespace PinkReader {

/**
 * A mutable 2D axis-aligned rectangle defined by top-left and bottom-right points.
 * Equivalent to: org.quantumbadger.redreader.common.MutableFloatRectangle2D
 */
class MutableFloatRectangle2D {
public:
	MutableFloatPoint2D mTopLeft;
	MutableFloatPoint2D mBottomRight;

	/**
	 * Returns true if this rectangle intersects with the other rectangle.
	 * Uses the standard AABB overlap test (negated separating axis).
	 */
	bool intersect(const MutableFloatRectangle2D& other) const {
		return !(mTopLeft.x > other.mBottomRight.x
			|| mTopLeft.y > other.mBottomRight.y
			|| other.mTopLeft.x > mBottomRight.x
			|| other.mTopLeft.y > mBottomRight.y);
	}
};

} // namespace PinkReader
