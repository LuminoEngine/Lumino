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
#include "../UI/UIManager.h"
#include <Lumino/Graphics/GraphicsContext.h>
#include <Lumino/Graphics/Viewport.h>
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
const Size& ViewportLayer::GetViewportSize() const
{
	LN_CHECK_STATE(m_owner != nullptr);
	return m_owner->GetSize();
}

//------------------------------------------------------------------------------
void ViewportLayer::PostRender(GraphicsContext* graphicsContext, RenderTarget** primaryLayerTarget, RenderTarget** secondaryLayerTarget)
{
	for (ImageEffect* e : *m_imageEffects)
	{
		e->OnRender(graphicsContext, *primaryLayerTarget, *secondaryLayerTarget);
		std::swap(*primaryLayerTarget, *secondaryLayerTarget);
	}
}

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
Viewport* Viewport::GetMainWindowViewport()
{
	return detail::UIManager::GetInstance()->GetMainWindow()->GetViewport();
}

//------------------------------------------------------------------------------
Viewport::Viewport()
	: m_manager(nullptr)
	, m_renderTarget(nullptr)
	, m_viewportLayerList(RefPtr<ViewportLayerList>::MakeRef())
	, m_backgroundColor(ColorF::White)
	, m_primaryLayerTarget(nullptr)
	, m_secondaryLayerTarget(nullptr)
{
}

//------------------------------------------------------------------------------
Viewport::~Viewport()
{
	LN_SAFE_RELEASE(m_primaryLayerTarget);
	LN_SAFE_RELEASE(m_secondaryLayerTarget);
}

//------------------------------------------------------------------------------
void Viewport::Initialize(GraphicsManager* manager, RenderTarget* renderTarget)
{
	m_manager = manager;
	m_renderTarget = renderTarget;
	TryRemakeLayerTargets();
}

//------------------------------------------------------------------------------
const Size& Viewport::GetSize() const
{
	return m_renderTarget->GetSize();
}

//------------------------------------------------------------------------------
void Viewport::SetBackgroundColor(const Color32& color)
{
	m_backgroundColor = color;
}

//------------------------------------------------------------------------------
void Viewport::Render()
{
	TryRemakeLayerTargets();

	GraphicsContext* context = m_manager->GetGraphicsContext();
	context->SetRenderTarget(0, m_primaryLayerTarget);
	context->Clear(ClearFlags::All, m_backgroundColor, 1.0f, 0x00);

	// ZIndex でソート
	std::stable_sort(m_viewportLayerList->begin(), m_viewportLayerList->end(),
		[](const ViewportLayer* lhs, const ViewportLayer* rhs) { return lhs->GetZIndex() < rhs->GetZIndex(); });

	for (auto& layer : *m_viewportLayerList)
	{
		layer->Render(m_primaryLayerTarget);
		layer->PostRender(context, &m_primaryLayerTarget, &m_secondaryLayerTarget);
	}

	Matrix m;
	MakeViewBoxTransform(m_renderTarget->GetSize(), m_primaryLayerTarget->GetSize(), &m);
	context->Blt(m_primaryLayerTarget, m_renderTarget, m);
}

//------------------------------------------------------------------------------
void Viewport::TryRemakeLayerTargets()
{
	if (m_primaryLayerTarget == nullptr || m_renderTarget->GetSize() != m_primaryLayerTarget->GetSize())
	{
		LN_SAFE_RELEASE(m_primaryLayerTarget);
		LN_SAFE_RELEASE(m_secondaryLayerTarget);

		// TODO: できればこういうのは Resize 関数を作りたい。作り直したくない
		m_primaryLayerTarget = LN_NEW RenderTarget();
		m_primaryLayerTarget->CreateImpl(m_manager, m_renderTarget->GetSize(), 1, TextureFormat::R8G8B8X8);
		m_secondaryLayerTarget = LN_NEW RenderTarget();
		m_secondaryLayerTarget->CreateImpl(m_manager, m_renderTarget->GetSize(), 1, TextureFormat::R8G8B8X8);
	}
}

//------------------------------------------------------------------------------
void Viewport::MakeViewBoxTransform(const Size& dstSize, const Size& srcSize, Matrix* mat)
{
	//LSize backbufferSize = mWindowSize;
	//if (isLetterBox()) {
	//	backbufferSize = getSize();
	//}

	float sw = static_cast<float>(srcSize.width);   // 転送元
	float sh = static_cast<float>(srcSize.height);
	float dw = static_cast<float>(dstSize.width);	// 転送先
	float dh = static_cast<float>(dstSize.height);

	//if ( isFixedAspect() ) {
	//	sw = (float)mRequestedBackbufferSize.w;
	//	sh = (float)mRequestedBackbufferSize.h;
	//}

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

	//this->mViewDestRect.set((int)new_x, (int)new_y, (int)new_w, (int)new_h);

	//this->mScreenTransform.identity();
	//this->mScreenTransform.translation(-new_x, -new_y, 0);
	//this->mScreenTransform.scaling(1.0f / (new_w / sw), 1.0f / (new_h / sh), 1);

	*mat = Matrix::Identity;
	mat->Scale(new_w / sw, new_h / sh, 1.0f);
	mat->Translate(new_x, new_y, 0.0f);
}

LN_NAMESPACE_END
