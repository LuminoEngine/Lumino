
#pragma once
#include "Common.h"
#include "RenderState.h"
#include "Color.h"
#include "ContextInterface.h"

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

	//void SetBlendMode(BlendMode mode);



	/**
		@brief	現在設定されているレンダリングターゲット、深度バッファをクリアします。
	*/
	void Clear(ClearFlags flags, const ColorF& color, float z = 1.0f, uint8_t stencil = 0x00);

	/**
		@brief	現在設定されている頂点バッファを使用してプリミティブをレンダリングします。
	*/
	void DrawPrimitive(VertexBuffer* vertexBuffer, PrimitiveType primitive, int startVertex, int primitiveCount);

	/**
		@brief	現在設定されている頂点バッファとインデックスバッファを使用してプリミティブをレンダリングします。
	*/
	void DrawPrimitiveIndexed(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, PrimitiveType primitive, int startIndex, int primitiveCount);	// TODO: DrawIndexedPrimitive



	// pass が null のときに使われるデフォルトシェーダ用
	void SetViewProjection(const Matrix& view, const Matrix& proj);

	void DrawLine(const Vector3& from, const ColorF& fromColor, const Vector3& to, const ColorF& toColor);

	void DrawSquare(
		const Vector3& position1, const Vector2& uv1, const ColorF& color1,
		const Vector3& position2, const Vector2& uv2, const ColorF& color2,
		const Vector3& position3, const Vector2& uv3, const ColorF& color3,
		const Vector3& position4, const Vector2& uv4, const ColorF& color4);


	// ステート固定。
	void Blt(Texture* source, RenderTarget* dest);
	void Blt(Texture* source, RenderTarget* dest, Shader* shader);


	SpriteRenderer* GetSpriteRenderer() const { return m_spriteRenderer; }

LN_INTERNAL_ACCESS:
	RenderingContext2();
	virtual ~RenderingContext2();
	void Initialize(GraphicsManager* manager);




	virtual void OnStateFlushRequested();

private:
	GraphicsManager*			m_manager;

	SpriteRenderer*				m_spriteRenderer;
	detail::PrimitiveRenderer*	m_primitiveRenderer;

	//bool						m_stateModified;
};

LN_NAMESPACE_END
