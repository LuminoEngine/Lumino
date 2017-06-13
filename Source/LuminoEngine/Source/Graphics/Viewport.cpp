
#include "Internal.h"
#include "GraphicsManager.h"
#include "RendererImpl.h"
#include "../UI/UIManager.h"
#include <Lumino/Graphics/Viewport.h>
#include <Lumino/Rendering/Rendering.h>
#include <Lumino/Scene/Camera.h>			// for make proj matrix
#include <Lumino/UI/UIFrameWindow.h>

LN_NAMESPACE_BEGIN
#if 0
//==============================================================================
// ViewportLayer
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(ViewportLayer, Object);

//------------------------------------------------------------------------------
ViewportLayer::ViewportLayer()
	: m_owner(nullptr)
	, m_imageEffects(RefPtr<ImageEffectList>::makeRef())
	, m_zIndex(0)
{
}

//------------------------------------------------------------------------------
ViewportLayer::~ViewportLayer()
{
}

//------------------------------------------------------------------------------
void ViewportLayer::UpdateTransform(const Size& viewSize)
{
	m_size = viewSize;
}

////------------------------------------------------------------------------------
//void ViewportLayer::PreRender()
//{
//}

////------------------------------------------------------------------------------
//void ViewportLayer::AddRenderingPass(detail::RenderingPass2* pass)
//{
//	m_renderingPasses.Add(pass);
//}

//------------------------------------------------------------------------------
const Size& ViewportLayer::GetSize() const
{
	return m_size;
}

//------------------------------------------------------------------------------
bool ViewportLayer::OnPlatformEvent(const PlatformEventArgs& e)
{
	return false;
}

//------------------------------------------------------------------------------
void ViewportLayer::PostRender(DrawList* context, RefPtr<RenderTargetTexture>* primaryLayerTarget, RefPtr<RenderTargetTexture>* secondaryLayerTarget)
{
	for (ImageEffect* e : *m_imageEffects)
	{
		e->OnRender(context, *primaryLayerTarget, *secondaryLayerTarget);
		std::swap(*primaryLayerTarget, *secondaryLayerTarget);
	}
}

//------------------------------------------------------------------------------
void ViewportLayer::ExecuteDrawListRendering(DrawList* parentDrawList, RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer)
{
}

//------------------------------------------------------------------------------
//void ViewportLayer::OnRenderDrawElementList(RenderTarget* renderTarget, DepthBuffer* depthBuffer, detail::RenderingPass2* pass)
//{
//}


//==============================================================================
// ViewportLayerList
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(ViewportLayerList, ObjectList<ViewportLayer*>);

//------------------------------------------------------------------------------
ViewportLayerList::ViewportLayerList()
{
}

//------------------------------------------------------------------------------
ViewportLayerList::~ViewportLayerList()
{
}


//==============================================================================
// Viewport
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(Viewport, Object);

//------------------------------------------------------------------------------
Viewport::Viewport()
	: m_manager(nullptr)
	, m_placement(ViewportPlacement::Stretch)
	, m_viewportLayerList(RefPtr<ViewportLayerList>::makeRef())
	, m_backgroundColor(Color::White)
	, m_primaryLayerTarget(nullptr)
	, m_secondaryLayerTarget(nullptr)
	, m_depthBuffer()
{
}

//------------------------------------------------------------------------------
Viewport::~Viewport()
{
}

//------------------------------------------------------------------------------
void Viewport::initialize(detail::GraphicsManager* manager, const SizeI& viewSize)
{
	m_manager = manager;
	//m_renderTarget = renderTarget;
	//m_size.Set((float)m_renderTarget->getWidth(), (float)m_renderTarget->getHeight());

	m_renderer = RefPtr<DrawList>::makeRef();
	m_renderer->initialize(manager);

	auto internalRenderer = RefPtr<detail::NonShadingRenderer>::makeRef();
	internalRenderer->initialize(manager);
	m_internalRenderer = internalRenderer;

	//m_pass = RefPtr<detail::RenderingPass2>::MakeRef();
	//m_pass->initialize(manager);

	TryRemakeLayerTargets(viewSize);
}

//------------------------------------------------------------------------------
const Size& Viewport::GetSize() const
{
	return m_size;
}

//------------------------------------------------------------------------------
void Viewport::SetBackgroundColor(const Color& color)
{
	m_backgroundColor = color;
}

//------------------------------------------------------------------------------
void Viewport::SetPlacement(ViewportPlacement placement)
{
	m_placement = placement;
}

//------------------------------------------------------------------------------
void Viewport::AddViewportLayer(ViewportLayer* layer)
{
	m_viewportLayerList->Add(layer);
	layer->SetOwner(this);
}

//------------------------------------------------------------------------------
void Viewport::RemoveViewportLayer(ViewportLayer* layer)
{
	if (m_viewportLayerList->Remove(RefPtr<ViewportLayer>(layer)))
	{
		layer->SetOwner(nullptr);
	}
}

//------------------------------------------------------------------------------
void Viewport::UpdateLayersTransform(const Size& viewSize)
{
	m_size = viewSize;

	for (ViewportLayer* layer : *m_viewportLayerList)
	{
		layer->UpdateTransform(viewSize);
	}
}

//------------------------------------------------------------------------------
bool Viewport::DoPlatformEvent(const PlatformEventArgs& e)
{
	for (ViewportLayer* layer : *m_viewportLayerList)
	{
		if (layer->OnPlatformEvent(e)) return true;
	}
	return false;
}

//------------------------------------------------------------------------------
void Viewport::Render(DrawList* parentDrawList, const SizeI& targetSize)
{
	auto* oldRenderTarget = parentDrawList->GetRenderTarget(0);
	auto* oldDepthBuffer = parentDrawList->GetDepthBuffer();

	parentDrawList->SetRenderTarget(0, m_primaryLayerTarget);
	parentDrawList->SetDepthBuffer(m_depthBuffer);
	parentDrawList->Clear(ClearFlags::All, m_backgroundColor, 1.0f, 0x00);

	// ZIndex でソート
	std::stable_sort(m_viewportLayerList->begin(), m_viewportLayerList->end(),
		[](const ViewportLayer* lhs, const ViewportLayer* rhs) { return lhs->GetZIndex() < rhs->GetZIndex(); });
	
	for (ViewportLayer* layer : *m_viewportLayerList)
	{
		layer->Render();
	}


	// 全てのレイヤーの描画リストを実行し m_primaryLayerTarget へ書き込む
	for (ViewportLayer* layer : *m_viewportLayerList)
	{
		layer->ExecuteDrawListRendering(parentDrawList, m_primaryLayerTarget, m_depthBuffer);

		// TODO: Posteffect
		//BeginBlitRenderer();
		//layer->PostRender(m_renderer, &m_primaryLayerTarget, &m_secondaryLayerTarget);
		//FlushBlitRenderer(renderTarget);
	}


	Matrix viewBoxTransform = Matrix::Identity;
	if (m_placement == ViewportPlacement::ViewBox)
	{
		MakeViewBoxTransform(targetSize, m_primaryLayerTarget->GetSize(), &viewBoxTransform);
	}

	//BeginBlitRenderer();
	//m_renderer->blit(m_primaryLayerTarget, viewBoxTransform);
	//FlushBlitRenderer(renderTarget);


	parentDrawList->SetRenderTarget(0, oldRenderTarget);
	parentDrawList->SetDepthBuffer(oldDepthBuffer);
	parentDrawList->Blit(m_primaryLayerTarget, viewBoxTransform);

	// TODO: 暫定。blit の中で深度書き込みしないようにしてほしいかも。
	parentDrawList->Clear(ClearFlags::Depth, Color());

}

//------------------------------------------------------------------------------
void Viewport::PresentRenderingContexts()
{

	TryRemakeLayerTargets(SizeI(m_size.width, m_size.height));
}

//------------------------------------------------------------------------------
void Viewport::TryRemakeLayerTargets(const SizeI& ownerViewPixelSize)
{
	bool create = false;
	SizeI newSize;

	// 初回、まだ作成されていなければ作りたい
	if (m_primaryLayerTarget == nullptr)
	{
		create = true;
		newSize = ownerViewPixelSize;//SizeI((float)m_size.width, (float)m_size.height);
	}

	// 自動リサイズONで、描画先とサイズが異なるなら再作成
	if (m_placement == ViewportPlacement::AutoResize/* && ownerViewPixelSize != m_primaryLayerTarget->GetSize()*/)
	{
		if (m_primaryLayerTarget != nullptr && ownerViewPixelSize != m_primaryLayerTarget->GetSize())
		{
			create = true;
		}

		newSize = ownerViewPixelSize;
	}

	if (create)
	{
		// RenderTargetTexture
		// TODO: できればこういうのは Resize 関数を作りたい。作り直したくない
		// TODO: というか UE4 みたいにキャッシュしたい
		m_primaryLayerTarget = RefPtr<RenderTargetTexture>::makeRef();
		m_primaryLayerTarget->CreateImpl(GetManager(), newSize, 1, TextureFormat::R8G8B8X8);
		m_secondaryLayerTarget = RefPtr<RenderTargetTexture>::makeRef();
		m_secondaryLayerTarget->CreateImpl(GetManager(), newSize, 1, TextureFormat::R8G8B8X8);

		// DepthBuffer
		m_depthBuffer = RefPtr<DepthBuffer>::makeRef();
		m_depthBuffer->CreateImpl(GetManager(), newSize, TextureFormat::D24S8);
	}
}

//------------------------------------------------------------------------------
void Viewport::MakeViewBoxTransform(const SizeI& dstSize, const SizeI& srcSize, Matrix* mat)
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
	*mat = Matrix::Identity;
	mat->Scale(new_w / sw, new_h / sh, 1.0f);
	mat->Translate(new_x, new_y, 0.0f);
#else	// screen coord based
	*mat = Matrix::Identity;
	mat->Scale(new_w / dw, new_h / dh, 1.0f);
#endif
}

//------------------------------------------------------------------------------
void Viewport::BeginBlitRenderer()
{
	m_renderer->BeginMakeElements();
	m_renderer->SetRenderTarget(0, nullptr);
	m_renderer->SetDepthBuffer(nullptr);
	m_renderer->SetDepthTestEnabled(false);
	m_renderer->SetDepthWriteEnabled(false);
}

//------------------------------------------------------------------------------
void Viewport::FlushBlitRenderer(RenderTargetTexture* renderTarget)
{
	Size targetSize((float)renderTarget->GetWidth(), (float)renderTarget->GetHeight());

	detail::CameraInfo cameraInfo;
	cameraInfo.dataSourceId = reinterpret_cast<intptr_t>(this);
	cameraInfo.viewPosition = Vector3::Zero;
	cameraInfo.viewPixelSize = targetSize;
	cameraInfo.viewMatrix = Matrix::Identity;
	//Camera::Perspective2DLH(cameraInfo.viewPixelSize.width, cameraInfo.viewPixelSize.height, 0, 1, &cameraInfo.projMatrix);
	cameraInfo.projMatrix = Matrix::MakePerspective2DLH(cameraInfo.viewPixelSize.width, cameraInfo.viewPixelSize.height, 0, 1);
	cameraInfo.viewProjMatrix = cameraInfo.viewMatrix * cameraInfo.projMatrix;
	cameraInfo.viewFrustum = ViewFrustum(cameraInfo.projMatrix);
	cameraInfo.zSortDistanceBase = ZSortDistanceBase::NodeZ;

	m_internalRenderer->Render(
		m_renderer->GetDrawElementList(),
		cameraInfo,
		renderTarget,
		m_depthBuffer);		// TODO: バックバッファサイズ
	m_renderer->EndFrame();
}

////==============================================================================
//// MainViewport
////==============================================================================
//
////------------------------------------------------------------------------------
//ViewportLayer* MainViewport::getDefault2DLayer()
//{
//
//}
//
////------------------------------------------------------------------------------
//ViewportLayer* MainViewport::getDefault3DLayer()
//{
//
//}
#endif

LN_NAMESPACE_END
