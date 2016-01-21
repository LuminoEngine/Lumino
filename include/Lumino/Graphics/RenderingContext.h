
#pragma once
#include "Common.h"
#include "RenderState.h"
#include "Color.h"

LN_NAMESPACE_BEGIN

/**
	@brief		
*/
class RenderingContext
{
public:
	static const int MaxMultiRenderTargets = 4;

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
	void DrawPrimitiveIndexed(PrimitiveType primitive, int startIndex, int primitiveCount);

};

LN_NAMESPACE_END
