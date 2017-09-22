
#include "../Internal.h"
#include "../Graphics/GraphicsManager.h"
#include <Lumino/UI/UIEventArgs.h>
#include <Lumino/Rendering/RenderView.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// WorldRenderView
//==============================================================================
WorldRenderView::WorldRenderView()
	: m_layerCullingMask(0)
{
}

WorldRenderView::~WorldRenderView()
{
}



//==============================================================================
// RenderViewLayerList
//==============================================================================
namespace detail {

void RenderViewLayerList::addRenderView(RenderLayer* renderView)
{
	m_viewportLayerList.add(renderView);
}

void RenderViewLayerList::updateLayout(const Size& viewSize)
{
	for (auto& layer : m_viewportLayerList)
	{
		layer->updateLayout(viewSize);
	}
}

void RenderViewLayerList::onRoutedEvent(UIEventArgs* e)
{
	// UI 要素は通常 UIViewport の上に張り付けられる。
	// デフォルトの MainWindow などは全体に UILayoutPanel が乗るので、
	// 通常のイベントではなく RoutedEvent でなければハンドリングできない。

	for (auto& layer : m_viewportLayerList)
	{
		layer->onRoutedEvent(e);
		if (e->handled) return;
	}
}

UIElement* RenderViewLayerList::checkMouseHoverElement(const Point& globalPt)
{
	for (auto& layer : m_viewportLayerList)
	{
		auto* element = layer->hitTestUIElement(globalPt);
		if (element != nullptr) return element;
	}
	return nullptr;
}

void RenderViewLayerList::render(DrawList* context, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer)
{
	for (auto& layer : m_viewportLayerList)
	{
		layer->render(context, renderTarget, depthBuffer);
	}
}

} // namespace detail


//==============================================================================
// RenderLayer
//==============================================================================
//LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIViewportLayer, Object);

//------------------------------------------------------------------------------
RenderLayer::RenderLayer()
	: m_clearMode(RenderLayerClearMode::None)
	, m_backgroundColor(Color::White)
{
}

//------------------------------------------------------------------------------
RenderLayer::~RenderLayer()
{
}

void RenderLayer::addChildRenderView(RenderLayer* renderView)
{
	m_layerList.addRenderView(renderView);
}

//------------------------------------------------------------------------------
void RenderLayer::addPostEffect(PostEffect* postEffect)
{
	if (LN_REQUIRE(postEffect)) return;
	m_postEffects.add(postEffect);
	postEffect->m_ownerLayer = this;
}

//------------------------------------------------------------------------------
UIElement* RenderLayer::hitTestUIElement(const Point& globalPt)
{
	return m_layerList.checkMouseHoverElement(globalPt);
}

//------------------------------------------------------------------------------
void RenderLayer::onRoutedEvent(UIEventArgs* e)
{
	m_layerList.onRoutedEvent(e);
}

//------------------------------------------------------------------------------
void RenderLayer::updateLayout(const Size& viewSize)
{
	setViewSize(viewSize);
	m_layerList.updateLayout(viewSize);
}

void RenderLayer::render(DrawList* context, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer)
{
	updateFramebufferIfNeeded();

	if (m_postEffects.isEmpty())
	{
		renderScene(renderTarget, depthBuffer);
		m_layerList.render(context, renderTarget, depthBuffer);
	}
	else
	{
		// m_primaryLayerTarget へ描いてもらう
		renderScene(m_primaryLayerTarget, m_depthBuffer);
		m_layerList.render(context, m_primaryLayerTarget, m_depthBuffer);

		// Posteffect
		postRender(context, &m_primaryLayerTarget, &m_secondaryLayerTarget);

		//context->pushState();

		////context->clear(ClearFlags::Depth, Color());
		//context->setRenderTarget(0, m_primaryLayerTarget);
		//context->clear(ClearFlags::All, Color::Blue);
		//context->popState();

		context->blit(m_primaryLayerTarget, renderTarget, Matrix::Identity);

	}
}

//------------------------------------------------------------------------------
void RenderLayer::postRender(DrawList* context, Ref<RenderTargetTexture>* primaryLayerTarget, Ref<RenderTargetTexture>* secondaryLayerTarget)
{
	for (auto& e : m_postEffects)
	{
		e->onRender(context, *primaryLayerTarget, *secondaryLayerTarget);
		std::swap(*primaryLayerTarget, *secondaryLayerTarget);
	}
}

void RenderLayer::updateFramebufferIfNeeded()
{
	if (m_postEffects.isEmpty())
	{
		m_primaryLayerTarget.safeRelease();
		m_secondaryLayerTarget.safeRelease();
		m_depthBuffer.safeRelease();
	}
	else
	{
		const SizeI& newSize = SizeI::fromFloatSize(getViewSize());

		if (m_primaryLayerTarget == nullptr ||
			(m_primaryLayerTarget != nullptr && newSize != m_primaryLayerTarget->getSize()))
		{
			// RenderTargetTexture
			// TODO: できればこういうのは Resize 関数を作りたい。作り直したくない
			// TODO: というか UE4 みたいにキャッシュしたい
			m_primaryLayerTarget = Ref<RenderTargetTexture>::makeRef();
			m_primaryLayerTarget->createImpl(detail::GraphicsManager::getInstance(), newSize, 1, TextureFormat::R8G8B8X8);
			m_secondaryLayerTarget = Ref<RenderTargetTexture>::makeRef();
			m_secondaryLayerTarget->createImpl(detail::GraphicsManager::getInstance(), newSize, 1, TextureFormat::R8G8B8X8);

			// DepthBuffer
			m_depthBuffer = Ref<DepthBuffer>::makeRef();
			m_depthBuffer->createImpl(detail::GraphicsManager::getInstance(), newSize, TextureFormat::D24S8);
		}
	}
}

//==============================================================================
// PostEffect
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(PostEffect, Object);

//------------------------------------------------------------------------------
PostEffect::PostEffect()
	: Object()
{
}

//------------------------------------------------------------------------------
PostEffect::~PostEffect()
{
}

//------------------------------------------------------------------------------
void PostEffect::initialize()
{
}

LN_NAMESPACE_END
