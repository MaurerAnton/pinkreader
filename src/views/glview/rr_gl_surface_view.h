// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/glview/RRGLSurfaceView.java
#pragma once

#include "rr_gl_display_list_renderer.h"

namespace PinkReader {

// Forward-declare FingerTracker (defined in views/imageview/)
class FingerTracker;

class RRGLSurfaceView {
private:
	FingerTracker* mFingerTracker = nullptr;
	DisplayListManager* mDisplayListManager = nullptr;
	RRGLDisplayListRenderer* mRenderer = nullptr;

	float mScreenDensity = 1.0f;

public:
	RRGLSurfaceView(float screenDensity, DisplayListManager* displayListManager);
	~RRGLSurfaceView();

	// Non-copyable
	RRGLSurfaceView(const RRGLSurfaceView&) = delete;
	RRGLSurfaceView& operator=(const RRGLSurfaceView&) = delete;

	bool onTouchEvent(const void* event);
	void requestRender();

	void onAttachedToWindow();
	void onDetachedFromWindow();

	float getScreenDensity() const;

	RRGLDisplayListRenderer* getRenderer() const;
};

} // namespace PinkReader
