#pragma once
#include "ImageEffect.hpp"

namespace ln {

class ScreenBlurImageEffect
	: public ImageEffect
{
protected:
    virtual void onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination) override;

LN_CONSTRUCT_ACCESS:
    ScreenBlurImageEffect();
	virtual ~ScreenBlurImageEffect();
	void initialize();

private:
};

} // namespace ln

