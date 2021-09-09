#pragma once
#include "PostEffect.hpp"

namespace ln {
namespace detail { class SSAOPostEffectInstance; }

class SSAOPostEffect
    : public PostEffect
{
public:

protected:
    virtual Ref<PostEffectInstance> onCreateInstance() override;

LN_CONSTRUCT_ACCESS:
    SSAOPostEffect();
    void init();

private:
    friend class detail::SSAOPostEffectInstance;
};

namespace detail {

class SSAOPostEffectInstance
    : public PostEffectInstance
{
protected:
    bool onRender(RenderView* renderView, CommandList* context, RenderTargetTexture* source, RenderTargetTexture* destination) override;

LN_CONSTRUCT_ACCESS:
    SSAOPostEffectInstance();
    bool init(SSAOPostEffect* owner);

private:
    SSAOPostEffect* m_owner;
    Ref<SamplerState> m_samplerState;
    Ref<Material> m_occlusionMaterial;
    Ref<Material> m_blurAndCompositeMaterial;
};

} // namespace detail
} // namespace ln

