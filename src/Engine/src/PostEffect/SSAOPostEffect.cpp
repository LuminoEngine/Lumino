﻿
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Graphics/SamplerState.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Rendering/RenderView.hpp>
#include <LuminoEngine/PostEffect/SSAOPostEffect.hpp>
#include "../Rendering/RenderingManager.hpp"

namespace ln {

//==============================================================================
// SSAOPostEffect

SSAOPostEffect::SSAOPostEffect()
{
}

void SSAOPostEffect::init()
{
    PostEffect::init();
}

Ref<PostEffectInstance> SSAOPostEffect::onCreateInstance()
{
    return makeObject<detail::SSAOPostEffectInstance>(this);
}

//==============================================================================
// SSAOPostEffectInstance

namespace detail {

SSAOPostEffectInstance::SSAOPostEffectInstance()
    : m_owner(nullptr)
    , m_occlusionMaterial(nullptr)
{
}

bool SSAOPostEffectInstance::init(SSAOPostEffect* owner)
{
    if (!PostEffectInstance::init()) return false;

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

bool SSAOPostEffectInstance::onRender(RenderView* renderView, CommandList* context, RenderTargetTexture* source, RenderTargetTexture* destination)
{
    LN_NOTIMPLEMENTED();
#if 0
    // TODO: density などのパラメータを渡していない

    Ref<RenderTargetTexture> occlusionMap = RenderTargetTexture::getTemporary(source->width(), source->height(), TextureFormat::RGBA8, false);
    occlusionMap->setSamplerState(m_samplerState);

    Texture* viewNormalMap = context->gbuffer(GBuffer::ViewNormalMap);
    Texture* viewDepthMap = context->gbuffer(GBuffer::ViewDepthMap);

    m_occlusionMaterial->setTexture(u"_viewSpaceNormalMap", viewNormalMap);
    m_occlusionMaterial->setTexture(u"_viewDepthMap", viewDepthMap);
    m_occlusionMaterial->setMainTexture(source);
    context->blit(m_occlusionMaterial, occlusionMap);

    m_blurAndCompositeMaterial->setTexture(u"_occlusionMap", occlusionMap);
    m_blurAndCompositeMaterial->setMainTexture(source);
    context->blit(m_blurAndCompositeMaterial, destination);

    RenderTargetTexture::releaseTemporary(occlusionMap);
#endif
    return true;
}

} // namespace detail
} // namespace ln

