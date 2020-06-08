#pragma once
#include "ImageEffect.hpp"

namespace ln {
namespace detail { class SSRImageEffectInstance; }

class SSRImageEffect
    : public ImageEffect
{
public:

protected:
    virtual Ref<ImageEffectInstance> onCreateInstance() override;

LN_CONSTRUCT_ACCESS:
    SSRImageEffect();
    void init();

private:
    friend class detail::SSRImageEffectInstance;
};


namespace detail {

class SSRImageEffectInstance
    : public ImageEffectInstance
{
protected:
    void onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination) override;

LN_CONSTRUCT_ACCESS:
    SSRImageEffectInstance();
    bool init(SSRImageEffect* owner);

private:
    void resetResources(int resx, int resy);

    SSRImageEffect* m_owner;
    Ref<Material> m_ssrMaterial;
    Ref<Material> m_ssrBlurMaterial1;
    Ref<Material> m_ssrBlurMaterial2;
    Ref<Material> m_ssrCompositeMaterial;

    int m_viewWidth;
    int m_viewHeight;
    Ref<RenderTargetTexture> m_ssrTarget;
    Ref<RenderTargetTexture> m_blurTarget1;
    Ref<RenderTargetTexture> m_blurTarget2;
    Ref<RenderTargetTexture> m_compositeTarget;
    Ref<SamplerState> m_samplerState;

    // Find optimization
    ShaderParameter2* m_paramColorSampler;
    ShaderParameter2* m_paramSSRSampler;

    //List<Ref<RenderTargetTexture>> m_renderTargetsHorizontal;
    //List<Ref<RenderTargetTexture>> m_renderTargetsVertical;
    //Ref<Material> m_compositeMaterial;

    //List<Ref<Material>> m_separableBlurMaterialsH;
    //List<Ref<Material>> m_separableBlurMaterialsV;
    //Ref<Material> m_materialHighPassFilter;
};

} // namespace detail
} // namespace ln

