#pragma once
#include "PostEffect.hpp"

namespace ln {
namespace detail { class FXAAImageEffectInstance; }

class FXAAImageEffect
    : public ImageEffect
{
public:

protected:
    virtual Ref<ImageEffectInstance> onCreateInstance() override;

LN_CONSTRUCT_ACCESS:
    FXAAImageEffect();
    void init();

private:
    friend class detail::FXAAImageEffectInstance;
};

namespace detail {

class FXAAImageEffectInstance
    : public ImageEffectInstance
{
protected:
    bool onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination) override;

LN_CONSTRUCT_ACCESS:
    FXAAImageEffectInstance();
    bool init(FXAAImageEffect* owner);

private:
    FXAAImageEffect* m_owner;
    Ref<Material> m_material;
};

} // namespace detail
} // namespace ln

