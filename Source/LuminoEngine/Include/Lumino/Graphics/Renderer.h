
#pragma once
#include <Lumino/Graphics/Common.h>
#include <Lumino/Graphics/RenderState.h>
#include <Lumino/Graphics/Color.h>

LN_NAMESPACE_BEGIN

/**
	@brief		
*/
class Renderer
{
public:

#if 0
public:
	
	/**
		@brief		レンダリングを開始します。
		@details	Application::updateFrame() を使用する場合、この関数を呼び出す必要はありません。
	*/
	static void BeginRendering();
	
	/**
		@brief		レンダリングを終了します。
	*/
	static void EndRendering();

	/**
		@brief	レンダリングステートを設定します。
	*/
	static void SetRenderState(const RenderState& state);

	/**
		@brief	現在設定されているレンダリングステートを取得します。
	*/
	static const RenderState& GetRenderState();

	/**
		@brief	深度テスト及びステンシルテストステートを設定します。
	*/
	static void SetDepthStencilState(const DepthStencilState& state);
	
	/**
		@brief	深度テスト及びステンシルテストステートを取得します。
	*/
	static const DepthStencilState& GetDepthStencilState();

	/**
		@brief	レンダリングターゲットを設定します。
	*/
	static void SetRenderTarget(int index, Texture* texture);

	/**
		@brief	現在設定されているレンダリングターゲットを取得します。
	*/
	static Texture* GetRenderTarget(int index);

	/**
		@brief	深度バッファを設定します。
	*/
	static void SetDepthBuffer(Texture* depthBuffer);

	/**
		@brief	現在設定されている深度バッファを取得します。
	*/
	static Texture* GetDepthBuffer();

	/**
		@brief	ビューポートを設定します。
	*/
	static void SetViewport(const RectI& rect);

	/**
		@brief	現在設定されているビューポートを取得します。
	*/
	static const RectI& GetViewport();

	///**
	//	@brief	頂点バッファを設定します。
	//*/
	//static void SetVertexBuffer(VertexBuffer* vertexBuffer);

	///**
	//	@brief	インデックスバッファを設定します。
	//*/
	//static void SetIndexBuffer(IndexBuffer* indexBuffer);

	/**
		@brief	現在設定されているレンダリングターゲット、深度バッファをクリアします。
	*/
	static void Clear(ClearFlags flags, const ColorF& color, float z = 1.0f, uint8_t stencil = 0x00);

	/**
		@brief	現在設定されている頂点バッファを使用してプリミティブをレンダリングします。
	*/
	static void DrawPrimitive(PrimitiveType primitive, int startVertex, int primitiveCount);

	/**
		@brief	現在設定されている頂点バッファとインデックスバッファを使用してプリミティブをレンダリングします。
	*/
	static void DrawPrimitiveIndexed(PrimitiveType primitive, int startIndex, int primitiveCount);

#endif
};
LN_NAMESPACE_END
