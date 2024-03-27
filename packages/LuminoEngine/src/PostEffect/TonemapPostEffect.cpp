
#include "Internal.hpp"
#include <LuminoEngine/GPU/Texture.hpp>
#include <LuminoEngine/GPU/SamplerState.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/CommandList.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Rendering/RenderView.hpp>
#include <LuminoEngine/PostEffect/TonemapPostEffect.hpp>
#include <LuminoEngine/Rendering/detail/RenderingManager.hpp>

namespace ln {

//==============================================================================
// TonemapPostEffect

TonemapPostEffect::TonemapPostEffect()
{
}

void TonemapPostEffect::init()
{
    PostEffect::init();
}

Ref<PostEffectInstance> TonemapPostEffect::onCreateInstance()
{
    return makeObject_deprecated<detail::TonemapPostEffectInstance>(this);
}

//==============================================================================
// TonemapPostEffectInstance

namespace detail {

void TonemapPostEffectParams::setup(
    const float linearWhite,
    const float shoulderStrength,
    const float linearStrength,
    const float linearAngle,
    const float toeStrength,
    const float toeNumerator,
    const float toeDenominator,
    const float exposure)
{
    _paramA = shoulderStrength;
    _paramB = linearStrength;
    _paramCB = linearStrength * linearAngle;
    _paramDE = toeStrength * toeNumerator;
    _paramDF = toeStrength * toeDenominator;
    _paramEperF = toeNumerator / toeDenominator;
    float w = linearWhite;
    _paramF_White = ((w * (_paramA * w + _paramCB) + _paramDE)
        / (w * (_paramA * w + _paramB) + _paramDF))
        - _paramEperF;
    _exposure = exposure;
}

TonemapPostEffectInstance::TonemapPostEffectInstance()
    : m_owner(nullptr)
    , m_material(nullptr)
{
}

bool TonemapPostEffectInstance::init(TonemapPostEffect* owner)
{
    if (!PostEffectInstance::init()) return false;

    auto shader1 = Shader::create(_TT("C:/Proj/LN/Lumino/src/LuminoEngine/src/PostEffect/Resource/TonemapEffect.fx"));
    m_material = makeObject_deprecated<Material>();
    m_material->setShader(shader1);

    return true;
}

bool TonemapPostEffectInstance::onRender(RenderView* renderView, CommandList* context, RenderTargetTexture* source, RenderTargetTexture* destination)
{
    const float linearWhite = 5.0f;//11.2;
    const float shoulderStrength = 0.015f;// 0.15f;
    const float linearStrength = 0.5;
    const float linearAngle = 0.1;
    const float toeStrength = 0.2f;
    const float toeNumerator = 0.02;
    const float toeDenominator = 0.3;
    const float Exposure = 0.0f;// 2.0f;//5.0f;// 
    //const ColorTone tone(-0.0, -0.01, -0.02, 0);

    TonemapPostEffectParams d;
    d.setup(
        linearWhite, shoulderStrength, linearStrength, linearAngle,
        toeStrength, toeNumerator, toeDenominator, Exposure);

    m_material->setFloat(_TT("paramA"), d._paramA);
    m_material->setFloat(_TT("paramB"), d._paramB);
    m_material->setFloat(_TT("paramCB"), d._paramCB);
    m_material->setFloat(_TT("paramDE"), d._paramDE);
    m_material->setFloat(_TT("paramDF"), d._paramDF);
    m_material->setFloat(_TT("paramEperF"), d._paramEperF);
    m_material->setFloat(_TT("paramF_White"), d._paramF_White);
    m_material->setFloat(_TT("Exposure"), d._exposure);

    m_material->setMainTexture(source);
    context->blit(m_material, destination);
    return true;
}

} // namespace detail
} // namespace ln

