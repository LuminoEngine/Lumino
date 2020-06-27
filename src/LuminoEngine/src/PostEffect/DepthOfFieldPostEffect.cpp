
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Graphics/SamplerState.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Rendering/RenderView.hpp>
#include <LuminoEngine/PostEffect/DepthOfFieldPostEffect.hpp>
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
    , m_copyMaterial(nullptr)
{
}

bool DepthOfFieldImageEffectInstance::init(DepthOfFieldImageEffect* owner)
{
    if (!ImageEffectInstance::init()) return false;

    auto shader1 = Shader::create(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/PostEffect/Resource/Copy.fx");
    m_copyMaterial = makeObject<Material>();
    m_copyMaterial->setShader(shader1);

    auto shader2 = Shader::create(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/PostEffect/Resource/DepthOfFieldImageEffect.fx");
    m_dofMaterial = makeObject<Material>();
    m_dofMaterial->setShader(shader2);

    // TODO: 他と共有したいところ
    m_samplerState = makeObject<SamplerState>(TextureFilterMode::Linear, TextureAddressMode::Clamp);

    return true;
}

bool DepthOfFieldImageEffectInstance::onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination)
{
    auto dofTexture = RenderTargetTexture::getTemporary(source->width() / 2, source->height(), TextureFormat::RGBA8, false);
    Texture* viewDepthMap = context->gbuffer(GBuffer::ViewDepthMap);

    int margin = 2;

    auto rect = RectI(0, 0, source->width() / 2, source->height() / 2);
    for (int i = 0; i < 8; i++)
    {
        context->setViewportRect(rect);
        //context->setViewportRect(
        //    RectI(
        //        rect.x + margin,
        //        rect.y + margin,
        //        std::max(1, rect.width - margin * 2),
        //        std::max(1, rect.height - margin * 2)));
        m_copyMaterial->setMainTexture(source);
        context->blit(m_copyMaterial, dofTexture);
        //context->blit(m_copyMaterial, destination);

        rect.y += rect.height;
        rect.width /= 2;
        rect.height /= 2;
    }
    context->setViewportRect(RectI::Empty);

    //context->setScissorRect(RectI(0, 0, 320, 240));
    dofTexture->setSamplerState(m_samplerState);
    viewDepthMap->setSamplerState(m_samplerState);

    m_dofMaterial->setMainTexture(source);
    m_dofMaterial->setTexture(u"_depthTex", viewDepthMap);
    m_dofMaterial->setTexture(u"_dofTex", dofTexture);
    context->blit(m_dofMaterial, destination);

    RenderTargetTexture::releaseTemporary(dofTexture);
    
    return true;
}

} // namespace detail
} // namespace ln

