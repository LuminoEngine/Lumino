#pragma once

namespace ln {
class RenderTargetTexture;
class RenderingContext;
namespace detail { class ImageEffectRenderer; }

class ImageEffect
	: public Object
{

protected:
    virtual void onUpdateFrame(float elapsedSeconds);
	virtual void onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination) = 0;

LN_CONSTRUCT_ACCESS:
	ImageEffect();
	virtual ~ImageEffect();
	void init();
    void updateFrame(float elapsedSeconds);

private:

    friend class detail::ImageEffectRenderer;
};

//using ImageEffectInstanceRenderer = ImageEffect;

} // namespace ln

