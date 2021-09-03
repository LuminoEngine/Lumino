#pragma once
#include "PostEffect.hpp"

namespace ln {
namespace detail { class FXAAPostEffectInstance; }

class FXAAPostEffect
    : public PostEffect
{
public:

protected:
    virtual Ref<PostEffectInstance> onCreateInstance() override;

LN_CONSTRUCT_ACCESS:
    FXAAPostEffect();
    void init();

private:
    friend class detail::FXAAPostEffectInstance;
};

namespace detail {

class FXAAPostEffectInstance
    : public PostEffectInstance
{
protected:
    bool onRender(RenderView* renderView, CommandList* context, RenderTargetTexture* source, RenderTargetTexture* destination) override;

LN_CONSTRUCT_ACCESS:
    FXAAPostEffectInstance();
    bool init(FXAAPostEffect* owner);

private:
    FXAAPostEffect* m_owner;
    Ref<Material> m_material;
};

} // namespace detail
} // namespace ln

