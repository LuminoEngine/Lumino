
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Graphics/SamplerState.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Rendering/RenderView.hpp>
#include <LuminoEngine/ImageEffect/TonemapImageEffect.hpp>
#include "../Rendering/RenderingManager.hpp"

namespace ln {

//==============================================================================
// TonemapImageEffect

TonemapImageEffect::TonemapImageEffect()
{
}

void TonemapImageEffect::init()
{
    ImageEffect::init();
}

Ref<ImageEffectInstance> TonemapImageEffect::onCreateInstance()
{
    return makeObject<detail::TonemapImageEffectInstance>(this);
}

//==============================================================================
// TonemapImageEffectInstance

namespace detail {

TonemapImageEffectInstance::TonemapImageEffectInstance()
    : m_owner(nullptr)
    , m_material(nullptr)
{
}

bool TonemapImageEffectInstance::init(TonemapImageEffect* owner)
{
    if (!ImageEffectInstance::init()) return false;

    auto shader1 = Shader::create(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/ImageEffect/Resource/TonemapImageEffect.fx");
    m_material = makeObject<Material>();
    m_material->setShader(shader1);

    return true;
}

bool TonemapImageEffectInstance::onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination)
{
    m_material->setMainTexture(source);
    context->blit(m_material, destination);
    return true;
}

} // namespace detail
} // namespace ln

