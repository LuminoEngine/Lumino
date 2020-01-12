
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Graphics/SamplerState.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/ImageEffect/BloomImageEffect.hpp>
#include "../Rendering/RenderingManager.hpp"

namespace ln {

//==============================================================================
// BloomImageEffect

static const int MIPS = 5;
static const Vector2 BlurDirectionX(1.0, 0.0);
static const Vector2 BlurDirectionY(0.0, 1.0);

Ref<BloomImageEffect> BloomImageEffect::create()
{
    return makeObject<BloomImageEffect>();
}

BloomImageEffect::BloomImageEffect()
    : m_luminosityThreshold(0.9f)
    , m_bloomStrength(1.0f)
    , m_bloomRadius(1.0f)
    , m_viewWidth(0)
    , m_viewHeight(0)
{
}

void BloomImageEffect::init()
{
    ImageEffect::init();
    const auto* manager = detail::EngineDomain::renderingManager();

    auto luminosityHighPassShader = manager->builtinShader(detail::BuiltinShader::LuminosityHighPassShader);
    m_materialHighPassFilter = makeObject<Material>();
    m_materialHighPassFilter->setShader(luminosityHighPassShader);
    m_materialHighPassFilter->setBlendMode(BlendMode::Normal);

    auto separableBlurShader = manager->builtinShader(detail::BuiltinShader::SeperableBlur);
    int kernelSizeArray[] = { 3, 5, 8, 13, 21 };
    for (int i = 0; i < MIPS; i++) {
        auto materialH = makeObject<Material>();
        materialH->setShader(separableBlurShader);
        materialH->setBlendMode(BlendMode::Normal);
        materialH->setInt(u"KERNEL_RADIUS", kernelSizeArray[i]);
        materialH->setFloat(u"SIGMA", (float)kernelSizeArray[i]);
        materialH->setVector(u"_Direction", Vector4(BlurDirectionX, 0.0f, 0.0f));
        m_separableBlurMaterialsH.add(materialH);

        auto materialV = makeObject<Material>();
        materialV->setShader(separableBlurShader);
        materialV->setBlendMode(BlendMode::Normal);
        materialV->setInt(u"KERNEL_RADIUS", kernelSizeArray[i]);
        materialV->setFloat(u"SIGMA", (float)kernelSizeArray[i]);
        materialV->setVector(u"_Direction", Vector4(BlurDirectionY, 0.0f, 0.0f));
        m_separableBlurMaterialsV.add(materialV);
    }

    auto bloomCompositeShader = manager->builtinShader(detail::BuiltinShader::BloomComposite);
    m_compositeMaterial = makeObject<Material>();
    m_compositeMaterial->setBlendMode(BlendMode::Normal);
    m_compositeMaterial->setShader(bloomCompositeShader);

    m_samplerState = makeObject<SamplerState>(TextureFilterMode::Linear, TextureAddressMode::Clamp);
}

void BloomImageEffect::setThreshold(float value)
{
    m_luminosityThreshold = value;
}

void BloomImageEffect::setStrength(float value)
{
    m_bloomStrength = value;
}

void BloomImageEffect::setSize(float value)
{
    m_bloomRadius = value;
}

void BloomImageEffect::onUpdateFrame(float elapsedSeconds)
{
}

void BloomImageEffect::onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination)
{
    int resx = source->width();
    int resy = source->height();
    if (m_viewWidth != resx || m_viewHeight != resy) {
        resetResources(resx, resy);
    }

    for (int i = 0; i < MIPS; i++) {
        m_separableBlurMaterialsH[i]->setVector(u"_TexSize", Vector4(resx, resy, 0.0f, 0.0f));
        m_separableBlurMaterialsV[i]->setVector(u"_TexSize", Vector4(resx, resy, 0.0f, 0.0f));
    }

    // Extract Bright Areas
    {
        m_materialHighPassFilter->setVector(u"_Color", Vector4(0.0f, 0.0f, 0.0f, 0.0f));
        m_materialHighPassFilter->setFloat(u"_Opacity", 0.0f);
        m_materialHighPassFilter->setFloat(u"_LuminosityThreshold", m_luminosityThreshold);
        m_materialHighPassFilter->setFloat(u"_SmoothWidth", 0.01);

        m_materialHighPassFilter->setMainTexture(source);
        context->blit(m_materialHighPassFilter, m_renderTargetBright);
    }

    // Blur All the mips progressively
    RenderTargetTexture* inputRenderTarget = m_renderTargetBright;
    for (int i = 0; i < MIPS; i++) {
        m_separableBlurMaterialsH[i]->setMainTexture(inputRenderTarget);
        context->blit(m_separableBlurMaterialsH[i], m_renderTargetsHorizontal[i]);

        m_separableBlurMaterialsV[i]->setMainTexture(m_renderTargetsHorizontal[i]);
        context->blit(m_separableBlurMaterialsV[i], m_renderTargetsVertical[i]);

        inputRenderTarget = m_renderTargetsVertical[i];
    }

    // Composite All the mips
    m_compositeMaterial->setMainTexture(source);
    Vector4 bloomTintColors[] = { Vector4(1, 1, 1, 1), Vector4(1, 1, 1, 1), Vector4(1, 1, 1, 1), Vector4(1, 1, 1, 1), Vector4(1, 1, 1, 1) };
    m_compositeMaterial->setFloat(u"_BloomStrength", m_bloomStrength);
    m_compositeMaterial->setFloat(u"_BloomRadius", m_bloomRadius);
    m_compositeMaterial->setVectorArray(u"_BloomTintColors", bloomTintColors, MIPS);
    context->blit(m_compositeMaterial, destination);
}

void BloomImageEffect::resetResources(int resx, int resy)
{
    m_renderTargetsHorizontal.clear();
    m_renderTargetsVertical.clear();

    m_renderTargetBright = makeObject<RenderTargetTexture>(resx, resy, TextureFormat::RGBA8, false);
    m_renderTargetBright->setSamplerState(m_samplerState);

    // Create render targets for down sampling.
    auto rx = std::round(resx / 2);
    auto ry = std::round(resy / 2);
    for (int i = 0; i < MIPS; i++) {
        auto renderTargetHorizonal = makeObject<RenderTargetTexture>(rx, ry, TextureFormat::RGBA8, false);
        renderTargetHorizonal->setSamplerState(m_samplerState);
        m_renderTargetsHorizontal.add(renderTargetHorizonal);

        auto renderTargetVertical = makeObject<RenderTargetTexture>(rx, ry, TextureFormat::RGBA8, false);
        renderTargetVertical->setSamplerState(m_samplerState);
        m_renderTargetsVertical.add(renderTargetVertical);

        rx = std::round(resx / 2);
        ry = std::round(resy / 2);
    }

    // Composite material
    float bloomFactors[] = { 1.0, 0.8, 0.6, 0.4, 0.2 };
    m_compositeMaterial->setTexture(u"_BlurTexture1", m_renderTargetsVertical[0]);
    m_compositeMaterial->setTexture(u"_BlurTexture2", m_renderTargetsVertical[1]);
    m_compositeMaterial->setTexture(u"_BlurTexture3", m_renderTargetsVertical[2]);
    m_compositeMaterial->setTexture(u"_BlurTexture4", m_renderTargetsVertical[3]);
    m_compositeMaterial->setTexture(u"_BlurTexture5", m_renderTargetsVertical[4]);
    m_compositeMaterial->setFloatArray(u"_BloomFactors", bloomFactors, MIPS);

    m_viewWidth = resx;
    m_viewHeight = resy;
}

} // namespace ln

