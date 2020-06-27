
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Graphics/SamplerState.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Rendering/RenderView.hpp>
#include <LuminoEngine/PostEffect/TonemapPostEffect.hpp>
#include "../Rendering/RenderingManager.hpp"

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
    return makeObject<detail::TonemapPostEffectInstance>(this);
}

//==============================================================================
// TonemapPostEffectInstance

namespace detail {

TonemapPostEffectInstance::TonemapPostEffectInstance()
    : m_owner(nullptr)
    , m_material(nullptr)
{
}

bool TonemapPostEffectInstance::init(TonemapPostEffect* owner)
{
    if (!PostEffectInstance::init()) return false;

    auto shader1 = Shader::create(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/PostEffect/Resource/TonemapEffect.fx");
    m_material = makeObject<Material>();
    m_material->setShader(shader1);

    return true;
}

bool TonemapPostEffectInstance::onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination)
{
    const float linearWhite = 11.2;
    const float shoulderStrength = 0.15f;
    const float linearStrength = 0.5;
    const float linearAngle = 0.1;
    const float toeStrength = 0.2f;
    const float toeNumerator = 0.02;
    const float toeDenominator = 0.3;
    const float Exposure = 0.0f;// 2.0f;//5.0f;// 
    const ColorTone tone(0, 0, 0, 0);
    //const ColorTone tone(-0.0, -0.01, -0.02, 0);

    struct Data
    {
        float	paramA;  // shoulderStrength
        float	paramB;  // linearStrength
        float	paramCB;    // param.linearStrength * param.linearAngle
        float	paramDE;    // param.toeStrength * param.toeNumerator
        float	paramDF;    // param.toeStrength * param.toeDenominator
        float	paramEperF;  // param.toeNumerator / param.toeDenominator
        float	paramF_White;//
        float Exposure;
    };

    Data d;
    d.paramA = shoulderStrength;
    d.paramB = linearStrength;
    d.paramCB = linearStrength * linearAngle;
    d.paramDE = toeStrength * toeNumerator;
    d.paramDF = toeStrength * toeDenominator;
    d.paramEperF = toeNumerator / toeDenominator;
    float	w = linearWhite;
    d.paramF_White = ((w * (d.paramA * w + d.paramCB) + d.paramDE)
        / (w * (d.paramA * w + d.paramB) + d.paramDF))
        - d.paramEperF;
    d.Exposure = Exposure;

    m_material->setFloat(u"paramA", d.paramA);
    m_material->setFloat(u"paramB", d.paramB);
    m_material->setFloat(u"paramCB", d.paramCB);
    m_material->setFloat(u"paramDE", d.paramDE);
    m_material->setFloat(u"paramDF", d.paramDF);
    m_material->setFloat(u"paramEperF", d.paramEperF);
    m_material->setFloat(u"paramF_White", d.paramF_White);
    m_material->setFloat(u"Exposure", d.Exposure);
    m_material->setVector(u"_Tone", tone.toVector4());

    m_material->setMainTexture(source);
    context->blit(m_material, destination);
    return true;
}

} // namespace detail
} // namespace ln

