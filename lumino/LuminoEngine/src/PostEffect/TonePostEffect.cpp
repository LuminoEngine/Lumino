
#include "Internal.hpp"
#include <LuminoGraphics/GPU/Texture.hpp>
#include <LuminoGraphics/Rendering/Material.hpp>
#include <LuminoGraphics/Rendering/CommandList.hpp>
#include <LuminoGraphics/Rendering/RenderingContext.hpp>
#include <LuminoEngine/PostEffect/TonePostEffect.hpp>

namespace ln {

//==============================================================================
// TonePostEffect

Ref<TonePostEffect> TonePostEffect::create()
{
    return makeObject_deprecated<TonePostEffect>();
}

TonePostEffect::TonePostEffect()
{
}

TonePostEffect::~TonePostEffect()
{
}

void TonePostEffect::init()
{
    PostEffect::init();
}

void TonePostEffect::play(const ColorTone& tone, double time)
{
    m_toneValue.start(tone.toVector4(), time);
}

void TonePostEffect::onUpdateFrame(float elapsedSeconds)
{
    m_toneValue.advanceTime(elapsedSeconds);
}

Ref<PostEffectInstance> TonePostEffect::onCreateInstance()
{
    return makeObject_deprecated<detail::TonePostEffectInstance>(this);
}

//==============================================================================
// TonePostEffectInstance

namespace detail {

TonePostEffectInstance::TonePostEffectInstance()
{
}

bool TonePostEffectInstance::init(TonePostEffect* owner)
{
    if (!PostEffectInstance::init()) return false;

    m_owner = owner;

    auto shader = makeObject_deprecated<Shader>(_TT("C:/Proj/LN/Lumino/src/LuminoEngine/src/PostEffect/Resource/ToneEffect.fx"));
    m_material = makeObject_deprecated<Material>();
    m_material->setShader(shader);

    return true;
}

bool TonePostEffectInstance::onRender(RenderView* renderView, CommandList* context, RenderTargetTexture* source, RenderTargetTexture* destination)
{
    m_material->setMainTexture(source);
    //m_material->setMainTexture(Texture2D::whiteTexture());
    m_material->setVector(_TT("_Tone"), m_owner->m_toneValue.value());
    context->blit(m_material, destination);
    return true;
}

} // namespace detail
} // namespace ln

