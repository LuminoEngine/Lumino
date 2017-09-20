
#include "Internal.h"
#include <Lumino/UI/UIViewport.h>
#include <Lumino/UI/UILayoutView.h>
#include <Lumino/UI/UIContext.h>
#include <Lumino/UI/UIFrameWindow.h>
#include <Lumino/Rendering/DrawingContext.h>
#include <Lumino/Rendering/SceneRenderer.h>
#include "../EngineManager.h"
#include "UIManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// UIViewport
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIViewport, UIElement);

//------------------------------------------------------------------------------
UIViewport::UIViewport()
	: UIElement()
	, m_backbufferSize(0, 0)
	, m_placement(ViewportPlacement::ViewBox)
	, m_backgroundColor(Color::White)
{
}

//------------------------------------------------------------------------------
UIViewport::~UIViewport()
{
}

//------------------------------------------------------------------------------
void UIViewport::initialize()
{
	UIElement::initialize();
	setHitTestVisible(true);

	m_drawingContext = newObject<DrawingContext>();
	m_sceneRenderer = Ref<detail::NonShadingRenderer>::makeRef();
	m_sceneRenderer->initialize(detail::EngineDomain::getGraphicsManager());
	m_renderView = newObject<RenderView>();
	m_renderView->m_lists.add(m_drawingContext->getDrawElementList());
}

//------------------------------------------------------------------------------
void UIViewport::setViewBackgroundColor(const Color& color)
{
	m_backgroundColor = color;
}

//------------------------------------------------------------------------------
void UIViewport::setPlacement(ViewportPlacement placement)
{
	m_placement = placement;
}

//------------------------------------------------------------------------------
void UIViewport::setBackbufferSize(int width, int height)
{
	m_backbufferSize.set(width, height);
}

//------------------------------------------------------------------------------
void UIViewport::addViewportLayer(UIViewportLayer* layer)
{
	m_viewportLayerList.add(layer);
}

//------------------------------------------------------------------------------
//void UIViewport::RemoveViewportLayer(ViewportLayer* layer)
//{
//	if (m_viewportLayerList->Remove(Ref<ViewportLayer>(layer)))
//	{
//		layer->SetOwner(nullptr);
//	}
//}

//------------------------------------------------------------------------------
void UIViewport::onRoutedEvent(UIEventArgs* e)
{
	// UI 要素は通常 UIViewport の上に張り付けられる。
	// デフォルトの MainWindow などは全体に UILayoutPanel が乗るので、
	// 通常のイベントではなく RoutedEvent でなければハンドリングできない。


	for (auto& layer : m_viewportLayerList)
	{
		layer->onRoutedEvent(e);
		if (e->handled) return;
	}

	return UIElement::onRoutedEvent(e);
}

//------------------------------------------------------------------------------
Size UIViewport::arrangeOverride(const Size& finalSize)
{
	Size renderSize = UIElement::arrangeOverride(finalSize);

	for (auto& layer : m_viewportLayerList)
	{
		layer->updateLayout(finalSize);
	}

	// バックバッファサイズの調整
	{
		if (m_placement == ViewportPlacement::AutoResize)
			m_viewSize = getActualSize();
		else
			m_viewSize = m_backbufferSize.toFloatSize();
	}

	return renderSize;
}

//------------------------------------------------------------------------------
UIElement* UIViewport::checkMouseHoverElement(const Point& globalPt)
{
	for (auto& layer : m_viewportLayerList)
	{
		auto* element = layer->hitTestUIElement(globalPt);
		if (element != nullptr) return element;
	}

	return UIElement::checkMouseHoverElement(globalPt);
}

//------------------------------------------------------------------------------
void UIViewport::onRender(DrawingContext* g)
{
	updateFramebufferSizeIfNeeded();

	//TODO: state push/pop

	//Ref<RenderTargetTexture> oldRT = g->getRenderTarget(0);
	//Ref<DepthBuffer> oldDB = g->getDepthBuffer();

	//g->setRenderTarget(0, m_primaryLayerTarget);
	//g->setDepthBuffer(m_depthBuffer);
	//g->clear(ClearFlags::All, m_backgroundColor, 1.0f, 0);




	for (auto& layer : m_viewportLayerList)
	{
		layer->render();
	}

	g->setBuiltinEffectData(detail::BuiltinEffectData::DefaultData);

	// 全てのレイヤーの描画リストを実行し m_primaryLayerTarget へ書き込む
	for (auto& layer : m_viewportLayerList)
	{
		layer->executeDrawListRendering(g, m_primaryLayerTarget, m_depthBuffer);

		// Posteffect
		layer->postRender(g, &m_primaryLayerTarget, &m_secondaryLayerTarget);
	}





	//g->setRenderTarget(0, oldRT);
	//g->setDepthBuffer(oldDB);

	Matrix transform;
	makeViewBoxTransform(SizeI::fromFloatSize(getActualSize()), m_backbufferSize, &transform);

	g->setViewportRect(RectI::fromFloatRect(getFinalGlobalRect()));
	g->blit(m_primaryLayerTarget, transform);
	g->setViewportRect(RectI(0, 0, -1, -1));

	// TODO: 暫定。blit の中で深度書き込みしないようにしてほしいかも。
	g->clear(ClearFlags::Depth, Color());

	UIElement::onRender(g);
}

//------------------------------------------------------------------------------
void UIViewport::updateFramebufferSizeIfNeeded()
{
	const SizeI& newSize = SizeI::fromFloatSize(m_viewSize);

	if (m_primaryLayerTarget == nullptr || 
		(m_primaryLayerTarget != nullptr && newSize != m_primaryLayerTarget->getSize()))
	{
		// RenderTargetTexture
		// TODO: できればこういうのは Resize 関数を作りたい。作り直したくない
		// TODO: というか UE4 みたいにキャッシュしたい
		m_primaryLayerTarget = Ref<RenderTargetTexture>::makeRef();
		m_primaryLayerTarget->createImpl(getManager()->getGraphicsManager(), newSize, 1, TextureFormat::R8G8B8X8);
		m_secondaryLayerTarget = Ref<RenderTargetTexture>::makeRef();
		m_secondaryLayerTarget->createImpl(getManager()->getGraphicsManager(), newSize, 1, TextureFormat::R8G8B8X8);

		// DepthBuffer
		m_depthBuffer = Ref<DepthBuffer>::makeRef();
		m_depthBuffer->createImpl(getManager()->getGraphicsManager(), newSize, TextureFormat::D24S8);
	}
}

//------------------------------------------------------------------------------
void UIViewport::makeViewBoxTransform(const SizeI& dstSize, const SizeI& srcSize, Matrix* mat)
{
	float sw = static_cast<float>(srcSize.width);   // 転送元
	float sh = static_cast<float>(srcSize.height);
	float dw = static_cast<float>(dstSize.width);	// 転送先
	float dh = static_cast<float>(dstSize.height);

	float new_x, new_y;
	float new_w, new_h;

	float ratio_w;
	float ratio_h;

	// バックバッファサイズとスクリーンサイズが同じ場合
	if (sw == dw && sh == dh)
	{
		// そのまま設定
		new_x = 0;
		new_y = 0;
		new_w = sw;
		new_h = sh;
		ratio_w = 1.0f;
		ratio_h = 1.0f;
	}
	else
	{
		// 現在のスクリーンサイズ(デフォルトビューポートのサイズ)と画面サイズの比率計算
		ratio_w = dw / sw;
		ratio_h = dh / sh;

		// 縦方向に合わせる ( 左右が余る )
		if (ratio_w > ratio_h)
		{
			new_w = static_cast< float >(sw * ratio_h);
			new_h = static_cast< float >(dh);
			new_x = static_cast< float >((dw / 2) - (new_w / 2));
			new_y = 0;
		}
		//横方向にあわせる
		else
		{
			new_w = static_cast< float >(dw);
			new_h = static_cast< float >(sh * ratio_w);
			new_x = 0;
			new_y = static_cast< float >((dh / 2) - (new_h / 2));
		}
	}

#if 0	// pxel based
	* mat = Matrix::Identity;
	mat->scale(new_w / sw, new_h / sh, 1.0f);
	mat->translate(new_x, new_y, 0.0f);
#else	// screen coord based
	*mat = Matrix::Identity;
	mat->scale(new_w / dw, new_h / dh, 1.0f);
#endif
}


//==============================================================================
// UIViewportLayer
//==============================================================================
//LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIViewportLayer, Object);

//------------------------------------------------------------------------------
UIViewportLayer::UIViewportLayer()
	: SceneRenderView()
	, m_clearMode(ViewClearMode::None)
	, m_backgroundColor(Color::White)
{
}

//------------------------------------------------------------------------------
UIViewportLayer::~UIViewportLayer()
{
}

//------------------------------------------------------------------------------
void UIViewportLayer::addPostEffect(PostEffect* postEffect)
{
	m_postEffects.add(postEffect);
	postEffect->m_ownerLayer = this;
}

//------------------------------------------------------------------------------
UIElement* UIViewportLayer::hitTestUIElement(const Point& globalPt)
{
	return nullptr;
}

//------------------------------------------------------------------------------
void UIViewportLayer::onRoutedEvent(UIEventArgs* e)
{
}

//------------------------------------------------------------------------------
void UIViewportLayer::updateLayout(const Size& viewSize)
{
	setViewSize(viewSize);
}

//------------------------------------------------------------------------------
void UIViewportLayer::postRender(DrawList* context, Ref<RenderTargetTexture>* primaryLayerTarget, Ref<RenderTargetTexture>* secondaryLayerTarget)
{
	for (auto& e : m_postEffects)
	{
		e->onRender(context, *primaryLayerTarget, *secondaryLayerTarget);
		std::swap(*primaryLayerTarget, *secondaryLayerTarget);
	}
}


//==============================================================================
// UILayoutLayer
//==============================================================================
//------------------------------------------------------------------------------
UILayoutLayer::UILayoutLayer()
{
}

//------------------------------------------------------------------------------
UILayoutLayer::~UILayoutLayer()
{
}

//------------------------------------------------------------------------------
void UILayoutLayer::initialize()
{
	UIViewportLayer::initialize();
	m_root = newObject<UILayoutView>(UIContext::getMainContext(), nullptr);	// TODO: コンテキスト変更とか

	// このルート要素はビュー全体に広がるが、ヒットテストは行わない。
	// 行ってしまうと、その後ろにあるシーンのビューにイベントが流れなくなる。
	m_root->setHitTestVisible(false);

	m_drawingContext = newObject<DrawingContext>();

	// lighting disabled.
	// TODO: newObject
	auto internalRenderer = Ref<detail::NonShadingRenderer>::makeRef();
	internalRenderer->initialize(detail::EngineDomain::getGraphicsManager());
	m_internalRenderer = internalRenderer;

	m_lists.add(m_drawingContext->getDrawElementList());
}

//------------------------------------------------------------------------------
UILayoutView* UILayoutLayer::GetLayoutView() const
{
	return m_root;
}

//------------------------------------------------------------------------------
UIElement* UILayoutLayer::hitTestUIElement(const Point& globalPt)
{
	auto* element = m_root->checkMouseHoverElement(globalPt);
	if (element != nullptr) return element;
	return UIViewportLayer::hitTestUIElement(globalPt);
}

//------------------------------------------------------------------------------
void UILayoutLayer::onRoutedEvent(UIEventArgs* e)
{
	//m_root->raiseEvent(e->GetType(), e->sender, e);
}

//------------------------------------------------------------------------------
void UILayoutLayer::updateLayout(const Size& viewSize)
{
	m_root->updateLayout(viewSize);
}

//------------------------------------------------------------------------------
void UILayoutLayer::render()
{
	m_drawingContext->beginMakeElements();
	m_drawingContext->setBlendMode(BlendMode::Alpha);
	//m_drawingContext->Clear(ClearFlags::All, Color::Black);;	// TODO
	m_root->render(m_drawingContext);
}

//------------------------------------------------------------------------------
void UILayoutLayer::executeDrawListRendering(DrawList* parentDrawList, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer)
{
	// TODO: float
	Size viewPixelSize((float)renderTarget->getWidth(), (float)renderTarget->getHeight());

	bool clearColorBuffer = (getClearMode() == ViewClearMode::ColorDepth || getClearMode() == ViewClearMode::Color);

	this->m_cameraInfo.dataSourceId = reinterpret_cast<intptr_t>(this);
	this->m_cameraInfo.viewPixelSize = viewPixelSize;
	this->m_cameraInfo.viewPosition = Vector3::Zero;
	this->m_cameraInfo.viewDirection = Vector3::UnitZ;
	this->m_cameraInfo.viewMatrix = Matrix::Identity;
	this->m_cameraInfo.projMatrix = Matrix::makePerspective2DLH(this->m_cameraInfo.viewPixelSize.width, this->m_cameraInfo.viewPixelSize.height, 0, 1);
	this->m_cameraInfo.viewProjMatrix = this->m_cameraInfo.viewMatrix * this->m_cameraInfo.projMatrix;
	this->m_cameraInfo.viewFrustum = ViewFrustum(this->m_cameraInfo.projMatrix);
	this->m_cameraInfo.zSortDistanceBase = ZSortDistanceBase::NodeZ;
	m_internalRenderer->render(this, renderTarget, depthBuffer, nullptr, clearColorBuffer, getBackgroundColor());	// TODO: diag
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
