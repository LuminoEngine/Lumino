
#include "../../Internal.h"
#include "../../EngineDiagCore.h"
#include "GraphicsDriverInterface.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace Driver
{

//==============================================================================
// IRenderer
//==============================================================================

//------------------------------------------------------------------------------
IRenderer::IRenderer()
	: m_diag(nullptr)
	, m_modifiedFlags(Modified_None)
	, m_requestedRenderState()
	, m_currentRenderState()
	, m_requestedDepthStencilState()
	, m_currentDepthStencilState()
	, m_currentVertexDeclaration()
	, m_currentVertexBuffers()
	, m_currentIndexBuffer()
	, m_currentShaderPass(nullptr)
	, m_currentRenderTargets()
	, m_currentDepthBuffer(nullptr)
	, m_rendering(false)
{
}

//------------------------------------------------------------------------------
IRenderer::~IRenderer()
{
}

//------------------------------------------------------------------------------
void IRenderer::Begin()
{
	if (LN_CHECK_STATE(!m_rendering)) return;
	OnBeginRendering();
	m_rendering = true;
}

//------------------------------------------------------------------------------
void IRenderer::End()
{
	if (LN_CHECK_STATE(m_rendering)) return;
	OnEndRendering();
	m_rendering = false;
}

//------------------------------------------------------------------------------
void IRenderer::SetRenderTarget(int index, ITexture* target)
{
	if (LN_CHECK_RANGE(index, 0, Graphics::MaxMultiRenderTargets)) return;
	if (m_currentRenderTargets[index] != target)
	{
		m_currentRenderTargets[index] = target;
		m_modifiedFlags |= Modified_FrameBuffer;
	}
}

//------------------------------------------------------------------------------
ITexture* IRenderer::GetRenderTarget(int index)
{
	if (LN_CHECK_RANGE(index, 0, Graphics::MaxMultiRenderTargets)) return nullptr;
	return m_currentRenderTargets[index];
}

//------------------------------------------------------------------------------
void IRenderer::SetDepthBuffer(ITexture* buffer)
{
	if (m_currentDepthBuffer != buffer)
	{
		m_currentDepthBuffer = buffer;
		m_modifiedFlags |= Modified_FrameBuffer;
	}
}

//------------------------------------------------------------------------------
ITexture* IRenderer::GetDepthBuffer()
{
	return m_currentDepthBuffer;
}

//------------------------------------------------------------------------------
void IRenderer::SetVertexDeclaration(IVertexDeclaration* vertexDeclaration)
{
	if (m_currentVertexDeclaration != vertexDeclaration)
	{
		m_currentVertexDeclaration = vertexDeclaration;
		m_modifiedFlags |= Modified_VertexDeclaration;
	}
}

//------------------------------------------------------------------------------
IVertexDeclaration* IRenderer::GetVertexDeclaration() const
{
	return m_currentVertexDeclaration;
}

//------------------------------------------------------------------------------
void IRenderer::SetVertexBuffer(int streamIndex, IVertexBuffer* vertexBuffer)
{
	if (m_currentVertexBuffers.GetCount() <= streamIndex)
		m_currentVertexBuffers.Resize(streamIndex + 1);		// 配列が小さいので増やす

	if (m_currentVertexBuffers[streamIndex] != vertexBuffer)
	{
		m_currentVertexBuffers[streamIndex] = vertexBuffer;
		m_modifiedFlags |= Modified_VertexBuffer;
	}
}

//------------------------------------------------------------------------------
IVertexBuffer* IRenderer::GetVertexBuffer(int streamIndex)
{
	return m_currentVertexBuffers[streamIndex];
}

//------------------------------------------------------------------------------
void IRenderer::SetIndexBuffer(IIndexBuffer* indexBuffer)
{
	if (m_currentIndexBuffer != indexBuffer)
	{
		m_currentIndexBuffer = indexBuffer;
		m_modifiedFlags |= Modified_IndexBuffer;
	}
}

//------------------------------------------------------------------------------
void IRenderer::SetShaderPass(IShaderPass* pass)
{
	// ShaderPass はたとえ同じでも次の Draw で必ず Apply する。
	m_currentShaderPass = pass;
}

//------------------------------------------------------------------------------
void IRenderer::Clear(ClearFlags flags, const Color& color, float z, uint8_t stencil)
{
	// ※レンダリングターゲットと深度バッファのサイズが一致している必要がある。
	//   していない場合、エラーとならないがクリアされない。
	if (m_currentDepthBuffer != nullptr)
	{
		if (LN_CHECK_STATE(m_currentRenderTargets[0]->GetSize() == m_currentDepthBuffer->GetSize())) return;
	}

	FlushStates();
	OnClear(flags, color, z, stencil);
}

//------------------------------------------------------------------------------
void IRenderer::DrawPrimitive(PrimitiveType primitive, int startVertex, int primitiveCount)
{
	FlushStates();
	OnDrawPrimitive(primitive, startVertex, primitiveCount);
	if (m_diag != nullptr) m_diag->IncreaseGraphicsDeviceDrawCount();
}

//------------------------------------------------------------------------------
void IRenderer::DrawPrimitiveIndexed(PrimitiveType primitive, int startIndex, int primitiveCount)
{
	FlushStates();
	OnDrawPrimitiveIndexed(primitive, startIndex, primitiveCount);
	if (m_diag != nullptr) m_diag->IncreaseGraphicsDeviceDrawCount();
}

//------------------------------------------------------------------------------
void IRenderer::FlushStates()
{
	if (m_modifiedFlags & Modified_FrameBuffer)
	{
		ITexture* renderTargets[Graphics::MaxMultiRenderTargets];
		for (int i = 0; i < Graphics::MaxMultiRenderTargets; i++) renderTargets[i] = m_currentRenderTargets[i];
		OnUpdateFrameBuffers(renderTargets, Graphics::MaxMultiRenderTargets, m_currentDepthBuffer);
	}

	if (!m_requestedRenderState.Equals(m_currentRenderState))
	{
		OnUpdateRenderState(m_requestedRenderState, m_currentRenderState, false);
		m_currentRenderState = m_requestedRenderState;
	}
	if (!m_requestedDepthStencilState.Equals(m_currentDepthStencilState))
	{
		OnUpdateDepthStencilState(m_requestedDepthStencilState, m_currentDepthStencilState, false);
		m_currentDepthStencilState = m_requestedDepthStencilState;
	}

	// VertexBuffer
	if (m_modifiedFlags & Modified_VertexBuffer ||
		m_modifiedFlags & Modified_IndexBuffer)
	{
		OnUpdatePrimitiveData(m_currentVertexDeclaration, m_currentVertexBuffers, m_currentIndexBuffer);
	}

	// ShaderPass
	if (m_currentShaderPass != nullptr)
	{
		m_currentShaderPass->Apply();
	}

	m_modifiedFlags = Modified_None;
}

//------------------------------------------------------------------------------
void IRenderer::EnterRenderState()
{
	m_modifiedFlags = Modified_All;
	OnEnterRenderState();
	OnUpdateRenderState(m_requestedRenderState, m_currentRenderState, true);
	OnUpdateDepthStencilState(m_requestedDepthStencilState, m_currentDepthStencilState, true);
}

//------------------------------------------------------------------------------
void IRenderer::LeaveRenderState()
{
	OnLeaveRenderState();
}

//==============================================================================
// ITexture
//==============================================================================

//------------------------------------------------------------------------------
ITexture::~ITexture()
{
}


//==============================================================================
// IShader
//==============================================================================

//------------------------------------------------------------------------------
IShaderVariable* IShader::GetVariableByName(const TCHAR* name) const
{
	int count = GetVariableCount();
	for (int i = 0; i < count; ++i)
	{
		IShaderVariable* v = GetVariable(i);
		if (v->GetName() == name) {
			return v;
		}
	}
	return NULL;
}

} // namespace Driver
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
