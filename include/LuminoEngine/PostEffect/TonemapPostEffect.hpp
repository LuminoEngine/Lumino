#pragma once
#include "PostEffect.hpp"

namespace ln {
namespace detail { class TonemapPostEffectInstance; }

class TonemapPostEffect
    : public PostEffect
{
public:

protected:
    virtual Ref<PostEffectInstance> onCreateInstance() override;

LN_CONSTRUCT_ACCESS:
    TonemapPostEffect();
    void init();

private:
    friend class detail::TonemapPostEffectInstance;
};

namespace detail {

class TonemapPostEffectInstance
    : public PostEffectInstance
{
protected:
    bool onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination) override;

LN_CONSTRUCT_ACCESS:
    TonemapPostEffectInstance();
    bool init(TonemapPostEffect* owner);

private:
    TonemapPostEffect* m_owner;
    Ref<Material> m_material;
};

} // namespace detail
} // namespace ln

