/*

	- Viewport は Layer の塊。
	- Layer は Camera や UI の描画先となるレンダーターゲットと考える。


	Viewport
	- Viewport は必ずレンダーターゲットを持つ。

	RenderingUnit (RenderingCompositeItem)
	- Camera や UIManager? のベースクラスとなる。
	- RenderingUnit は Viewport の子。Camera などに「この RT に書いてね」を伝えるのが仕事。ポストエフェクトはおまけ。別のクラスに分けても良い。
	- Render を呼べばポストエフェクト込みで描画される。
	- 書き込み先のレンダーターゲットを持つ。他の RenderingUnit と共有しているバックバッファかもしれないし、スナップショットを保存する独立したものかもしれない。
	- ClearMode の動作もここで行う。
	- ポストエフェクト用の RT のキャッシュも、RenderingUnit 単位となる。RenderingUnit をまたぐ場合は使いまわしてよい。
	- 優先度は関係なし。レイヤー順で描画する。

	RT のキャッシュ

	ケース：マリカー

	ケース：SnapShotCamera が通常の Camera に切り替わることはある？
		監視カメラの切り替えとか？
		→ SnapShot の RT を Sprite で 2D ビューに表示しましょう。


	↑	まとめ

	Viewport は「スクリーンに表示するためのレンダーターゲット」と考える。
	レンダリング開始前に Viewport を集計し、そのフレームで、主にポストエフェクトに使われる
	レンダーターゲットをキャッシュするのも仕事。

	このレンダーターゲットは「ある Viewport をオーナーとする自動リサイズするレンダーターゲット」とすれば
	Scene の Camera とか ImageEffect とかからは独立して使える。 Graphics に入れておいてもいい。


	- 複数のカメラのレンダリングを1つのビューポートにまとめたい。
	- ポストエフェクトはカメラに割り当てたい。取り外しも気軽に。
	- 複数カメラが1つのビューポートに共有して書き込む場合、後のカメラのポストエフェクトは前のカメラの描画結果に影響する。
		しないようにするのは難しい。ポストエフェクト開始前は基本的にクリアが走る。
		もししないようにしたいならレンダーターゲットを分けてもらう。

	-	クリアの有無や背景色は、Viewport と Camera それぞれが持つ。
		Camera のクリアは、Viewport とは関係ない、Scene のスナップショットを撮る時に使うべき。デフォルトは OFF。
		単にウィンドウ背景色を変えたいとかのときは Viewport の背景色を変更する。

	-	カメラには、ポストエフェクト開始後、Sceneのレンダリングを始める前に、
		カメラに描画先として指定されているレンダーターゲットの内容を現在のレンダーターゲットに転送する。
		これで、すでに描画済みのシーンに上書きする形でポストエフェクトを適用できる。
		オプションにしてもいいかも。

	- カメラの描画順の指定はどうする？
		Viewport と Camera は独立させたい。
		Viewport はあくまで「ここに書いてねレンダリングターゲット」を公開するだけ。
		これを Camera にセットしたり、あるいは UI が直接書いたりする。
		Camera の描画順は Scene 側で制御する必要がある。

		Unity は 優先度(Camera.Depth) UE4 はサポートされてない？https://answers.unrealengine.com/questions/53233/render-in-offset-order.html

		Viewport にぶら下がるレイヤーで管理する？
		→ でも、Viewport から独立している SceneSnapShot とかの優先度はどうやって指定する？
			→ 同時には不可能な気がする。
				Scene 内と、それの上位にいる Viewport+Layer？
				Layer → 個々の優先度 の順でソートしてはどうか？
				優先度は SnapShot 用カメラだけに付けてもいいかも。ウィンドウに描画するのは基本的に一番最後だし。


	ポストエフェクトの描画先を自動リサイズしたい
	→ サイズをキーとしてキャッシュしたい。

	- Viewport に Layer を持たせるなら、その Layer が1つのポストエフェクトを含む描画のまとまりとなる。

	- UE4 の FRCPassPostProcessAA::Process とか参考になるかも。

	- UE4 みたいにすると、1つのポストエフェクトオブジェクトは複数のカメラなどのビューから参照される。
		→ 固有の状態を持つのはよろしくない。ウィンドウサイズとか。



*/
#include "Internal.h"
#include "GraphicsManager.h"
#include "RendererImpl.h"
#include "../UI/UIManager.h"
#include <Lumino/Graphics/Viewport.h>
#include <Lumino/Graphics/Rendering.h>
#include <Lumino/Scene/Camera.h>			// for make proj matrix
#include <Lumino/UI/UIFrameWindow.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// ViewportLayer
//==============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(ViewportLayer, Object);

//------------------------------------------------------------------------------
ViewportLayer::ViewportLayer()
	: m_owner(nullptr)
	, m_imageEffects(RefPtr<ImageEffectList>::MakeRef())
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
void ViewportLayer::PreRender(const SizeI& ownerViewPixelSize)
{
	//bool create = false;
	//SizeI newSize;

	//// 初回、まだ作成されていなければ作りたい
	//if (m_primaryLayerTarget == nullptr)
	//{
	//	create = true;
	//	newSize = SizeI((float)m_size.width, (float)m_size.height);
	//}

	//// 自動リサイズONで、描画先とサイズが異なるなら再作成
	//if (m_placement == ViewportLayerPlacement::AutoResize && ownerViewPixelSize != m_primaryLayerTarget->GetSize())
	//{
	//	newSize = ownerViewPixelSize;
	//	create = true;
	//}

	//if (create)
	//{
	//	// RenderTargetTexture
	//	// TODO: できればこういうのは Resize 関数を作りたい。作り直したくない
	//	// TODO: というか UE4 みたいにキャッシュしたい
	//	m_primaryLayerTarget = RefPtr<RenderTargetTexture>::MakeRef();
	//	m_primaryLayerTarget->CreateImpl(m_owner->GetManager(), newSize, 1, TextureFormat::R8G8B8X8);
	//	m_secondaryLayerTarget = RefPtr<RenderTargetTexture>::MakeRef();
	//	m_secondaryLayerTarget->CreateImpl(m_owner->GetManager(), newSize, 1, TextureFormat::R8G8B8X8);

	//	// DepthBuffer
	//	m_depthBuffer = RefPtr<DepthBuffer>::MakeRef();
	//	m_depthBuffer->CreateImpl(m_owner->GetManager(), newSize, TextureFormat::D24S8);
	//}
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
void ViewportLayer::ExecuteDrawListRendering(RenderTargetTexture* renderTarget, DepthBuffer* depthBuffer)
{
	//for (detail::RenderingPass2* pass : m_renderingPasses)
	//{
	//	OnRenderDrawElementList(renderTarget, depthBuffer, pass);
	//}
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
	, m_viewportLayerList(RefPtr<ViewportLayerList>::MakeRef())
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
void Viewport::Initialize(detail::GraphicsManager* manager)
{
	m_manager = manager;
	//m_renderTarget = renderTarget;
	//m_size.Set((float)m_renderTarget->GetWidth(), (float)m_renderTarget->GetHeight());

	m_renderer = RefPtr<DrawList>::MakeRef();
	m_renderer->Initialize(manager);

	auto internalRenderer = RefPtr<detail::NonShadingRenderer>::MakeRef();
	internalRenderer->Initialize(manager);
	m_internalRenderer = internalRenderer;

	//m_pass = RefPtr<detail::RenderingPass2>::MakeRef();
	//m_pass->Initialize(manager);

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
void Viewport::BeginRender(Details::Renderer* renderer, const SizeI& viewSize)
{
	TryRemakeLayerTargets(viewSize);

	for (ViewportLayer* layer : *m_viewportLayerList)
	{
		layer->PreRender(viewSize);
	}
}

//------------------------------------------------------------------------------
void Viewport::Render(Details::Renderer* renderer)
{
	renderer->SetRenderTarget(0, m_primaryLayerTarget);
	renderer->SetDepthBuffer(m_depthBuffer);
	renderer->Clear(ClearFlags::All, m_backgroundColor, 1.0f, 0x00);

	// ZIndex でソート
	std::stable_sort(m_viewportLayerList->begin(), m_viewportLayerList->end(),
		[](const ViewportLayer* lhs, const ViewportLayer* rhs) { return lhs->GetZIndex() < rhs->GetZIndex(); });
	
	for (ViewportLayer* layer : *m_viewportLayerList)
	{
		layer->Render();
	}
}

//------------------------------------------------------------------------------
void Viewport::EndRender(Details::Renderer* renderer, RenderTargetTexture* renderTarget)
{

	// 全てのレイヤーの描画リストを実行し m_primaryLayerTarget へ書き込む
	for (ViewportLayer* layer : *m_viewportLayerList)
	{
		layer->ExecuteDrawListRendering(m_primaryLayerTarget, m_depthBuffer);
		
		BeginBlitRenderer();
		layer->PostRender(m_renderer, &m_primaryLayerTarget, &m_secondaryLayerTarget);
		FlushBlitRenderer(renderTarget);
	}

	Matrix viewBoxTransform = Matrix::Identity;
	if (m_placement == ViewportPlacement::ViewBox)
	{
		MakeViewBoxTransform(renderTarget->GetSize(), m_primaryLayerTarget->GetSize(), &viewBoxTransform);
	}

	BeginBlitRenderer();
	m_renderer->Blit(m_primaryLayerTarget, viewBoxTransform);
	FlushBlitRenderer(renderTarget);
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
		newSize = SizeI((float)m_size.width, (float)m_size.height);
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
		m_primaryLayerTarget = RefPtr<RenderTargetTexture>::MakeRef();
		m_primaryLayerTarget->CreateImpl(GetManager(), newSize, 1, TextureFormat::R8G8B8X8);
		m_secondaryLayerTarget = RefPtr<RenderTargetTexture>::MakeRef();
		m_secondaryLayerTarget->CreateImpl(GetManager(), newSize, 1, TextureFormat::R8G8B8X8);

		// DepthBuffer
		m_depthBuffer = RefPtr<DepthBuffer>::MakeRef();
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
//ViewportLayer* MainViewport::GetDefault2DLayer()
//{
//
//}
//
////------------------------------------------------------------------------------
//ViewportLayer* MainViewport::GetDefault3DLayer()
//{
//
//}

LN_NAMESPACE_END
