
#include "Internal.hpp"
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Rendering/ImageEffect.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include "ImageEffectRenderer.hpp"
#include "RenderingManager.hpp"
#include "RenderTargetTextureCache.hpp"

namespace ln {
namespace detail {

//==============================================================================
// ImageEffectRenderer

ImageEffectRenderer::ImageEffectRenderer()
    : m_manager(detail::EngineDomain::renderingManager())
{
    m_blitMaterial = newObject<Material>();
}

void ImageEffectRenderer::addImageEffect(ImageEffect* effect)
{
    if (LN_REQUIRE(effect)) return;
    m_imageEffects.add(effect);
}

void ImageEffectRenderer::removeImageEffect(ImageEffect* effect)
{
    if (LN_REQUIRE(effect)) return;
    m_imageEffects.remove(effect);
}

void ImageEffectRenderer::render(RenderingContext* context, RenderTargetTexture* inout)
{
    if (!m_imageEffects.isEmpty())
    {
        RenderTargetTexture* primaryTarget = m_manager->frameBufferCache()->requestRenderTargetTexture(
            SizeI(inout->width(), inout->height()), TextureFormat::RGBA32, false);
        RenderTargetTexture* secondaryTarget = m_manager->frameBufferCache()->requestRenderTargetTexture(
            SizeI(inout->width(), inout->height()), TextureFormat::RGBA32, false);

        context->pushState(true);
        context->setDepthBuffer(nullptr);

        for (int i = 0; i < m_imageEffects.size(); i++)
        {
            if (i == 0) {
                m_imageEffects[i]->onRender(context, inout, secondaryTarget);
            }
            else {
                m_imageEffects[i]->onRender(context, primaryTarget, secondaryTarget);
            }
            std::swap(primaryTarget, secondaryTarget);
        }

        context->resetState();

        m_blitMaterial->setMainTexture(primaryTarget); // TODO: blit が source 指定未対応なので
        context->blit(nullptr, inout, m_blitMaterial);

        context->popState();

        m_manager->frameBufferCache()->release(secondaryTarget);
        m_manager->frameBufferCache()->release(primaryTarget);
    }
}

} // namespace detail
} // namespace ln

