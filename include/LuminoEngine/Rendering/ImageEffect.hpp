#pragma once

namespace ln {
class RenderingContext;
namespace detail { class ImageEffectRenderer; }

class ImageEffect
	: public Object
{
protected:
	virtual void onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination) = 0;

LN_CONSTRUCT_ACCESS:
	ImageEffect();
	virtual ~ImageEffect();
	void initialize();

private:

    friend class detail::ImageEffectRenderer;
};

} // namespace ln

