
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Graphics/SamplerState.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Rendering/RenderView.hpp>
#include <LuminoEngine/ImageEffect/DepthOfFieldImageEffect.hpp>
#include "../Rendering/RenderingManager.hpp"

namespace ln {

//==============================================================================
// DepthOfFieldImageEffect

DepthOfFieldImageEffect::DepthOfFieldImageEffect()
{
}

void DepthOfFieldImageEffect::init()
{
    ImageEffect::init();
}

Ref<ImageEffectInstance> DepthOfFieldImageEffect::onCreateInstance()
{
    return makeObject<detail::DepthOfFieldImageEffectInstance>(this);
}

//==============================================================================
// DepthOfFieldImageEffectInstance

namespace detail {

DepthOfFieldImageEffectInstance::DepthOfFieldImageEffectInstance()
    : m_owner(nullptr)
    , m_material(nullptr)
{
}

bool DepthOfFieldImageEffectInstance::init(DepthOfFieldImageEffect* owner)
{
    if (!ImageEffectInstance::init()) return false;

    auto shader1 = Shader::create(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/ImageEffect/Resource/DepthOfFieldImageEffect.fx");
    m_material = makeObject<Material>();
    m_material->setShader(shader1);

    return true;
}

bool DepthOfFieldImageEffectInstance::onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination)
{
    m_material->setMainTexture(source);
    context->blit(m_material, destination);
    return true;
}

} // namespace detail
} // namespace ln

