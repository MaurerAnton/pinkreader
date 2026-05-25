// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/glview/displaylist/RRGLRenderableGroup.java
#pragma once

#include "rr_gl_renderable.h"
#include <vector>

namespace PinkReader {

class RRGLRenderableGroup : public RRGLRenderable {
private:
	std::vector<RRGLRenderable*> mChildren;

public:
	RRGLRenderableGroup() {
		mChildren.reserve(16);
	}

	void add(RRGLRenderable* child) {
		mChildren.push_back(child);
		if (isAdded()) {
			child->onAdded();
		}
	}

	void remove(RRGLRenderable* child) {
		if (isAdded()) {
			child->onRemoved();
		}
		for (auto it = mChildren.begin(); it != mChildren.end(); ++it) {
			if (*it == child) {
				mChildren.erase(it);
				break;
			}
		}
	}

	void onAdded() override {
		if (!isAdded()) {
			for (auto* entity : mChildren) {
				entity->onAdded();
			}
		}
		RRGLRenderable::onAdded();
	}

	void onRemoved() override {
		RRGLRenderable::onRemoved();
		if (!isAdded()) {
			for (auto* entity : mChildren) {
				entity->onRemoved();
			}
		}
	}

	bool isAnimating() override {
		for (auto* entity : mChildren) {
			if (entity->isAnimating()) {
				return true;
			}
		}
		return false;
	}

	void setOverallAlpha(float alpha) override {
		for (auto* entity : mChildren) {
			entity->setOverallAlpha(alpha);
		}
	}

protected:
	void renderInternal(RRGLMatrixStack* matrixStack, long time) override {
		for (auto* entity : mChildren) {
			entity->startRender(matrixStack, time);
		}
	}
};

} // namespace PinkReader
