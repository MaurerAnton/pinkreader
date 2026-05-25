// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/glview/displaylist/RRGLDisplayList.java
#pragma once

#include "rr_gl_renderable_group.h"

namespace PinkReader {

class RRGLMatrixStack;

class RRGLDisplayList : public RRGLRenderableGroup {
public:
	bool isAdded() const override {
		return true;
	}

protected:
	void renderInternal(RRGLMatrixStack* matrixStack, long time) override {
		RRGLRenderableGroup::renderInternal(matrixStack, time);
	}
};

} // namespace PinkReader
