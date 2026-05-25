// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/glview/RRGLSurfaceView.java (implementation)
#include "rr_gl_surface_view.h"

// FingerTracker will be ported separately under views/imageview/
// For now, we use a stub that forwards to the DisplayListManager

namespace PinkReader {

// Minimal FingerTracker stub - calls onTouchEvent on the listener
// The full FingerTracker will be a separate port
class FingerTracker {
private:
	DisplayListManager* mListener = nullptr;

public:
	explicit FingerTracker(DisplayListManager* listener)
		: mListener(listener) {}

	bool onTouchEvent(const void* /* event */) {
		// Actual finger tracking will be implemented in the full port
		// For now, just return true indicating the event was handled
		return true;
	}
};

RRGLSurfaceView::RRGLSurfaceView(float screenDensity, DisplayListManager* displayListManager)
	: mDisplayListManager(displayListManager)
	, mScreenDensity(screenDensity) {

	mRenderer = new RRGLDisplayListRenderer(displayListManager, this);
	mFingerTracker = new FingerTracker(displayListManager);
}

RRGLSurfaceView::~RRGLSurfaceView() {
	delete mFingerTracker;
	delete mRenderer;
}

bool RRGLSurfaceView::onTouchEvent(const void* event) {
	bool result = mFingerTracker->onTouchEvent(event);
	requestRender();
	return true;
}

void RRGLSurfaceView::requestRender() {
	// In the Android version, this calls GLSurfaceView.requestRender()
	// Here it triggers the renderer's onDrawFrame directly
	if (mRenderer) {
		mRenderer->onDrawFrame();
	}
}

void RRGLSurfaceView::onAttachedToWindow() {
	mDisplayListManager->onUIAttach();
}

void RRGLSurfaceView::onDetachedFromWindow() {
	mDisplayListManager->onUIDetach();
}

float RRGLSurfaceView::getScreenDensity() const {
	return mScreenDensity;
}

RRGLDisplayListRenderer* RRGLSurfaceView::getRenderer() const {
	return mRenderer;
}

} // namespace PinkReader
