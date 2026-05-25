// Origin: RedReader/src/main/java/org/quantumbadger/redreader/common/MutableFloatPoint2D.java
#pragma once

#include <cmath>

namespace PinkReader {

// Forward declaration
class MutableFloatPoint2D;

/**
 * A mutable 2D point with float coordinates.
 * Equivalent to: org.quantumbadger.redreader.common.MutableFloatPoint2D
 */
class MutableFloatPoint2D {
public:
	float x = 0.0f;
	float y = 0.0f;

	/** Resets both coordinates to zero. */
	void reset() {
		x = 0.0f;
		y = 0.0f;
	}

	/** Sets coordinates from another point. */
	void set(const MutableFloatPoint2D& other) {
		x = other.x;
		y = other.y;
	}

	/** Sets coordinates directly. */
	void set(float newX, float newY) {
		x = newX;
		y = newY;
	}

	/** Adds rhs to this point and stores in result. */
	void add(const MutableFloatPoint2D& rhs, MutableFloatPoint2D& result) const {
		result.x = x + rhs.x;
		result.y = y + rhs.y;
	}

	/** Subtracts rhs from this point and stores in result. */
	void sub(const MutableFloatPoint2D& rhs, MutableFloatPoint2D& result) const {
		result.x = x - rhs.x;
		result.y = y - rhs.y;
	}

	/** Adds rhs to this point (in-place). */
	void add(const MutableFloatPoint2D& rhs) {
		add(rhs, *this);
	}

	/** Subtracts rhs from this point (in-place). */
	void sub(const MutableFloatPoint2D& rhs) {
		sub(rhs, *this);
	}

	/** Scales both coordinates by the given factor. */
	void scale(double factor) {
		x = static_cast<float>(x * factor);
		y = static_cast<float>(y * factor);
	}

	/** Returns the Euclidean distance to another point. */
	double euclideanDistanceTo(const MutableFloatPoint2D& other) const {
		float xDistance = x - other.x;
		float yDistance = y - other.y;
		return std::sqrt(xDistance * xDistance + yDistance * yDistance);
	}

	/** Returns the squared distance from origin (x*x + y*y). */
	float distanceSquared() const {
		return x * x + y * y;
	}
};

} // namespace PinkReader
