
#include "Internal.hpp"
#include <LuminoGraphics/RHI/Texture.hpp>
#include <LuminoGraphics/RHI/SamplerState.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/CommandList.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Rendering/RenderView.hpp>
#include <LuminoEngine/PostEffect/FilmicPostEffect.hpp>
#include "../Rendering/RenderingManager.hpp"

namespace ln {

//==============================================================================
// FilmicPostEffect

FilmicPostEffect::FilmicPostEffect()
    : m_luminosityThreshold(0.8f)
    , m_bloomStrength(1.0f)
    , m_bloomRadius(1.0f)
    , m_vignetteColor(0.0f, 0.0f, 0.0f, 1.0)
    , m_vignetteCenter(0.5f, 0.5f)
    , m_vignetteSettings(0.65f, 0.5f, 1.0f, 0.0f) // x: intensity, y: smoothness, z: roundness, w: rounded
    , m_antialiasEnabled(true)
    , m_ssrEnabled(false)
    , m_ssaoEnabled(false)
    , m_bloomEnabled(false)
    , m_dofEnabled(false)
    , m_tonemapEnabled(false)
    , m_vignetteEnabled(false)
    , m_gammaEnabled(false)
{
}

void FilmicPostEffect::init()
{
    PostEffect::init();
}

Ref<PostEffectInstance> FilmicPostEffect::onCreateInstance()
{
    return makeObject<detail::FilmicPostEffectInstance>(this);
}

//==============================================================================
// FilmicPostEffectInstance

namespace detail {

FilmicPostEffectInstance::FilmicPostEffectInstance()
    : m_owner(nullptr)
    , m_integrationMaterial(nullptr)
{
}

bool FilmicPostEffectInstance::init(FilmicPostEffect* owner)
{
    if (!PostEffectInstance::init()) return false;
    m_owner = owner;

    auto shader2 = RenderingManager::instance()->builtinShader(BuiltinShader::SSAOOcclusionMap);
    m_ssaoMaterial = makeObject<Material>();
    m_ssaoMaterial->setShader(shader2);

    auto shader1 = RenderingManager::instance()->builtinShader(BuiltinShader::FilmicPostEffect);
    //auto shader1 = Shader::create(_TT("C:/Proj/LN/Lumino/src/LuminoEngine/src/PostEffect/Resource/FilmicPostEffect.fx");
    m_integrationMaterial = makeObject<Material>();
    m_integrationMaterial->setShader(shader1);

    // TODO: 他と共有したいところ
    m_samplerState = makeObject<SamplerState>(TextureFilterMode::Linear, TextureAddressMode::Clamp);

    if (!m_ssrEffect.init(m_integrationMaterial)) {
        return false;
    }

    if (!m_bloomEffect.init(m_integrationMaterial)) {
        return false;
    }

    if (!m_dofEffect.init(m_integrationMaterial)) {
        return false;
    }
    
    return true;
}

bool FilmicPostEffectInstance::onRender(RenderView* renderView, CommandList* context, RenderTargetTexture* source, RenderTargetTexture* destination)
{
    const bool toneEnabled = m_owner->m_screenBlendColor.a > 0.0f || !m_owner->m_screenColorTone.isZero();

    if (!m_owner->m_antialiasEnabled &&
        !m_owner->m_ssrEnabled &&
        !m_owner->m_ssaoEnabled &&
        !m_owner->m_bloomEnabled &&
        !m_owner->m_dofEnabled &&
        !m_owner->m_tonemapEnabled &&
        !m_owner->m_vignetteEnabled &&
        !m_owner->m_gammaEnabled &&
        !toneEnabled) {
        return false;
    }

    bool actualSSREnabled = false;
    if (m_owner->m_ssrEnabled) {
        actualSSREnabled = m_ssrEffect.prepare(renderView, context, source);
        m_integrationMaterial->setTexture(_TT("_SSRSampler"), m_ssrEffect.ssrResultTexture());
    }

    if (m_owner->m_bloomEnabled) {
        m_bloomEffect.setLuminosityThreshold(m_owner->m_luminosityThreshold);
        m_bloomEffect.setBloomStrength(m_owner->m_bloomStrength);
        m_bloomEffect.setBloomRadius(m_owner->m_bloomRadius);
        m_bloomEffect.prepare(context, source);
    }

    if (m_owner->m_dofEnabled) {
        m_dofEffect.prepare(renderView, context, source);
    }

    if (m_owner->m_tonemapEnabled) {
#if 1
        TonemapPostEffectParams params;
        params.setup(
            m_owner->m_linearWhite, m_owner->m_shoulderStrength, m_owner->m_linearStrength, m_owner->m_linearAngle,
            m_owner->m_toeStrength, m_owner->m_toeNumerator, m_owner->m_toeDenominator, m_owner->m_exposure);
#else
        const float linearWhite = 3.0f;//0.5;//2.2;//1.2; //5.0f;//
        const float shoulderStrength = 0.0015;//0.15f;
        const float linearStrength = 0.01;// 0.5;
        const float linearAngle = 0.05;
        const float toeStrength = 0.02f;
        const float toeNumerator = 0.02;
        const float toeDenominator = 0.3;
        const float Exposure = 0.0f;// 2.0f;//5.0f;// 
        //const ColorTone tone(0, 0, 0, 0);

        TonemapPostEffectParams params;
        params.setup(
            linearWhite, shoulderStrength, linearStrength, linearAngle,
            toeStrength, toeNumerator, toeDenominator, Exposure);
#endif
        m_integrationMaterial->setBufferData(_TT("TonemapPostEffectParams"), &params, sizeof(params));
    }

    // cbuffer EffectSettings
    struct EffectSettings
    {
        Vector4 vignetteColor;
        Vector4 vignettePosition;
        Vector4 vignetteSettings;
        Vector4 blendColor;
        Vector4 colorTone;
        int antialiasEnabled;
        int ssrEnabled;
        int ssaoEnabled;
        int bloomEnabled;
        int dofEnabled;
        int tonemapEnabled;
        int _vignetteEnabled;
        int gammaEnabled;
        float focusedLinearDepth;
    };

    EffectSettings settings;
    settings.vignetteColor = m_owner->m_vignetteColor.toVector4();
    settings.vignettePosition = Vector4(m_owner->m_vignetteCenter, 0, 0);
    settings.vignetteSettings = m_owner->m_vignetteSettings;
    settings.blendColor = m_owner->m_screenBlendColor.toVector4();
    settings.colorTone = m_owner->m_screenColorTone.toVector4();
    settings.antialiasEnabled = m_owner->m_antialiasEnabled ? 1 : 0;
    settings.ssrEnabled = actualSSREnabled ? 1 : 0;
    settings.ssaoEnabled = m_owner->m_ssaoEnabled ? 1 : 0;
    settings.bloomEnabled = m_owner->m_bloomEnabled ? 1 : 0;
    settings.dofEnabled = m_owner->m_dofEnabled ? 1 : 0;
    settings.tonemapEnabled = m_owner->m_tonemapEnabled ? 1 : 0;
    settings._vignetteEnabled = m_owner->m_vignetteEnabled ? 1 : 0;
    settings.gammaEnabled = m_owner->m_gammaEnabled ? 1 : 0;
    settings.focusedLinearDepth = m_owner->m_focusedLinearDepth;

    Ref<RenderTargetTexture> occlusionMap = RenderTargetTexture::getTemporary(source->width(), source->height(), TextureFormat::RGBA8, false);
    occlusionMap->setSamplerState(m_samplerState);

    Texture* viewNormalMap = renderView->gbuffer(GBuffer::ViewNormalMap);
    Texture* viewDepthMap = renderView->gbuffer(GBuffer::ViewDepthMap);
    Texture* randomTexture = RenderingManager::instance()->randomTexture();

    viewNormalMap->setSamplerState(SamplerState::pointClamp());
    viewDepthMap->setSamplerState(SamplerState::pointClamp());

    struct LocalBuffer
    {
        float density;
        float scaling;
    };
    LocalBuffer buf;
    buf.density = 0.8;
    buf.scaling = 0.7;
    m_ssaoMaterial->setBufferData(_TT("LocalBuffer"), &buf, sizeof(buf));

    m_ssaoMaterial->setTexture(_TT("_viewSpaceNormalMap"), viewNormalMap);
    m_ssaoMaterial->setTexture(_TT("_viewDepthMap"), viewDepthMap);
    m_ssaoMaterial->setTexture(_TT("_randomSampler"), randomTexture);
    m_ssaoMaterial->setMainTexture(source);
    context->blit(m_ssaoMaterial, occlusionMap);
    //context->blit(m_ssaoMaterial, destination);


    source->setSamplerState(SamplerState::linearClamp());
    m_integrationMaterial->setBufferData(_TT("EffectSettings"), &settings, sizeof(settings));
    m_integrationMaterial->setTexture(_TT("_occlusionMap"), occlusionMap);
    m_integrationMaterial->setMainTexture(source);
    context->blit(m_integrationMaterial, destination);

    //m_material->setMainTexture(source);
    //context->blit(m_material, destination);


    RenderTargetTexture::releaseTemporary(occlusionMap);
    return true;
}

} // namespace detail
} // namespace ln

