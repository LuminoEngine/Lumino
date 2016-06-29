
#include "Internal.h"
#include <Lumino/Graphics/Texture.h>
#include <Lumino/Graphics/VertexBuffer.h>
#include <Lumino/Graphics/IndexBuffer.h>
#include <Lumino/Graphics/Shader.h>
#include <Lumino/Graphics/SwapChain.h>
#include <Lumino/Graphics/ContextInterface.h>
#include <Lumino/Graphics/Brush.h>
#include "RendererImpl.h"
#include "GraphicsManager.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace detail
{

//==============================================================================
// ContextState
//==============================================================================


//------------------------------------------------------------------------------
ContextState::ContextState()
	: m_fillBrush(nullptr)
{
}

//------------------------------------------------------------------------------
ContextState::~ContextState()
{
	for (Texture* t : m_renderTargets) {
		LN_SAFE_RELEASE(t);
	}
	LN_SAFE_RELEASE(depthBuffer);
	//LN_SAFE_RELEASE(vertexBuffer);
	//LN_SAFE_RELEASE(indexBuffer);
	LN_SAFE_RELEASE(m_ownerShader);
	LN_SAFE_RELEASE(m_fillBrush);
}

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
Texture* ContextState::GetRenderTarget(int index) const
{
	return m_renderTargets[index];
}

//------------------------------------------------------------------------------
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

//------------------------------------------------------------------------------
void ContextState::SetFillBrush(Brush* brush)
{
	LN_REFOBJ_SET(m_fillBrush, brush);
	modifiedFlags |= ContextStateFlags::CommonState;
}


} // namespace detail
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
