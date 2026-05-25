// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/glview/displaylist/RRGLRenderableColouredQuad.java
#pragma once

#include "rr_gl_renderable.h"

namespace PinkReader {

class RRGLContext;
class RRGLMatrixStack;

class RRGLRenderableColouredQuad : public RRGLRenderable {
private:
	RRGLContext* mGLContext = nullptr;

	float mRed = 0.0f;
	float mGreen = 0.0f;
	float mBlue = 0.0f;
	float mAlpha = 0.0f;
	float mOverallAlpha = 1.0f;

	static const float vertexData[];
	static const float* mVertexBuffer;

public:
	explicit RRGLRenderableColouredQuad(RRGLContext* glContext);

	void setColour(float r, float g, float b, float a);
	void setOverallAlpha(float alpha) override;

protected:
	void renderInternal(RRGLMatrixStack* matrixStack, long time) override;
};

} // namespace PinkReader
