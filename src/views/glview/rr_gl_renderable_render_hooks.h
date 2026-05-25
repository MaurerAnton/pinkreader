// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/glview/displaylist/RRGLRenderableRenderHooks.java
#pragma once

#include "rr_gl_renderable.h"

namespace PinkReader {

class RRGLRenderableRenderHooks : public RRGLRenderable {
private:
	RRGLRenderable* mEntity;

public:
	explicit RRGLRenderableRenderHooks(RRGLRenderable* entity)
		: mEntity(entity) {}

	void renderInternal(RRGLMatrixStack* stack, long time) override {
		preRender(stack, time);
		mEntity->startRender(stack, time);
		postRender(stack, time);
	}

	void onAdded() override {
		mEntity->onAdded();
		RRGLRenderable::onAdded();
	}

	void onRemoved() override {
		RRGLRenderable::onRemoved();
		mEntity->onRemoved();
	}

	bool isAnimating() override {
		return mEntity->isAnimating();
	}

	void setOverallAlpha(float alpha) override {
		mEntity->setOverallAlpha(alpha);
	}

protected:
	virtual void preRender(RRGLMatrixStack* stack, long time) = 0;
	virtual void postRender(RRGLMatrixStack* stack, long time) = 0;
};

} // namespace PinkReader
