// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/glview/displaylist/RRGLRenderableTexturedQuad.java
#pragma once

#include "rr_gl_renderable.h"

namespace PinkReader {

class RRGLContext;
class RRGLTexture;
class RRGLMatrixStack;

class RRGLRenderableTexturedQuad : public RRGLRenderable {
private:
	RRGLTexture* mTexture = nullptr;
	RRGLContext* mGLContext = nullptr;

	static const float vertexData[];
	static const float uvData[];
	static const float* mVertexBuffer;
	static const float* mUVBuffer;

public:
	RRGLRenderableTexturedQuad(RRGLContext* glContext, RRGLTexture* texture);

	void setTexture(RRGLTexture* newTexture);

	void onAdded() override;
	void onRemoved() override;

protected:
	void renderInternal(RRGLMatrixStack* matrixStack, long time) override;
};

} // namespace PinkReader
