
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Graphics/SamplerState.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Rendering/RenderView.hpp>
#include <LuminoEngine/PostEffect/SSAOImageEffect.hpp>
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
    , m_occlusionMaterial(nullptr)
{
}

bool SSAOImageEffectInstance::init(SSAOImageEffect* owner)
{
    if (!ImageEffectInstance::init()) return false;

    auto shader1 = Shader::create(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/PostEffect/Resource/SSAOOcclusionMap.fx");
    m_occlusionMaterial = makeObject<Material>();
    m_occlusionMaterial->setShader(shader1);

    auto shader2 = Shader::create(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/PostEffect/Resource/SSAOBlurAndComposite.fx");
    m_blurAndCompositeMaterial = makeObject<Material>();
    m_blurAndCompositeMaterial->setShader(shader2);

    //m_blurAndCompositeMaterial

    // TODO: 他と共有したいところ
    m_samplerState = makeObject<SamplerState>(TextureFilterMode::Linear, TextureAddressMode::Clamp);

    return true;
}

bool SSAOImageEffectInstance::onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination)
{
    Ref<RenderTargetTexture> occlusionMap = RenderTargetTexture::getTemporary(source->width(), source->height(), TextureFormat::RGBA8, false);
    occlusionMap->setSamplerState(m_samplerState);

    Texture* viewNormalMap = context->gbuffer(GBuffer::ViewNormalMap);
    Texture* viewDepthMap = context->gbuffer(GBuffer::ViewDepthMap);

    m_occlusionMaterial->setTexture(u"_screenSampler", viewNormalMap);
    m_occlusionMaterial->setTexture(u"_viewDepthMap", viewDepthMap);
    m_occlusionMaterial->setMainTexture(source);
    context->blit(m_occlusionMaterial, occlusionMap);

    m_blurAndCompositeMaterial->setTexture(u"_occlusionMap", occlusionMap);
    m_blurAndCompositeMaterial->setMainTexture(source);
    context->blit(m_blurAndCompositeMaterial, destination);

    RenderTargetTexture::releaseTemporary(occlusionMap);
    return true;
}

} // namespace detail
} // namespace ln

