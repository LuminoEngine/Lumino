
#pragma once
#include "Common.h"
#include "ContextInterface.h"
#include "Brush.h"

LN_NAMESPACE_BEGIN
class GraphicsContext;
class SpriteRenderer;
namespace detail { class TextRenderer; }
namespace detail { class PrimitiveRenderer; }
namespace detail { class GeometryRenderer; }
namespace detail { class FrameRectRenderer; }

namespace detail
{
class RenderStateBlock	// TODO: Scoped は別定義したい
	: public Object
{
public:
	RenderStateBlock(GraphicsContext* context);
	~RenderStateBlock();

	void Apply(GraphicsContext* context);

private:
	GraphicsContext*		m_context;
	RenderState				m_renderState;
	DepthStencilState		m_depthStencilState;
	// TODO: Viewportサイズも必要かな。RenderTarget はいらない気がする。
};
} // namespace detail


class GraphicsContext
	: public RefObject
{
public:	// TODO
	//RenderingPass*				Pass;
	Details::Renderer*			Renderer;	// TODO 内部クラス
	//Camera*						CurrentCamera;
	//MMEShader*					Shader;				// 本当に必要なシェーダ (VisualNode::Render() 以下で使用可能)

	
	/**
		@brief	
	*/
	static GraphicsContext* GetContext();

public:


	//--------------------------------------------------------------------------
	/** @name Render status. */
	/** @{ */
	
	/**
		@brief	レンダリングステートを設定します。
	*/
	void SetRenderState(const RenderState& state);

	/** アルファブレンドの有無 (default: false) */
	void SetAlphaBlendEnabled(bool enabled);
	/** ブレンディングの演算方法 (default: Add) */
	void SetBlendOp(BlendOp op);
	/** ブレンディングの係数 (default: One) */
	void SetSourceBlend(BlendFactor blend);
	/** ブレンディングの係数 (default: Zero) */
	void SetDestinationBlend(BlendFactor blend);


	void SetBlendMode(BlendMode mode);
	void SetCullingMode(CullingMode mode);
	void SetFillMode(FillMode mode);
	void SetAlphaTestEnabled(bool enabled);


	/**
		@brief	現在設定されているレンダリングステートを取得します。
	*/
	const RenderState& GetRenderState() const;

	/**
		@brief	深度テスト及びステンシルテストステートを設定します。
	*/
	void SetDepthStencilState(const DepthStencilState& state);

	void SetDepthTestEnabled(bool enabled);
	void SetDepthWriteEnabled(bool enabled);
	void SetDepthTestFunc(CompareFunc func);
	void SetStencilEnabled(bool enabled);
	void SetStencilFunc(CompareFunc func);
	void SetStencilReferenceValue(uint8_t value);
	void SetStencilFailOp(StencilOp op);
	void SetStencilDepthFailOp(StencilOp op);
	void SetStencilPassOp(StencilOp op);
	

	void ResetStates();


	/** @} */

	

	/**
		@brief	深度テスト及びステンシルテストステートを取得します。
	*/
	const DepthStencilState& GetDepthStencilState() const;

	/**
		@brief	レンダリングターゲットを設定します。
	*/
	void SetRenderTarget(int index, Texture* texture);

	/**
		@brief	現在設定されているレンダリングターゲットを取得します。
	*/
	Texture* GetRenderTarget(int index) const;

	/**
		@brief	深度バッファを設定します。
	*/
	void SetDepthBuffer(Texture* depthBuffer);

	/**
		@brief	現在設定されている深度バッファを取得します。
	*/
	Texture* GetDepthBuffer() const;

	/**
		@brief	ビューポートを設定します。
	*/
	void SetViewport(const Rect& rect);

	/**
		@brief	現在設定されているビューポートを取得します。
	*/
	const Rect& GetViewport() const;


	void SetShaderPass(ShaderPass* pass);

	ShaderPass* GetShaderPass() const;




	void Set2DRenderingMode(float minZ = 0.0f, float maxZ = 1.0f);
	void SetViewProjectionTransform(const Matrix& view, const Matrix& proj);

	void SetTransform(const Matrix& matrix);
	void SetOpacity(float opacity);	// 0~1
	void SetBrush(Brush* brush);
	void SetFont(Font* font);

	void Clear(ClearFlags flags, const ColorF& color, float z = 1.0f, uint8_t stencil = 0x00);

	void MoveTo(const Vector3& point, const ColorF& color);
	void LineTo(const Vector3& point, const ColorF& color);
	void BezierCurveTo(const Vector3& cp1, const Vector3& cp2, const Vector3& endPt, const ColorF& color);
	void ClosePath();

	//void DrawPoint(const Vector3& point, const ColorF& color);
	void DrawTriangle(const Vector3& p1, const ColorF& p1Color, const Vector3& p2, const ColorF& p2Color, const Vector3& p3, const ColorF& p3Color);
	void DrawRectangle(const RectF& rect, const Color& color);
	void DrawEllipse(const Vector3& center, const Vector2& radius);
	void DrawTexture(const RectF& rect, Texture* texture, const Rect& srcRect, const ColorF& color);
	

	void DrawText(const StringRef& text, const PointF& position);
	void DrawText(const StringRef& text, const RectF& rect, StringFormatFlags flags);

	

	/**
		@brief	頂点バッファを使用してプリミティブをレンダリングします。
	*/
	void DrawPrimitive(VertexBuffer* vertexBuffer, PrimitiveType primitive, int startVertex, int primitiveCount);

	/**
		@brief	頂点バッファとインデックスバッファを使用してプリミティブをレンダリングします。
	*/
	void DrawPrimitiveIndexed(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, PrimitiveType primitive, int startIndex, int primitiveCount);	// TODO: DrawIndexedPrimitive



	void DrawLinePrimitive(const Vector3& from, const ColorF& fromColor, const Vector3& to, const ColorF& toColor);

	void DrawSquarePrimitive(
		const Vector3& position1, const Vector2& uv1, const ColorF& color1,
		const Vector3& position2, const Vector2& uv2, const ColorF& color2,
		const Vector3& position3, const Vector2& uv3, const ColorF& color3,
		const Vector3& position4, const Vector2& uv4, const ColorF& color4);

	void Flush();


	// ステート固定。
	void Blt(Texture* source, RenderTarget* dest);
	void Blt(Texture* source, RenderTarget* dest, const Matrix& transform);
	void Blt(Texture* source, RenderTarget* dest, Shader* shader);



	//detail::GeometryRenderer* BeginDrawingContext();
	SpriteRenderer* GetSpriteRenderer() const { return m_spriteRenderer; }


public:
	//enum class RendererType
	//{
	//	None,
	//	GeometryRenderer,
	//	SpriteRenderer,
	//	TextRenderer,
	//	PrimitiveRenderer,
	//};

	friend class GraphicsManager;
	GraphicsContext();
	virtual ~GraphicsContext();
	void Initialize(GraphicsManager* manager);


	//RendererType				m_currentRenderer;
	detail::PrimitiveRenderer*	m_primitiveRenderer;
	detail::GeometryRenderer*	m_geometryRenderer;
	SpriteRenderer*				m_spriteRenderer;
	detail::TextRenderer*		m_textRenderer;
	detail::FrameRectRenderer*	m_frameRectRenderer;

	// このクラスが管理するステート。VertexBuffer などは描画時に各種 RendererCore 内で変更されるからちょっと考える必要がありそう。
	//Shader*						m_shader;

protected:
	virtual void OnStateFlushRequested();
	virtual void OnPrimitiveFlushRequested();

private:
	void BltInternal(Texture* source, RenderTarget* dest, const Matrix& transform, Shader* shader);

	Details::Renderer* GetCommonRenderer() const { return m_ploxy; }
	const detail::ContextState& GetContextState() const { return m_state; }

	void SwitchActiveRendererPloxy(detail::IRendererPloxy* rendererPloxy);

	friend class ShaderVariable;
	void OnStateChanging();
	void OnDrawing(detail::IRendererPloxy* rendererPloxy);

	friend class GraphicsManager;
	virtual void OnActivated();
	virtual void OnDeactivated();

	friend class detail::RenderStateBlock;	// TODO

	GraphicsManager*		m_manager;
	Details::Renderer*		m_ploxy;
	detail::ContextState	m_state;
	detail::IRendererPloxy*	m_activeRendererPloxy;
};

LN_NAMESPACE_END
