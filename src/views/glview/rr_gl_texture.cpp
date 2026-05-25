// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/glview/program/RRGLTexture.java (implementation)
#include "rr_gl_texture.h"
#include "rr_gl_context.h"
#include <GLES2/gl2.h>
#include <stdexcept>

namespace PinkReader {

// Static helper: allocate an OpenGL texture from raw RGBA pixel data
int RRGLTexture::loadTexture(const void* bitmapData, int width, int height, bool smooth) {
	int textureHandle = 0;
	glGenTextures(1, reinterpret_cast<GLuint*>(&textureHandle));

	if (textureHandle == 0) {
		throw std::runtime_error("OpenGL error: glGenTextures failed.");
	}

	int filter = smooth ? GL_LINEAR : GL_NEAREST;

	glBindTexture(GL_TEXTURE_2D, textureHandle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
	             GL_RGBA, GL_UNSIGNED_BYTE, bitmapData);

	return textureHandle;
}

void RRGLTexture::deleteTexture(int handle) {
	GLuint handles[1] = { static_cast<GLuint>(handle) };
	glDeleteTextures(1, handles);
}

RRGLTexture::RRGLTexture(RRGLContext* glContext, const void* bitmapData,
                         int width, int height, bool smooth)
	: mTextureHandle(loadTexture(bitmapData, width, height, smooth))
	, mGLContext(glContext)
	, mRefCount(1) {}

RRGLTexture::~RRGLTexture() {
	if (mRefCount > 0) {
		deleteTexture(mTextureHandle);
	}
}

void RRGLTexture::addReference() {
	mRefCount++;
}

void RRGLTexture::releaseReference() {
	mRefCount--;
	if (mRefCount == 0) {
		deleteTexture(mTextureHandle);
		mTextureHandle = 0;
	}
}

void RRGLTexture::activate() {
	mGLContext->activateTextureByHandle(mTextureHandle);
}

} // namespace PinkReader
