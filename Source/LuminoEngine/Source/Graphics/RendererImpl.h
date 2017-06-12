
#pragma once
#include <Lumino/Threading/Mutex.h>
#include <Lumino/Graphics/Common.h>
#include <Lumino/Graphics/RenderState.h>
#include <Lumino/Graphics/Color.h>
#include <Lumino/Graphics/GraphicsResourceObject.h>
#include <Lumino/Graphics/ContextInterface.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
class SwapChain;

namespace Details
{

/**
	@brief		
*/
class Renderer
	: public GraphicsResourceObject
	, public detail::IRenderFeature
{
public:
	Renderer(detail::GraphicsManager* manager);
	~Renderer();

	/**
		@brief	この Renderer による描画を開始します。
	*/
	void Begin();

	/**
		@brief	この Renderer による描画を終了します。
	*/
	void End();

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
	void SetDepthBuffer(DepthBuffer* depthBuffer);

	/**
		@brief	現在設定されている深度バッファを取得します。
	*/
	DepthBuffer* GetDepthBuffer() const;

	///**
	//	@brief	ビューポートを設定します。
	//*/
	//void SetViewport(const RectI& rect);

	///**
	//	@brief	現在設定されているビューポートを取得します。
	//*/
	//const RectI& GetViewport();

	///**
	//	@brief	頂点バッファを設定します。
	//*/
	//void SetVertexBuffer(VertexBuffer* vertexBuffer);

	///**
	//	@brief	インデックスバッファを設定します。
	//*/
	//void SetIndexBuffer(IndexBuffer* indexBuffer);
	
	/**
		@brief	
	*/
	void SetShaderPass(ShaderPass* pass);

	/**
		@brief	現在設定されているレンダリングターゲット、深度バッファをクリアします。
	*/
	void clear(ClearFlags flags, const Color& color, float z = 1.0f, uint8_t stencil = 0x00);

	/**
	*/
	void DrawPrimitive(VertexDeclaration* vertexDeclaration, VertexBuffer* vertexBuffer, PrimitiveType primitive, int startVertex, int primitiveCount);

	/**
	*/
	void DrawPrimitiveIndexed(VertexDeclaration* vertexDeclaration, VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, PrimitiveType primitive, int startIndex, int primitiveCount);

	void FlushState(const detail::ContextState& state);
	virtual bool IsStandaloneShader() const { return false; }
	virtual void Flush() override;
	virtual void OnActivated() {}
	virtual void OnDeactivated() { }

protected:
	virtual void OnChangeDevice(Driver::IGraphicsDevice* device);

LN_INTERNAL_ACCESS:
	friend class Helper;
	friend class SwapChain;
	friend class ShaderPass;
	friend class Texture;
	friend class ShaderVariable;
	void PresentCommandList(SwapChain* swapChain);	// m_primaryCommandList をバックグラウンドへ送る
	void VerifyFrameBuffers();

	Driver::IRenderer*		m_internal;
	RenderingCommandList*	m_primaryCommandList;

	RenderState				m_currentRenderState;
	DepthStencilState		m_currentDepthStencilState;
	Texture*				m_currentRenderTargets[Graphics::MaxMultiRenderTargets];
	DepthBuffer*			m_currentDepthBuffer;
	//RectI					m_currentViewport;

};

} // namespace Details


//namespace detail
//{
//
//class VertexBufferRenderer
//	: public RefObject
//	, public detail::IRendererPloxy
//{
//public:
//	void SetVertexBuffer(VertexBuffer* vertexBuffer);
//	void SetIndexBuffer(IndexBuffer* indexBuffer);
//	void DrawPrimitive(VertexBuffer* vertexBuffer, PrimitiveType primitive, int startVertex, int primitiveCount);
//	void DrawPrimitiveIndexed(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, PrimitiveType primitive, int startIndex, int primitiveCount);
//
//
//
//private:
//
//};

//} // namespace detail

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
