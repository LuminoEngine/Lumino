
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
void IRenderer::FlushStates()
{
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
