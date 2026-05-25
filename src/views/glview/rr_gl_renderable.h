// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/glview/displaylist/RRGLRenderable.java
#pragma once

#include <stdexcept>

namespace PinkReader {

class RRGLMatrixStack;

class RRGLRenderable {
private:
	bool mVisible = true;
	int mAttachmentCount = 0;

public:
	virtual ~RRGLRenderable() = default;

	void hide() {
		mVisible = false;
	}

	void show() {
		mVisible = true;
	}

	bool isVisible() const {
		return mVisible;
	}

	void startRender(RRGLMatrixStack* stack, long time) {
		if (mVisible) {
			renderInternal(stack, time);
		}
	}

	virtual void onAdded() {
		mAttachmentCount++;
	}

	virtual bool isAdded() const {
		return mAttachmentCount > 0;
	}

	virtual void onRemoved() {
		mAttachmentCount--;
	}

	virtual bool isAnimating() {
		return false;
	}

	virtual void setOverallAlpha(float alpha) {
		(void)alpha;
		throw std::runtime_error("UnsupportedOperationException");
	}

protected:
	virtual void renderInternal(RRGLMatrixStack* stack, long time) = 0;
};

} // namespace PinkReader
