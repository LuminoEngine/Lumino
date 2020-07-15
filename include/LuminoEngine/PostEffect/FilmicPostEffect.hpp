#pragma once
#include <LuminoEngine/PostEffect/BloomPostEffect.hpp>

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
    float m_luminosityThreshold;
    float m_bloomStrength;
    float m_bloomRadius;

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
    Ref<Material> m_integrationMaterial;
    Ref<Material> m_ssaoMaterial;
    Ref<SamplerState> m_samplerState;
    BloomPostEffectCore m_bloomEffect;

    bool m_antialiasEnabled = true;
    bool m_ssaoEnabled = true;
    bool m_bloomEnabled = true;
    bool m_dofEnabled = false;
};

} // namespace detail
} // namespace ln

