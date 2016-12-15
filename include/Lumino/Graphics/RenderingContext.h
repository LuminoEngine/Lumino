
#pragma once
#include <stack>
#include "Common.h"
#include "ContextInterface.h"

LN_NAMESPACE_BEGIN
class Material;
class MeshResource;
class StaticMeshModel;
namespace detail { class PrimitiveRenderer; }
namespace detail { class MeshRendererProxy; }

class RenderingContext
	: public detail::ContextInterface
{
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
	void SetViewport(const RectI& rect);

	/**
		@brief	現在設定されているビューポートを取得します。
	*/
	const RectI& GetViewport() const;









	void SetShaderPass(ShaderPass* pass);

	ShaderPass* GetShaderPass() const;




	void PushState();
	void PopState();

	/** @} */

	void Clear(ClearFlags flags, const Color& color, float z = 1.0f, uint8_t stencil = 0x00);


	

	/**
		@brief	頂点バッファを使用してプリミティブをレンダリングします。
	*/
	void DrawPrimitive(VertexDeclaration* vertexDeclaration, VertexBuffer* vertexBuffer, PrimitiveType primitive, int startVertex, int primitiveCount);

	/**
		@brief	頂点バッファとインデックスバッファを使用してプリミティブをレンダリングします。
	*/
	void DrawPrimitiveIndexed(VertexDeclaration* vertexDeclaration, VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, PrimitiveType primitive, int startIndex, int primitiveCount);	// TODO: DrawIndexedPrimitive



	void DrawLinePrimitive(const Vector3& from, const Color& fromColor, const Vector3& to, const Color& toColor);

	void DrawSquarePrimitive(
		const Vector3& position1, const Vector2& uv1, const Color& color1,
		const Vector3& position2, const Vector2& uv2, const Color& color2,
		const Vector3& position3, const Vector2& uv3, const Color& color3,
		const Vector3& position4, const Vector2& uv4, const Color& color4);

	void Flush();


	// ステート固定。
	void Blt(Texture* source, RenderTarget* dest);
	void Blt(Texture* source, RenderTarget* dest, const Matrix& transform);
	void Blt(Texture* source, RenderTarget* dest, Shader* shader);


LN_INTERNAL_ACCESS:
	RenderingContext();
	virtual ~RenderingContext();
	void Initialize(detail::GraphicsManager* manager);
	void InitializeFrame(RenderTarget* renderTarget);
	void DrawMesh(MeshResource* mesh, int startIndex, int triangleCount);
	void DrawMesh(MeshResource* mesh, int subset);
	void DrawMesh(StaticMeshModel* mesh, int subset);
	void DrawMesh(MeshResource* mesh, int startIndex, int triangleCount, Material* material);
	void DrawMesh(StaticMeshModel* mesh, int startIndex, int triangleCount, Material* material);
	void InheritStatus(RenderingContext* parent);

	detail::PrimitiveRenderer* GetPrimitiveRenderer() const { return m_primitiveRenderer; }	// TODO: test
	const detail::BasicContextState& GetCurrentContxtState() const { return m_state; }

protected:
	virtual bool OnCheckStateChanged() override;
	virtual void OnStateFlush(detail::IRendererPloxy* activeRenderer) override;
	virtual void OnPrimitiveFlush() override;
	virtual void OnShaderVariableModified(ShaderVariable* var) override;

private:
	void BltInternal(Texture* source, RenderTarget* dest, const Matrix& transform, Shader* shader);

	class RenderStateBlock2;

	detail::PrimitiveRenderer*				m_primitiveRenderer;
	detail::MeshRendererProxy*				m_meshRendererProxy;
	detail::BasicContextState				m_state;
	detail::BasicContextState				m_backendState;
	std::stack<detail::BasicContextState>	m_stateStack;
};

LN_NAMESPACE_END
