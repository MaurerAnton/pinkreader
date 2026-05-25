// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/glview/program/RRGLProgramTexture.java
#pragma once

#include <GLES2/gl2.h>
#include "rr_gl_program_vertices.h"

namespace PinkReader {

class RRGLProgramTexture : public RRGLProgramVertices {
private:
	int mUVDataHandle = -1;
	int mTextureUniformHandle = -1;

	static const char* vertexShaderSource;
	static const char* fragmentShaderSource;

public:
	RRGLProgramTexture()
		: RRGLProgramVertices(vertexShaderSource, fragmentShaderSource) {

		setVertexBufferHandle(getAttributeHandle("a_Position"));
		setMatrixUniformHandle(getUniformHandle("u_Matrix"));
		setPixelMatrixHandle(getUniformHandle("u_PixelMatrix"));

		mUVDataHandle = getAttributeHandle("a_TexCoordinate");
		mTextureUniformHandle = getUniformHandle("u_Texture");
	}

	void activateTextureByHandle(int textureHandle) {
		glBindTexture(GL_TEXTURE_2D, textureHandle);
		glUniform1i(mTextureUniformHandle, 0);
	}

	void activateUVBuffer(const float* uvBuffer) {
		glVertexAttribPointer(
				mUVDataHandle,
				2,
				GL_FLOAT,
				GL_FALSE,
				0,
				uvBuffer);
	}

	void onActivated() override {
		RRGLProgramVertices::onActivated();
		glEnableVertexAttribArray(mUVDataHandle);
		glActiveTexture(GL_TEXTURE0);
	}

	void onDeactivated() override {
		RRGLProgramVertices::onDeactivated();
		glDisableVertexAttribArray(mUVDataHandle);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
};

} // namespace PinkReader
