// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/imageview/BasicGestureHandler.java
#include "views/imageview/basic_gesture_handler.h"

namespace PinkReader {

// Port of: public BasicGestureHandler(final Listener listener)
//   mListener = listener;
BasicGestureHandler::BasicGestureHandler(Listener* listener)
	// Port of: private final FingerTracker mFingerTracker = new FingerTracker(this)
	: mFingerTracker(this)
	// Port of: private final Listener mListener
	, mListener(listener)
{
}

// Port of: @Override public boolean onTouch(final View v, final MotionEvent event)
//   mFingerTracker.onTouchEvent(event);
//   return true;
bool BasicGestureHandler::onTouch(int actionMasked, int actionIndex, int pointerId,
                                   float x, float y, int64_t eventTime, int64_t downTime) {
	mFingerTracker.onTouchEvent(actionMasked, actionIndex, pointerId, x, y, eventTime, downTime);
	return true;
}

// Port of: @Override public void onFingerDown(final FingerTracker.Finger finger)
//   mCurrentFingerCount++;
//   if(mCurrentFingerCount > 1) {
//     mFirstFinger = null;
//   } else {
//     mFirstFinger = finger;
//   }
void BasicGestureHandler::onFingerDown(FingerTracker::Finger& finger) {
	mCurrentFingerCount++;

	if (mCurrentFingerCount > 1) {
		mFirstFinger = nullptr;
	} else {
		mFirstFinger = &finger;
	}
}

// Port of: @Override public void onFingersMoved()
//   if(mFirstFinger != null) {
//     mListener.onHorizontalSwipe(mFirstFinger.mTotalPosDifference.x);
//   }
void BasicGestureHandler::onFingersMoved() {
	if (mFirstFinger != nullptr) {
		mListener->onHorizontalSwipe(mFirstFinger->mTotalPosDifference.x);
	}
}

// Port of: @Override public void onFingerUp(final FingerTracker.Finger finger)
void BasicGestureHandler::onFingerUp(FingerTracker::Finger& finger) {
	// Port of: mCurrentFingerCount--;
	mCurrentFingerCount--;

	// Port of: if(mFirstFinger != null) {
	if (mFirstFinger != nullptr) {
		// Port of: mListener.onHorizontalSwipeEnd();
		mListener->onHorizontalSwipeEnd();

		// Port of: // TODO
		//          if(mFirstFinger.mDownDuration < 300
		//              && mFirstFinger.mPosDifference.x < 20
		//              && mFirstFinger.mPosDifference.y < 20) {
		//              mListener.onSingleTap();
		//          }
		if (mFirstFinger->mDownDuration < 300
				&& mFirstFinger->mPosDifference.x < 20.0f
				&& mFirstFinger->mPosDifference.y < 20.0f) {
			mListener->onSingleTap();
		}

		// Port of: mFirstFinger = null;
		mFirstFinger = nullptr;
	}
}

} // namespace PinkReader
