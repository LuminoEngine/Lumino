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
#include <Lumino/Graphics/RenderingContext.h>
#include <Lumino/Graphics/Viewport.h>

LN_NAMESPACE_BEGIN

//=============================================================================
// ViewportLayer
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ViewportLayer::ViewportLayer()
	: m_owner(nullptr)
	, m_imageEffects(RefPtr<ImageEffectList>::MakeRef())
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ViewportLayer::~ViewportLayer()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const Size& ViewportLayer::GetViewportSize() const
{
	LN_CHECK_STATE_RETURNV(m_owner != nullptr, Size::Zero);
	return m_owner->GetSize();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ViewportLayer::PostRender(RenderingContext2* renderingContext, RenderTarget** primaryLayerTarget, RenderTarget** secondaryLayerTarget)
{
	for (ImageEffect* e : *m_imageEffects)
	{
		e->OnRender(renderingContext, *primaryLayerTarget, *secondaryLayerTarget);
		std::swap(*primaryLayerTarget, *secondaryLayerTarget);
	}
}

//=============================================================================
// Viewport
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Viewport* Viewport::GetMainWindowViewport()
{
	return GraphicsManager::GetInstance()->GetMainViewport();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Viewport::Viewport()
	: m_manager(nullptr)
	, m_renderTarget(nullptr)
	, m_backgroundColor(ColorF::White)
	, m_primaryLayerTarget(nullptr)
	, m_secondaryLayerTarget(nullptr)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Viewport::~Viewport()
{
	LN_SAFE_RELEASE(m_primaryLayerTarget);
	LN_SAFE_RELEASE(m_secondaryLayerTarget);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Viewport::Initialize(GraphicsManager* manager, RenderTarget* renderTarget)
{
	m_manager = manager;
	m_renderTarget = renderTarget;
	TryRemakeLayerTargets();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const Size& Viewport::GetSize() const
{
	return m_renderTarget->GetSize();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Viewport::SetBackgroundColor(const Color& color)
{
	m_backgroundColor = color;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Viewport::Render()
{
	TryRemakeLayerTargets();

	RenderingContext2* context = m_manager->GetRenderingContext();
	m_manager->GetRenderingContext()->SetRenderTarget(0, m_primaryLayerTarget);
	context->Clear(ClearFlags::All, m_backgroundColor, 1.0f, 0x00);

	for (auto& layer : m_viewportLayerList)
	{
		layer->Render(m_primaryLayerTarget);
		layer->PostRender(context, &m_primaryLayerTarget, &m_secondaryLayerTarget);
	}

	context->Blt(m_primaryLayerTarget, m_renderTarget);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Viewport::TryRemakeLayerTargets()
{
	if (m_primaryLayerTarget == nullptr || m_renderTarget->GetSize() != m_primaryLayerTarget->GetSize())
	{
		LN_SAFE_RELEASE(m_primaryLayerTarget);
		LN_SAFE_RELEASE(m_secondaryLayerTarget);

		m_primaryLayerTarget = LN_NEW RenderTarget();
		m_primaryLayerTarget->CreateImpl(m_manager, m_renderTarget->GetSize(), 1, TextureFormat_R8G8B8X8);
		m_secondaryLayerTarget = LN_NEW RenderTarget();
		m_secondaryLayerTarget->CreateImpl(m_manager, m_renderTarget->GetSize(), 1, TextureFormat_R8G8B8X8);
	}
}


LN_NAMESPACE_END
