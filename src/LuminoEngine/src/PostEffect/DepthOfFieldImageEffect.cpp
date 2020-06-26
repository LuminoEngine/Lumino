
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Graphics/SamplerState.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Rendering/RenderView.hpp>
#include <LuminoEngine/PostEffect/DepthOfFieldImageEffect.hpp>
#include "../Rendering/RenderingManager.hpp"

namespace ln {

//==============================================================================
// DepthOfFieldImageEffect

DepthOfFieldImageEffect::DepthOfFieldImageEffect()
{
}

void DepthOfFieldImageEffect::init()
{
    ImageEffect::init();
}

Ref<ImageEffectInstance> DepthOfFieldImageEffect::onCreateInstance()
{
    return makeObject<detail::DepthOfFieldImageEffectInstance>(this);
}

//==============================================================================
// DepthOfFieldImageEffectInstance

namespace detail {

DepthOfFieldImageEffectInstance::DepthOfFieldImageEffectInstance()
    : m_owner(nullptr)
    , m_copyMaterial(nullptr)
{
}

bool DepthOfFieldImageEffectInstance::init(DepthOfFieldImageEffect* owner)
{
    if (!ImageEffectInstance::init()) return false;

    auto shader1 = Shader::create(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/PostEffect/Resource/Copy.fx");
    m_copyMaterial = makeObject<Material>();
    m_copyMaterial->setShader(shader1);

    auto shader2 = Shader::create(u"C:/Proj/LN/Lumino/src/LuminoEngine/src/PostEffect/Resource/DepthOfFieldImageEffect.fx");
    m_dofMaterial = makeObject<Material>();
    m_dofMaterial->setShader(shader2);

    return true;
}

bool DepthOfFieldImageEffectInstance::onRender(RenderingContext* context, RenderTargetTexture* source, RenderTargetTexture* destination)
{
    auto dofTexture = RenderTargetTexture::getTemporary(source->width() / 2, source->height(), TextureFormat::RGBA8, false);
    Texture* viewDepthMap = context->gbuffer(GBuffer::ViewDepthMap);

    auto rect = RectI(0, 0, source->width() / 2, source->height() / 2);
    for (int i = 0; i < 8; i++)
    {
        context->setViewportRect(rect);
        m_copyMaterial->setMainTexture(source);
        context->blit(m_copyMaterial, dofTexture);
        //context->blit(m_copyMaterial, destination);

        rect.y += rect.height;
        rect.width /= 2;
        rect.height /= 2;
    }

    //context->setScissorRect(RectI(0, 0, 320, 240));

    m_dofMaterial->setTexture(u"_depthTex", viewDepthMap);
    m_dofMaterial->setTexture(u"_dofTex", dofTexture);
    context->blit(m_dofMaterial, destination);

    RenderTargetTexture::releaseTemporary(dofTexture);
    
    return true;
}

} // namespace detail
} // namespace ln

