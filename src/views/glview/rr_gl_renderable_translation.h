// Origin: RedReader/src/main/java/org/quantumbadger/redreader/views/glview/displaylist/RRGLRenderableTranslation.java
// Note: setPosition(MutableFloatPoint2D) overload references PinkReader::MutableFloatPoint2D
#pragma once

#include "rr_gl_renderable_render_hooks.h"
#include "../../common/mutable_float_point_2d.h"

namespace PinkReader {

class RRGLMatrixStack;

class RRGLRenderableTranslation : public RRGLRenderableRenderHooks {
private:
	float mPositionX = 0.0f;
	float mPositionY = 0.0f;

public:
	explicit RRGLRenderableTranslation(RRGLRenderable* entity)
		: RRGLRenderableRenderHooks(entity) {}

	void setPosition(float x, float y) {
		mPositionX = x;
		mPositionY = y;
	}

	void setPosition(const MutableFloatPoint2D& mPositionOffset) {
		mPositionX = mPositionOffset.x;
		mPositionY = mPositionOffset.y;
	}

protected:
	void preRender(RRGLMatrixStack* stack, long time) override;
	void postRender(RRGLMatrixStack* stack, long time) override;
};

} // namespace PinkReader
