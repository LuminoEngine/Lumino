
#pragma once
#include "Common.h"
#include "RenderState.h"
#include "Shader.h"
#include "Texture.h"

LN_NAMESPACE_BEGIN
class DrawList;

namespace detail {
class IRendererPloxy;
class PrimitiveRenderer;
class MeshRendererProxy;
class SpriteRenderer;

class InternalContext
	: public RefObject
{
public:
	InternalContext();
	void Initialize(detail::GraphicsManager* manager);
	Details::Renderer* GetRenderStateManager();
	Details::Renderer* BeginBaseRenderer();
	PrimitiveRenderer* BeginPrimitiveRenderer();
	MeshRendererProxy* BeginMeshRenderer();
	SpriteRenderer* BeginSpriteRenderer();

	void SetViewInfo(const SizeF& viewPixelSize, const Matrix& viewMatrix, const Matrix& projMatrix);
	detail::SpriteRenderer* GetSpriteRenderer();

	void Flush();

private:
	void SwitchActiveRenderer(detail::IRendererPloxy* renderer);

	IRendererPloxy*				m_current;
	Details::Renderer*			m_baseRenderer;
	RefPtr<PrimitiveRenderer>	m_primitiveRenderer;
	RefPtr<MeshRendererProxy>	m_meshRenderer;
	RefPtr<SpriteRenderer>		m_spriteRenderer;

	friend class DrawList;
};

class DrawElement
{
public:
	Matrix	transform;
	int		batchIndex;

	DrawElement();
	virtual ~DrawElement();

	virtual void Execute(InternalContext* context) = 0;
	virtual detail::Sphere GetBoundingSphere();
};

class DrawElementBatch
{
public:
	static const int MaxMultiRenderTargets = 4;

	struct ShaderValuePair
	{
		ShaderVariable*		variable;
		ShaderValue			value;
	};

	bool Equal(const DrawElementBatch& obj) const;
	void Reset(/*RenderTarget* renderTarget, DepthBuffer* depthBuffer*/);
	void ApplyStatus(InternalContext* context, RenderTarget* defaultRenderTarget, DepthBuffer* defaultDepthBuffer);

	intptr_t				m_rendererId;

	// render state
	bool					m_alphaBlendEnabled;
	BlendMode				m_blendMode;
	CullingMode				m_cullingMode;
	bool					m_alphaTestEnabled;

	// depth stencil
	bool					m_depthTestEnabled;
	bool					m_depthWriteEnabled;

	// shader	TODO: サブセット単位でステート変えられるようにしたいこともあるけど、毎回変数値を作るのはちょっと無駄な気がする
	RefPtr<ShaderPass>		m_shaderPass;
	List<ShaderValuePair>	m_shaderValueList;

	// screen
	RefPtr<RenderTarget>	m_renderTargets[MaxMultiRenderTargets];
	RefPtr<DepthBuffer>		m_depthBuffer;
	Rect					m_scissorRect;
};

class BatchStateBlock
{
public:
	DrawElementBatch	state;
	Matrix				transfrom;

	void Reset()
	{
		state.Reset();
		transfrom = Matrix::Identity;
	}
};

class DrawElementList
{
public:
	typedef size_t	CommandHandle;

	DrawElementList();

	int GetElementCount() { return m_commandList.GetCount(); }
	DrawElement* GetElement(int index) { return GetCommand(m_commandList[index]); }
	DrawElementBatch* GetBatch(int index) { return &m_batchList[index]; }

	DrawElement* GetCommand(CommandHandle handle);

	void ClearCommands();

	template<typename T, typename... TArgs>
	T* AddCommand(const DrawElementBatch& state, detail::IRendererPloxy* renderer, TArgs... args)
	{
		CommandHandle handle = AllocCommand(sizeof(T));
		T* t = new (GetCommand(handle))T(args...);
		m_commandList.Add(handle);
		PostAddCommandInternal(state, renderer, t);
		return t;
	}

private:
	CommandHandle AllocCommand(size_t byteCount);
	void PostAddCommandInternal(const DrawElementBatch& state, detail::IRendererPloxy* renderer, DrawElement* element);

	List<CommandHandle>		m_commandList;
	ByteBuffer				m_commandDataBuffer;
	size_t					m_commandDataBufferUsed;
	List<DrawElementBatch>	m_batchList;
};


class InternalRenderer
	: public RefObject
{
public:
	InternalRenderer();
	virtual ~InternalRenderer();
	void Initialize(GraphicsManager* manager);

	void Render(
		DrawElementList* elementList,
		const SizeF& viewPixelSize,
		const Matrix& viewMatrix,
		const Matrix& projMatrix,
		const ViewFrustum& viewFrustum,
		RenderTarget* defaultRenderTarget,
		DepthBuffer* defaultDepthBuffer);

private:
	GraphicsManager*	m_manager;
	List<DrawElement*>	m_renderingElementList;
};


class ScopedStateBlock2
{
public:
	ScopedStateBlock2(DrawList* renderer);
	~ScopedStateBlock2();
	void Apply();

private:
	DrawList*		m_renderer;
	BatchStateBlock	m_state;
};

} // namespace detail

/**
	@brief	
*/
class DrawList	// TODO: Renderer
	: public Object
{
public:

	//--------------------------------------------------------------------------
	/** @name Render targets */
	/** @{ */

	/** レンダリングターゲットを設定します。*/
	void SetRenderTarget(int index, RenderTarget* texture);

	/** 現在設定されているレンダリングターゲットを取得します。*/
	RenderTarget* GetRenderTarget(int index) const;

	/** 深度バッファを設定します。*/
	void SetDepthBuffer(DepthBuffer* depthBuffer);

	/** 現在設定されている深度バッファを取得します。*/
	DepthBuffer* GetDepthBuffer() const;

	/** シザー領域を設定します。*/
	void SetViewport(const Rect& rect);

	/** 現在設定されているシザー領域を取得します。*/
	const Rect& GetViewport() const;

	/** @} */

	void SetTransform(const Matrix& transform);

	void Clear(ClearFlags flags, const Color& color, float z = 1.0f, uint8_t stencil = 0x00);

	/**
		@brief
		@details	デフォルトでは反時計回りが表面となります。
	*/
	void DrawSquarePrimitive(
		const Vector3& position1, const Vector2& uv1, const Color& color1,
		const Vector3& position2, const Vector2& uv2, const Color& color2,
		const Vector3& position3, const Vector2& uv3, const Color& color3,
		const Vector3& position4, const Vector2& uv4, const Color& color4,
		ShaderPass* shaderPass);

	void DrawSprite2D(
		const SizeF& size,
		Texture* texture,
		const RectF& srcRect,
		const Color& color);

LN_INTERNAL_ACCESS:
	DrawList();
	virtual ~DrawList();
	void Initialize(detail::GraphicsManager* manager);
	detail::DrawElementList* GetDrawElementList() { return &m_drawElementList; }
	void Clear();
	//void BeginFrame(RenderTarget* defaultRenderTarget, DepthBuffer* defaultDepthBuffer);
	void EndFrame();
	const detail::BatchStateBlock& GetState() const { return m_state; }
	void SetState(const detail::BatchStateBlock& state) { m_state = state; }
	void BltInternal(Texture* source, RenderTarget* dest, const Matrix& transform, Shader* shader);

private:
	detail::GraphicsManager*		m_manager;
	detail::BatchStateBlock			m_state;
	detail::DrawElementList			m_drawElementList;
};

LN_NAMESPACE_END

