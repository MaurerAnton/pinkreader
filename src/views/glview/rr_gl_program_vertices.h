// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/glview/program/RRGLProgramVertices.java
#pragma once

#include <GLES2/gl2.h>
#include "rr_gl_program.h"

namespace PinkReader {

class RRGLProgramVertices : public RRGLProgram {
private:
	int mVertexBufferHandle = -1;
	int mMatrixUniformHandle = -1;
	int mPixelMatrixUniformHandle = -1;

public:
	RRGLProgramVertices(const std::string& vertexShaderSource,
	                    const std::string& fragmentShaderSource)
		: RRGLProgram(vertexShaderSource, fragmentShaderSource) {}

	void activateVertexBuffer(const float* vertexBuffer) {
		glVertexAttribPointer(
				mVertexBufferHandle,
				3,
				GL_FLOAT,
				GL_FALSE,
				0,
				vertexBuffer);
	}

	void drawTriangleStrip(int vertices) {
		glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices);
	}

	void activateMatrix(const float* buf, int offset) {
		glUniformMatrix4fv(mMatrixUniformHandle, 1, GL_FALSE, buf + offset);
	}

	void activatePixelMatrix(const float* buf, int offset) {
		glUniformMatrix4fv(mPixelMatrixUniformHandle, 1, GL_FALSE, buf + offset);
	}

	void onActivated() override {
		glEnableVertexAttribArray(mVertexBufferHandle);
	}

	void onDeactivated() override {
		glDisableVertexAttribArray(mVertexBufferHandle);
	}

protected:
	void setVertexBufferHandle(int handle) {
		mVertexBufferHandle = handle;
	}

	void setMatrixUniformHandle(int handle) {
		mMatrixUniformHandle = handle;
	}

	void setPixelMatrixHandle(int handle) {
		mPixelMatrixUniformHandle = handle;
	}
};

} // namespace PinkReader
