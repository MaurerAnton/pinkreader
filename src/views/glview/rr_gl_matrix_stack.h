// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/glview/program/RRGLMatrixStack.java
#pragma once

#include <GLES2/gl2.h>
#include <stdexcept>
#include <cstring>

namespace PinkReader {

class RRGLContext;

class RRGLMatrixStack {
private:
	int mTopMatrixPos = 0;
	float mMatrices[16 * 128] = {};
	RRGLContext* mGLContext = nullptr;

public:
	explicit RRGLMatrixStack(RRGLContext* glContext);

	int pushAndTranslate(float offsetX, float offsetY);
	int pushAndScale(float factorX, float factorY);
	int pop();
	void setIdentity();
	void scale(float factorX, float factorY, float factorZ);
	void flush();
	void assertAtRoot();
};

} // namespace PinkReader
