// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/glview/program/RRGLContext.java
#pragma once

#include <GLES2/gl2.h>

namespace PinkReader {

class RRGLProgramTexture;
class RRGLProgramColour;
class RRGLProgramVertices;

class RRGLContext {
private:
	RRGLProgramTexture* mProgramTexture = nullptr;
	RRGLProgramColour* mProgramColour = nullptr;

	float* mPixelMatrix = nullptr;
	int mPixelMatrixOffset = 0;

	RRGLProgramVertices* mProgramCurrent = nullptr;

	// Platform context abstraction: stores screen density
	float mScreenDensity = 1.0f;

	void activateProgram(RRGLProgramVertices* program);

public:
	explicit RRGLContext(float screenDensity);
	~RRGLContext();

	// Non-copyable
	RRGLContext(const RRGLContext&) = delete;
	RRGLContext& operator=(const RRGLContext&) = delete;

	int dpToPixels(float dp);
	float getScreenDensity() const;

	void activateProgramColour();
	void activateProgramTexture();
	void activateTextureByHandle(int textureHandle);

	void activateVertexBuffer(const float* vertexBuffer);
	void activateColour(float r, float g, float b, float a);
	void activateUVBuffer(const float* uvBuffer);

	void drawTriangleStrip(int vertices);

	void activateMatrix(const float* buf, int offset);
	void activatePixelMatrix(const float* buf, int offset);

	void setClearColor(float r, float g, float b, float a);
	void clear();
	void setViewport(int width, int height);
};

} // namespace PinkReader
