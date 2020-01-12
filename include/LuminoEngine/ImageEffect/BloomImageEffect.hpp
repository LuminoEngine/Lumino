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

	float m_luminosityThreshold;
	float m_strength;
	float m_bloomRadius;

	Ref<Material> m_materialHighPassFilter;
	List<Ref<Material>> m_separableBlurMaterialsH;
	List<Ref<Material>> m_separableBlurMaterialsV;
	Ref<Material> m_compositeMaterial;
	Ref<SamplerState> m_samplerState;

	Ref<RenderTargetTexture> m_renderTargetBright;
	List<Ref<RenderTargetTexture>> m_renderTargetsHorizontal;
	List<Ref<RenderTargetTexture>> m_renderTargetsVertical;

	int m_viewWidth;
	int m_viewHeight;

};

} // namespace ln

