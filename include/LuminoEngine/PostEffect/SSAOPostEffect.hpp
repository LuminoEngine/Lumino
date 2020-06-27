#pragma once
#include "PostEffect.hpp"

namespace ln {
namespace detail { class SSAOImageEffectInstance; }

class SSAOImageEffect
    : public ImageEffect
{
public:

protected:
    virtual Ref<ImageEffectInstance> onCreateInstance() override;

LN_CONSTRUCT_ACCESS:
    SSAOImageEffect();
    void init();

private:
    friend class detail::SSAOImageEffectInstance;
};

namespace detail {

class SSAOImageEffectInstance
    : public ImageEffectInstance
{
protected:
    bool onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination) override;

LN_CONSTRUCT_ACCESS:
    SSAOImageEffectInstance();
    bool init(SSAOImageEffect* owner);

private:
    SSAOImageEffect* m_owner;
    Ref<SamplerState> m_samplerState;
    Ref<Material> m_occlusionMaterial;
    Ref<Material> m_blurAndCompositeMaterial;
};

} // namespace detail
} // namespace ln

