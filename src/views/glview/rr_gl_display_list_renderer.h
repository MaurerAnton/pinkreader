// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/glview/displaylist/RRGLDisplayListRenderer.java
#pragma once

#include "refreshable.h"

namespace PinkReader {

class RRGLDisplayList;
class RRGLContext;
class RRGLMatrixStack;
class RRGLSurfaceView;

// Forward-declare FingerListener; actual definition elsewhere
class FingerListener {
public:
	virtual ~FingerListener() = default;
};

class DisplayListManager : public FingerListener {
public:
	virtual ~DisplayListManager() = default;

	virtual void onGLSceneCreate(
			RRGLDisplayList* scene,
			RRGLContext* context,
			Refreshable* refreshable) = 0;

	virtual void onGLSceneResolutionChange(
			RRGLDisplayList* scene,
			RRGLContext* context,
			int width,
			int height) = 0;

	virtual bool onGLSceneUpdate(RRGLDisplayList* scene, RRGLContext* context) = 0;

	virtual void onUIAttach() = 0;
	virtual void onUIDetach() = 0;
};

class RRGLDisplayListRenderer : public Refreshable {
private:
	float mPixelMatrix[16] = {};

	RRGLDisplayList* mScene = nullptr;
	RRGLContext* mGLContext = nullptr;
	RRGLMatrixStack* mMatrixStack = nullptr;

	DisplayListManager* mDisplayListManager = nullptr;
	RRGLSurfaceView* mSurfaceView = nullptr;

	int frames = 0;
	long startTime = -1;

public:
	RRGLDisplayListRenderer(
			DisplayListManager* displayListManager,
			RRGLSurfaceView* surfaceView);

	void onSurfaceCreated();
	void onSurfaceChanged(int width, int height);
	void onDrawFrame();
	void refresh() override;
};

} // namespace PinkReader
