// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/glview/program/RRGLContext.java (implementation)
#include "rr_gl_context.h"
#include "rr_gl_program_texture.h"
#include "rr_gl_program_colour.h"
#include "rr_gl_program_vertices.h"

namespace PinkReader {

RRGLContext::RRGLContext(float screenDensity)
	: mScreenDensity(screenDensity) {
	mProgramTexture = new RRGLProgramTexture();
	mProgramColour = new RRGLProgramColour();
}

RRGLContext::~RRGLContext() {
	delete mProgramTexture;
	delete mProgramColour;
}

int RRGLContext::dpToPixels(float dp) {
	return static_cast<int>(dp * mScreenDensity + 0.5f);
}

float RRGLContext::getScreenDensity() const {
	return mScreenDensity;
}

void RRGLContext::activateProgramColour() {
	if (mProgramCurrent != static_cast<RRGLProgramVertices*>(mProgramColour)) {
		activateProgram(mProgramColour);
	}
}

void RRGLContext::activateProgramTexture() {
	if (mProgramCurrent != static_cast<RRGLProgramVertices*>(mProgramTexture)) {
		activateProgram(mProgramTexture);
	}
}

void RRGLContext::activateProgram(RRGLProgramVertices* program) {
	if (mProgramCurrent != nullptr) {
		mProgramCurrent->onDeactivated();
	}

	glUseProgram(program->getHandle());
	mProgramCurrent = program;

	program->onActivated();

	if (mPixelMatrix != nullptr) {
		program->activatePixelMatrix(mPixelMatrix, mPixelMatrixOffset);
	}
}

void RRGLContext::activateTextureByHandle(int textureHandle) {
	mProgramTexture->activateTextureByHandle(textureHandle);
}

void RRGLContext::activateVertexBuffer(const float* vertexBuffer) {
	mProgramCurrent->activateVertexBuffer(vertexBuffer);
}

void RRGLContext::activateColour(float r, float g, float b, float a) {
	mProgramColour->activateColour(r, g, b, a);
}

void RRGLContext::activateUVBuffer(const float* uvBuffer) {
	mProgramTexture->activateUVBuffer(uvBuffer);
}

void RRGLContext::drawTriangleStrip(int vertices) {
	mProgramCurrent->drawTriangleStrip(vertices);
}

void RRGLContext::activateMatrix(const float* buf, int offset) {
	mProgramCurrent->activateMatrix(buf, offset);
}

void RRGLContext::activatePixelMatrix(const float* buf, int offset) {
	mPixelMatrix = const_cast<float*>(buf);
	mPixelMatrixOffset = offset;

	if (mProgramCurrent != nullptr) {
		mProgramCurrent->activatePixelMatrix(buf, offset);
	}
}

void RRGLContext::setClearColor(float r, float g, float b, float a) {
	glClearColor(r, g, b, a);
}

void RRGLContext::clear() {
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void RRGLContext::setViewport(int width, int height) {
	glViewport(0, 0, width, height);
}

} // namespace PinkReader
