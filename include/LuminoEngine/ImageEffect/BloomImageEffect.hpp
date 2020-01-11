#pragma once
#include "ImageEffect.hpp"

namespace ln {
class Material;

class BloomImageEffect
	: public ImageEffect
{
public:
    static Ref<BloomImageEffect> create();

protected:
    virtual void onUpdateFrame(float elapsedSeconds) override;
    virtual void onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination) override;

LN_CONSTRUCT_ACCESS:
	BloomImageEffect();
	void init();

private:
	void resetResources(int resx, int resy);

	Ref<Material> m_materialHighPassFilter;
	List<Ref<Material>> m_separableBlurMaterials;
	Ref<Material> m_compositeMaterial;

	int m_viewWidth;
	int m_viewHeight;

};

} // namespace ln

