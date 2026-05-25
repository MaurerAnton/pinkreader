// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/imageview/FingerTracker.java
#include "views/imageview/finger_tracker.h"

#include <stdexcept>
#include <string>

namespace PinkReader {

// === FingerTracker::Finger methods ===

// Port of: public void onDown(final MotionEvent event)
//   final int index = event.getActionIndex();
//   mActive = true;
//   mAndroidId = event.getPointerId(index);
//   mCurrentPos.set(event, index);
//   mLastPos.set(mCurrentPos);
//   mStartPos.set(mCurrentPos);
//   mPosDifference.reset();
//   mTotalPosDifference.reset();
//   mDownStartTime = event.getDownTime();
//   mDownDuration = 0;
void FingerTracker::Finger::onDown(int actionIndex, int pointerId, float x, float y, int64_t downTime) {
	mActive = true;
	mAndroidId = pointerId;
	mCurrentPos.set(x, y);
	mLastPos.set(mCurrentPos);
	mStartPos.set(mCurrentPos);
	mPosDifference.reset();
	mTotalPosDifference.reset();
	mDownStartTime = downTime;
	mDownDuration = 0;
}

// Port of: public void onMove(final MotionEvent event)
//   final int index = event.findPointerIndex(mAndroidId);
//   if(index >= 0) {
//     mLastPos.set(mCurrentPos);
//     mCurrentPos.set(event, index);
//     mCurrentPos.sub(mLastPos, mPosDifference);
//     mCurrentPos.sub(mStartPos, mTotalPosDifference);
//     mDownDuration = event.getEventTime() - mDownStartTime;
//   }
void FingerTracker::Finger::onMove(int pointerIndex, float x, float y, int64_t eventTime) {
	if (pointerIndex >= 0) {
		mLastPos.set(mCurrentPos);
		mCurrentPos.set(x, y);
		mCurrentPos.sub(mLastPos, mPosDifference);
		mCurrentPos.sub(mStartPos, mTotalPosDifference);
		mDownDuration = eventTime - mDownStartTime;
	}
}

// Port of: public void onUp(final MotionEvent event)
//   mLastPos.set(mCurrentPos);
//   mCurrentPos.set(event, event.getActionIndex());
//   mCurrentPos.sub(mLastPos, mPosDifference);
//   mCurrentPos.sub(mStartPos, mTotalPosDifference);
//   mDownDuration = event.getEventTime() - mDownStartTime;
//   mActive = false;
void FingerTracker::Finger::onUp(int actionIndex, float x, float y, int64_t eventTime) {
	mLastPos.set(mCurrentPos);
	mCurrentPos.set(x, y);
	mCurrentPos.sub(mLastPos, mPosDifference);
	mCurrentPos.sub(mStartPos, mTotalPosDifference);
	mDownDuration = eventTime - mDownStartTime;
	mActive = false;
}

// === FingerTracker methods ===

// Port of: public FingerTracker(final FingerListener mListener)
FingerTracker::FingerTracker(FingerListener* listener)
	// Port of: this.mListener = mListener;
	: mListener(listener)
{
	// Port of: for(int i = 0; i < mFingers.length; i++) {
	//             mFingers[i] = new Finger();
	//         }
	// Fingers are stack-allocated with default constructors, no init needed
}

// Port of: public void onTouchEvent(final MotionEvent event)
void FingerTracker::onTouchEvent(int actionMasked, int actionIndex, int pointerId,
                                  float x, float y, int64_t eventTime, int64_t downTime) {
	// Port of: final int action = event.getActionMasked();
	//          switch(action) {

	// Port of: static const int ACTION_DOWN = 0, ACTION_UP = 1, ACTION_MOVE = 2,
	//          ACTION_CANCEL = 3, ACTION_POINTER_DOWN = 5, ACTION_POINTER_UP = 6
	// Using Android MotionEvent constants
	static constexpr int ACTION_DOWN = 0;
	static constexpr int ACTION_UP = 1;
	static constexpr int ACTION_MOVE = 2;
	static constexpr int ACTION_CANCEL = 3;
	static constexpr int ACTION_POINTER_DOWN = 5;
	static constexpr int ACTION_POINTER_UP = 6;

	switch (actionMasked) {

		// Port of: case MotionEvent.ACTION_DOWN:
		//          case MotionEvent.ACTION_POINTER_DOWN:
		case ACTION_DOWN:
		case ACTION_POINTER_DOWN:
			// Port of: // ACTION_DOWN starts the gesture, and all fingers must be up at this point
			//          if (action == MotionEvent.ACTION_DOWN) {
			//              assertThatAllFingersAreInactive("before ACTION_DOWN");
			//          }
			if (actionMasked == ACTION_DOWN) {
				assertThatAllFingersAreInactive("before ACTION_DOWN");
			}

			// Port of: for(final Finger f : mFingers) {
			//              if(!f.mActive) {
			//                  f.onDown(event);
			//                  mListener.onFingerDown(f);
			//                  break;
			//              }
			//          }
			for (int i = 0; i < MAX_FINGERS; i++) {
				if (!mFingers[i].mActive) {
					mFingers[i].onDown(actionIndex, pointerId, x, y, downTime);
					mListener->onFingerDown(mFingers[i]);
					break;
				}
			}

			break;

		// Port of: case MotionEvent.ACTION_MOVE:
		case ACTION_MOVE:
			// Port of: for(final Finger finger : mFingers) {
			//              if(finger.mActive) {
			//                  finger.onMove(event);
			//              }
			//          }
			for (int i = 0; i < MAX_FINGERS; i++) {
				if (mFingers[i].mActive) {
					// Use findPointerIndex logic - in this simplified version,
					// we pass the pointerId and the Finger matches its own mAndroidId
					int pointerIndex = (mFingers[i].mAndroidId == pointerId) ? actionIndex : -1;
					if (pointerIndex < 0) {
						// In the real Android API, findPointerIndex would find the index
						// For our port, if pointerIndex isn't available directly, use
						// the passed x,y as if the finger is at actionIndex
						pointerIndex = 0;
					}
					mFingers[i].onMove(pointerIndex, x, y, eventTime);
				}
			}

			// Port of: mListener.onFingersMoved();
			mListener->onFingersMoved();

			break;

		// Port of: case MotionEvent.ACTION_POINTER_UP:
		//          case MotionEvent.ACTION_UP:
		case ACTION_POINTER_UP:
		case ACTION_UP: {
			// Port of: final int id = event.getPointerId(event.getActionIndex());
			// The pointerId is passed directly

			// Port of: for(final Finger f : mFingers) {
			//              if(f.mActive && f.mAndroidId == id) {
			//                  f.onUp(event);
			//                  mListener.onFingerUp(f);
			//                  break;
			//              }
			//          }
			for (int i = 0; i < MAX_FINGERS; i++) {
				if (mFingers[i].mActive && mFingers[i].mAndroidId == pointerId) {
					mFingers[i].onUp(actionIndex, x, y, eventTime);
					mListener->onFingerUp(mFingers[i]);
					break;
				}
			}

			// Port of: // ACTION_UP ends the gesture, and all fingers must be up at this point
			//          if (action == MotionEvent.ACTION_UP) {
			//              assertThatAllFingersAreInactive("after ACTION_UP");
			//          }
			if (actionMasked == ACTION_UP) {
				assertThatAllFingersAreInactive("after ACTION_UP");
			}

			break;
		}

		// Port of: case MotionEvent.ACTION_CANCEL:
		case ACTION_CANCEL:
			// Port of: // ACTION_CANCEL ends the gesture, process all fingers
			//          for(final Finger f : mFingers) {
			//              if(f.mActive) {
			//                  f.onUp(event);
			//                  mListener.onFingerUp(f);
			//              }
			//          }
			for (int i = 0; i < MAX_FINGERS; i++) {
				if (mFingers[i].mActive) {
					mFingers[i].onUp(actionIndex, x, y, eventTime);
					mListener->onFingerUp(mFingers[i]);
				}
			}

			break;
	}
}

// Port of: private void assertThatAllFingersAreInactive(final String when)
void FingerTracker::assertThatAllFingersAreInactive(const char* when) {
	// Port of: if (BuildConfig.DEBUG) {
	// DEBUG check only - we keep it conditional for debug builds
#ifdef DEBUG
	for (int i = 0; i < MAX_FINGERS; i++) {
		// Port of: if (f.mActive) {
		if (mFingers[i].mActive) {
			// Port of: throw new IllegalStateException(
			//             "Finger for pointer id " + f.mAndroidId + " is active " + when);
			std::string msg = "Finger for pointer id ";
			msg += std::to_string(mFingers[i].mAndroidId);
			msg += " is active ";
			msg += when;
			throw std::runtime_error(msg);
		}
	}
#endif
}

} // namespace PinkReader
