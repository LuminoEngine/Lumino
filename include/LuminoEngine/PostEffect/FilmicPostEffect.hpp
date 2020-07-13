#pragma once
#include "PostEffect.hpp"

namespace ln {
class SamplerState;
namespace detail { class FilmicPostEffectInstance; }

class FilmicPostEffect
    : public PostEffect
{
public:

protected:
    virtual Ref<PostEffectInstance> onCreateInstance() override;

LN_CONSTRUCT_ACCESS:
    FilmicPostEffect();
    void init();

private:
    friend class detail::FilmicPostEffectInstance;
};

namespace detail {

class FilmicPostEffectInstance
    : public PostEffectInstance
{
protected:
    bool onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination) override;

LN_CONSTRUCT_ACCESS:
    FilmicPostEffectInstance();
    bool init(FilmicPostEffect* owner);

private:
    FilmicPostEffect* m_owner;
    Ref<Material> m_material;
    Ref<Material> m_ssaoMaterial;
    Ref<SamplerState> m_samplerState;
};

} // namespace detail
} // namespace ln

