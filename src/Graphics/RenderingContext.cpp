
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
// RenderingContext2
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RenderingContext2* RenderingContext2::GetContext()
{
	return GraphicsManager::GetInstance()->GetRenderingContext();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RenderingContext2::RenderingContext2()
	: m_manager(nullptr)
	, m_spriteRenderer(nullptr)
	, m_primitiveRenderer(nullptr)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
RenderingContext2::~RenderingContext2()
{
	LN_SAFE_RELEASE(m_spriteRenderer);
	LN_SAFE_RELEASE(m_primitiveRenderer);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext2::Initialize(GraphicsManager* manager)
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
//void RenderingContext2::SetBlendMode(BlendMode mode)
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
void RenderingContext2::Clear(ClearFlags flags, const ColorF& color, float z, uint8_t stencil)
{
	OnDrawing(GetCommonRenderer());
	GetCommonRenderer()->Clear(flags, color, z, stencil);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext2::DrawPrimitive(VertexBuffer* vertexBuffer, PrimitiveType primitive, int startVertex, int primitiveCount)
{
	OnDrawing(GetCommonRenderer());
	GetCommonRenderer()->DrawPrimitive(vertexBuffer, primitive, startVertex, primitiveCount);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext2::DrawPrimitiveIndexed(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, PrimitiveType primitive, int startIndex, int primitiveCount)
{
	OnDrawing(GetCommonRenderer());
	GetCommonRenderer()->DrawPrimitiveIndexed(vertexBuffer, indexBuffer, primitive, startIndex, primitiveCount);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext2::SetViewProjection(const Matrix& view, const Matrix& proj)
{
	OnStateChanging();
	m_primitiveRenderer->SetViewProjMatrix(view * proj);
	m_spriteRenderer->SetViewProjMatrix(view, proj);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext2::DrawLine(const Vector3& from, const ColorF& fromColor, const Vector3& to, const ColorF& toColor)
{
	OnDrawing(m_primitiveRenderer);
	m_primitiveRenderer->DrawLine(from, fromColor, to, toColor);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext2::DrawSquare(
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
void RenderingContext2::Blt(Texture* source, RenderTarget* dest)
{
	Blt(source, dest, nullptr);
	//Texture* oldTarget = GetRenderTarget(0);
	//SetRenderTarget(0, dest);


	//m_primitiveRenderer->DrawSquare(
	//	Vector3(-1, 1, 0), Vector2(0, 0), ColorF::White,
	//	Vector3(-1, -1, 0), Vector2(0, 1), ColorF::White,
	//	Vector3(1, 1, 0), Vector2(1, 0), ColorF::White,
	//	Vector3(1,-1, 0), Vector2(1, 1), ColorF::White);

	//SetRenderTarget(0, oldTarget);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext2::Blt(Texture* source, RenderTarget* dest, Shader* shader)
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
	m_primitiveRenderer->Blt(source, dest, shader);


	SetRenderState(oldState1);
	SetDepthStencilState(oldState2);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext2::OnStateFlushRequested()
{
	IContext::OnStateFlushRequested();

	const Size& size = GetRenderTarget(0)->GetSize();
	m_primitiveRenderer->SetViewPixelSize(size);
	m_spriteRenderer->SetViewPixelSize(size);

	m_primitiveRenderer->SetUseInternalShader(GetShaderPass() == nullptr);
}



LN_NAMESPACE_END
