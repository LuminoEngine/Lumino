
#include "Internal.hpp"
#include <LuminoGraphics/GPU/Texture.hpp>
#include <LuminoGraphics/Rendering/CommandList.hpp>
#include <LuminoGraphics/Rendering/RenderingContext.hpp>
#include <LuminoGraphics/Rendering/Material.hpp>
#include <LuminoEngine/PostEffect/PostEffect.hpp>
#include "PostEffectRenderer.hpp"
//#include "../../../Graphics/src/RenderTargetTextureCache.hpp"
#include <LuminoGraphics/Rendering/detail/RenderingManager.hpp>

namespace ln {
namespace detail {

//==============================================================================
// PostEffectRenderer

PostEffectRenderer::PostEffectRenderer()
    : m_manager(detail::RenderingManager::instance())
	, m_copyMaterial(makeObject_deprecated<Material>()) {
    m_copyMaterial->setShader(RenderingManager::instance()->builtinShader(BuiltinShader::CopyScreen));
}

void PostEffectRenderer::dispose()
{

}

void PostEffectRenderer::addPostEffect(PostEffect* effect)
{
    if (LN_REQUIRE(effect)) return;
    m_imageEffects.add(effect);
    refreshPostEffectInstances();
}

void PostEffectRenderer::removePostEffect(PostEffect* effect)
{
    if (LN_REQUIRE(effect)) return;
    m_imageEffects.remove(effect);
    refreshPostEffectInstances();
}

void PostEffectRenderer::updateFrame(float elapsedSeconds)
{
    for (auto& imageEffect : m_imageEffects) {
        imageEffect->updateFrame(elapsedSeconds);
    }
}

void PostEffectRenderer::applyInScenePostEffects(const List<PostEffect*>& imageEffects)
{
    //m_imageEffectRenderers.resize(imageEffects.size());
    //for (int i = 0; i < imageEffects.size(); i++) {
    //    // TODO: PostEffect のうち、更新部分と Render 部分を分離して、Render 部分だけここで複製する
    //    PostEffect* im = imageEffects[i];
    //    m_imageEffectRenderers[i] = (im);
    //}

    for (auto& imageEffect : imageEffects) {
        int i = m_collectedPostEffectInstances.indexOfIf([&](auto& x) { return x.instance->m_owner == imageEffect; });
        if (i >= 0) {
            m_collectedPostEffectInstances[i].found = true;
        }
        else {
            auto instance = imageEffect->createInstance();
            m_collectedPostEffectInstances.add({ instance, true });
        }
    }

    for (int i = m_collectedPostEffectInstances.size() - 1; i >= 0; i--) {
        if (!m_collectedPostEffectInstances[i].found) {
            m_collectedPostEffectInstances.removeAt(i);
        }
        else {
            m_collectedPostEffectInstances[i].found = false;
        }
    }
}

void PostEffectRenderer::render(RenderingContext* context, RenderTargetTexture* input, RenderTargetTexture* output)
{



    if (!m_imageEffectInstances.isEmpty() || !m_collectedPostEffectInstances.isEmpty())
    {
		Ref<RenderTargetTexture> primaryTarget = RenderTargetTexture::getTemporary(input->width(), input->height(), input->format(), false);
		Ref<RenderTargetTexture> secondaryTarget = RenderTargetTexture::getTemporary(input->width(), input->height(), input->format(), false);


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
        CommandList* commandList = context->getCommandList(RenderPart::PostEffect);

        commandList->pushState(true);

        bool renderd = false;
        int renderCount = 0;
        for (auto& effect : m_collectedPostEffectInstances)
        {
            if (renderCount == 0) {
                renderd = effect.instance->onRender(context->currentRenderView, commandList, input, secondaryTarget);
            }
            else {
                renderd = effect.instance->onRender(context->currentRenderView, commandList, primaryTarget, secondaryTarget);
            }

            if (renderd) {
                std::swap(primaryTarget, secondaryTarget);
                renderCount++;
            }
        }
        for (auto& effect : m_imageEffectInstances)
        {
            if (renderCount == 0) {
                renderd = effect.instance->onRender(context->currentRenderView, commandList, input, secondaryTarget);
            }
            else {
                renderd = effect.instance->onRender(context->currentRenderView, commandList, primaryTarget, secondaryTarget);
            }
            
            if (renderd) {
                std::swap(primaryTarget, secondaryTarget);
                renderCount++;
            }
        }

        commandList->resetState();

        if (renderCount >= 1) {
            m_copyMaterial->setMainTexture(primaryTarget);
            commandList->blit(m_copyMaterial, output);
        }

        commandList->popState();
#endif
		RenderTargetTexture::releaseTemporary(secondaryTarget);
		RenderTargetTexture::releaseTemporary(primaryTarget);
    }
}

void PostEffectRenderer::refreshPostEffectInstances()
{
    //for (auto& i : m_imageEffectInstances) {
    //    i.found = false;
    //}

    for (auto& imageEffect : m_imageEffects) {
        int i = m_imageEffectInstances.indexOfIf([&](auto& x) { return x.instance->m_owner == imageEffect; });
        if (i >= 0) {
            m_imageEffectInstances[i].found = true;
        }
        else {
            auto instance = imageEffect->createInstance();
            m_imageEffectInstances.add({ instance, true });
        }
    }

    for (int i = m_imageEffectInstances.size() - 1; i >= 0; i--) {
        if (!m_imageEffectInstances[i].found) {
            m_imageEffectInstances.removeAt(i);
        }
        else {
            m_imageEffectInstances[i].found = false;
        }
    }
}

} // namespace detail
} // namespace ln

