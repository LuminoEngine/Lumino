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
#include <Lumino/Graphics/Shader.h>
#include "Text/TextRenderer.h"
#include "PrimitiveRenderer.h"
#include "GeometryRenderer.h"
#include "RendererImpl.h"
#include "FrameRectRenderer.h"
#include "MeshRendererProxy.h"

LN_NAMESPACE_BEGIN


namespace detail
{
//==============================================================================
// RenderStateBlock
//==============================================================================

//------------------------------------------------------------------------------
RenderStateBlock::RenderStateBlock(GraphicsContext* context)
	: m_context(context)
	, m_renderState(context->m_state.renderState)
	, m_depthStencilState(context->m_state.depthStencilState)
{
}

//------------------------------------------------------------------------------
RenderStateBlock::~RenderStateBlock()
{
	Apply(m_context);
}

//------------------------------------------------------------------------------
void RenderStateBlock::Apply(GraphicsContext* context)
{
	context->SetRenderState(m_renderState);
	context->SetDepthStencilState(m_depthStencilState);
}
} // namespace detail


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
	: m_primitiveRenderer(nullptr)
	, m_spriteRenderer(nullptr)
	, m_textRenderer(nullptr)
	, m_frameRectRenderer(nullptr)
	, m_meshRendererProxy(nullptr)


	, m_manager(nullptr)
	, m_activeRendererPloxy(nullptr)
{
}

//------------------------------------------------------------------------------
GraphicsContext::~GraphicsContext()
{
	LN_SAFE_RELEASE(m_meshRendererProxy);
	LN_SAFE_RELEASE(m_frameRectRenderer);
	LN_SAFE_RELEASE(m_textRenderer);
	LN_SAFE_RELEASE(m_spriteRenderer);
	LN_SAFE_RELEASE(m_geometryRenderer);
	LN_SAFE_RELEASE(m_primitiveRenderer);
}

//------------------------------------------------------------------------------
void GraphicsContext::Initialize(GraphicsManager* manager)
{
	LN_CHECK_ARG(manager != nullptr);
	m_manager = manager;
	m_ploxy = m_manager->GetRenderer();
	m_activeRendererPloxy = nullptr;

	// ステート初期値
	m_state.SetRenderTarget(0, m_manager->GetMainSwapChain()->GetBackBuffer());
	LN_REFOBJ_SET(m_state.depthBuffer, m_manager->GetMainSwapChain()->GetBackBufferDepth());

	// 最初は全部変更状態
	m_state.modifiedFlags = detail::ContextStateFlags::All;



	Renderer = manager->GetRenderer();

	m_primitiveRenderer = LN_NEW detail::PrimitiveRenderer();
	m_primitiveRenderer->Initialize(manager);
	m_primitiveRenderer->SetUseInternalShader(true);	// TODO

	m_geometryRenderer = LN_NEW detail::GeometryRenderer();
	m_geometryRenderer->Initialize(manager);

	m_spriteRenderer = LN_NEW SpriteRenderer(manager, 2048);	// TODO:

	m_textRenderer = LN_NEW detail::TextRenderer();
	m_textRenderer->Initialize(manager);

	m_frameRectRenderer = LN_NEW detail::FrameRectRenderer();
	m_frameRectRenderer->Initialize(manager);

	m_meshRendererProxy = LN_NEW detail::MeshRendererProxy();
	m_meshRendererProxy->Initialize(manager);
}



//------------------------------------------------------------------------------
void GraphicsContext::SetRenderState(const RenderState& state)
{
	if (!m_state.renderState.Equals(state))
	{
		OnStateChanging();
		m_state.renderState = state;
		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
	}
}

void GraphicsContext::SetAlphaBlendEnabled(bool enabled)
{
	if (m_state.renderState.alphaBlendEnabled != enabled)
	{
		OnStateChanging();
		m_state.renderState.alphaBlendEnabled = enabled;
		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
	}
}
void GraphicsContext::SetBlendOp(BlendOp op)
{
	if (m_state.renderState.blendOp != op)
	{
		OnStateChanging();
		m_state.renderState.blendOp = op;
		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
	}
}
void GraphicsContext::SetSourceBlend(BlendFactor blend)
{
	if (m_state.renderState.sourceBlend != blend)
	{
		OnStateChanging();
		m_state.renderState.sourceBlend = blend;
		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
	}
}
void GraphicsContext::SetDestinationBlend(BlendFactor blend)
{
	if (m_state.renderState.destinationBlend != blend)
	{
		OnStateChanging();
		m_state.renderState.destinationBlend = blend;
		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
	}
}
void GraphicsContext::SetBlendMode(BlendMode mode)
{
	switch (mode)
	{
		// もっといろいろ http://d.hatena.ne.jp/Ko-Ta/20070618/p1
	case BlendMode::Normal:
		SetAlphaBlendEnabled(false);
		SetBlendOp(BlendOp::Add);
		SetSourceBlend(BlendFactor::One);
		SetDestinationBlend(BlendFactor::Zero);
		break;
	case BlendMode::Alpha:
		SetAlphaBlendEnabled(true);
		SetBlendOp(BlendOp::Add);
		SetSourceBlend(BlendFactor::SourceAlpha);
		SetDestinationBlend(BlendFactor::InverseSourceAlpha);
		break;
	case BlendMode::Add:
		SetAlphaBlendEnabled(true);
		SetBlendOp(BlendOp::Add);
		SetSourceBlend(BlendFactor::SourceAlpha);
		SetDestinationBlend(BlendFactor::One);
		break;
	case BlendMode::AddAlphaDisabled:
		SetAlphaBlendEnabled(true);
		SetBlendOp(BlendOp::Add);
		SetSourceBlend(BlendFactor::One);
		SetDestinationBlend(BlendFactor::One);
		break;
	case BlendMode::Subtract:
		SetAlphaBlendEnabled(true);
		SetBlendOp(BlendOp::ReverseSubtract);
		SetSourceBlend(BlendFactor::SourceAlpha);
		SetDestinationBlend(BlendFactor::One);
		break;
	case BlendMode::SubtractAlphaDisabled:
		SetAlphaBlendEnabled(true);
		SetBlendOp(BlendOp::ReverseSubtract);
		SetSourceBlend(BlendFactor::One);
		SetDestinationBlend(BlendFactor::One);
		break;
	case BlendMode::MultiplyAlphaDisabled:
		SetAlphaBlendEnabled(true);
		SetBlendOp(BlendOp::Add);
		// AlphaDisable (Alpha を別指定できない今の仕様では Alpha を考慮できない)
		SetSourceBlend(BlendFactor::Zero);
		SetDestinationBlend(BlendFactor::SourceColor);
		break;
		//case BlendMode_Screen:
		//	m_dxDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		//	m_dxDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		//	m_dxDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHASAT);
		//	m_dxDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR);
		//	m_dxDevice->SetRenderState(D3DRS_ALPHAREF, 255);
		//	break;
		//case BlendMode_Reverse:
		//	m_dxDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		//	m_dxDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		//	m_dxDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		//	m_dxDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVSRCCOLOR);
		//	m_dxDevice->SetRenderState(D3DRS_ALPHAREF, 1);
		//	break;
	default:
		LN_CHECK_ARG(0);
		break;
	}
}
void GraphicsContext::SetCullingMode(CullingMode mode)
{
	if (m_state.renderState.Culling != mode)
	{
		OnStateChanging();
		m_state.renderState.Culling = mode;
		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
	}
}
void GraphicsContext::SetFillMode(FillMode mode)
{
	if (m_state.renderState.Fill != mode)
	{
		OnStateChanging();
		m_state.renderState.Fill = mode;
		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
	}
}
void GraphicsContext::SetAlphaTestEnabled(bool enabled)
{
	if (m_state.renderState.AlphaTest != enabled)
	{
		OnStateChanging();
		m_state.renderState.AlphaTest = enabled;
		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
	}
}

//------------------------------------------------------------------------------
const RenderState& GraphicsContext::GetRenderState() const
{
	return m_state.renderState;
}

//------------------------------------------------------------------------------
void GraphicsContext::SetDepthStencilState(const DepthStencilState& state)
{
	if (!m_state.depthStencilState.Equals(state))
	{
		OnStateChanging();
		m_state.depthStencilState = state;
		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
	}
}

void GraphicsContext::SetDepthTestEnabled(bool enabled)
{
	if (m_state.depthStencilState.DepthTestEnabled != enabled)
	{
		OnStateChanging();
		m_state.depthStencilState.DepthTestEnabled = enabled;
		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
	}
}
void GraphicsContext::SetDepthWriteEnabled(bool enabled)
{
	if (m_state.depthStencilState.DepthWriteEnabled != enabled)
	{
		OnStateChanging();
		m_state.depthStencilState.DepthWriteEnabled = enabled;
		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
	}
}
void GraphicsContext::SetDepthTestFunc(CompareFunc func)
{
	if (m_state.depthStencilState.DepthTestFunc != func)
	{
		OnStateChanging();
		m_state.depthStencilState.DepthTestFunc = func;
		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
	}
}
void GraphicsContext::SetStencilEnabled(bool enabled)
{
	if (m_state.depthStencilState.StencilEnabled != enabled)
	{
		OnStateChanging();
		m_state.depthStencilState.StencilEnabled = enabled;
		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
	}
}
void GraphicsContext::SetStencilFunc(CompareFunc func)
{
	if (m_state.depthStencilState.StencilFunc != func)
	{
		OnStateChanging();
		m_state.depthStencilState.StencilFunc = func;
		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
	}
}
void GraphicsContext::SetStencilReferenceValue(uint8_t value)
{
	if (m_state.depthStencilState.StencilReferenceValue != value)
	{
		OnStateChanging();
		m_state.depthStencilState.StencilReferenceValue = value;
		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
	}
}
void GraphicsContext::SetStencilFailOp(StencilOp op)
{
	if (m_state.depthStencilState.StencilFailOp != op)
	{
		OnStateChanging();
		m_state.depthStencilState.StencilFailOp = op;
		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
	}
}
void GraphicsContext::SetStencilDepthFailOp(StencilOp op)
{
	if (m_state.depthStencilState.StencilDepthFailOp != op)
	{
		OnStateChanging();
		m_state.depthStencilState.StencilDepthFailOp = op;
		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
	}
}
void GraphicsContext::SetStencilPassOp(StencilOp op)
{
	if (m_state.depthStencilState.StencilPassOp != op)
	{
		OnStateChanging();
		m_state.depthStencilState.StencilPassOp = op;
		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
	}
}
const DepthStencilState& GraphicsContext::GetDepthStencilState() const
{
	return m_state.depthStencilState;
}

void GraphicsContext::ResetStates()
{
	SetAlphaBlendEnabled(RenderState::Default.alphaBlendEnabled);
	SetBlendOp(RenderState::Default.blendOp);
	SetSourceBlend(RenderState::Default.sourceBlend);
	SetDestinationBlend(RenderState::Default.destinationBlend);
	SetCullingMode(RenderState::Default.Culling);
	SetFillMode(RenderState::Default.Fill);
	SetAlphaTestEnabled(RenderState::Default.alphaBlendEnabled);

	SetDepthTestEnabled(DepthStencilState::Default.DepthTestEnabled);
	SetDepthWriteEnabled(DepthStencilState::Default.DepthWriteEnabled);
	SetDepthTestFunc(DepthStencilState::Default.DepthTestFunc);
	SetStencilEnabled(DepthStencilState::Default.StencilEnabled);
	SetStencilFunc(DepthStencilState::Default.StencilFunc);
	SetStencilReferenceValue(DepthStencilState::Default.StencilReferenceValue);
	SetStencilFailOp(DepthStencilState::Default.StencilFailOp);
	SetStencilDepthFailOp(DepthStencilState::Default.StencilDepthFailOp);
	SetStencilPassOp(DepthStencilState::Default.StencilPassOp);
}

//------------------------------------------------------------------------------
void GraphicsContext::SetRenderTarget(int index, Texture* texture)
{
	if (texture != m_state.GetRenderTarget(index))
	{
		OnStateChanging();
		m_state.SetRenderTarget(index, texture);
	}
}

//------------------------------------------------------------------------------
Texture* GraphicsContext::GetRenderTarget(int index) const
{
	return m_state.GetRenderTarget(index);
}

//------------------------------------------------------------------------------
void GraphicsContext::SetDepthBuffer(Texture* depthBuffer)
{
	if (m_state.depthBuffer != depthBuffer)
	{
		OnStateChanging();
		LN_REFOBJ_SET(m_state.depthBuffer, depthBuffer);
		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
	}
}

//------------------------------------------------------------------------------
Texture* GraphicsContext::GetDepthBuffer() const
{
	return m_state.depthBuffer;
}

//------------------------------------------------------------------------------
void GraphicsContext::SetViewport(const Rect& rect)
{
	if (m_state.viewport != rect)
	{
		OnStateChanging();
		m_state.viewport = rect;
		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
	}
}

//------------------------------------------------------------------------------
const Rect& GraphicsContext::GetViewport() const
{
	return m_state.viewport;
}

//------------------------------------------------------------------------------
void GraphicsContext::SetShaderPass(ShaderPass* pass)
{
	if (pass != m_state.GetShaderPass() || (pass != nullptr && pass->GetOwnerShader()->IsModifiedVariables()))
	{
		OnStateChanging();
		m_state.SetShaderPass(pass);
	}
}

//------------------------------------------------------------------------------
ShaderPass* GraphicsContext::GetShaderPass() const
{
	return m_state.GetShaderPass();
}








//------------------------------------------------------------------------------
void GraphicsContext::Set2DRenderingMode(float minZ, float maxZ)
{
	OnStateChanging();
	const Size& size = Renderer->GetRenderTarget(0)->GetSize();
	m_state.viewTransform = Matrix::Identity;
	m_state.projectionTransform = Matrix::MakePerspective2DLH((float)size.width, (float)size.height, minZ, maxZ);

}

//------------------------------------------------------------------------------
void GraphicsContext::SetViewProjectionTransform(const Matrix& view, const Matrix& proj)
{
	OnStateChanging();
	m_state.viewTransform = view;
	m_state.projectionTransform = proj;

	//m_spriteRenderer->SetViewProjMatrix(view, proj);
}

//------------------------------------------------------------------------------
void GraphicsContext::SetTransform(const Matrix& matrix)
{
	// TODO: ここは viewproj と同じように OnFlushState でしなくていいんだっけ？
	m_geometryRenderer->SetTransform(matrix);
	m_textRenderer->SetTransform(matrix);
	m_meshRendererProxy->SetTransform(matrix);
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
	m_state.SetFillBrush(brush);
}

//------------------------------------------------------------------------------
void GraphicsContext::SetFont(Font* font)
{
	m_textRenderer->SetFont(font);
}

//------------------------------------------------------------------------------
void GraphicsContext::PushState()
{
	m_stateStack.push(m_state);
}

//------------------------------------------------------------------------------
void GraphicsContext::PopState()
{
	m_state = m_stateStack.top();
	m_stateStack.pop();
	m_state.modifiedFlags = detail::ContextStateFlags::All;
}

//------------------------------------------------------------------------------
void GraphicsContext::Clear(ClearFlags flags, const Color& color, float z, uint8_t stencil)
{
	OnDrawing(GetCommonRenderer());
	GetCommonRenderer()->Clear(flags, color, z, stencil);
}

//------------------------------------------------------------------------------
void GraphicsContext::MoveTo(const Vector3& point, const Color& color)
{
	OnDrawing(m_geometryRenderer);
	m_geometryRenderer->MoveTo(point, color);
}

//------------------------------------------------------------------------------
void GraphicsContext::LineTo(const Vector3& point, const Color& color)
{
	OnDrawing(m_geometryRenderer);
	m_geometryRenderer->LineTo(point, color);
}

//------------------------------------------------------------------------------
void GraphicsContext::BezierCurveTo(const Vector3& cp1, const Vector3& cp2, const Vector3& endPt, const Color& color)
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
//void GraphicsContext::DrawPoint(const Vector3& point, const Color& color)
//{
//	OnDrawing(m_geometryRenderer);
//	m_geometryRenderer->DrawPoint(point, color);
//}

//------------------------------------------------------------------------------
void GraphicsContext::DrawTriangle(const Vector3& p1, const Color& p1Color, const Vector3& p2, const Color& p2Color, const Vector3& p3, const Color& p3Color)
{
	OnDrawing(m_geometryRenderer);
	m_geometryRenderer->DrawTriangle(p1, p1Color, p2, p2Color, p3, p3Color);
}

//------------------------------------------------------------------------------
void GraphicsContext::DrawRectangle(const RectF& rect)
{
	OnDrawing(m_geometryRenderer);

	if (m_state.GetFillBrush()->GetType() == BrushType_FrameTexture)
	{
		m_frameRectRenderer->Draw(rect);
	}
	else
	{
		m_geometryRenderer->DrawRectangle(rect);
	}
}

//------------------------------------------------------------------------------
void GraphicsContext::DrawEllipse(const Vector3& center, const Vector2& radius)
{
	OnDrawing(m_geometryRenderer);
	m_geometryRenderer->DrawEllipse(center, radius);
}

//------------------------------------------------------------------------------
void GraphicsContext::DrawTexture(const RectF& rect, Texture* texture, const Rect& srcRect, const Color& color)
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

//------------------------------------------------------------------------------
void GraphicsContext::DrawPrimitive(VertexDeclaration* vertexDeclaration, VertexBuffer* vertexBuffer, PrimitiveType primitive, int startVertex, int primitiveCount)
{
	OnDrawing(GetCommonRenderer());
	GetCommonRenderer()->DrawPrimitive(vertexDeclaration, vertexBuffer, primitive, startVertex, primitiveCount);
}

//------------------------------------------------------------------------------
void GraphicsContext::DrawPrimitiveIndexed(VertexDeclaration* vertexDeclaration, VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, PrimitiveType primitive, int startIndex, int primitiveCount)
{
	OnDrawing(GetCommonRenderer());
	GetCommonRenderer()->DrawPrimitiveIndexed(vertexDeclaration, vertexBuffer, indexBuffer, primitive, startIndex, primitiveCount);
}

//------------------------------------------------------------------------------
void GraphicsContext::DrawLinePrimitive(const Vector3& from, const Color& fromColor, const Vector3& to, const Color& toColor)
{
	OnDrawing(m_primitiveRenderer);
	m_primitiveRenderer->DrawLine(from, fromColor, to, toColor);
}

//------------------------------------------------------------------------------
void GraphicsContext::DrawSquarePrimitive(
	const Vector3& position1, const Vector2& uv1, const Color& color1,
	const Vector3& position2, const Vector2& uv2, const Color& color2,
	const Vector3& position3, const Vector2& uv3, const Color& color3,
	const Vector3& position4, const Vector2& uv4, const Color& color4)
{
	OnDrawing(m_primitiveRenderer);
	m_primitiveRenderer->DrawSquare(
		position1, uv1, color1,
		position2, uv2, color2,
		position3, uv3, color3,
		position4, uv4, color4);
}

//------------------------------------------------------------------------------
void GraphicsContext::Flush()
{
	m_manager->SwitchActiveContext(this);
	OnPrimitiveFlushRequested();
}

//------------------------------------------------------------------------------
void GraphicsContext::Blt(Texture* source, RenderTarget* dest)
{
	BltInternal(source, dest, Matrix::Identity, nullptr);
}

//------------------------------------------------------------------------------
void GraphicsContext::Blt(Texture* source, RenderTarget* dest, const Matrix& transform)
{
	BltInternal(source, dest, transform, nullptr);
}

//------------------------------------------------------------------------------
void GraphicsContext::Blt(Texture* source, RenderTarget* dest, Shader* shader)
{
	BltInternal(source, dest, Matrix::Identity, shader);
}

//------------------------------------------------------------------------------
void GraphicsContext::BltInternal(Texture* source, RenderTarget* dest, const Matrix& transform, Shader* shader)
{
	// TODO: ここで null にしておかないとPrimitiveRendererが古いシェーダを適用してしまう。
	// が、内部でステートを変えてしまうのはどうなのか。。。
	SetShaderPass(nullptr);

	//RenderState oldState1 = GetRenderState();
	//DepthStencilState oldState2 = GetDepthStencilState();

	//RenderState newState1;
	//newState1.Blend = BlendMode::Alpha;	// TODO:指定できた方がいい？
	//newState1.AlphaTest = false;
	//SetRenderState(newState1);

	//DepthStencilState newState2;
	//newState2.DepthTestEnabled = false;
	//newState2.DepthWriteEnabled = false;
	//SetDepthStencilState(newState2);

	detail::RenderStateBlock stateBlock(this);
	SetBlendMode(BlendMode::Alpha);
	SetAlphaTestEnabled(false);
	SetDepthTestEnabled(false);
	SetDepthWriteEnabled(false);

	OnDrawing(m_primitiveRenderer);
	m_primitiveRenderer->Blt(source, dest, transform, shader);


	//SetRenderState(oldState1);
	//SetDepthStencilState(oldState2);
}

//------------------------------------------------------------------------------
void GraphicsContext::DrawMesh(StaticMeshModel* mesh, int startIndex, int triangleCount)
{
	OnDrawing(m_meshRendererProxy);
	m_meshRendererProxy->DrawMesh(mesh, startIndex, triangleCount);
}

//------------------------------------------------------------------------------
void GraphicsContext::OnStateFlushRequested()
{
	m_ploxy->FlushState(GetContextState());

	const Size& size = Renderer->GetRenderTarget(0)->GetSize();
	Matrix viewProj = m_state.viewTransform * m_state.projectionTransform;

	m_primitiveRenderer->SetViewPixelSize(size);
	m_geometryRenderer->SetViewProjection(m_state.viewTransform, m_state.projectionTransform, size);
	m_spriteRenderer->SetViewProjMatrix(m_state.viewTransform, m_state.projectionTransform);
	m_spriteRenderer->SetViewPixelSize(size);
	m_textRenderer->SetViewProjMatrix(viewProj);
	m_textRenderer->SetViewPixelSize(size);
	m_meshRendererProxy->SetViewProjMatrix(viewProj);

	if (m_state.GetFillBrush() != nullptr &&
		m_state.GetFillBrush()->GetType() == BrushType_FrameTexture)
	{
		m_frameRectRenderer->SetState(static_cast<FrameTextureBrush*>(m_state.GetFillBrush()), m_state.worldTransform, viewProj);
	}

	m_primitiveRenderer->SetUseInternalShader(GetShaderPass() == nullptr);
}



//------------------------------------------------------------------------------
void GraphicsContext::OnPrimitiveFlushRequested()
{
	if (m_activeRendererPloxy != nullptr)
	{
		m_activeRendererPloxy->Flush();
	}
}

//------------------------------------------------------------------------------
void GraphicsContext::OnStateChanging()
{
	m_manager->SwitchActiveContext(this);
	OnPrimitiveFlushRequested();
}

//------------------------------------------------------------------------------
void GraphicsContext::OnDrawing(detail::IRendererPloxy* rendererPloxy)
{
	m_manager->SwitchActiveContext(this);
	if (m_state.modifiedFlags != detail::ContextStateFlags::None)
	{
		OnStateFlushRequested();
		m_state.modifiedFlags = detail::ContextStateFlags::None;
	}
	SwitchActiveRendererPloxy(rendererPloxy);
}

//------------------------------------------------------------------------------
void GraphicsContext::OnDeactivated()
{
	SwitchActiveRendererPloxy(nullptr);
}

//------------------------------------------------------------------------------
void GraphicsContext::OnActivated()
{
	// ステート強制送信
	//m_state.Reset();
	m_state.modifiedFlags = detail::ContextStateFlags::All;
	//m_ploxy->FlushState(m_state);
	//m_state.modifiedFlags = detail::ContextStateFlags::None;
}

//------------------------------------------------------------------------------
void GraphicsContext::SwitchActiveRendererPloxy(detail::IRendererPloxy* rendererPloxy)
{
	if (rendererPloxy != m_activeRendererPloxy)
	{
		if (m_activeRendererPloxy != nullptr)
		{
			m_activeRendererPloxy->OnDeactivated();	// 古いものを Deactivate
		}

		m_activeRendererPloxy = rendererPloxy;

		if (m_activeRendererPloxy != nullptr)
		{
			m_activeRendererPloxy->OnActivated();	// 新しいものを Activate
		}
	}
}

LN_NAMESPACE_END
