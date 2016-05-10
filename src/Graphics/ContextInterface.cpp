
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
			viewport.Set(0, 0, size.width, size.height);
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
			LN_SAFE_RELEASE(m_ownerShader);
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

void IContext::SetAlphaBlendEnabled(bool enabled)
{
	if (m_state.renderState.alphaBlendEnabled != enabled)
	{
		OnStateChanging();
		m_state.renderState.alphaBlendEnabled = enabled;
		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
	}
}
void IContext::SetBlendOp(BlendOp op)
{
	if (m_state.renderState.blendOp != op)
	{
		OnStateChanging();
		m_state.renderState.blendOp = op;
		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
	}
}
void IContext::SetSourceBlend(BlendFactor blend)
{
	if (m_state.renderState.sourceBlend != blend)
	{
		OnStateChanging();
		m_state.renderState.sourceBlend = blend;
		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
	}
}
void IContext::SetDestinationBlend(BlendFactor blend)
{
	if (m_state.renderState.destinationBlend != blend)
	{
		OnStateChanging();
		m_state.renderState.destinationBlend = blend;
		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
	}
}
void IContext::SetBlendMode(BlendMode mode)
{
	switch (mode)
	{
		// もっといろいろ http://d.hatena.ne.jp/Ko-Ta/20070618/p1
		case BlendMode::Normal:
			SetAlphaBlendEnabled(false);
			SetBlendOp(BlendOp::Add);
			SetSourceBlend(BlendFactor::One);
			SetDestinationBlend(BlendFactor::Zero);
			break;
		case BlendMode::Alpha:
			SetAlphaBlendEnabled(true);
			SetBlendOp(BlendOp::Add);
			SetSourceBlend(BlendFactor::SourceAlpha);
			SetDestinationBlend(BlendFactor::InverseSourceAlpha);
			break;
		case BlendMode::Add:
			SetAlphaBlendEnabled(true);
			SetBlendOp(BlendOp::Add);
			SetSourceBlend(BlendFactor::SourceAlpha);
			SetDestinationBlend(BlendFactor::One);
			break;
		case BlendMode::AddAlphaDisabled:
			SetAlphaBlendEnabled(true);
			SetBlendOp(BlendOp::Add);
			SetSourceBlend(BlendFactor::One);
			SetDestinationBlend(BlendFactor::One);
			break;
		case BlendMode::Subtract:
			SetAlphaBlendEnabled(true);
			SetBlendOp(BlendOp::ReverseSubtract);
			SetSourceBlend(BlendFactor::SourceAlpha);
			SetDestinationBlend(BlendFactor::One);
			break;
		case BlendMode::SubtractAlphaDisabled:
			SetAlphaBlendEnabled(true);
			SetBlendOp(BlendOp::ReverseSubtract);
			SetSourceBlend(BlendFactor::One);
			SetDestinationBlend(BlendFactor::One);
			break;
		case BlendMode::MultiplyAlphaDisabled:
			SetAlphaBlendEnabled(true);
			SetBlendOp(BlendOp::Add);
			// AlphaDisable (Alpha を別指定できない今の仕様では Alpha を考慮できない)
			SetSourceBlend(BlendFactor::Zero);
			SetDestinationBlend(BlendFactor::SourceColor);
			break;
		//case BlendMode_Screen:
		//	m_dxDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		//	m_dxDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		//	m_dxDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHASAT);
		//	m_dxDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVDESTCOLOR);
		//	m_dxDevice->SetRenderState(D3DRS_ALPHAREF, 255);
		//	break;
		//case BlendMode_Reverse:
		//	m_dxDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		//	m_dxDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		//	m_dxDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		//	m_dxDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_INVSRCCOLOR);
		//	m_dxDevice->SetRenderState(D3DRS_ALPHAREF, 1);
		//	break;
		default:
			LN_CHECK_ARGS_RETURN(0);
			break;
	}
}
void IContext::SetCullingMode(CullingMode mode)
{
	if (m_state.renderState.Culling != mode)
	{
		OnStateChanging();
		m_state.renderState.Culling = mode;
		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
	}
}
void IContext::SetFillMode(FillMode mode)
{
	if (m_state.renderState.Fill != mode)
	{
		OnStateChanging();
		m_state.renderState.Fill = mode;
		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
	}
}
void IContext::SetAlphaTestEnabled(bool enabled)
{
	if (m_state.renderState.AlphaTest != enabled)
	{
		OnStateChanging();
		m_state.renderState.AlphaTest = enabled;
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

void IContext::SetDepthTestEnabled(bool enabled)
{
	if (m_state.depthStencilState.DepthTestEnabled != enabled)
	{
		OnStateChanging();
		m_state.depthStencilState.DepthTestEnabled = enabled;
		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
	}
}
void IContext::SetDepthWriteEnabled(bool enabled)
{
	if (m_state.depthStencilState.DepthWriteEnabled != enabled)
	{
		OnStateChanging();
		m_state.depthStencilState.DepthWriteEnabled = enabled;
		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
	}
}
void IContext::SetDepthTestFunc(CompareFunc func)
{
	if (m_state.depthStencilState.DepthTestFunc != func)
	{
		OnStateChanging();
		m_state.depthStencilState.DepthTestFunc = func;
		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
	}
}
void IContext::SetStencilEnabled(bool enabled)
{
	if (m_state.depthStencilState.StencilEnabled != enabled)
	{
		OnStateChanging();
		m_state.depthStencilState.StencilEnabled = enabled;
		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
	}
}
void IContext::SetStencilFunc(CompareFunc func)
{
	if (m_state.depthStencilState.StencilFunc != func)
	{
		OnStateChanging();
		m_state.depthStencilState.StencilFunc = func;
		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
	}
}
void IContext::SetStencilReferenceValue(uint8_t value)
{
	if (m_state.depthStencilState.StencilReferenceValue != value)
	{
		OnStateChanging();
		m_state.depthStencilState.StencilReferenceValue = value;
		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
	}
}
void IContext::SetStencilFailOp(StencilOp op)
{
	if (m_state.depthStencilState.StencilFailOp != op)
	{
		OnStateChanging();
		m_state.depthStencilState.StencilFailOp = op;
		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
	}
}
void IContext::SetStencilDepthFailOp(StencilOp op)
{
	if (m_state.depthStencilState.StencilDepthFailOp != op)
	{
		OnStateChanging();
		m_state.depthStencilState.StencilDepthFailOp = op;
		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
	}
}
void IContext::SetStencilPassOp(StencilOp op)
{
	if (m_state.depthStencilState.StencilPassOp != op)
	{
		OnStateChanging();
		m_state.depthStencilState.StencilPassOp = op;
		m_state.modifiedFlags |= detail::ContextStateFlags::CommonState;
	}
}


//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void IContext::ResetStates()
{
	SetAlphaBlendEnabled(RenderState::Default.alphaBlendEnabled);
	SetBlendOp(RenderState::Default.blendOp);
	SetSourceBlend(RenderState::Default.sourceBlend);
	SetDestinationBlend(RenderState::Default.destinationBlend);
	SetCullingMode(RenderState::Default.Culling);
	SetFillMode(RenderState::Default.Fill);
	SetAlphaTestEnabled(RenderState::Default.alphaBlendEnabled);

	SetDepthTestEnabled(DepthStencilState::Default.DepthTestEnabled);
	SetDepthWriteEnabled(DepthStencilState::Default.DepthWriteEnabled);
	SetDepthTestFunc(DepthStencilState::Default.DepthTestFunc);
	SetStencilEnabled(DepthStencilState::Default.StencilEnabled);
	SetStencilFunc(DepthStencilState::Default.StencilFunc);
	SetStencilReferenceValue(DepthStencilState::Default.StencilReferenceValue);
	SetStencilFailOp(DepthStencilState::Default.StencilFailOp);
	SetStencilDepthFailOp(DepthStencilState::Default.StencilDepthFailOp);
	SetStencilPassOp(DepthStencilState::Default.StencilPassOp);
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
