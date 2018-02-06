
#pragma once
#include <Lumino/Threading/Mutex.h>
#include <Lumino/Graphics/Common.h>
#include <Lumino/Graphics/RenderState.h>
#include <Lumino/Graphics/Color.h>
#include <Lumino/Graphics/GraphicsResourceObject.h>
#include <Lumino/Graphics/ContextInterface.h>
#include <Lumino/Rendering/Common.h>

LN_NAMESPACE_BEGIN
class SwapChain;

namespace detail
{

/**
	@brief		
*/
class CoreGraphicsRenderFeature
	: public GraphicsResourceObject
	, public detail::IRenderFeature
{
public:
	CoreGraphicsRenderFeature(detail::GraphicsManager* manager);
	virtual ~CoreGraphicsRenderFeature();

	/**
		@brief	この Renderer による描画を開始します。
	*/
	void begin();

	/**
		@brief	この Renderer による描画を終了します。
	*/
	void end();

	/**
		@brief	レンダリングステートを設定します。
	*/
	void setRenderState(const RenderState& state);

	/**
		@brief	現在設定されているレンダリングステートを取得します。
	*/
	const RenderState& getRenderState() const;

	/**
		@brief	深度テスト及びステンシルテストステートを設定します。
	*/
	void setDepthStencilState(const DepthStencilState& state);
	
	/**
		@brief	深度テスト及びステンシルテストステートを取得します。
	*/
	const DepthStencilState& getDepthStencilState() const;

	/**
		@brief	レンダリングターゲットを設定します。
	*/
	void setRenderTarget(int index, Texture* texture);

	/**
		@brief	現在設定されているレンダリングターゲットを取得します。
	*/
	Texture* getRenderTarget(int index) const;

	/**
		@brief	深度バッファを設定します。
	*/
	void setDepthBuffer(DepthBuffer* depthBuffer);

	/**
		@brief	現在設定されている深度バッファを取得します。
	*/
	DepthBuffer* getDepthBuffer() const;

	///**
	//	@brief	ビューポートを設定します。
	//*/
	void setViewport(const RectI& rect);

	///**
	//	@brief	現在設定されているビューポートを取得します。
	//*/
	//const RectI& getViewport();

	///**
	//	@brief	頂点バッファを設定します。
	//*/
	//void setVertexBuffer(VertexBuffer* vertexBuffer);

	///**
	//	@brief	インデックスバッファを設定します。
	//*/
	//void setIndexBuffer(IndexBuffer* indexBuffer);
	
	/**
		@brief	
	*/
	void setShaderPass(ShaderPass* pass);

	/**
		@brief	現在設定されているレンダリングターゲット、深度バッファをクリアします。
	*/
	void clear(ClearFlags flags, const Color& color, float z = 1.0f, uint8_t stencil = 0x00);

	/**
	*/
	void drawPrimitive(VertexDeclaration* vertexDeclaration, VertexBuffer* vertexBuffer, PrimitiveType primitive, int startVertex, int primitiveCount);

	/**
	*/
	void drawPrimitiveIndexed(VertexDeclaration* vertexDeclaration, VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, PrimitiveType primitive, int startIndex, int primitiveCount);

	void flushState(const detail::ContextState& state);
	virtual bool isStandaloneShader() const override { return false; }
	virtual void flush() override;
	virtual void onActivated() override {}
	virtual void onDeactivated() override { }

protected:
	virtual void onChangeDevice(Driver::IGraphicsDevice* device) override;

LN_INTERNAL_ACCESS:
	friend class Helper;
	friend class SwapChain;
	friend class ShaderPass;
	friend class Texture;
	friend class ShaderVariable;
	void presentSwapChain(SwapChain* swapChain);
	void presentCommandList(SwapChain* swapChain);	// m_primaryCommandList をバックグラウンドへ送る
	void verifyFrameBuffers();

	Driver::IRenderer*		m_internal;
	detail::RenderingCommandList*	m_primaryCommandList;
	Mutex							m_lockPresentCommandList;

	RenderState				m_currentRenderState;
	DepthStencilState		m_currentDepthStencilState;
	Texture*				m_currentRenderTargets[Graphics::MaxMultiRenderTargets];
	DepthBuffer*			m_currentDepthBuffer;
	//RectI					m_currentViewport;

};

} // namespace detail
LN_NAMESPACE_END
