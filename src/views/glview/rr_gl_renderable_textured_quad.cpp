// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/glview/displaylist/RRGLRenderableTexturedQuad.java (implementation)
#include "rr_gl_renderable_textured_quad.h"
#include "rr_gl_texture.h"
#include "rr_gl_context.h"
#include "rr_gl_matrix_stack.h"

namespace PinkReader {

const float RRGLRenderableTexturedQuad::vertexData[] = {
	0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 1.0f, 0.0f
};

const float RRGLRenderableTexturedQuad::uvData[] = {
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 1.0f
};

const float* RRGLRenderableTexturedQuad::mVertexBuffer = vertexData;
const float* RRGLRenderableTexturedQuad::mUVBuffer = uvData;

RRGLRenderableTexturedQuad::RRGLRenderableTexturedQuad(RRGLContext* glContext, RRGLTexture* texture)
	: mTexture(texture)
	, mGLContext(glContext) {}

void RRGLRenderableTexturedQuad::setTexture(RRGLTexture* newTexture) {
	if (isAdded()) {
		mTexture->releaseReference();
	}

	mTexture = newTexture;

	if (isAdded()) {
		mTexture->addReference();
	}
}

void RRGLRenderableTexturedQuad::onAdded() {
	RRGLRenderable::onAdded();
	mTexture->addReference();
}

void RRGLRenderableTexturedQuad::onRemoved() {
	mTexture->releaseReference();
	RRGLRenderable::onRemoved();
}

void RRGLRenderableTexturedQuad::renderInternal(RRGLMatrixStack* matrixStack, long time) {
	(void)time;

	mGLContext->activateProgramTexture();

	mTexture->activate();
	matrixStack->flush();

	mGLContext->activateVertexBuffer(mVertexBuffer);
	mGLContext->activateUVBuffer(mUVBuffer);

	mGLContext->drawTriangleStrip(4);
}

} // namespace PinkReader
