
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/PostEffect/TonePostEffect.hpp>

namespace ln {

//==============================================================================
// ToneImageEffect

Ref<ToneImageEffect> ToneImageEffect::create()
{
    return makeObject<ToneImageEffect>();
}

ToneImageEffect::ToneImageEffect()
{
}

ToneImageEffect::~ToneImageEffect()
{
}

void ToneImageEffect::init()
{
    ImageEffect::init();
}

void ToneImageEffect::play(const ColorTone& tone, double time)
{
    m_toneValue.start(tone.toVector4(), time);
}

void ToneImageEffect::onUpdateFrame(float elapsedSeconds)
{
    m_toneValue.advanceTime(elapsedSeconds);
}

Ref<ImageEffectInstance> ToneImageEffect::onCreateInstance()
{
    return makeObject<detail::ToneImageEffectInstance>(this);
}

//==============================================================================
// ToneImageEffectInstance

namespace detail {

ToneImageEffectInstance::ToneImageEffectInstance()
{
}

bool ToneImageEffectInstance::init(ToneImageEffect* owner)
{
    if (!ImageEffectInstance::init()) return false;

    m_owner = owner;

    auto shader = makeObject<Shader>(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/PostEffect/Resource/ToneImageEffect.fx");
    m_material = makeObject<Material>();
    m_material->setShader(shader);

    return true;
}

bool ToneImageEffectInstance::onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination)
{
    m_material->setMainTexture(source);
    //m_material->setMainTexture(Texture2D::whiteTexture());
    m_material->setVector(u"_Tone", m_owner->m_toneValue.value());
    context->blit(m_material, destination);
    return true;
}

} // namespace detail
} // namespace ln

