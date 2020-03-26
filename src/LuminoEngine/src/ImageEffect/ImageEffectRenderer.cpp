
#include "Internal.hpp"
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/ImageEffect/ImageEffect.hpp>
#include "ImageEffectRenderer.hpp"
#include "../Graphics/RenderTargetTextureCache.hpp"
#include "../Rendering/RenderingManager.hpp"

namespace ln {
namespace detail {

//==============================================================================
// ImageEffectRenderer

ImageEffectRenderer::ImageEffectRenderer()
    : m_manager(detail::EngineDomain::renderingManager())
	, m_copyMaterial(makeObject<Material>())
{
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

void ImageEffectRenderer::updateFrame(float elapsedSeconds)
{
    for (auto& imageEffect : m_imageEffects) {
        imageEffect->updateFrame(elapsedSeconds);
    }
}

void ImageEffectRenderer::applyInSceneImageEffects(const List<ImageEffect*>& imageEffects)
{
    m_imageEffectRenderers.resize(imageEffects.size());
    for (int i = 0; i < imageEffects.size(); i++) {
        // TODO: ImageEffect のうち、更新部分と Render 部分を分離して、Render 部分だけここで複製する
        ImageEffect* im = imageEffects[i];
        m_imageEffectRenderers[i] = (im);
    }
}

void ImageEffectRenderer::render(RenderingContext* context, RenderTargetTexture* inout)
{



    if (!m_imageEffects.isEmpty() || !m_imageEffectRenderers.isEmpty())
    {
		Ref<RenderTargetTexture> primaryTarget = RenderTargetTexture::getTemporary(inout->width(), inout->height(), TextureFormat::RGBA8, false);
		Ref<RenderTargetTexture> secondaryTarget = RenderTargetTexture::getTemporary(inout->width(), inout->height(), TextureFormat::RGBA8, false);


#if 0
		context->pushState(true);
		context->setDepthBuffer(nullptr);
		context->setDepthTestEnabled(false);
		context->setDepthWriteEnabled(false);
		m_imageEffects[0]->onRender(context, inout, primaryTarget);
		m_copyMaterial->setMainTexture(primaryTarget);
		context->blit(m_copyMaterial, inout);
		context->popState();
#else
        context->pushState(true);
        context->setDepthBuffer(nullptr);

        int i = 0;
        for (auto& renderer : m_imageEffects)
        {
            if (i == 0) {
                renderer->onRender(context, inout, secondaryTarget);
            }
            else {
                renderer->onRender(context, primaryTarget, secondaryTarget);
            }
            std::swap(primaryTarget, secondaryTarget);
            i++;
        }
        for (auto& renderer : m_imageEffectRenderers)
        {
            if (i == 0) {
                renderer->onRender(context, inout, secondaryTarget);
            }
            else {
                renderer->onRender(context, primaryTarget, secondaryTarget);
            }
            std::swap(primaryTarget, secondaryTarget);
            i++;
        }

        context->resetState();

		m_copyMaterial->setMainTexture(primaryTarget);
        context->blit(m_copyMaterial, inout);

        context->popState();
#endif
		RenderTargetTexture::releaseTemporary(secondaryTarget);
		RenderTargetTexture::releaseTemporary(primaryTarget);
    }
}

} // namespace detail
} // namespace ln

