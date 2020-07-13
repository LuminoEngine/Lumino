
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
    , m_material(nullptr)
{
}

bool FilmicPostEffectInstance::init(FilmicPostEffect* owner)
{
    if (!PostEffectInstance::init()) return false;

    auto shader2 = Shader::create(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/PostEffect/Resource/SSAOOcclusionMap.fx");
    m_ssaoMaterial = makeObject<Material>();
    m_ssaoMaterial->setShader(shader2);

    auto shader1 = Shader::create(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/PostEffect/Resource/FilmicPostEffect.fx");
    m_material = makeObject<Material>();
    m_material->setShader(shader1);

    // TODO: 他と共有したいところ
    m_samplerState = makeObject<SamplerState>(TextureFilterMode::Linear, TextureAddressMode::Clamp);

    return true;
}

bool FilmicPostEffectInstance::onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination)
{

    Ref<RenderTargetTexture> occlusionMap = RenderTargetTexture::getTemporary(source->width(), source->height(), TextureFormat::RGBA8, false);
    occlusionMap->setSamplerState(m_samplerState);

    Texture* viewNormalMap = context->gbuffer(GBuffer::ViewNormalMap);
    Texture* viewDepthMap = context->gbuffer(GBuffer::ViewDepthMap);

    m_ssaoMaterial->setTexture(u"_screenSampler", viewNormalMap);
    m_ssaoMaterial->setTexture(u"_viewDepthMap", viewDepthMap);
    m_ssaoMaterial->setMainTexture(source);
    context->blit(m_ssaoMaterial, destination);



    //m_material->setMainTexture(source);
    //context->blit(m_material, destination);


    RenderTargetTexture::releaseTemporary(occlusionMap);
    return true;
}

} // namespace detail
} // namespace ln

