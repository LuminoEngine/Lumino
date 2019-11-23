
#include "Internal.hpp"
//#include <LuminoEngine/Graphics/Bitmap.hpp>
#include <LuminoEngine/Graphics/RenderPass.hpp>
#include <LuminoEngine/Graphics/GraphicsContext.hpp>
#include <LuminoEngine/UI/UIRenderingContext.hpp>
#include <LuminoEngine/UI/UIEvents.hpp>
#include <LuminoEngine/UI/UIViewport.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderView.hpp>
#include "../Graphics/GraphicsManager.hpp"
#include "../ImageEffect/ImageEffectRenderer.hpp"
#include "UIManager.hpp"

namespace ln {

//==============================================================================
// UIViewport

UIViewport::UIViewport()
    : m_manager(detail::EngineDomain::uiManager())
{
}

UIViewport::~UIViewport()
{
}

void UIViewport::init()
{
	UIContainerElement::init();
	setHorizontalAlignment(HAlignment::Stretch);
	setVerticalAlignment(VAlignment::Stretch);

    m_imageEffectRenderer = makeRef<detail::ImageEffectRenderer>();
    m_blitMaterial = makeObject<Material>();
	m_blitMaterial->setBlendMode(BlendMode::Normal);

	//m_renderPass = makeObject<RenderPass>();
}

void UIViewport::onDispose(bool explicitDisposing)
{
	if (m_primaryTarget) {
		RenderTargetTexture::releaseTemporary(m_primaryTarget);
		m_primaryTarget = nullptr;
	}

    m_renderViews.clear();
	UIContainerElement::onDispose(explicitDisposing);
}

void UIViewport::addRenderView(RenderView* view)
{
    m_renderViews.add(view);
}

void UIViewport::removeRenderView(RenderView* view)
{
    m_renderViews.remove(view);
}

void UIViewport::addImageEffect(ImageEffect* effect)
{
    m_imageEffectRenderer->addImageEffect(effect);
}

void UIViewport::removeImageEffect(ImageEffect* effect)
{
    m_imageEffectRenderer->removeImageEffect(effect);
}

UIElement* UIViewport::lookupMouseHoverElement(const Point& frameClientPosition)
{
    for (auto& view : m_renderViews) {
        UIElement* e = view->onLookupMouseHoverElement(frameClientPosition);
        if (e) return e;
    }
    return UIContainerElement::lookupMouseHoverElement(frameClientPosition);
}

void UIViewport::onUpdateFrame(float elapsedTimer)
{
	for (auto& view : m_renderViews) {
		view->updateFrame(elapsedTimer);
	}

    m_imageEffectRenderer->updateFrame(elapsedTimer);
}

void UIViewport::onUpdateStyle(const UIStyleContext* styleContext, const detail::UIStyleInstance* finalStyle)
{
	for (auto& view : m_renderViews) {
		view->updateUIStyle(styleContext, finalStyle);
	}
}

Size UIViewport::arrangeOverride(UILayoutContext* layoutContext, const Size& finalSize)
{
    // TODO: tmp
    for (auto& rv : m_renderViews) {
        rv->setActualSize(finalSize);
    }
    m_actualViewboxSize = finalSize;
    return UIContainerElement::arrangeOverride(layoutContext, finalSize);
}


void UIViewport::onUpdateLayout(UILayoutContext* layoutContext)
{
	for (auto& view : m_renderViews) {
		view->updateUILayout(layoutContext);
		// TODO: view box
		//view->setActualScreenOffset(Point(finalGlobalRect.x, finalGlobalRect.y));
	}
}

//void UIViewport::render(UIRenderingContext* context)
//{
//}

void UIViewport::onRender(UIRenderingContext* context)
{

    GraphicsContext* graphicsContext = context->m_frameWindowRenderingGraphicsContext;
    //auto* renderTarget = graphicsContext->renderTarget(0);
	//Ref<RenderPass> oldRenderPass = graphicsContext->renderPass();

    // TODO: dp -> px 変換
    Size viewSize = actualSize();

	// このスコープ終端で RenderTargetTexture::releaseTemporary() するわけにはいかない。
	// この RenderTarget は context->drawImage() に乗ってこのスコープの外側でも使われるため、次回の描画までは再利用されないようにしたい。
	if (m_primaryTarget) {
		RenderTargetTexture::releaseTemporary(m_primaryTarget);
	}

	m_primaryTarget = RenderTargetTexture::getTemporary(viewSize.width, viewSize.height, TextureFormat::RGBA8, false);

    //graphicsContext->setRenderTarget(0, primaryTarget);
	//m_renderPass->setRenderTarget(0, primaryTarget);
	//graphicsContext->beginRenderPass(m_renderPass);
 //   graphicsContext->clear(ClearFlags::All, Color::Gray);	// TODO: renderPass の clear でカバー

    for (auto& view : m_renderViews) {
        view->render(graphicsContext, m_primaryTarget);
    }
    m_imageEffectRenderer->render(context, m_primaryTarget);


    //context->blit(primaryTarget, renderTarget);

    //detail::TextureInternal::readData(m_primaryTarget, graphicsContext)->save(u"test.png");

    m_blitMaterial->setMainTexture(m_primaryTarget);
    context->drawImage(Rect(0, 0, viewSize), m_blitMaterial);

    //RenderTargetTexture::releaseTemporary(primaryTarget);

    //graphicsContext->setRenderTarget(0, renderTarget);
	//graphicsContext->endRenderPass();

    // TODO: ViewBoxTransform

    //graphicsContext->resetState();
}

void UIViewport::onRoutedEvent(UIEventArgs* e)
{
    for (auto& view : m_renderViews)
    {
        view->onRoutedEvent(e);
		if (e->handled) {
			invalidateVisual();	// TODO: Editor Mode で、Scene 上を D&Dしたらカメラ動かしたいのでとりあえずここで処理してるが、無駄が多いかも
			return;
		}
    }

    UIContainerElement::onRoutedEvent(e);
}

} // namespace ln

