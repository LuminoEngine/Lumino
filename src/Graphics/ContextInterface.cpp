
#include "Internal.h"
#include <Lumino/Graphics/Texture.h>
#include <Lumino/Graphics/VertexBuffer.h>
#include <Lumino/Graphics/IndexBuffer.h>
#include <Lumino/Graphics/Shader.h>
#include <Lumino/Graphics/ContextInterface.h>
#include "RendererImpl.h"
#include "GraphicsManager.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace detail
{

//=============================================================================
// ContextState
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ContextState::~ContextState()
{
	for (Texture* t : m_renderTargets) {
		LN_SAFE_RELEASE(t);
	}
	LN_SAFE_RELEASE(depthBuffer);
	//LN_SAFE_RELEASE(vertexBuffer);
	//LN_SAFE_RELEASE(indexBuffer);
	LN_SAFE_RELEASE(m_ownerShader);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ContextState::SetRenderTarget(int index, Texture* texture)
{
	if (index == 0 && texture == nullptr)
	{
		// index0 は null であってはならない
		LN_THROW(0, ArgumentException);
	}

	if (m_renderTargets[index] != texture)
	{
		LN_REFOBJ_SET(m_renderTargets[index], texture);

		if (index == 0)
		{
			const Size& size = texture->GetSize();
			viewport.Set(0, 0, size.Width, size.Height);
		}

		modifiedFlags |= ContextStateFlags::CommonState;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Texture* ContextState::GetRenderTarget(int index) const
{
	return m_renderTargets[index];
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ContextState::SetShaderPass(ShaderPass* pass)
{
	if (m_shaderPass != pass || (pass != nullptr && pass->GetOwnerShader()->IsModifiedVariables()))
	{
		if (pass != nullptr)
		{
			LN_REFOBJ_SET(m_ownerShader, pass->GetOwnerShader());
			m_shaderPass = pass;
		}
		else
		{
			m_ownerShader = nullptr;
			m_shaderPass = nullptr;
		}
		modifiedFlags |= ContextStateFlags::ShaderPass;
	}
}

//=============================================================================
// IContext
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
IContext::IContext()
	: m_manager(nullptr)
	, m_activeRendererPloxy(nullptr)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
IContext::~IContext()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void IContext::Initialize(GraphicsManager* manager)
{
	LN_CHECK_ARGS_RETURN(manager != nullptr);
	m_manager = manager;
	m_ploxy = m_manager->GetRenderer();
	m_activeRendererPloxy = nullptr;

	// ステート初期値
	m_state.SetRenderTarget(0, m_manager->GetMainSwapChain()->GetBackBuffer());
	LN_REFOBJ_SET(m_state.depthBuffer, m_manager->GetMainSwapChain()->GetBackBufferDepth());

	// 最初は全部変更状態
	m_state.modifiedFlags = detail::ContextStateFlags::All;
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void IContext::SetRenderState(const RenderState& state)
{
	if (!m_state.renderState.Equals(state))
	{
		OnStateChanging();
		m_state.renderState = state;
		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const RenderState& IContext::GetRenderState() const
{
	return m_state.renderState;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void IContext::SetDepthStencilState(const DepthStencilState& state)
{
	if (!m_state.depthStencilState.Equals(state))
	{
		OnStateChanging();
		m_state.depthStencilState = state;
		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const DepthStencilState& IContext::GetDepthStencilState() const
{
	return m_state.depthStencilState;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void IContext::SetRenderTarget(int index, Texture* texture)
{
	if (texture != m_state.GetRenderTarget(index))
	{
		OnStateChanging();
		m_state.SetRenderTarget(index, texture);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Texture* IContext::GetRenderTarget(int index) const
{
	return m_state.GetRenderTarget(index);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void IContext::SetDepthBuffer(Texture* depthBuffer)
{
	if (m_state.depthBuffer != depthBuffer)
	{
		OnStateChanging();
		LN_REFOBJ_SET(m_state.depthBuffer, depthBuffer);
		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Texture* IContext::GetDepthBuffer() const
{
	return m_state.depthBuffer;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void IContext::SetViewport(const Rect& rect)
{
	if (m_state.viewport != rect)
	{
		OnStateChanging();
		m_state.viewport = rect;
		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const Rect& IContext::GetViewport() const
{
	return m_state.viewport;
}

////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void IContext::SetVertexBuffer(VertexBuffer* vertexBuffer)
//{
//	if (m_state.vertexBuffer != vertexBuffer)
//	{
//		OnStateChanging();
//		LN_REFOBJ_SET(m_state.vertexBuffer, vertexBuffer);
//		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
//	}
//}
//
////-----------------------------------------------------------------------------
////
////-----------------------------------------------------------------------------
//void IContext::SetIndexBuffer(IndexBuffer* indexBuffer)
//{
//	if (m_state.indexBuffer != indexBuffer)
//	{
//		OnStateChanging();
//		LN_REFOBJ_SET(m_state.indexBuffer, indexBuffer);
//		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
//	}
//}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void IContext::SetShaderPass(ShaderPass* pass)
{
	if (pass != m_state.GetShaderPass() || (pass != nullptr && pass->GetOwnerShader()->IsModifiedVariables()))
	{
		OnStateChanging();
		m_state.SetShaderPass(pass);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ShaderPass* IContext::GetShaderPass() const
{
	return m_state.GetShaderPass();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void IContext::Flush()
{
	m_manager->SwitchActiveContext(this);
	OnPrimitiveFlushRequested();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void IContext::OnStateFlushRequested()
{
	m_ploxy->FlushState(GetContextState());
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void IContext::OnPrimitiveFlushRequested()
{
	if (m_activeRendererPloxy != nullptr)
	{
		m_activeRendererPloxy->Flush();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void IContext::OnStateChanging()
{
	m_manager->SwitchActiveContext(this);
	OnPrimitiveFlushRequested();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void IContext::OnDrawing(detail::IRendererPloxy* rendererPloxy)
{
	m_manager->SwitchActiveContext(this);
	if (m_state.modifiedFlags != detail::ContextStateFlags::None)
	{
		OnStateFlushRequested();
		m_state.modifiedFlags = detail::ContextStateFlags::None;
	}
	SwitchActiveRendererPloxy(rendererPloxy);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void IContext::OnDeactivated()
{
	SwitchActiveRendererPloxy(nullptr);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void IContext::OnActivated()
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
void IContext::SwitchActiveRendererPloxy(detail::IRendererPloxy* rendererPloxy)
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

} // namespace detail
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
