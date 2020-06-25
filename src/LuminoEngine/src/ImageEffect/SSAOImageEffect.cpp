
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Graphics/SamplerState.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Rendering/RenderView.hpp>
#include <LuminoEngine/ImageEffect/SSAOImageEffect.hpp>
#include "../Rendering/RenderingManager.hpp"

namespace ln {

//==============================================================================
// SSAOImageEffect

SSAOImageEffect::SSAOImageEffect()
{
}

void SSAOImageEffect::init()
{
    ImageEffect::init();
}

Ref<ImageEffectInstance> SSAOImageEffect::onCreateInstance()
{
    return makeObject<detail::SSAOImageEffectInstance>(this);
}

//==============================================================================
// SSAOImageEffectInstance

namespace detail {

SSAOImageEffectInstance::SSAOImageEffectInstance()
    : m_owner(nullptr)
    , m_material(nullptr)
{
}

bool SSAOImageEffectInstance::init(SSAOImageEffect* owner)
{
    if (!ImageEffectInstance::init()) return false;

    auto shader1 = Shader::create(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/ImageEffect/Resource/SSAOImageEffect.fx");
    m_material = makeObject<Material>();
    m_material->setShader(shader1);

    return true;
}

bool SSAOImageEffectInstance::onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination)
{
    m_material->setMainTexture(source);
    context->blit(m_material, destination);
    return true;
}

} // namespace detail
} // namespace ln

