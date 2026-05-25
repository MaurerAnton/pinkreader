// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/imageview/BasicGestureHandler.java
#pragma once

#include "views/imageview/finger_tracker.h"

namespace PinkReader {

/**
 * Port of org.quantumbadger.redreader.views.imageview.BasicGestureHandler
 * implements View.OnTouchListener, FingerTracker.FingerListener
 */
class BasicGestureHandler : public FingerTracker::FingerListener {
public:
	// Port of: public interface Listener
	class Listener {
	public:
		virtual ~Listener() = default;

		// Port of: void onSingleTap()
		virtual void onSingleTap() = 0;

		// Port of: void onHorizontalSwipe(float pixels)
		virtual void onHorizontalSwipe(float pixels) = 0;

		// Port of: void onHorizontalSwipeEnd()
		virtual void onHorizontalSwipeEnd() = 0;
	};

	// Port of: public BasicGestureHandler(final Listener listener)
	explicit BasicGestureHandler(Listener* listener);

	// Port of: @Override public boolean onTouch(final View v, final MotionEvent event)
	bool onTouch(int actionMasked, int actionIndex, int pointerId,
	             float x, float y, int64_t eventTime, int64_t downTime);

	// Port of: @Override public void onFingerDown(final FingerTracker.Finger finger)
	void onFingerDown(FingerTracker::Finger& finger) override;

	// Port of: @Override public void onFingersMoved()
	void onFingersMoved() override;

	// Port of: @Override public void onFingerUp(final FingerTracker.Finger finger)
	void onFingerUp(FingerTracker::Finger& finger) override;

private:
	// Port of: private final FingerTracker mFingerTracker = new FingerTracker(this)
	FingerTracker mFingerTracker;

	// Port of: private final Listener mListener
	Listener* mListener;

	// Port of: private FingerTracker.Finger mFirstFinger
	FingerTracker::Finger* mFirstFinger = nullptr;

	// Port of: private int mCurrentFingerCount
	int mCurrentFingerCount = 0;
};

} // namespace PinkReader
