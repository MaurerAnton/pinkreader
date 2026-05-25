// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/glview/program/RRGLProgramColour.java
#pragma once

#include <GLES2/gl2.h>
#include "rr_gl_program_vertices.h"

namespace PinkReader {

class RRGLProgramColour : public RRGLProgramVertices {
private:
	int mColorHandle = -1;

	static const char* vertexShaderSource;
	static const char* fragmentShaderSource;

public:
	RRGLProgramColour()
		: RRGLProgramVertices(vertexShaderSource, fragmentShaderSource) {

		setVertexBufferHandle(getAttributeHandle("a_Position"));
		setMatrixUniformHandle(getUniformHandle("u_Matrix"));
		setPixelMatrixHandle(getUniformHandle("u_PixelMatrix"));

		mColorHandle = getUniformHandle("u_Color");
	}

	void activateColour(float r, float g, float b, float a) {
		glUniform4f(mColorHandle, r, g, b, a);
	}

	void onActivated() override {
		RRGLProgramVertices::onActivated();
		glEnableVertexAttribArray(mColorHandle);
	}

	void onDeactivated() override {
		RRGLProgramVertices::onDeactivated();
		glDisableVertexAttribArray(mColorHandle);
	}
};

} // namespace PinkReader
