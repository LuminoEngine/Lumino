
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

	// TODO: あとで IContex は GraphicsContext と統合する。なのでこれらはこのままここに置いておく
	Matrix					worldTransform;
	Matrix					viewTransform;
	Matrix					projectionTransform;


	ContextStateFlags		modifiedFlags = ContextStateFlags::None;

	ContextState();
	~ContextState();


	void SetRenderTarget(int index, Texture* texture);
	Texture* GetRenderTarget(int index) const;
	void SetShaderPass(ShaderPass* pass);
	ShaderPass* GetShaderPass() const { return m_shaderPass; }

	void SetFillBrush(Brush* brush);
	Brush* GetFillBrush() const { return m_fillBrush; }

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
	Brush*		m_fillBrush;
};



class IContext;

class RenderStateBlock	// TODO: Scoped は別定義したい
	: public Object
{
public:
	RenderStateBlock(IContext* context);
	~RenderStateBlock();

	void Apply(IContext* context);

private:
	IContext*				m_context;
	RenderState				m_renderState;
	DepthStencilState		m_depthStencilState;
	// TODO: Viewportサイズも必要かな。RenderTarget はいらない気がする。
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

protected:	// TODO
	void SwitchActiveRendererPloxy(detail::IRendererPloxy* rendererPloxy);

	friend class RenderStateBlock;

	GraphicsManager*		m_manager;
	Details::Renderer*		m_ploxy;
	detail::ContextState	m_state;
	detail::IRendererPloxy*	m_activeRendererPloxy;
};

} // namespace detail


LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END

