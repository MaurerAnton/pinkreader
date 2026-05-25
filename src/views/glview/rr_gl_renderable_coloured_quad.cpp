// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/glview/displaylist/RRGLRenderableColouredQuad.java (implementation)
#include "rr_gl_renderable_coloured_quad.h"
#include "rr_gl_matrix_stack.h"
#include "rr_gl_context.h"

namespace PinkReader {

const float RRGLRenderableColouredQuad::vertexData[] = {
	0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 1.0f, 0.0f
};

// Static vertex buffer pointer — data is in vertexData[], no allocation needed
const float* RRGLRenderableColouredQuad::mVertexBuffer = vertexData;

RRGLRenderableColouredQuad::RRGLRenderableColouredQuad(RRGLContext* glContext)
	: mGLContext(glContext) {}

void RRGLRenderableColouredQuad::setColour(float r, float g, float b, float a) {
	mRed = r;
	mGreen = g;
	mBlue = b;
	mAlpha = a;
}

void RRGLRenderableColouredQuad::setOverallAlpha(float alpha) {
	mOverallAlpha = alpha;
}

void RRGLRenderableColouredQuad::renderInternal(RRGLMatrixStack* matrixStack, long time) {
	(void)time;

	mGLContext->activateProgramColour();

	matrixStack->flush();

	mGLContext->activateVertexBuffer(mVertexBuffer);
	mGLContext->activateColour(mRed, mGreen, mBlue, mAlpha * mOverallAlpha);

	mGLContext->drawTriangleStrip(4);
}

} // namespace PinkReader
