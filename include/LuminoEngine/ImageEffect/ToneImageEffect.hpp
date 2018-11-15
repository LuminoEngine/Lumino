#pragma once
#include "ImageEffect.hpp"

namespace ln {
class Material;

class ToneImageEffect
	: public ImageEffect
{
protected:
    virtual void onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination) override;

LN_CONSTRUCT_ACCESS:
    ToneImageEffect();
	virtual ~ToneImageEffect();
	void initialize();

private:
    Ref<Material> m_material;
};

} // namespace ln

