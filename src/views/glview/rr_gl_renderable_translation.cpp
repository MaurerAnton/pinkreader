// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/glview/displaylist/RRGLRenderableTranslation.java (implementation)
#include "rr_gl_renderable_translation.h"
#include "rr_gl_matrix_stack.h"

namespace PinkReader {

void RRGLRenderableTranslation::preRender(RRGLMatrixStack* stack, long time) {
	(void)time;
	stack->pushAndTranslate(mPositionX, mPositionY);
}

void RRGLRenderableTranslation::postRender(RRGLMatrixStack* stack, long time) {
	(void)time;
	stack->pop();
}

} // namespace PinkReader
