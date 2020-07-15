
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

    auto shader2 = Shader::create(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/PostEffect/Resource/SSAOOcclusionMap.fx");
    m_ssaoMaterial = makeObject<Material>();
    m_ssaoMaterial->setShader(shader2);

    auto shader1 = Shader::create(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/PostEffect/Resource/FilmicPostEffect.fx");
    m_integrationMaterial = makeObject<Material>();
    m_integrationMaterial->setShader(shader1);

    // TODO: 他と共有したいところ
    m_samplerState = makeObject<SamplerState>(TextureFilterMode::Linear, TextureAddressMode::Clamp);

    return true;
}

bool FilmicPostEffectInstance::onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination)
{
    if (!m_antialiasEnabled && !m_ssaoEnabled && !m_bloomEnabled && !m_dofEnabled) {
        return false;
    }

    struct EffectSettings
    {
        int antialiasEnabled;
        int ssaoEnabled;
        int bloomEnabled;
        int dofEnabled;
    };

    EffectSettings settings;
    settings.antialiasEnabled = m_antialiasEnabled ? 1 : 0;
    settings.ssaoEnabled = m_ssaoEnabled ? 1 : 0;
    settings.bloomEnabled = m_bloomEnabled ? 1 : 0;
    settings.dofEnabled = m_dofEnabled ? 1 : 0;


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

