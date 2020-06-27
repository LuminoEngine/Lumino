#pragma once
#include "PostEffect.hpp"

namespace ln {
namespace detail { class DepthOfFieldPostEffectInstance; }

class DepthOfFieldPostEffect
    : public PostEffect
{
public:

protected:
    virtual Ref<PostEffectInstance> onCreateInstance() override;

LN_CONSTRUCT_ACCESS:
    DepthOfFieldPostEffect();
    void init();

private:
    friend class detail::DepthOfFieldPostEffectInstance;
};

namespace detail {

class DepthOfFieldPostEffectInstance
    : public PostEffectInstance
{
protected:
    bool onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination) override;

LN_CONSTRUCT_ACCESS:
    DepthOfFieldPostEffectInstance();
    bool init(DepthOfFieldPostEffect* owner);

private:
    DepthOfFieldPostEffect* m_owner;
    Ref<Material> m_copyMaterial;
    Ref<Material> m_dofMaterial;
    Ref<SamplerState> m_samplerState;
};

} // namespace detail
} // namespace ln

