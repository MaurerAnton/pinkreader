// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/glview/displaylist/RRGLRenderableBlend.java
#pragma once

#include "rr_gl_renderable_render_hooks.h"
#include <GLES2/gl2.h>

namespace PinkReader {

class RRGLRenderableBlend : public RRGLRenderableRenderHooks {
public:
	explicit RRGLRenderableBlend(RRGLRenderable* entity)
		: RRGLRenderableRenderHooks(entity) {}

protected:
	void preRender(RRGLMatrixStack* stack, long time) override {
		(void)stack;
		(void)time;
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void postRender(RRGLMatrixStack* stack, long time) override {
		(void)stack;
		(void)time;
		glDisable(GL_BLEND);
	}
};

} // namespace PinkReader
