
#include "Internal.h"
#include <Lumino/UI/UIViewport.h>
#include <Lumino/UI/UILayoutView.h>
#include <Lumino/UI/UIContext.h>
#include <Lumino/Graphics/DrawingContext.h>
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
	, m_placement(ViewportPlacement::Stretch)
	, m_backgroundColor(Color::White)
{
}

//------------------------------------------------------------------------------
UIViewport::~UIViewport()
{
}

//------------------------------------------------------------------------------
void UIViewport::Initialize()
{
	UIElement::Initialize();
}

//------------------------------------------------------------------------------
void UIViewport::SetViewBackgroundColor(const Color& color)
{
	m_backgroundColor = color;
}

//------------------------------------------------------------------------------
void UIViewport::SetPlacement(ViewportPlacement placement)
{
	m_placement = placement;
}

//------------------------------------------------------------------------------
void UIViewport::SetBackbufferSize(int width, int height)
{
	m_backbufferSize.Set(width, height);
}

//------------------------------------------------------------------------------
void UIViewport::AddViewportLayer(UIViewportLayer* layer)
{
	m_viewportLayerList.Add(layer);
	layer->m_owner = this;
}

//------------------------------------------------------------------------------
//void UIViewport::RemoveViewportLayer(ViewportLayer* layer)
//{
//	if (m_viewportLayerList->Remove(RefPtr<ViewportLayer>(layer)))
//	{
//		layer->SetOwner(nullptr);
//	}
//}

//------------------------------------------------------------------------------
void UIViewport::OnRoutedEvent(UIEventArgs* e)
{
	// UI 要素は通常 UIViewport の上に張り付けられる。
	// デフォルトの MainWindow などは全体に UILayoutPanel が乗るので、
	// 通常のイベントではなく RoutedEvent でなければハンドリングできない。

	for (auto& layer : m_viewportLayerList)
	{
		layer->OnRoutedEvent(e);
		if (e->handled) return;
	}

	return UIElement::OnRoutedEvent(e);
}

//------------------------------------------------------------------------------
Size UIViewport::ArrangeOverride(const Size& finalSize)
{
	Size renderSize = UIElement::ArrangeOverride(finalSize);


	for (auto& layer : m_viewportLayerList)
	{
		layer->UpdateLayout(finalSize);
	}


	return renderSize;
}

//------------------------------------------------------------------------------
void UIViewport::OnRender(DrawingContext* g)
{
	// バックバッファサイズの調整
	{
		SizeI bakcbufferSize;
		if (m_placement == ViewportPlacement::AutoResize)
			bakcbufferSize = SizeI::FromFloatSize(GetRenderSize());
		else
			bakcbufferSize = m_backbufferSize;

		UpdateFramebufferSizeIfNeeded(bakcbufferSize);
	}


	//TODO: state push/pop

	RefPtr<RenderTargetTexture> oldRT = g->GetRenderTarget(0);
	RefPtr<DepthBuffer> oldDB = g->GetDepthBuffer();

	g->SetRenderTarget(0, m_primaryLayerTarget);
	g->SetDepthBuffer(m_depthBuffer);
	g->Clear(ClearFlags::All, m_backgroundColor, 1.0f, 0);





	for (auto& layer : m_viewportLayerList)
	{
		layer->Render();
	}


	// 全てのレイヤーの描画リストを実行し m_primaryLayerTarget へ書き込む
	for (auto& layer : m_viewportLayerList)
	{
		layer->ExecuteDrawListRendering(g, m_primaryLayerTarget, m_depthBuffer);

		// Posteffect
		layer->PostRender(g, &m_primaryLayerTarget, &m_secondaryLayerTarget);
	}





	g->SetRenderTarget(0, oldRT);
	g->SetDepthBuffer(oldDB);
	g->Blit(m_primaryLayerTarget);	// TODO: 転送先指定

	// TODO: 暫定。Blit の中で深度書き込みしないようにしてほしいかも。
	g->Clear(ClearFlags::Depth, Color());

	UIElement::OnRender(g);
}

//------------------------------------------------------------------------------
void UIViewport::UpdateFramebufferSizeIfNeeded(const SizeI& viewSize)
{
	bool create = false;
	SizeI newSize(0, 0);

	// 初回、まだ作成されていなければ作りたい
	if (m_primaryLayerTarget == nullptr)
	{
		create = true;
		newSize = viewSize;
	}

	// 自動リサイズONで、描画先とサイズが異なるなら再作成
	if (m_placement == ViewportPlacement::AutoResize)
	{
		if (m_primaryLayerTarget != nullptr && viewSize != m_primaryLayerTarget->GetSize())
		{
			create = true;
		}
		newSize = viewSize;
	}

	if (create)
	{
		// RenderTargetTexture
		// TODO: できればこういうのは Resize 関数を作りたい。作り直したくない
		// TODO: というか UE4 みたいにキャッシュしたい
		m_primaryLayerTarget = RefPtr<RenderTargetTexture>::MakeRef();
		m_primaryLayerTarget->CreateImpl(GetManager()->GetGraphicsManager(), newSize, 1, TextureFormat::R8G8B8X8);
		m_secondaryLayerTarget = RefPtr<RenderTargetTexture>::MakeRef();
		m_secondaryLayerTarget->CreateImpl(GetManager()->GetGraphicsManager(), newSize, 1, TextureFormat::R8G8B8X8);

		// DepthBuffer
		m_depthBuffer = RefPtr<DepthBuffer>::MakeRef();
		m_depthBuffer->CreateImpl(GetManager()->GetGraphicsManager(), newSize, TextureFormat::D24S8);

		m_viewSize = newSize.ToFloatSize();
	}
}

//------------------------------------------------------------------------------
void UIViewport::MakeViewBoxTransform(const SizeI& dstSize, const SizeI& srcSize, Matrix* mat)
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

#if 1	// pxel based
	* mat = Matrix::Identity;
	mat->Scale(new_w / sw, new_h / sh, 1.0f);
	mat->Translate(new_x, new_y, 0.0f);
#else	// screen coord based
	*mat = Matrix::Identity;
	mat->Scale(new_w / dw, new_h / dh, 1.0f);
#endif
}


//==============================================================================
// UIViewportLayer
//==============================================================================
//LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(UIViewportLayer, Object);

//------------------------------------------------------------------------------
UIViewportLayer::UIViewportLayer()
	: Object()
	, m_owner(nullptr)
{
}

//------------------------------------------------------------------------------
UIViewportLayer::~UIViewportLayer()
{
}

//------------------------------------------------------------------------------
void UIViewportLayer::AddPostEffect(PostEffect* postEffect)
{
	m_postEffects.Add(postEffect);
	postEffect->m_ownerLayer = this;
}

//------------------------------------------------------------------------------
void UIViewportLayer::OnRoutedEvent(UIEventArgs* e)
{
}

//------------------------------------------------------------------------------
void UIViewportLayer::UpdateLayout(const Size& viewSize)
{
}

//------------------------------------------------------------------------------
void UIViewportLayer::PostRender(DrawList* context, RefPtr<RenderTargetTexture>* primaryLayerTarget, RefPtr<RenderTargetTexture>* secondaryLayerTarget)
{
	for (auto& e : m_postEffects)
	{
		e->OnRender(context, *primaryLayerTarget, *secondaryLayerTarget);
		std::swap(*primaryLayerTarget, *secondaryLayerTarget);
	}
}


//==============================================================================
// PostEffect
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
void UILayoutLayer::Initialize(DrawingContext* cc)
{
	UIViewportLayer::Initialize();
	m_root = NewObject<UILayoutView>(UIContext::GetMainContext(), nullptr);	// TODO: コンテキスト変更とか

	m_drawingContext = cc;;// NewObject<DrawingContext>();

	// lighting disabled.
	// TODO: NewObject
	auto internalRenderer = RefPtr<detail::NonShadingRenderer>::MakeRef();
	internalRenderer->Initialize(detail::EngineDomain::GetGraphicsManager());
	m_internalRenderer = internalRenderer;
}

//------------------------------------------------------------------------------
UILayoutView* UILayoutLayer::GetLayoutView() const
{
	return m_root;
}

//------------------------------------------------------------------------------
void UILayoutLayer::UpdateLayout(const Size& viewSize)
{
	m_root->UpdateLayout(viewSize);
}

//------------------------------------------------------------------------------
void UILayoutLayer::Render()
{
	//m_drawingContext->BeginMakeElements();
	//m_drawingContext->SetBlendMode(BlendMode::Alpha);
	m_root->Render(m_drawingContext);
}

//------------------------------------------------------------------------------
void UILayoutLayer::ExecuteDrawListRendering(DrawList* parentDrawList, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer)
{
	//// TODO: float
	//Size viewPixelSize((float)renderTarget->GetWidth(), (float)renderTarget->GetHeight());

	//detail::CameraInfo cameraInfo;
	//cameraInfo.dataSourceId = reinterpret_cast<intptr_t>(this);
	//cameraInfo.viewPixelSize = viewPixelSize;
	//cameraInfo.viewPosition = Vector3::Zero;
	//cameraInfo.viewMatrix = Matrix::Identity;
	//cameraInfo.projMatrix = Matrix::MakePerspective2DLH(cameraInfo.viewPixelSize.width, cameraInfo.viewPixelSize.height, 0, 1);
	//cameraInfo.viewProjMatrix = cameraInfo.viewMatrix * cameraInfo.projMatrix;
	//cameraInfo.viewFrustum = ViewFrustum(cameraInfo.projMatrix);
	//cameraInfo.zSortDistanceBase = ZSortDistanceBase::NodeZ;
	//parentDrawList->RenderSubDrawList(
	//	m_drawingContext->GetDrawElementList(),
	//	cameraInfo,
	//	m_internalRenderer,
	//	renderTarget,
	//	depthBuffer);
	//m_drawingContext->EndFrame();
}

//------------------------------------------------------------------------------
void UILayoutLayer::OnRoutedEvent(UIEventArgs* e)
{
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
void PostEffect::Initialize()
{
}


LN_NAMESPACE_END
