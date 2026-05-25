// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/glview/displaylist/RRGLRenderableScale.java
#pragma once

#include "rr_gl_renderable_render_hooks.h"

namespace PinkReader {

class RRGLMatrixStack;

class RRGLRenderableScale : public RRGLRenderableRenderHooks {
private:
	float mScaleX = 1.0f;
	float mScaleY = 1.0f;

public:
	explicit RRGLRenderableScale(RRGLRenderable* entity)
		: RRGLRenderableRenderHooks(entity) {}

	void setScale(float x, float y) {
		mScaleX = x;
		mScaleY = y;
	}

protected:
	void preRender(RRGLMatrixStack* stack, long time) override;
	void postRender(RRGLMatrixStack* stack, long time) override;
};

} // namespace PinkReader
