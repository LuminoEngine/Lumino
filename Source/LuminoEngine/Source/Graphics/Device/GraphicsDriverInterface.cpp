
#include "../../Internal.h"
#include "../../EngineDiagCore.h"
#include "GraphicsDriverInterface.h"

LN_NAMESPACE_BEGIN
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
	, m_viewportRect(0, 0, -1, -1)
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
void IRenderer::begin()
{
	if (LN_REQUIRE(!m_rendering)) return;
	onBeginRendering();
	m_rendering = true;
}

//------------------------------------------------------------------------------
void IRenderer::end()
{
	if (LN_REQUIRE(m_rendering)) return;
	onEndRendering();
	m_rendering = false;
}

//------------------------------------------------------------------------------
void IRenderer::setRenderTarget(int index, ITexture* target)
{
	if (LN_REQUIRE_RANGE(index, 0, Graphics::MaxMultiRenderTargets)) return;
	if (m_currentRenderTargets[index] != target)
	{
		m_currentRenderTargets[index] = target;
		m_modifiedFlags |= Modified_FrameBuffer;
	}
}

//------------------------------------------------------------------------------
ITexture* IRenderer::getRenderTarget(int index)
{
	if (LN_REQUIRE_RANGE(index, 0, Graphics::MaxMultiRenderTargets)) return nullptr;
	return m_currentRenderTargets[index];
}

//------------------------------------------------------------------------------
void IRenderer::setDepthBuffer(ITexture* buffer)
{
	if (m_currentDepthBuffer != buffer)
	{
		m_currentDepthBuffer = buffer;
		m_modifiedFlags |= Modified_FrameBuffer;
	}
}

//------------------------------------------------------------------------------
ITexture* IRenderer::getDepthBuffer()
{
	return m_currentDepthBuffer;
}

void IRenderer::setViewport(const RectI& rect)
{
	if (rect != m_viewportRect)
	{
		m_viewportRect = rect;
		m_modifiedFlags |= Modified_Viewport;
	}
}

//------------------------------------------------------------------------------
void IRenderer::setVertexDeclaration(IVertexDeclaration* vertexDeclaration)
{
	if (m_currentVertexDeclaration != vertexDeclaration)
	{
		m_currentVertexDeclaration = vertexDeclaration;
		m_modifiedFlags |= Modified_VertexDeclaration;
	}
}

//------------------------------------------------------------------------------
IVertexDeclaration* IRenderer::getVertexDeclaration() const
{
	return m_currentVertexDeclaration;
}

//------------------------------------------------------------------------------
void IRenderer::setVertexBuffer(int streamIndex, IVertexBuffer* vertexBuffer)
{
	if (m_currentVertexBuffers.getCount() <= streamIndex)
		m_currentVertexBuffers.resize(streamIndex + 1);		// 配列が小さいので増やす

	if (m_currentVertexBuffers[streamIndex] != vertexBuffer)
	{
		m_currentVertexBuffers[streamIndex] = vertexBuffer;
		m_modifiedFlags |= Modified_VertexBuffer;
	}
}

//------------------------------------------------------------------------------
IVertexBuffer* IRenderer::getVertexBuffer(int streamIndex)
{
	return m_currentVertexBuffers[streamIndex];
}

//------------------------------------------------------------------------------
void IRenderer::setIndexBuffer(IIndexBuffer* indexBuffer)
{
	if (m_currentIndexBuffer != indexBuffer)
	{
		m_currentIndexBuffer = indexBuffer;
		m_modifiedFlags |= Modified_IndexBuffer;
	}
}

//------------------------------------------------------------------------------
void IRenderer::setShaderPass(IShaderPass* pass)
{
	// ShaderPass はたとえ同じでも次の draw で必ず apply する。
	m_currentShaderPass = pass;
}

//------------------------------------------------------------------------------
IShaderPass* IRenderer::getShaderPass() const
{
	return m_currentShaderPass;
}

//------------------------------------------------------------------------------
void IRenderer::clear(ClearFlags flags, const Color& color, float z, uint8_t stencil)
{
	// ※レンダリングターゲットと深度バッファのサイズが一致している必要がある。
	//   していない場合、エラーとならないがクリアされない。
	if (m_currentDepthBuffer != nullptr)
	{
		if (LN_REQUIRE(m_currentRenderTargets[0]->getSize() == m_currentDepthBuffer->getSize())) return;
	}

	flushStates();
	onClear(flags, color, z, stencil);
}

//------------------------------------------------------------------------------
void IRenderer::drawPrimitive(PrimitiveType primitive, int startVertex, int primitiveCount)
{
	flushStates();
	onDrawPrimitive(primitive, startVertex, primitiveCount);
	if (m_diag != nullptr)
	{
		m_diag->increaseGraphicsDeviceDrawCount();
	}
}

//------------------------------------------------------------------------------
void IRenderer::drawPrimitiveIndexed(PrimitiveType primitive, int startIndex, int primitiveCount)
{
	flushStates();
	onDrawPrimitiveIndexed(primitive, startIndex, primitiveCount);
	if (m_diag != nullptr)
	{
		m_diag->increaseGraphicsDeviceDrawCount();
	}
}

//------------------------------------------------------------------------------
void IRenderer::flushStates()
{
	if (m_modifiedFlags & Modified_FrameBuffer)
	{
		ITexture* renderTargets[Graphics::MaxMultiRenderTargets];
		for (int i = 0; i < Graphics::MaxMultiRenderTargets; i++) renderTargets[i] = m_currentRenderTargets[i];
		onUpdateFrameBuffers(renderTargets, Graphics::MaxMultiRenderTargets, m_currentDepthBuffer);
	}

	if (!m_requestedRenderState.equals(m_currentRenderState))
	{
		onUpdateRenderState(m_requestedRenderState, m_currentRenderState, false);
		m_currentRenderState = m_requestedRenderState;
	}
	if (!m_requestedDepthStencilState.equals(m_currentDepthStencilState))
	{
		onUpdateDepthStencilState(m_requestedDepthStencilState, m_currentDepthStencilState, false);
		m_currentDepthStencilState = m_requestedDepthStencilState;
	}

	// Viewport
	if (m_modifiedFlags & Modified_Viewport)
	{
		if (m_viewportRect.width < 0)
		{
			onUpdateViewport(RectI(0, 0, getRenderTarget(0)->getSize()));
		}
		else
		{
			onUpdateViewport(m_viewportRect);
		}
	}

	// VertexBuffer
	if (m_modifiedFlags & Modified_VertexBuffer ||
		m_modifiedFlags & Modified_IndexBuffer)
	{
		onUpdatePrimitiveData(m_currentVertexDeclaration, m_currentVertexBuffers, m_currentIndexBuffer);
	}

	// ShaderPass
	if (m_currentShaderPass != nullptr)
	{
		m_currentShaderPass->apply();
	}

	m_modifiedFlags = Modified_None;
}

//------------------------------------------------------------------------------
void IRenderer::enterRenderState()
{
	m_modifiedFlags = Modified_All;
	onEnterRenderState();
	onUpdateRenderState(m_requestedRenderState, m_currentRenderState, true);
	onUpdateDepthStencilState(m_requestedDepthStencilState, m_currentDepthStencilState, true);
}

//------------------------------------------------------------------------------
void IRenderer::leaveRenderState()
{
	onLeaveRenderState();
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
IShaderVariable* IShader::getVariableByName(const Char* name) const
{
	int count = getVariableCount();
	for (int i = 0; i < count; ++i)
	{
		IShaderVariable* v = getVariable(i);
		if (v->getName() == name) {
			return v;
		}
	}
	return NULL;
}

} // namespace Driver
LN_NAMESPACE_END
