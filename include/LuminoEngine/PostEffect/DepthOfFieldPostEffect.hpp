#pragma once
#include "PostEffect.hpp"

namespace ln {
namespace detail { class DepthOfFieldImageEffectInstance; }

class DepthOfFieldImageEffect
    : public ImageEffect
{
public:

protected:
    virtual Ref<ImageEffectInstance> onCreateInstance() override;

LN_CONSTRUCT_ACCESS:
    DepthOfFieldImageEffect();
    void init();

private:
    friend class detail::DepthOfFieldImageEffectInstance;
};

namespace detail {

class DepthOfFieldImageEffectInstance
    : public ImageEffectInstance
{
protected:
    bool onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination) override;

LN_CONSTRUCT_ACCESS:
    DepthOfFieldImageEffectInstance();
    bool init(DepthOfFieldImageEffect* owner);

private:
    DepthOfFieldImageEffect* m_owner;
    Ref<Material> m_copyMaterial;
    Ref<Material> m_dofMaterial;
    Ref<SamplerState> m_samplerState;
};

} // namespace detail
} // namespace ln

