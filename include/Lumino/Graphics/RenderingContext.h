
#pragma once
#include "Common.h"
#include "RenderState.h"
#include "Color.h"

LN_NAMESPACE_BEGIN
class SpriteRenderer;
namespace detail { class PrimitiveRenderer; }

/**
	@brief		
*/
class RenderingContext2
	: public RefObject
	, public detail::IContext
{
public:
	static const int MaxMultiRenderTargets = 4;

	// TODO: Sceneなどで内部的に使用するものと共用にしない。ステートがどこで変わるかわかりづらい。あるいはちゃんと push/pop する。
	static RenderingContext2* GetContext();

	/**
		@brief	レンダリングステートを設定します。
	*/
	void SetRenderState(const RenderState& state);

	/**
		@brief	現在設定されているレンダリングステートを取得します。
	*/
	const RenderState& GetRenderState() const;

	/**
		@brief	深度テスト及びステンシルテストステートを設定します。
	*/
	void SetDepthStencilState(const DepthStencilState& state);
	
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

	/**
		@brief	頂点バッファを設定します。
	*/
	void SetVertexBuffer(VertexBuffer* vertexBuffer);

	/**
		@brief	インデックスバッファを設定します。
	*/
	void SetIndexBuffer(IndexBuffer* indexBuffer);

	void SetShaderPass(ShaderPass* pass);

	/**
		@brief	現在設定されているレンダリングターゲット、深度バッファをクリアします。
	*/
	void Clear(ClearFlags flags, const ColorF& color, float z = 1.0f, uint8_t stencil = 0x00);

	/**
		@brief	現在設定されている頂点バッファを使用してプリミティブをレンダリングします。
	*/
	void DrawPrimitive(PrimitiveType primitive, int startVertex, int primitiveCount);

	/**
		@brief	現在設定されている頂点バッファとインデックスバッファを使用してプリミティブをレンダリングします。
	*/
	void DrawPrimitiveIndexed(PrimitiveType primitive, int startIndex, int primitiveCount);	// TODO: DrawIndexedPrimitive



	// pass が null のときに使われるデフォルトシェーダ用
	void SetViewProjection(const Matrix& view, const Matrix& proj);

	void DrawLine(const Vector3& from, const ColorF& fromColor, const Vector3& to, const ColorF& toColor);

	void DrawSquare(
		const Vector3& position1, const Vector2& uv1, const ColorF& color1,
		const Vector3& position2, const Vector2& uv2, const ColorF& color2,
		const Vector3& position3, const Vector2& uv3, const ColorF& color3,
		const Vector3& position4, const Vector2& uv4, const ColorF& color4);

	void Flush();

LN_INTERNAL_ACCESS:
	RenderingContext2();
	virtual ~RenderingContext2();
	void Initialize(GraphicsManager* manager);
	void CheckFlushRendererState();
	void SwitchActiveRendererPloxy(detail::IRendererPloxy* rendererPloxy);
	virtual void OnActivated() override;
	virtual void OnDeactivated() override;

private:
	GraphicsManager*			m_manager;
	Details::Renderer*			m_ploxy;
	detail::ContextState		m_state;

	detail::IRendererPloxy*		m_activeRendererPloxy;
	SpriteRenderer*				m_spriteRenderer;
	detail::PrimitiveRenderer*	m_primitiveRenderer;

	//bool						m_stateModified;
};

LN_NAMESPACE_END
