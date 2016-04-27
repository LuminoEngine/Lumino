
#pragma once
#include "Common.h"
#include "RenderState.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
class GraphicsManager;

namespace detail
{
class RenderTargetTextureCache;

LN_ENUM_FLAGS(ContextStateFlags)
{
	None = 0x00,
	CommonState = 0x01,
	ShaderPass = 0x02,		// これ以外のステートに比べて割と頻繁に変更されるので個別に分ける
	All = 0xFFFF,
};
LN_ENUM_FLAGS_DECLARE(ContextStateFlags);

struct ContextState
{


	static const int MaxMultiRenderTargets = 4;	// TODO

	RenderState				renderState;
	DepthStencilState		depthStencilState;
	Texture*				depthBuffer = nullptr;
	Rect					viewport;
	//VertexBuffer*			vertexBuffer = nullptr;
	//IndexBuffer*			indexBuffer = nullptr;

	ContextStateFlags		modifiedFlags = ContextStateFlags::None;

	~ContextState();


	void SetRenderTarget(int index, Texture* texture);
	Texture* GetRenderTarget(int index) const;
	void SetShaderPass(ShaderPass* pass);
	ShaderPass* GetShaderPass() const { return m_shaderPass; }

	void Reset()
	{
		renderState = RenderState();
		depthStencilState = DepthStencilState();
		depthBuffer = nullptr;
		viewport = Rect();
		//vertexBuffer = nullptr;
		//indexBuffer = nullptr;
	}

private:
	std::array<Texture*, MaxMultiRenderTargets>	m_renderTargets = {};
	Shader*		m_ownerShader = nullptr;
	ShaderPass*	m_shaderPass = nullptr;
};


class IRendererPloxy// : public RefObject
{
public:
	virtual void Flush() = 0;
	virtual void OnActivated() = 0;
	virtual void OnDeactivated() = 0;
};

class HiLevelRendererCore : public RefObject
{
public:

	void ActivateFront(IRendererPloxy* renderer)
	{
		if (renderer != m_rendererFront)
		{
			if (m_rendererFront != nullptr)
			{
				m_rendererFront->Flush();
			}
			m_rendererFront = renderer;
		}
	}

private:
	IRendererPloxy*	m_rendererFront = nullptr;
};




class IContext
{
public:
	
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

	///**
	//	@brief	頂点バッファを設定します。
	//*/
	//void SetVertexBuffer(VertexBuffer* vertexBuffer);

	///**
	//	@brief	インデックスバッファを設定します。
	//*/
	//void SetIndexBuffer(IndexBuffer* indexBuffer);

	void SetShaderPass(ShaderPass* pass);

	ShaderPass* GetShaderPass() const;

	void Flush();

protected:
	IContext();
	virtual ~IContext();
	void Initialize(GraphicsManager* manager);
	Details::Renderer* GetCommonRenderer() const { return m_ploxy; }
	const detail::ContextState& GetContextState() const { return m_state; }

	virtual void OnStateFlushRequested();
	virtual void OnPrimitiveFlushRequested();
    
public: //TODO:
	friend class ShaderVariable;
	void OnStateChanging();
	void OnDrawing(detail::IRendererPloxy* rendererPloxy);

	friend class GraphicsManager;
	virtual void OnActivated();
	virtual void OnDeactivated();

//LN_INTERNAL_ACCESS:
//	virtual ShaderPass* GetShaderPass() const = 0;

private:
	void SwitchActiveRendererPloxy(detail::IRendererPloxy* rendererPloxy);

	GraphicsManager*		m_manager;
	Details::Renderer*		m_ploxy;
	detail::ContextState	m_state;
	detail::IRendererPloxy*	m_activeRendererPloxy;
};

} // namespace detail


LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END

