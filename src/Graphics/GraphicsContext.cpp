/*
	[2016/4/8] Brush? fillStyle?

		- WPF
			Brush/Pen
		- Qt (QPainter)
			Brush/Pen



	[2016/1/29] Begin End Swap
		・End と Swap は1つにまとめてもいい。(OpenGL とか Direct2D はこんなかんじ)
		・Begin と End は RenderingContext や GraphicsContext に含めないほうがいいと思う。

		ゲームモードではこう。
			Engine::BeginRender();
			r1 = RenderingContext::GetContext();
			r1->Draw～
			sceneGraph->Render(r1);
			Engine::EndRender();

		ツールモードではこんな感じが自然。
			window->BeginRender();
			r1 = RenderingContext::GetContext();
			r1->Draw～
			sceneGraph->Render(r1);
			window->EndRender();


	全体構成

		RenderingContext	Renderer	
							PrimitiveRenderer
							SpriteRenderer		
		GraphicsContext		Renderer
							GeometryRenderer
							TextRenderer


		最上位は RenderingContext と GraphicsContext。これ以外はユーザーや他のモジュールに公開しない。
		RenderingContext と GraphicsContext が持つステートは排他。
		根っこでは1つの IRenderer を使うが、互いのステート変更は影響しない。

		まず、そもそもの問題は GraphicsContext と Renderer を同時に使いたいということにある。
		例えば、Scene の中に GUI を描きたいとか。
		そうでなくても、最初は GraphicsContext に Renderer の機能を持たせてしまおうとしていたが、
		それだとある状態でしか使えない関数があまりにも多くなるし、クラスの規模も大きくなる。
		つまり【ユーザーが気を付けなければならないことが増えてしまう。】

		Context は、IRenderer に設定できる全ての設定を保持する。あとシェーダ。
		Context の Draw～ や Flush で、アクティブなコンテキストの切り替えを行う。
		このとき古いコンテキストは Flush し、新しいコンテキストのステートを全て適用する。
		切り替え通知は Context レベルに通知する。包含している各種 Renderer の Flush を呼ぶため。

		[2015/1/22]
			Renderer == RenderingContext ではないのか？RenderingContext は必要なのか？と思うかもしれない。
			Renderer にまとめてしまうと、GraphicsContext と Renderer が同レベルのクラスとなる。
			GraphicsContext は描画先のレンダリングターゲットをステートとして持たなければならないため、Renderer を包含する必要がある。
			Context の中に別の Context ができてしまうということ。非常に複雑。


	[2015/1/21] SceneGraph 向けに低レベル Renderer は公開する？
		
		そもそも各種 Renderer を隠しているのは、ユーザーの知らないところでステートが変わって
		原因のわかりにくい問題にならないようにするため。


	[2015/1/21] カレントの Shader を管理するのは誰？
		×GraphicsContext。
		×個々の Renderer に管理させるのはあまりよろしくない気がする。
		××PrimitiveRenderer はまぁ SetShader とかあってもいいと思うけど、
		××GeometryRenderer は WPF みたいにシステムによって必ず実行される Shader があって、
		×その後でユーザーの Shader が動くかもしれない。
		×（まぁ、ユーザー Shader の中で GeometryRenderer 用の関数を呼んでもらってもいいが）

		・・・でも、個々のRendereに任せてしまってもいいかも？
		Activate のとき GraphicsContext が面倒見る必要ないということだし。
		とりあえずこっちの方向で。


	[2015/1/11] PrimitiveRenderer とはなぜ区別する？
		PrimitiveRenderer は単純な形状を高速・大量に描画するために使用する。

	[2015/1/3] 面張りについて
		とりあえずねじれパスは想定しない。これは軽量な組み込みモードとする。
		本当にちゃんとしたのが使いたい要望あれば、ExpandFill だけを poly2tri で置き換えるモードを作る。

	[2015/12/3]
		Arc
		Polygon
		PolyLine
		Ellipse
		Pie
		Point
		Rect
		RoundRect
		Text
		Image
		Path

		・Path 以外は汎用的な面張りを行うよりも自前で頂点作ってしまったほうが高速。
		・ストロークとアンチエイリアスは面張りしなければならないので、外周の頂点は素早く参照できるようにする必要がある。

		アウトラインのポイントリスト作成
		(Path以外)頂点リスト作成
		(Path)面張り・頂点リスト作成
		Fill頂点バッファ作成
		描画
		Stroke頂点バッファ作成
		描画

	[2015/12/2] タイリング描画について
		デフォルトでは有効にしない。シェーダは分ける。
		タイル配置するブラシが指定された場合、描画モードを切り替える。
		常に有効にするにはあまりにも複雑すぎる。


	[2015/12/2] コマンドのキャッシュについて
		同一ステート間の Primitive 描画コマンドは一括で実行できる。
		まとめてコマンドリストに送るといいのだが、バッファは誰が持つの？
		→ ステートの切り替えは割りと頻繁だと思われる。
		むしろ new の数が増えてしまうかもしれない。
		memcpy でコマンドリストの一時バッファへコピーするほうが全体的に見てパフォーマンス良いかも。
		sizeof(Matrix) を1コマンド分の平均サイズとして 10000 子 memcpy するときは 30us くらいかかる。
		1000 個なら 3us。
		実際にはこんなに使わないだろうし、memcpy で問題ないはず。


	[2015/12/2] クラス名
		しばらく間空いて、やっぱり GeometryRenderer のほうがいいと思う。
		このライブラリはかなり低レベルなAPIも公開する。
		そうなってくると DirectX11 の RenderingContext の意味が強くなってくる。
		高レベルってことで差別化するなら GeometryRenderer。
		


	名前の元ネタはこのあたりから。
	https://developer.mozilla.org/ja/docs/Web/API/RenderingContext

	このモジュールは Scene と GUI で共有されるが、
	GeometryRenderer は Scene よりも GUI 寄り。
	ライブラリとしては Scene の方が意味が大きい。Draw より Render かな、と。


	Scene では、あらかじめ SceneNode のシェーダをユーザーシェーダとして
	設定したものを OnRender() に渡す。



	ユーザーシェーダに要求する頂点宣言は、
	・Pos0
	・TexUV0
	・Color0

	ユーザーシェーダに要求するパラメータは、
	・VIEWPORTPIXELSIZE
	・LNRC_TONE
		UI_TONE、NODE_TONE とかのほうがいいかも？
	↑Tone の計算はユーザーシェーダで色が決まった後に行わなければならない。
	（乗算だけで表現できないので頂点宣言だけでは不足）
	#include "lumino.fx" とかして、ピクセルシェーダで LNUI_PostPixelShader みたいな関数を
	ユーザーに呼んでもらう必要がある。

	描画方式は3つ。
	・プリミティブのみ
	・組み込みシェーダを使う
	・ユーザーシェーダを使う

	RenderingContext
	BasicRenderingContext
	UIRenderingContext
	SceneNodeRenderingContext
*/

#include "Internal.h"
#include "GraphicsManager.h"
#include <Lumino/Graphics/GraphicsContext.h>
#include <Lumino/Graphics/SpriteRenderer.h>
#include "Text/TextRenderer.h"
#include "PrimitiveRenderer.h"
#include "GeometryRenderer.h"
#include "RendererImpl.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// GraphicsContext
/*		GraphicsContext は JavaFX のクラス。
*/
//==============================================================================

//------------------------------------------------------------------------------
GraphicsContext* GraphicsContext::GetContext()
{
	return GraphicsManager::GetInstance()->GetGraphicsContext();
}

//------------------------------------------------------------------------------
GraphicsContext::GraphicsContext()
	:/* m_currentRenderer(RendererType::None)
	, */m_spriteRenderer(nullptr)
	, m_textRenderer(nullptr)
{
}

//------------------------------------------------------------------------------
GraphicsContext::~GraphicsContext()
{
	LN_SAFE_RELEASE(m_textRenderer);
	LN_SAFE_RELEASE(m_spriteRenderer);
	LN_SAFE_RELEASE(m_geometryRenderer);
}

//------------------------------------------------------------------------------
void GraphicsContext::Initialize(GraphicsManager* manager)
{
	IContext::Initialize(manager);

	Renderer = manager->GetRenderer();
	m_geometryRenderer = LN_NEW detail::GeometryRenderer();
	m_geometryRenderer->Initialize(manager);
	m_spriteRenderer = LN_NEW SpriteRenderer(manager, 2048);	// TODO:

	m_textRenderer = LN_NEW detail::TextRenderer();
	m_textRenderer->Initialize(manager);
}

//------------------------------------------------------------------------------
void GraphicsContext::Set2DRenderingMode(float minZ, float maxZ)
{
	const Size& size = Renderer->GetRenderTarget(0)->GetSize();
	Matrix proj = Matrix::MakePerspective2DLH((float)size.width, (float)size.height, minZ, maxZ);
	m_geometryRenderer->SetViewProjection(Matrix::Identity, proj, size);
	m_spriteRenderer->SetViewProjMatrix(Matrix::Identity, proj);
	m_textRenderer->SetViewProjMatrix(proj);
	m_textRenderer->SetViewPixelSize(size);
	// ↑TODO: OnStateFlushRequested に持っていったほうがいい？
}

//------------------------------------------------------------------------------
void GraphicsContext::SetTransform(const Matrix& matrix)
{
	m_geometryRenderer->SetTransform(matrix);
	m_textRenderer->SetTransform(matrix);
}

//------------------------------------------------------------------------------
void GraphicsContext::SetOpacity(float opacity)
{
	m_geometryRenderer->SetOpacity(opacity);
}

//------------------------------------------------------------------------------
void GraphicsContext::SetBrush(Brush* brush)
{
	m_geometryRenderer->SetBrush(brush);
}

//------------------------------------------------------------------------------
void GraphicsContext::SetFont(Font* font)
{
	m_textRenderer->SetFont(font);
}

//------------------------------------------------------------------------------
void GraphicsContext::Clear(ClearFlags flags, const ColorF& color, float z, uint8_t stencil)
{
	OnDrawing(GetCommonRenderer());
	GetCommonRenderer()->Clear(flags, color, z, stencil);
}

//------------------------------------------------------------------------------
void GraphicsContext::MoveTo(const Vector3& point, const ColorF& color)
{
	OnDrawing(m_geometryRenderer);
	m_geometryRenderer->MoveTo(point, color);
}

//------------------------------------------------------------------------------
void GraphicsContext::LineTo(const Vector3& point, const ColorF& color)
{
	OnDrawing(m_geometryRenderer);
	m_geometryRenderer->LineTo(point, color);
}

//------------------------------------------------------------------------------
void GraphicsContext::BezierCurveTo(const Vector3& cp1, const Vector3& cp2, const Vector3& endPt, const ColorF& color)
{
	OnDrawing(m_geometryRenderer);
	m_geometryRenderer->BezierCurveTo(cp1, cp2, endPt, color);
}

//------------------------------------------------------------------------------
void GraphicsContext::ClosePath()
{
	OnDrawing(m_geometryRenderer);
	m_geometryRenderer->ClosePath();
}

//------------------------------------------------------------------------------
void GraphicsContext::DrawPoint(const Vector3& point, const ColorF& color)
{
	OnDrawing(m_geometryRenderer);
	m_geometryRenderer->DrawPoint(point, color);
}

//------------------------------------------------------------------------------
void GraphicsContext::DrawTriangle(const Vector3& p1, const ColorF& p1Color, const Vector3& p2, const ColorF& p2Color, const Vector3& p3, const ColorF& p3Color)
{
	OnDrawing(m_geometryRenderer);
	m_geometryRenderer->DrawTriangle(p1, p1Color, p2, p2Color, p3, p3Color);
}

//------------------------------------------------------------------------------
void GraphicsContext::DrawRectangle(const RectF& rect, const Color& color)
{
	OnDrawing(m_geometryRenderer);
	m_geometryRenderer->DrawRectangle(rect, color);
}

//------------------------------------------------------------------------------
void GraphicsContext::DrawEllipse(const Vector3& center, const Vector2& radius)
{
	OnDrawing(m_geometryRenderer);
	m_geometryRenderer->DrawEllipse(center, radius);
}

//------------------------------------------------------------------------------
void GraphicsContext::DrawTexture(const RectF& rect, Texture* texture, const Rect& srcRect, const ColorF& color)
{
	OnDrawing(m_geometryRenderer);
	m_geometryRenderer->DrawTexture(rect, texture, srcRect, color);
}

//------------------------------------------------------------------------------
void GraphicsContext::DrawText(const StringRef& text, const PointF& position)
{
	OnDrawing(m_textRenderer);
	m_textRenderer->DrawString(text.GetBegin(), text.GetLength(), position);
}

//------------------------------------------------------------------------------
void GraphicsContext::DrawText(const StringRef& text, const RectF& rect, StringFormatFlags flags)
{
	OnDrawing(m_textRenderer);
	m_textRenderer->DrawString(text.GetBegin(), text.GetLength(), rect, flags);
}

////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
//void GraphicsContext::Flush()
//{
//	m_geometryRenderer->Flush();
//	m_spriteRenderer->Flush();
//	m_textRenderer->Flush();
//}

//------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
//detail::GeometryRenderer* GraphicsContext::BeginDrawingContext()
//{
//	if (m_currentRenderer != RendererType::GeometryRenderer)
//	{
//		Flush();
//		m_currentRenderer = RendererType::GeometryRenderer;
//	}
//	return m_geometryRenderer;
//}
//
//------------------------------------------------------------------------------
SpriteRenderer* GraphicsContext::BeginSpriteRendering()
{
	//if (m_currentRenderer != RendererType::GeometryRenderer)
	//{
		Flush();
	//	m_currentRenderer = RendererType::SpriteRenderer;
	//}
	return m_spriteRenderer;
}

////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
//void GraphicsContext::TryChangeRenderingClass(RendererType dc)
//{
//	if (dc != m_currentRenderer)
//	{
//		Flush();
//		m_currentRenderer = dc;
//	}
//

//}

//------------------------------------------------------------------------------
void GraphicsContext::OnStateFlushRequested()
{
	IContext::OnStateFlushRequested();

}

LN_NAMESPACE_END
