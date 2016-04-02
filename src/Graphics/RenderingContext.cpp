
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
	, m_activeRendererPloxy(nullptr)
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
	m_manager = manager;
	m_ploxy = m_manager->GetRenderer();

	// ステート初期値
	m_state.SetRenderTarget(0, m_manager->GetMainSwapChain()->GetBackBuffer());
	LN_REFOBJ_SET(m_state.depthBuffer, m_manager->GetMainSwapChain()->GetBackBufferDepth());

	

	m_spriteRenderer = LN_NEW SpriteRenderer(manager, 8192);	// TODO:

	m_primitiveRenderer = LN_NEW detail::PrimitiveRenderer();
	m_primitiveRenderer->Initialize(manager);
	m_primitiveRenderer->SetUseInternalShader(true);	// TODO


	const Size& size = m_state.GetRenderTarget(0)->GetSize();
	m_primitiveRenderer->SetViewPixelSize(size);
	m_spriteRenderer->SetViewPixelSize(size);

	m_state.modifiedFlags = detail::ContextStateFlags::All;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext2::SetBlendMode(BlendMode mode)
{
	m_manager->SwitchActiveContext(this);
	if (mode != m_state.renderState.Blend)
	{
		Flush();
		m_state.renderState.Blend = mode;
		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext2::SetRenderState(const RenderState& state)
{
	m_manager->SwitchActiveContext(this);
	if (!m_state.renderState.Equals(state))
	{
		Flush();
		m_state.renderState = state;
		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const RenderState& RenderingContext2::GetRenderState() const
{
	return m_state.renderState;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext2::SetDepthStencilState(const DepthStencilState& state)
{
	m_manager->SwitchActiveContext(this);
	if (!m_state.depthStencilState.Equals(state))
	{
		Flush();
		m_state.depthStencilState = state;
		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const DepthStencilState& RenderingContext2::GetDepthStencilState() const
{
	return m_state.depthStencilState;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext2::SetRenderTarget(int index, Texture* texture)
{
	m_manager->SwitchActiveContext(this);
	if (texture != m_state.GetRenderTarget(index))
	{
		Flush();
		m_state.SetRenderTarget(index, texture);
		if (index == 0)
		{
			const Size& size = m_state.GetRenderTarget(0)->GetSize();
			m_primitiveRenderer->SetViewPixelSize(size);
			m_spriteRenderer->SetViewPixelSize(size);
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Texture* RenderingContext2::GetRenderTarget(int index) const
{
	return m_state.GetRenderTarget(index);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext2::SetDepthBuffer(Texture* depthBuffer)
{
	m_manager->SwitchActiveContext(this);
	if (m_state.depthBuffer != depthBuffer)
	{
		Flush();
		LN_REFOBJ_SET(m_state.depthBuffer, depthBuffer);
		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Texture* RenderingContext2::GetDepthBuffer() const
{
	return m_state.depthBuffer;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext2::SetViewport(const Rect& rect)
{
	m_manager->SwitchActiveContext(this);
	if (m_state.viewport != rect)
	{
		Flush();
		m_state.viewport = rect;
		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const Rect& RenderingContext2::GetViewport() const
{
	return m_state.viewport;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext2::SetVertexBuffer(VertexBuffer* vertexBuffer)
{
	m_manager->SwitchActiveContext(this);
	if (m_state.vertexBuffer != vertexBuffer)
	{
		Flush();
		LN_REFOBJ_SET(m_state.vertexBuffer, vertexBuffer);
		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext2::SetIndexBuffer(IndexBuffer* indexBuffer)
{
	m_manager->SwitchActiveContext(this);
	if (m_state.indexBuffer != indexBuffer)
	{
		Flush();
		LN_REFOBJ_SET(m_state.indexBuffer, indexBuffer);
		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext2::SetShaderPass(ShaderPass* pass)
{
	m_manager->SwitchActiveContext(this);
	if (pass != m_state.GetShaderPass() || (pass != nullptr && pass->GetOwnerShader()->IsModifiedVariables()))
	{
		Flush();
		m_state.SetShaderPass(pass);
		m_primitiveRenderer->SetUseInternalShader(pass == nullptr);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ShaderPass* RenderingContext2::GetShaderPass() const
{
	return m_state.GetShaderPass();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext2::Clear(ClearFlags flags, const ColorF& color, float z, uint8_t stencil)
{
	m_manager->SwitchActiveContext(this);
	CheckFlushRendererState();
	SwitchActiveRendererPloxy(m_ploxy);
	m_ploxy->Clear(flags, color, z, stencil);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext2::DrawPrimitive(PrimitiveType primitive, int startVertex, int primitiveCount)
{
	m_manager->SwitchActiveContext(this);
	CheckFlushRendererState();
	SwitchActiveRendererPloxy(m_ploxy);
	m_ploxy->DrawPrimitive(primitive, startVertex, primitiveCount);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext2::DrawPrimitiveIndexed(PrimitiveType primitive, int startIndex, int primitiveCount)
{
	m_manager->SwitchActiveContext(this);
	CheckFlushRendererState();
	SwitchActiveRendererPloxy(m_ploxy);
	m_ploxy->DrawPrimitiveIndexed(primitive, startIndex, primitiveCount);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext2::SetViewProjection(const Matrix& view, const Matrix& proj)
{
	m_primitiveRenderer->SetViewProjMatrix(view * proj);
	m_spriteRenderer->SetViewProjMatrix(view, proj);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext2::DrawLine(const Vector3& from, const ColorF& fromColor, const Vector3& to, const ColorF& toColor)
{
	m_manager->SwitchActiveContext(this);
	CheckFlushRendererState();
	SwitchActiveRendererPloxy(m_primitiveRenderer);
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
	// TODO: この3行定型文?
	m_manager->SwitchActiveContext(this);
	CheckFlushRendererState();
	SwitchActiveRendererPloxy(m_primitiveRenderer);

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
	// TODO: この3行定型文?
	m_manager->SwitchActiveContext(this);
	CheckFlushRendererState();
	SwitchActiveRendererPloxy(m_primitiveRenderer);

	m_primitiveRenderer->Blt(source, dest, shader);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext2::Flush()
{
	if (m_activeRendererPloxy != nullptr)
	{
		m_activeRendererPloxy->Flush();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext2::CheckFlushRendererState()
{
	if (m_state.modifiedFlags != detail::ContextStateFlags::None)
	{
		m_ploxy->FlushState(m_state);
		m_state.modifiedFlags = detail::ContextStateFlags::None;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext2::SwitchActiveRendererPloxy(detail::IRendererPloxy* rendererPloxy)
{
	if (rendererPloxy != m_activeRendererPloxy)
	{
		if (m_activeRendererPloxy != nullptr)
		{
			m_activeRendererPloxy->OnDeactivated();	// 古いものを Deactivate
		}

		m_activeRendererPloxy = rendererPloxy;

		if (m_activeRendererPloxy != nullptr)
		{
			m_activeRendererPloxy->OnActivated();	// 新しいものを Activate
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext2::OnActivated()
{
	// ステート強制送信
	//m_state.Reset();
	m_state.modifiedFlags = detail::ContextStateFlags::All;
	//m_ploxy->FlushState(m_state);
	//m_state.modifiedFlags = detail::ContextStateFlags::None;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext2::OnDeactivated()
{
	SwitchActiveRendererPloxy(nullptr);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void RenderingContext2::RequestFlush()
{
	Flush();
}

LN_NAMESPACE_END
