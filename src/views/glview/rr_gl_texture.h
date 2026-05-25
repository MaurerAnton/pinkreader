// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/glview/program/RRGLTexture.java
#pragma once

#include <GLES2/gl2.h>
#include <stdexcept>

namespace PinkReader {

class RRGLContext;

class RRGLTexture {
private:
	int mTextureHandle = 0;
	RRGLContext* mGLContext = nullptr;
	int mRefCount = 0;

	static int loadTexture(const void* bitmapData, int width, int height, bool smooth);
	static void deleteTexture(int handle);

public:
	// Construct with raw RGBA pixel data (replaces Android Bitmap)
	RRGLTexture(RRGLContext* glContext, const void* bitmapData, int width, int height, bool smooth);

	~RRGLTexture();

	// Non-copyable
	RRGLTexture(const RRGLTexture&) = delete;
	RRGLTexture& operator=(const RRGLTexture&) = delete;

	void addReference();
	void releaseReference();
	void activate();
};

} // namespace PinkReader
