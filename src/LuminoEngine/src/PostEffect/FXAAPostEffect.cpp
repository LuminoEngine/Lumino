
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Graphics/SamplerState.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Rendering/RenderView.hpp>
#include <LuminoEngine/PostEffect/FXAAPostEffect.hpp>
#include "../Rendering/RenderingManager.hpp"

namespace ln {

//==============================================================================
// FXAAImageEffect

FXAAImageEffect::FXAAImageEffect()
{
}

void FXAAImageEffect::init()
{
    ImageEffect::init();
}

Ref<ImageEffectInstance> FXAAImageEffect::onCreateInstance()
{
    return makeObject<detail::FXAAImageEffectInstance>(this);
}

//==============================================================================
// FXAAImageEffectInstance

namespace detail {

FXAAImageEffectInstance::FXAAImageEffectInstance()
    : m_owner(nullptr)
    , m_material(nullptr)
{
}

bool FXAAImageEffectInstance::init(FXAAImageEffect* owner)
{
    if (!ImageEffectInstance::init()) return false;

    auto shader1 = Shader::create(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/PostEffect/Resource/FXAAImageEffect.fx");
    m_material = makeObject<Material>();
    m_material->setShader(shader1);

    return true;
}

bool FXAAImageEffectInstance::onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination)
{
    m_material->setMainTexture(source);
    context->blit(m_material, destination);
    return true;
}

} // namespace detail
} // namespace ln

