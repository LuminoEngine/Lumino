
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Graphics/SamplerState.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/CommandList.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/PostEffect/BloomPostEffect.hpp>
#include "../Rendering/RenderingManager.hpp"

namespace ln {

//==============================================================================
// BloomPostEffect

static const int MIPS = 5;
static const Vector2 BlurDirectionX(1.0, 0.0);
static const Vector2 BlurDirectionY(0.0, 1.0);

Ref<BloomPostEffect> BloomPostEffect::create()
{
    return makeObject<BloomPostEffect>();
}

BloomPostEffect::BloomPostEffect()
    : m_luminosityThreshold(0.9f)
    , m_bloomStrength(1.0f)
    , m_bloomRadius(1.0f)
{
}

void BloomPostEffect::init()
{
    PostEffect::init();

}

void BloomPostEffect::setThreshold(float value)
{
    m_luminosityThreshold = value;
}

void BloomPostEffect::setStrength(float value)
{
    m_bloomStrength = value;
}

void BloomPostEffect::setSize(float value)
{
    m_bloomRadius = value;
}

void BloomPostEffect::onUpdateFrame(float elapsedSeconds)
{
}

//void BloomPostEffect::onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination)
//{
//}

Ref<PostEffectInstance> BloomPostEffect::onCreateInstance()
{
    return makeObject<detail::BloomPostEffectInstance>(this);
}

//==============================================================================
// BloomPostEffectInstance
namespace detail {

BloomPostEffectCore::BloomPostEffectCore()
    : m_viewWidth(0)
    , m_viewHeight(0)
{
    static_assert(offsetof(BloomCompositeParams, _BloomTintColorsAndFactors) == 0, "Invalid offset: BloomCompositeParams._BloomTintColors");
    static_assert(offsetof(BloomCompositeParams, _BloomStrength) == 128, "Invalid offset: BloomCompositeParams._BloomStrength");
    static_assert(offsetof(BloomCompositeParams, _BloomRadius) == 132, "Invalid offset: BloomCompositeParams._BloomRadius");
    static_assert(sizeof(BloomCompositeParams) == 136, "Invalid size: BloomCompositeParams");
}

bool BloomPostEffectCore::init(Material* compositeMaterial)
{
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
        materialH->setInt(_TT("KERNEL_RADIUS"), kernelSizeArray[i]);
        materialH->setFloat(_TT("SIGMA"), (float)kernelSizeArray[i]);
        materialH->setVector(_TT("_Direction"), Vector4(BlurDirectionX, 0.0f, 0.0f));
        m_separableBlurMaterialsH.add(materialH);

        auto materialV = makeObject<Material>();
        materialV->setShader(separableBlurShader);
        materialV->setBlendMode(BlendMode::Normal);
        materialV->setInt(_TT("KERNEL_RADIUS"), kernelSizeArray[i]);
        materialV->setFloat(_TT("SIGMA"), (float)kernelSizeArray[i]);
        materialV->setVector(_TT("_Direction"), Vector4(BlurDirectionY, 0.0f, 0.0f));
        m_separableBlurMaterialsV.add(materialV);
    }

    if (compositeMaterial) {
        m_compositeMaterial = compositeMaterial;
    }
    else {
        auto bloomCompositeShader = manager->builtinShader(detail::BuiltinShader::BloomComposite);
        m_compositeMaterial = makeObject<Material>();
        m_compositeMaterial->setBlendMode(BlendMode::Normal);
        m_compositeMaterial->setShader(bloomCompositeShader);
    }

    m_samplerState = makeObject<SamplerState>(TextureFilterMode::Linear, TextureAddressMode::Clamp);

    return true;
}

void BloomPostEffectCore::prepare(CommandList* context, RenderTargetTexture* source)
{
    int resx = source->width();
    int resy = source->height();
    if (m_viewWidth != resx || m_viewHeight != resy) {
        resetResources(resx, resy);
    }

    for (int i = 0; i < MIPS; i++) {
        m_separableBlurMaterialsH[i]->setVector(_TT("_TexSize"), Vector4(resx, resy, 0.0f, 0.0f));
        m_separableBlurMaterialsV[i]->setVector(_TT("_TexSize"), Vector4(resx, resy, 0.0f, 0.0f));
    }

    // Extract Bright Areas
    {
        m_materialHighPassFilter->setVector(_TT("_Color"), Vector4(0.0f, 0.0f, 0.0f, 0.0f));
        m_materialHighPassFilter->setFloat(_TT("_Opacity"), 0.0f);
        m_materialHighPassFilter->setFloat(_TT("_LuminosityThreshold"), m_luminosityThreshold);
        m_materialHighPassFilter->setFloat(_TT("_SmoothWidth"), 0.01);

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

    // Composite params
    m_bloomCompositeParams._BloomStrength = m_bloomStrength;
    m_bloomCompositeParams._BloomRadius = m_bloomRadius;
    m_compositeMaterial->setBufferData(_TT("BloomCompositeParams"), &m_bloomCompositeParams, sizeof(m_bloomCompositeParams));
}

void BloomPostEffectCore::render(CommandList* context, RenderTargetTexture* source, RenderTargetTexture* destination)
{
    // Composite All the mips
    m_compositeMaterial->setMainTexture(source);
    context->blit(m_compositeMaterial, destination);
}

void BloomPostEffectCore::resetResources(int resx, int resy)
{
    m_renderTargetsHorizontal.clear();
    m_renderTargetsVertical.clear();

    m_renderTargetBright = makeObject<RenderTargetTexture>(resx, resy, TextureFormat::RGBA8, false, false);
    m_renderTargetBright->setSamplerState(m_samplerState);

    // Create render targets for down sampling.
    auto rx = std::round(resx / 2);
    auto ry = std::round(resy / 2);
    for (int i = 0; i < MIPS; i++) {
        auto renderTargetHorizonal = makeObject<RenderTargetTexture>(rx, ry, TextureFormat::RGBA8, false, false);
        renderTargetHorizonal->setSamplerState(m_samplerState);
        m_renderTargetsHorizontal.add(renderTargetHorizonal);

        auto renderTargetVertical = makeObject<RenderTargetTexture>(rx, ry, TextureFormat::RGBA8, false, false);
        renderTargetVertical->setSamplerState(m_samplerState);
        m_renderTargetsVertical.add(renderTargetVertical);

        rx = std::round(resx / 2);
        ry = std::round(resy / 2);
    }

    // Composite params
    m_bloomCompositeParams._BloomTintColorsAndFactors[0] = Vector4(1.0, 1.0, 1.0, 1.0);
    m_bloomCompositeParams._BloomTintColorsAndFactors[1] = Vector4(1.0, 1.0, 1.0, 0.8);
    m_bloomCompositeParams._BloomTintColorsAndFactors[2] = Vector4(1.0, 1.0, 1.0, 0.6);
    m_bloomCompositeParams._BloomTintColorsAndFactors[3] = Vector4(1.0, 1.0, 1.0, 0.4);
    m_bloomCompositeParams._BloomTintColorsAndFactors[4] = Vector4(1.0, 1.0, 1.0, 0.2);

    // Composite material
    m_compositeMaterial->setTexture(_TT("_BlurTexture1"), m_renderTargetsVertical[0]);
    m_compositeMaterial->setTexture(_TT("_BlurTexture2"), m_renderTargetsVertical[1]);
    m_compositeMaterial->setTexture(_TT("_BlurTexture3"), m_renderTargetsVertical[2]);
    m_compositeMaterial->setTexture(_TT("_BlurTexture4"), m_renderTargetsVertical[3]);
    m_compositeMaterial->setTexture(_TT("_BlurTexture5"), m_renderTargetsVertical[4]);

    m_viewWidth = resx;
    m_viewHeight = resy;
}

} // namespace detail

//==============================================================================
// BloomPostEffectInstance

namespace detail {

BloomPostEffectInstance::BloomPostEffectInstance()
    : m_owner(nullptr)
{
}

bool BloomPostEffectInstance::init(BloomPostEffect* owner)
{
    if (!PostEffectInstance::init()) return false;

    m_owner = owner;

    if (!m_effect.init(nullptr)) {
        return false;
    }

    return true;
}

bool BloomPostEffectInstance::onRender(RenderView* renderView, CommandList* context, RenderTargetTexture* source, RenderTargetTexture* destination)
{
    m_effect.setLuminosityThreshold(m_owner->m_luminosityThreshold);
    m_effect.setBloomStrength(m_owner->m_bloomStrength);
    m_effect.setBloomRadius(m_owner->m_bloomRadius);

    m_effect.prepare(context, source);
    m_effect.render(context, source, destination);

    return true;
}

} // namespace detail
} // namespace ln

