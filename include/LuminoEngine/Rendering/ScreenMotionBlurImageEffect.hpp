#pragma once
#include "ImageEffect.hpp"

namespace ln {

class ScreenMotionBlurImageEffect
	: public ImageEffect
{
protected:
    virtual void onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination) override;

LN_CONSTRUCT_ACCESS:
    ScreenMotionBlurImageEffect();
	virtual ~ScreenMotionBlurImageEffect();
	void initialize();

private:
};

} // namespace ln

