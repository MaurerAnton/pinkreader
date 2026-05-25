// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/glview/displaylist/RRGLDisplayListRenderer.java (implementation)
#include "rr_gl_display_list_renderer.h"
#include "rr_gl_display_list.h"
#include "rr_gl_context.h"
#include "rr_gl_matrix_stack.h"
#include "rr_gl_surface_view.h"

#include <cstdio>
#include <time.h>
#include <cstring>

// Simple time helper replacing System.currentTimeMillis()
static long currentTimeMillis() {
	struct timespec ts;
	clock_gettime(CLOCK_REALTIME, &ts);
	return ts.tv_sec * 1000L + ts.tv_nsec / 1000000L;
}

namespace PinkReader {

RRGLDisplayListRenderer::RRGLDisplayListRenderer(
		DisplayListManager* displayListManager,
		RRGLSurfaceView* surfaceView)
	: mDisplayListManager(displayListManager)
	, mSurfaceView(surfaceView) {}

void RRGLDisplayListRenderer::onSurfaceCreated() {
	mGLContext = new RRGLContext(mSurfaceView->getScreenDensity());
	mMatrixStack = new RRGLMatrixStack(mGLContext);
	mScene = new RRGLDisplayList();

	mGLContext->setClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	mDisplayListManager->onGLSceneCreate(mScene, mGLContext, this);
}

void RRGLDisplayListRenderer::onSurfaceChanged(int width, int height) {
	mGLContext->setViewport(width, height);

	float hScale = 2.0f / static_cast<float>(width);
	float vScale = -2.0f / static_cast<float>(height);

	// matrixSetIdentity
	memset(mPixelMatrix, 0, 16 * sizeof(float));
	mPixelMatrix[0] = 1.0f;
	mPixelMatrix[5] = 1.0f;
	mPixelMatrix[10] = 1.0f;
	mPixelMatrix[15] = 1.0f;

	// matrixTranslate by (-1, 1, 0)
	mPixelMatrix[12] = mPixelMatrix[0] * (-1.0f) + mPixelMatrix[4] * 1.0f + mPixelMatrix[8] * 0.0f + mPixelMatrix[12];
	mPixelMatrix[13] = mPixelMatrix[1] * (-1.0f) + mPixelMatrix[5] * 1.0f + mPixelMatrix[9] * 0.0f + mPixelMatrix[13];
	mPixelMatrix[14] = mPixelMatrix[2] * (-1.0f) + mPixelMatrix[6] * 1.0f + mPixelMatrix[10] * 0.0f + mPixelMatrix[14];
	mPixelMatrix[15] = mPixelMatrix[3] * (-1.0f) + mPixelMatrix[7] * 1.0f + mPixelMatrix[11] * 0.0f + mPixelMatrix[15];

	// matrixScaleM by (hScale, vScale, 1)
	for (int i = 0; i < 4; i++) {
		mPixelMatrix[i + 0] *= hScale;
		mPixelMatrix[i + 4] *= vScale;
		// z unchanged (factor 1)
	}

	mDisplayListManager->onGLSceneResolutionChange(mScene, mGLContext, width, height);
}

void RRGLDisplayListRenderer::onDrawFrame() {
	long time = currentTimeMillis();

	if (startTime == -1) {
		startTime = time;
	}

	frames++;

	if (time - startTime >= 1000) {
		startTime = time;
		fprintf(stderr, "FPS: Frames: %d\n", frames);
		frames = 0;
	}

	bool animating = mDisplayListManager->onGLSceneUpdate(mScene, mGLContext);

	mGLContext->clear();

	mGLContext->activatePixelMatrix(mPixelMatrix, 0);

	mMatrixStack->assertAtRoot();
	mScene->startRender(mMatrixStack, time);
	mMatrixStack->assertAtRoot();

	if (animating || mScene->isAnimating()) {
		mSurfaceView->requestRender();
	}
}

void RRGLDisplayListRenderer::refresh() {
	mSurfaceView->requestRender();
}

} // namespace PinkReader
