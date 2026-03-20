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

struct TonemapPostEffectParams
{
    float _paramA;  // shoulderStrength
    float _paramB;  // linearStrength
    float _paramCB;    // param.linearStrength * param.linearAngle
    float _paramDE;    // param.toeStrength * param.toeNumerator
    float _paramDF;    // param.toeStrength * param.toeDenominator
    float _paramEperF;  // param.toeNumerator / param.toeDenominator
    float _paramF_White;//
    float _exposure;

    void setup(
        const float linearWhite,
        const float shoulderStrength,
        const float linearStrength,
        const float linearAngle,
        const float toeStrength,
        const float toeNumerator,
        const float toeDenominator,
        const float exposure);
};

class TonemapPostEffectInstance
    : public PostEffectInstance
{
protected:
    bool onRender(RenderView* renderView, CommandList* context, RenderTargetTexture* source, RenderTargetTexture* destination) override;

LN_CONSTRUCT_ACCESS:
    TonemapPostEffectInstance();
    bool init(TonemapPostEffect* owner);

private:
    TonemapPostEffect* m_owner;
    Ref<Material> m_material;
};

} // namespace detail
} // namespace ln

