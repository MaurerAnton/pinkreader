// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/imageview/FingerTracker.java
#pragma once

#include "common/mutable_float_point_2d.h"

#include <cstdint>

namespace PinkReader {

// Forward declaration
class FingerTracker;

/**
 * Port of org.quantumbadger.redreader.views.imageview.FingerTracker
 */
class FingerTracker {
public:
	// Port of: public static class Finger
	// Inner class Finger
	class Finger {
	public:
		// Port of: boolean mActive = false
		bool mActive = false;

		// Port of: int mAndroidId
		int mAndroidId = 0;

		// Port of: final MutableFloatPoint2D mStartPos = new MutableFloatPoint2D()
		MutableFloatPoint2D mStartPos;

		// Port of: final MutableFloatPoint2D mCurrentPos = new MutableFloatPoint2D()
		MutableFloatPoint2D mCurrentPos;

		// Port of: final MutableFloatPoint2D mLastPos = new MutableFloatPoint2D()
		MutableFloatPoint2D mLastPos;

		// Port of: final MutableFloatPoint2D mPosDifference = new MutableFloatPoint2D()
		MutableFloatPoint2D mPosDifference;

		// Port of: final MutableFloatPoint2D mTotalPosDifference = new MutableFloatPoint2D()
		MutableFloatPoint2D mTotalPosDifference;

		// Port of: long mDownStartTime
		int64_t mDownStartTime = 0;

		// Port of: long mDownDuration
		int64_t mDownDuration = 0;

		// Port of: public void onDown(final MotionEvent event)
		void onDown(int actionIndex, int pointerId, float x, float y, int64_t downTime);

		// Port of: public void onMove(final MotionEvent event)
		void onMove(int pointerIndex, float x, float y, int64_t eventTime);

		// Port of: public void onUp(final MotionEvent event)
		void onUp(int actionIndex, float x, float y, int64_t eventTime);
	};

	// Port of: public interface FingerListener
	class FingerListener {
	public:
		virtual ~FingerListener() = default;

		// Port of: void onFingerDown(Finger finger)
		virtual void onFingerDown(Finger& finger) = 0;

		// Port of: void onFingersMoved()
		virtual void onFingersMoved() = 0;

		// Port of: void onFingerUp(Finger finger)
		virtual void onFingerUp(Finger& finger) = 0;
	};

	// Port of: public FingerTracker(final FingerListener mListener)
	explicit FingerTracker(FingerListener* listener);

	// Port of: public void onTouchEvent(final MotionEvent event)
	void onTouchEvent(int actionMasked, int actionIndex, int pointerId,
	                  float x, float y, int64_t eventTime, int64_t downTime);

	// Port of: private void assertThatAllFingersAreInactive(final String when)
	void assertThatAllFingersAreInactive(const char* when);

private:
	// Port of: private final Finger[] mFingers = new Finger[10]
	static constexpr int MAX_FINGERS = 10;
	Finger mFingers[MAX_FINGERS];

	// Port of: private final FingerListener mListener
	FingerListener* mListener;
};

} // namespace PinkReader
