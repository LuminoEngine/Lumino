
#include "Internal.hpp"
#include <LuminoGraphics/RHI/Texture.hpp>
#include <LuminoGraphics/RHI/SamplerState.hpp>
#include <LuminoGraphics/Rendering/Material.hpp>
#include <LuminoGraphics/Rendering/CommandList.hpp>
#include <LuminoGraphics/Rendering/RenderingContext.hpp>
#include <LuminoGraphics/Rendering/RenderView.hpp>
#include <LuminoEngine/PostEffect/FXAAPostEffect.hpp>
#include <LuminoGraphics/Rendering/detail/RenderingManager.hpp>

namespace ln {

//==============================================================================
// FXAAPostEffect

FXAAPostEffect::FXAAPostEffect()
{
}

void FXAAPostEffect::init()
{
    PostEffect::init();
}

Ref<PostEffectInstance> FXAAPostEffect::onCreateInstance()
{
    return makeObject<detail::FXAAPostEffectInstance>(this);
}

//==============================================================================
// FXAAPostEffectInstance

namespace detail {

FXAAPostEffectInstance::FXAAPostEffectInstance()
    : m_owner(nullptr)
    , m_material(nullptr)
{
}

bool FXAAPostEffectInstance::init(FXAAPostEffect* owner)
{
    if (!PostEffectInstance::init()) return false;

    auto shader1 = Shader::create(_TT("C:/Proj/LN/Lumino/src/LuminoEngine/src/PostEffect/Resource/FXAA.fx"));
    m_material = makeObject<Material>();
    m_material->setShader(shader1);

    return true;
}

bool FXAAPostEffectInstance::onRender(RenderView* renderView, CommandList* context, RenderTargetTexture* source, RenderTargetTexture* destination)
{
    m_material->setMainTexture(source);
    context->blit(m_material, destination);
    return true;
}

} // namespace detail
} // namespace ln

