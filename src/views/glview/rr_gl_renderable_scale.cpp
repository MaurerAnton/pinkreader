// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/glview/displaylist/RRGLRenderableScale.java (implementation)
#include "rr_gl_renderable_scale.h"
#include "rr_gl_matrix_stack.h"

namespace PinkReader {

void RRGLRenderableScale::preRender(RRGLMatrixStack* stack, long time) {
	(void)time;
	stack->pushAndScale(mScaleX, mScaleY);
}

void RRGLRenderableScale::postRender(RRGLMatrixStack* stack, long time) {
	(void)time;
	stack->pop();
}

} // namespace PinkReader
