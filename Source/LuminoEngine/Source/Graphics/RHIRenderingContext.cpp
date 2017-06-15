
#include "../Internal.h"
#include "GraphicsManager.h"
#include <Lumino/Graphics/VertexDeclaration.h>
#include <Lumino/Graphics/SwapChain.h>
#include <Lumino/Graphics/Shader.h>
#include "RHIRenderingContext.h"
#include "Internal.h"
#include "RenderingCommand.h"
#include "RenderingThread.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// Renderer
//==============================================================================

//------------------------------------------------------------------------------
static detail::RHIRenderingContext* getRenderer()
{
	return detail::GraphicsManager::getInstance()->getRenderer();
}


//==============================================================================
// Details::Renderer
//==============================================================================

LN_NAMESPACE_GRAPHICS_BEGIN
namespace detail
{

//------------------------------------------------------------------------------
RHIRenderingContext::RHIRenderingContext(detail::GraphicsManager* manager)
	: m_internal(manager->getGraphicsDevice()->getRenderer())
	, m_primaryCommandList(NULL)
	, m_currentRenderState()
	, m_currentDepthStencilState()
	, m_currentDepthBuffer(NULL)
{
	memset(m_currentRenderTargets, 0, sizeof(m_currentRenderTargets));
	GraphicsResourceObject::initialize();

	//if (m_manager->getRenderingType() == GraphicsRenderingType::Deferred) {
	m_primaryCommandList = LN_NEW detail::RenderingCommandList(manager, Thread::getCurrentThreadId());
	//}
}

//------------------------------------------------------------------------------
RHIRenderingContext::~RHIRenderingContext()
{
	if (m_primaryCommandList != NULL)
	{
		m_primaryCommandList->clearCommands();	// present される前に解放されることの対策
		LN_SAFE_RELEASE(m_primaryCommandList);
	}

	for (int i = 0; i < Graphics::MaxMultiRenderTargets; ++i) {
		LN_SAFE_RELEASE(m_currentRenderTargets[i]);
	}
	LN_SAFE_RELEASE(m_currentDepthBuffer);
}

//------------------------------------------------------------------------------
void RHIRenderingContext::begin()
{
	bool isStandalone = m_manager->getGraphicsDevice()->isStandalone();

	LN_ENQUEUE_RENDER_COMMAND_2(
		begin, m_manager,
		Driver::IRenderer*, m_internal,
		bool, isStandalone,
		{
			if (isStandalone)
			{
				m_internal->begin();
				m_internal->enterRenderState();
			}
			else
			{
				m_internal->enterRenderState();
			}
		});
}

//------------------------------------------------------------------------------
void RHIRenderingContext::end()
{
	bool isStandalone = m_manager->getGraphicsDevice()->isStandalone();

	LN_ENQUEUE_RENDER_COMMAND_2(
		end, m_manager,
		Driver::IRenderer*, m_internal,
		bool, isStandalone,
		{
			if (isStandalone)
			{
				m_internal->leaveRenderState();
				m_internal->end();
			}
			else
			{
				m_internal->leaveRenderState();
			}
		});
}

//------------------------------------------------------------------------------
void RHIRenderingContext::setRenderState(const RenderState& state)
{
	m_currentRenderState = state;

	LN_ENQUEUE_RENDER_COMMAND_2(
		setRenderState, m_manager,
		Driver::IRenderer*, m_internal,
		RenderState, state,
		{
			m_internal->setRenderState(state);
		});
}

//------------------------------------------------------------------------------
const RenderState& RHIRenderingContext::getRenderState() const
{
	return m_currentRenderState;
}

//------------------------------------------------------------------------------
void RHIRenderingContext::setDepthStencilState(const DepthStencilState& state)
{
	m_currentDepthStencilState = state;

	LN_ENQUEUE_RENDER_COMMAND_2(
		setDepthStencilState, m_manager,
		Driver::IRenderer*, m_internal,
		DepthStencilState, state,
		{
			m_internal->setDepthStencilState(state);
		});
}

//------------------------------------------------------------------------------
const DepthStencilState& RHIRenderingContext::getDepthStencilState() const
{
	return m_currentDepthStencilState;
}

//------------------------------------------------------------------------------
void RHIRenderingContext::setRenderTarget(int index, Texture* texture)
{
	Driver::ITexture* t = (texture != NULL) ? texture->resolveDeviceObject() : NULL;
	LN_REFOBJ_SET(m_currentRenderTargets[index], texture);

	LN_ENQUEUE_RENDER_COMMAND_3(
		setRenderTarget, m_manager,
		Driver::IRenderer*, m_internal,
		int, index,
		RefPtr<Driver::ITexture>, t,
		{
			m_internal->setRenderTarget(index, t);
		});
}

//------------------------------------------------------------------------------
Texture* RHIRenderingContext::getRenderTarget(int index) const
{
	LN_THROW(0 <= index && index < Graphics::MaxMultiRenderTargets, ArgumentException);
	return m_currentRenderTargets[index];
}

//------------------------------------------------------------------------------
void RHIRenderingContext::setDepthBuffer(DepthBuffer* depthBuffer)
{
	Driver::ITexture* t = (depthBuffer != nullptr) ? depthBuffer->resolveDeviceObject() : nullptr;
	LN_REFOBJ_SET(m_currentDepthBuffer, depthBuffer);

	LN_ENQUEUE_RENDER_COMMAND_2(
		setDepthBuffer, m_manager,
		Driver::IRenderer*, m_internal,
		RefPtr<Driver::ITexture>, t,
		{
			m_internal->setDepthBuffer(t);
		});
}

//------------------------------------------------------------------------------
DepthBuffer* RHIRenderingContext::getDepthBuffer() const
{
	return m_currentDepthBuffer;
}

//------------------------------------------------------------------------------
//void RHIRenderingContext::setViewport(const RectI& rect)
//{
//	//LN_CALL_RENDERER_COMMAND(setViewport, SetViewportCommand, rect);
//	m_currentViewport = rect;
//
//	LN_ENQUEUE_RENDER_COMMAND_2(
//		setViewport, m_manager,
//		Driver::IRenderer*, m_internal,
//		RectI, rect,
//		{
//			m_internal->setViewport(rect);
//		});
//}

////------------------------------------------------------------------------------
//const RectI& RHIRenderingContext::getViewport()
//{
//	return m_currentViewport;
//}

////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
//void RHIRenderingContext::setVertexBuffer(VertexBuffer* vertexBuffer)
//{
//	Driver::IVertexBuffer* t = (vertexBuffer != NULL) ? Helper::getDeviceObject(vertexBuffer) : NULL;
//	//LN_CALL_RENDERER_COMMAND(setVertexBuffer, SetVertexBufferCommand, t);
//
//	LN_ENQUEUE_RENDER_COMMAND_2(
//		setVertexBuffer, m_manager,
//		Driver::IRenderer*, m_internal,
//		RefPtr<Driver::IVertexBuffer>, t,
//		{
//			m_internal->setVertexBuffer(t);
//		});
//}
//
////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
//void RHIRenderingContext::setIndexBuffer(IndexBuffer* indexBuffer)
//{
//	Driver::IIndexBuffer* t = (indexBuffer != NULL) ? Helper::getDeviceObject(indexBuffer) : NULL;
//	//LN_CALL_RENDERER_COMMAND(setIndexBuffer, SetIndexBufferCommand, t);
//
//	LN_ENQUEUE_RENDER_COMMAND_2(
//		setIndexBuffer, m_manager,
//		Driver::IRenderer*, m_internal,
//		RefPtr<Driver::IIndexBuffer>, t,
//		{
//			m_internal->setIndexBuffer(t);
//		});
//}

//------------------------------------------------------------------------------
void RHIRenderingContext::setShaderPass(ShaderPass* pass)
{
	if (pass != nullptr)
		pass->apply();
}

//------------------------------------------------------------------------------
void RHIRenderingContext::clear(ClearFlags flags, const Color& color, float z, uint8_t stencil)
{
	verifyFrameBuffers();

	LN_ENQUEUE_RENDER_COMMAND_5(
		clear, m_manager,
		Driver::IRenderer*, m_internal,
		ClearFlags, flags,
		Color, color, 
		float, z, 
		uint8_t, stencil,
		{
			m_internal->clear(flags, color, z, stencil);
		});

	//LN_CALL_RENDERER_COMMAND(Clear, ClearCommand, flags, color, z, stencil);
}

//------------------------------------------------------------------------------
void RHIRenderingContext::drawPrimitive(VertexDeclaration* vertexDeclaration, VertexBuffer* vertexBuffer, PrimitiveType primitive, int startVertex, int primitiveCount)
{
	verifyFrameBuffers();

	Driver::IVertexDeclaration* decl = (vertexDeclaration != nullptr) ? vertexDeclaration->getDeviceObject() : nullptr;
	Driver::IVertexBuffer* vb = (vertexBuffer != nullptr) ? vertexBuffer->resolveRHIObject() : nullptr;
	LN_ENQUEUE_RENDER_COMMAND_6(
		drawPrimitive, m_manager,
		Driver::IRenderer*, m_internal,
		Driver::IVertexDeclaration*, decl,
		Driver::IVertexBuffer*, vb,
		PrimitiveType, primitive,
		int, startVertex,
		int, primitiveCount,
		{
			m_internal->setVertexDeclaration(decl);
			m_internal->setVertexBuffer(0, vb);
			m_internal->drawPrimitive(primitive, startVertex, primitiveCount);
		});

	//LN_CALL_RENDERER_COMMAND(drawPrimitive, DrawPrimitiveCommand, primitive, startVertex, primitiveCount);
}

//------------------------------------------------------------------------------
void RHIRenderingContext::drawPrimitiveIndexed(VertexDeclaration* vertexDeclaration, VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, PrimitiveType primitive, int startIndex, int primitiveCount)
{
	verifyFrameBuffers();

	Driver::IVertexDeclaration* decl = (vertexDeclaration != nullptr) ? vertexDeclaration->getDeviceObject() : nullptr;
	Driver::IVertexBuffer* vb = (vertexBuffer != nullptr) ? vertexBuffer->resolveRHIObject() : nullptr;
	Driver::IIndexBuffer* ib = (indexBuffer != nullptr) ? indexBuffer->resolveRHIObject() : nullptr;
	LN_ENQUEUE_RENDER_COMMAND_7(
		drawPrimitiveIndexed, m_manager,
		Driver::IRenderer*, m_internal,
		Driver::IVertexDeclaration*, decl,
		Driver::IVertexBuffer*, vb,
		Driver::IIndexBuffer*, ib,
		PrimitiveType, primitive,
		int, startIndex,
		int, primitiveCount,
		{
			m_internal->setVertexDeclaration(decl);
			m_internal->setVertexBuffer(0, vb);
			m_internal->setIndexBuffer(ib);
			m_internal->drawPrimitiveIndexed(primitive, startIndex, primitiveCount);
		});
	//LN_CALL_RENDERER_COMMAND(drawPrimitiveIndexed, DrawPrimitiveIndexedCommand, primitive, startIndex, primitiveCount);
}

//------------------------------------------------------------------------------
void RHIRenderingContext::flushState(const detail::ContextState& state)
{
	// TODO: 1つのコマンドで一括設定したい
	if (state.modifiedFlags.TestFlag(detail::ContextStateFlags::CommonState))
	{
		setRenderState(state.renderState);
		setDepthStencilState(state.depthStencilState);
		for (int i = 0; i < Graphics::MaxMultiRenderTargets; ++i) {
			setRenderTarget(i, state.getRenderTarget(i));
		}
		setDepthBuffer(state.depthBuffer);
		//setViewport(state.viewport);
		//setVertexBuffer(state.vertexBuffer);
		//setIndexBuffer(state.indexBuffer);
	}
	if (state.modifiedFlags.TestFlag(detail::ContextStateFlags::ShaderPass))
	{
		if (state.getShaderPass() != nullptr)
		{
			state.getShaderPass()->apply();
		}
	}
}

//------------------------------------------------------------------------------
void RHIRenderingContext::flush()
{

}

//------------------------------------------------------------------------------
void RHIRenderingContext::onChangeDevice(Driver::IGraphicsDevice* device)
{
	if (device == NULL) {
	}
	else {
		m_internal = device->getRenderer();
	}
}

//------------------------------------------------------------------------------
void RHIRenderingContext::presentCommandList(SwapChain* swapChain)
{

	m_primaryCommandList->addCommand<PresentCommand>(swapChain);
	
	auto* renderingThread = m_manager->getRenderingThread();
	renderingThread->pushRenderingCommand(m_primaryCommandList);

	// swapChain の持っているコマンドリストとスワップ。それをプライマリにする。
	auto* t = swapChain->m_commandList;
	swapChain->m_commandList = m_primaryCommandList;
	m_primaryCommandList = t;

	m_primaryCommandList->clearCommands();
}

//------------------------------------------------------------------------------
void RHIRenderingContext::verifyFrameBuffers()
{
	// レンダリングターゲットと深度バッファのサイズが一致している必要がある。
	if (m_currentDepthBuffer != nullptr)
	{
		if (LN_CHECK_STATE(m_currentRenderTargets[0]->getSize() == m_currentDepthBuffer->getSize())) return;
	}
}

} // namespace detail
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
