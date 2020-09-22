
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Graphics/SamplerState.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Rendering/RenderView.hpp>
#include <LuminoEngine/PostEffect/FilmicPostEffect.hpp>
#include "../Rendering/RenderingManager.hpp"

namespace ln {

//==============================================================================
// FilmicPostEffect

FilmicPostEffect::FilmicPostEffect()
    : m_luminosityThreshold(0.9f)
    , m_bloomStrength(1.0f)
    , m_bloomRadius(1.0f)
    , m_antialiasEnabled(false)
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

    auto shader2 = Shader::create(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/PostEffect/Resource/SSAOOcclusionMap.fx");
    m_ssaoMaterial = makeObject<Material>();
    m_ssaoMaterial->setShader(shader2);

    auto shader1 = Shader::create(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/PostEffect/Resource/FilmicPostEffect.fx");
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

bool FilmicPostEffectInstance::onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination)
{
    if (!m_owner->m_antialiasEnabled &&
        !m_owner->m_ssrEnabled &&
        !m_owner->m_ssaoEnabled &&
        !m_owner->m_bloomEnabled &&
        !m_owner->m_dofEnabled &&
        !m_owner->m_tonemapEnabled &&
        !m_owner->m_vignetteEnabled &&
        !m_owner->m_gammaEnabled) {
        return false;
    }

    bool actualSSREnabled = false;
    if (m_owner->m_ssrEnabled) {
        actualSSREnabled = m_ssrEffect.prepare(context, source);
        m_integrationMaterial->setTexture(u"_SSRSampler", m_ssrEffect.ssrResultTexture());
    }

    if (m_owner->m_bloomEnabled) {
        m_bloomEffect.setLuminosityThreshold(m_owner->m_luminosityThreshold);
        m_bloomEffect.setBloomStrength(m_owner->m_bloomStrength);
        m_bloomEffect.setBloomRadius(m_owner->m_bloomRadius);
        m_bloomEffect.prepare(context, source);
    }

    if (m_owner->m_dofEnabled) {
        m_dofEffect.prepare(context, source);
    }

    if (m_owner->m_tonemapEnabled) {
        const float linearWhite = 3.0f;//0.5;//2.2;//1.2; //5.0f;//
        const float shoulderStrength = 0.0015;//0.15f;
        const float linearStrength = 0.01;// 0.5;
        const float linearAngle = 0.05;
        const float toeStrength = 0.02f;
        const float toeNumerator = 0.02;
        const float toeDenominator = 0.3;
        const float Exposure = 0.0f;// 2.0f;//5.0f;// 
        const ColorTone tone(0, 0, 0, 0);

        TonemapPostEffectParams params;
        params.setup(
            linearWhite, shoulderStrength, linearStrength, linearAngle,
            toeStrength, toeNumerator, toeDenominator, Exposure, tone);
        m_integrationMaterial->setBufferData(u"TonemapPostEffectParams", &params, sizeof(params));
    }

    // cbuffer EffectSettings
    struct EffectSettings
    {
        int antialiasEnabled;
        int ssrEnabled;
        int ssaoEnabled;
        int bloomEnabled;
        int dofEnabled;
        int tonemapEnabled;
        int _vignetteEnabled;
        int gammaEnabled;
    };

    EffectSettings settings;
    settings.antialiasEnabled = m_owner->m_antialiasEnabled ? 1 : 0;
    settings.ssrEnabled = actualSSREnabled ? 1 : 0;
    settings.ssaoEnabled = m_owner->m_ssaoEnabled ? 1 : 0;
    settings.bloomEnabled = m_owner->m_bloomEnabled ? 1 : 0;
    settings.dofEnabled = m_owner->m_dofEnabled ? 1 : 0;
    settings.tonemapEnabled = m_owner->m_tonemapEnabled ? 1 : 0;
    settings._vignetteEnabled = m_owner->m_vignetteEnabled ? 1 : 0;
    settings.gammaEnabled = m_owner->m_gammaEnabled ? 1 : 0;


    Ref<RenderTargetTexture> occlusionMap = RenderTargetTexture::getTemporary(source->width(), source->height(), TextureFormat::RGBA8, false);
    occlusionMap->setSamplerState(m_samplerState);

    Texture* viewNormalMap = context->gbuffer(GBuffer::ViewNormalMap);
    Texture* viewDepthMap = context->gbuffer(GBuffer::ViewDepthMap);
    Texture* randomTexture = EngineDomain::renderingManager()->randomTexture();

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
    m_ssaoMaterial->setBufferData(u"LocalBuffer", &buf, sizeof(buf));

    m_ssaoMaterial->setTexture(u"_viewSpaceNormalMap", viewNormalMap);
    m_ssaoMaterial->setTexture(u"_viewDepthMap", viewDepthMap);
    m_ssaoMaterial->setTexture(u"_randomSampler", randomTexture);
    m_ssaoMaterial->setMainTexture(source);
    context->blit(m_ssaoMaterial, occlusionMap);
    //context->blit(m_ssaoMaterial, destination);


    m_integrationMaterial->setBufferData(u"EffectSettings", &settings, sizeof(settings));
    m_integrationMaterial->setTexture(u"_occlusionMap", occlusionMap);
    m_integrationMaterial->setMainTexture(source);
    context->blit(m_integrationMaterial, destination);

    //m_material->setMainTexture(source);
    //context->blit(m_material, destination);


    RenderTargetTexture::releaseTemporary(occlusionMap);
    return true;
}

} // namespace detail
} // namespace ln

