#pragma once
#include "ImageEffect.hpp"

namespace ln {
namespace detail { class TonemapImageEffectInstance; }

class TonemapImageEffect
    : public ImageEffect
{
public:

protected:
    virtual Ref<ImageEffectInstance> onCreateInstance() override;

LN_CONSTRUCT_ACCESS:
    TonemapImageEffect();
    void init();

private:
    friend class detail::TonemapImageEffectInstance;
};

namespace detail {

class TonemapImageEffectInstance
    : public ImageEffectInstance
{
protected:
    bool onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination) override;

LN_CONSTRUCT_ACCESS:
    TonemapImageEffectInstance();
    bool init(TonemapImageEffect* owner);

private:
    TonemapImageEffect* m_owner;
    Ref<Material> m_material;
};

} // namespace detail
} // namespace ln

