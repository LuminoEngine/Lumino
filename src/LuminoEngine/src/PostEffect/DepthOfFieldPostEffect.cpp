﻿
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
// DepthOfFieldPostEffect

DepthOfFieldPostEffect::DepthOfFieldPostEffect()
{
}

void DepthOfFieldPostEffect::init()
{
    PostEffect::init();
}

Ref<PostEffectInstance> DepthOfFieldPostEffect::onCreateInstance()
{
    return makeObject<detail::DepthOfFieldPostEffectInstance>(this);
}

//==============================================================================
// DepthOfFieldPostEffectCore

namespace detail {

DepthOfFieldPostEffectCore::DepthOfFieldPostEffectCore()
    : m_viewWidth(0)
    , m_viewHeight(0)
{
}

bool DepthOfFieldPostEffectCore::init(Material* compositeMaterial)
{
    m_compositeMaterial = compositeMaterial;

    auto shader1 = Shader::create(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/PostEffect/Resource/Copy.fx");
    m_copyMaterial = makeObject<Material>();
    m_copyMaterial->setShader(shader1);

    // TODO: 他と共有したいところ
    m_samplerState = makeObject<SamplerState>(TextureFilterMode::Linear, TextureAddressMode::Clamp);

    return true;
}

void DepthOfFieldPostEffectCore::prepare(RenderingContext* context, RenderTargetTexture* source)
{
    int resx = source->width();
    int resy = source->height();
    if (m_viewWidth != resx || m_viewHeight != resy) {
        resetResources(resx, resy);
    }

    // down sampling.
    m_copyMaterial->setMainTexture(source);
    for (int i = 0; i < MaxMips; i++) {
        context->blit(m_copyMaterial, m_mipTargets[i]);
    }

    Texture* viewDepthMap = context->gbuffer(GBuffer::ViewDepthMap);
    m_compositeMaterial->setTexture(u"_depthTexture", viewDepthMap);
    m_compositeMaterial->setTexture(u"_dofTexture0", m_mipTargets[0]);
    m_compositeMaterial->setTexture(u"_dofTexture1", m_mipTargets[1]);
    m_compositeMaterial->setTexture(u"_dofTexture2", m_mipTargets[2]);
    m_compositeMaterial->setTexture(u"_dofTexture3", m_mipTargets[3]);
}

void DepthOfFieldPostEffectCore::resetResources(int resx, int resy)
{
    // Create render targets for down sampling.
    auto rx = std::round(resx / 2);
    auto ry = std::round(resy / 2);
    for (int i = 0; i < MaxMips; i++) {
        auto renderTargetHorizonal = makeObject<RenderTargetTexture>(rx, ry, TextureFormat::RGBA8, false);
        renderTargetHorizonal->setSamplerState(m_samplerState);
        m_mipTargets.add(renderTargetHorizonal);

        rx = std::round(resx / 2);
        ry = std::round(resy / 2);
    }
}

} // namespace detail

//==============================================================================
// DepthOfFieldPostEffectInstance

namespace detail {

DepthOfFieldPostEffectInstance::DepthOfFieldPostEffectInstance()
    : m_owner(nullptr)
    , m_copyMaterial(nullptr)
{
}

bool DepthOfFieldPostEffectInstance::init(DepthOfFieldPostEffect* owner)
{
    if (!PostEffectInstance::init()) return false;

    auto shader1 = Shader::create(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/PostEffect/Resource/Copy.fx");
    m_copyMaterial = makeObject<Material>();
    m_copyMaterial->setShader(shader1);

    auto shader2 = Shader::create(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/PostEffect/Resource/DepthOfField.fx");
    m_dofMaterial = makeObject<Material>();
    m_dofMaterial->setShader(shader2);

    // TODO: 他と共有したいところ
    m_samplerState = makeObject<SamplerState>(TextureFilterMode::Linear, TextureAddressMode::Clamp);

    return true;
}

bool DepthOfFieldPostEffectInstance::onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination)
{
    auto dofTexture = RenderTargetTexture::getTemporary(source->width() / 2, source->height(), TextureFormat::RGBA8, false);
    Texture* viewDepthMap = context->gbuffer(GBuffer::ViewDepthMap);

    int margin = 2;

    auto rect = RectI(0, 0, source->width() / 2, source->height() / 2);
    for (int i = 0; i < 8; i++)
    {
        context->setViewportRect(rect);
        m_copyMaterial->setMainTexture(source);
        context->blit(m_copyMaterial, dofTexture);

        rect.y += rect.height;
        rect.width /= 2;
        rect.height /= 2;
    }
    context->setViewportRect(RectI::Empty);

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

