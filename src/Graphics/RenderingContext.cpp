
#pragma once
#include "Internal.h"
#include "GraphicsManager.h"
#include <Lumino/Graphics/Texture.h>
#include <Lumino/Graphics/Shader.h>
#include <Lumino/Graphics/VertexBuffer.h>
#include <Lumino/Graphics/IndexBuffer.h>
#include <Lumino/Graphics/RenderingContext.h>
#include "RendererImpl.h"

#include <Lumino/Graphics/SpriteRenderer.h>
#include "PrimitiveRenderer.h"

LN_NAMESPACE_BEGIN

//=============================================================================
// RenderingContext
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RenderingContext* RenderingContext::GetContext()
{
	return GraphicsManager::GetInstance()->GetRenderingContext();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RenderingContext::RenderingContext()
	: m_manager(nullptr)
	, m_spriteRenderer(nullptr)
	, m_primitiveRenderer(nullptr)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RenderingContext::~RenderingContext()
{
	LN_SAFE_RELEASE(m_spriteRenderer);
	LN_SAFE_RELEASE(m_primitiveRenderer);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext::Initialize(GraphicsManager* manager)
{
	IContext::Initialize(manager);

	m_manager = manager;	// TODO いらないかも


	

	m_spriteRenderer = LN_NEW SpriteRenderer(manager, 8192);	// TODO:

	m_primitiveRenderer = LN_NEW detail::PrimitiveRenderer();
	m_primitiveRenderer->Initialize(manager);
	m_primitiveRenderer->SetUseInternalShader(true);	// TODO


	//const Size& size = m_state.GetRenderTarget(0)->GetSize();
	//m_primitiveRenderer->SetViewPixelSize(size);
	//m_spriteRenderer->SetViewPixelSize(size);

}

////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void RenderingContext::SetBlendMode(BlendMode mode)
//{
//	if (mode != m_state.renderState.Blend)
//	{
//		OnStateChanging();
//		m_state.renderState.Blend = mode;
//		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
//	}
//}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext::Clear(ClearFlags flags, const ColorF& color, float z, uint8_t stencil)
{
	OnDrawing(GetCommonRenderer());
	GetCommonRenderer()->Clear(flags, color, z, stencil);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext::DrawPrimitive(VertexBuffer* vertexBuffer, PrimitiveType primitive, int startVertex, int primitiveCount)
{
	OnDrawing(GetCommonRenderer());
	GetCommonRenderer()->DrawPrimitive(vertexBuffer, primitive, startVertex, primitiveCount);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext::DrawPrimitiveIndexed(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, PrimitiveType primitive, int startIndex, int primitiveCount)
{
	OnDrawing(GetCommonRenderer());
	GetCommonRenderer()->DrawPrimitiveIndexed(vertexBuffer, indexBuffer, primitive, startIndex, primitiveCount);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext::SetViewProjection(const Matrix& view, const Matrix& proj)
{
	OnStateChanging();
	m_primitiveRenderer->SetViewProjMatrix(view * proj);
	m_spriteRenderer->SetViewProjMatrix(view, proj);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext::DrawLine(const Vector3& from, const ColorF& fromColor, const Vector3& to, const ColorF& toColor)
{
	OnDrawing(m_primitiveRenderer);
	m_primitiveRenderer->DrawLine(from, fromColor, to, toColor);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext::DrawSquare(
	const Vector3& position1, const Vector2& uv1, const ColorF& color1,
	const Vector3& position2, const Vector2& uv2, const ColorF& color2,
	const Vector3& position3, const Vector2& uv3, const ColorF& color3,
	const Vector3& position4, const Vector2& uv4, const ColorF& color4)
{
	OnDrawing(m_primitiveRenderer);
	m_primitiveRenderer->DrawSquare(
		position1, uv1, color1,
		position2, uv2, color2,
		position3, uv3, color3,
		position4, uv4, color4);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext::Blt(Texture* source, RenderTarget* dest)
{
	BltInternal(source, dest, Matrix::Identity, nullptr);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext::Blt(Texture* source, RenderTarget* dest, const Matrix& transform)
{
	BltInternal(source, dest, transform, nullptr);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext::Blt(Texture* source, RenderTarget* dest, Shader* shader)
{
	BltInternal(source, dest, Matrix::Identity, shader);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext::BltInternal(Texture* source, RenderTarget* dest, const Matrix& transform, Shader* shader)
{
	// TODO: ここで null にしておかないとPrimitiveRendererが古いシェーダを適用してしまう。
	// が、内部でステートを変えてしまうのはどうなのか。。。
	SetShaderPass(nullptr);

	RenderState oldState1 = GetRenderState();
	DepthStencilState oldState2 = GetDepthStencilState();

	RenderState newState1;
	newState1.Blend = BlendMode_Alpha;	// TODO:指定できた方がいい？
	newState1.AlphaTest = false;
	SetRenderState(newState1);

	DepthStencilState newState2;
	newState2.DepthTestEnabled = false;
	newState2.DepthWriteEnabled = false;
	SetDepthStencilState(newState2);


	OnDrawing(m_primitiveRenderer);
	m_primitiveRenderer->Blt(source, dest, transform, shader);


	SetRenderState(oldState1);
	SetDepthStencilState(oldState2);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext::OnStateFlushRequested()
{
	IContext::OnStateFlushRequested();

	const Size& size = GetRenderTarget(0)->GetSize();
	m_primitiveRenderer->SetViewPixelSize(size);
	m_spriteRenderer->SetViewPixelSize(size);

	m_primitiveRenderer->SetUseInternalShader(GetShaderPass() == nullptr);
}



LN_NAMESPACE_END
