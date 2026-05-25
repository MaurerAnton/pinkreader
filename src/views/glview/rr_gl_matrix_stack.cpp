// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/glview/program/RRGLMatrixStack.java (implementation)
#include "rr_gl_matrix_stack.h"
#include "rr_gl_context.h"
#include <cstring>
#include <stdexcept>

namespace PinkReader {

// Simple 4x4 matrix math (replaces android.opengl.Matrix)
static void matrixSetIdentity(float* m, int offset) {
	memset(m + offset, 0, 16 * sizeof(float));
	m[offset + 0] = 1.0f;
	m[offset + 5] = 1.0f;
	m[offset + 10] = 1.0f;
	m[offset + 15] = 1.0f;
}

static void matrixTranslate(float* m, int offset,
                            const float* src, int srcOffset,
                            float x, float y, float z) {
	// result = src translated by (x, y, z)
	for (int i = 0; i < 16; i++) {
		m[offset + i] = src[srcOffset + i];
	}
	m[offset + 12] += m[offset + 0] * x + m[offset + 4] * y + m[offset + 8] * z;
	m[offset + 13] += m[offset + 1] * x + m[offset + 5] * y + m[offset + 9] * z;
	m[offset + 14] += m[offset + 2] * x + m[offset + 6] * y + m[offset + 10] * z;
	m[offset + 15] += m[offset + 3] * x + m[offset + 7] * y + m[offset + 11] * z;
}

static void matrixScale(float* m, int offset,
                        const float* src, int srcOffset,
                        float x, float y, float z) {
	for (int i = 0; i < 4; i++) {
		int mi = offset + i;
		m[mi + 0] = src[srcOffset + i + 0] * x;
		m[mi + 4] = src[srcOffset + i + 4] * y;
		m[mi + 8] = src[srcOffset + i + 8] * z;
		m[mi + 12] = src[srcOffset + i + 12];
	}
}

RRGLMatrixStack::RRGLMatrixStack(RRGLContext* glContext)
	: mGLContext(glContext) {
	setIdentity();
}

int RRGLMatrixStack::pushAndTranslate(float offsetX, float offsetY) {
	mTopMatrixPos += 16;
	matrixTranslate(
			mMatrices,
			mTopMatrixPos,
			mMatrices,
			mTopMatrixPos - 16,
			offsetX,
			offsetY,
			0.0f);
	return mTopMatrixPos - 16;
}

int RRGLMatrixStack::pushAndScale(float factorX, float factorY) {
	mTopMatrixPos += 16;
	matrixScale(
			mMatrices,
			mTopMatrixPos,
			mMatrices,
			mTopMatrixPos - 16,
			factorX,
			factorY,
			0.0f);
	return mTopMatrixPos - 16;
}

int RRGLMatrixStack::pop() {
	mTopMatrixPos -= 16;
	return mTopMatrixPos;
}

void RRGLMatrixStack::setIdentity() {
	matrixSetIdentity(mMatrices, mTopMatrixPos);
}

void RRGLMatrixStack::scale(float factorX, float factorY, float factorZ) {
	matrixScale(mMatrices, mTopMatrixPos, mMatrices, mTopMatrixPos,
	            factorX, factorY, factorZ);
}

void RRGLMatrixStack::flush() {
	mGLContext->activateMatrix(mMatrices, mTopMatrixPos);
}

void RRGLMatrixStack::assertAtRoot() {
	if (mTopMatrixPos != 0) {
		throw std::runtime_error("assertAtRoot() failed!");
	}

	for (int i = 0; i < 16; i++) {
		switch (i) {
			case 0:
			case 5:
			case 10:
			case 15:
				if (mMatrices[i] != 1.0f) {
					throw std::runtime_error("Root matrix is not identity!");
				}
				break;
			default:
				if (mMatrices[i] != 0.0f) {
					throw std::runtime_error("Root matrix is not identity!");
				}
				break;
		}
	}
}

} // namespace PinkReader
