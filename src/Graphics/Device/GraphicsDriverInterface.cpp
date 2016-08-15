
#include "../../Internal.h"
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
	: m_modifiedFlags(Modified_None)
	, m_requestedRenderState()
	, m_currentRenderState()
	, m_requestedDepthStencilState()
	, m_currentDepthStencilState()
	, m_currentVertexDeclaration()
	, m_currentVertexBuffers()
	, m_currentIndexBuffer()
{
}

//------------------------------------------------------------------------------
IRenderer::~IRenderer()
{
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
void IRenderer::SetIndexBuffer(IIndexBuffer* indexBuffer)
{
	if (m_currentIndexBuffer != indexBuffer)
	{
		m_currentIndexBuffer = indexBuffer;
		m_modifiedFlags |= Modified_IndexBuffer;
	}
}

//------------------------------------------------------------------------------
void IRenderer::FlushStates()
{
	printf("IRenderer::FlushStates()\n");
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
}

////==============================================================================
//// ITexture2D
////==============================================================================
//
////------------------------------------------------------------------------------
//ITexture2D::~ITexture2D()
//{
//}
//
////==============================================================================
//// ITexture3D
////==============================================================================
//
////------------------------------------------------------------------------------
//ITexture3D::~ITexture3D()
//{
//}

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
