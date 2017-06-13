
#include "../Internal.h"
#include "GraphicsManager.h"
#include <Lumino/Graphics/VertexDeclaration.h>
#include <Lumino/Graphics/Renderer.h>
#include <Lumino/Graphics/SwapChain.h>
#include <Lumino/Graphics/Shader.h>
#include "RendererImpl.h"
#include "Internal.h"
#include "RenderingCommand.h"
#include "RenderingThread.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// Renderer
//==============================================================================

//------------------------------------------------------------------------------
static Details::Renderer* getRenderer()
{
	return detail::GraphicsManager::getInstance()->getRenderer();
}


//==============================================================================
// Details::Renderer
//==============================================================================

LN_NAMESPACE_GRAPHICS_BEGIN
namespace Details
{

//------------------------------------------------------------------------------
Renderer::Renderer(detail::GraphicsManager* manager)
	: m_internal(manager->getGraphicsDevice()->getRenderer())
	, m_primaryCommandList(NULL)
	, m_currentRenderState()
	, m_currentDepthStencilState()
	, m_currentDepthBuffer(NULL)
{
	memset(m_currentRenderTargets, 0, sizeof(m_currentRenderTargets));
	GraphicsResourceObject::initialize();

	//if (m_manager->getRenderingType() == GraphicsRenderingType::Deferred) {
	m_primaryCommandList = LN_NEW RenderingCommandList(manager);
	//}
}

//------------------------------------------------------------------------------
Renderer::~Renderer()
{
	if (m_primaryCommandList != NULL)
	{
		m_primaryCommandList->postExecute();	// Present される前に解放されることの対策
		LN_SAFE_RELEASE(m_primaryCommandList);
	}

	for (int i = 0; i < Graphics::MaxMultiRenderTargets; ++i) {
		LN_SAFE_RELEASE(m_currentRenderTargets[i]);
	}
	LN_SAFE_RELEASE(m_currentDepthBuffer);
}

//------------------------------------------------------------------------------
void Renderer::begin()
{
	bool isStandalone = m_manager->getGraphicsDevice()->IsStandalone();

	LN_ENQUEUE_RENDER_COMMAND_2(
		begin, m_manager,
		Driver::IRenderer*, m_internal,
		bool, isStandalone,
		{
			if (isStandalone)
			{
				m_internal->begin();
				m_internal->EnterRenderState();
			}
			else
			{
				m_internal->EnterRenderState();
			}
		});
}

//------------------------------------------------------------------------------
void Renderer::end()
{
	bool isStandalone = m_manager->getGraphicsDevice()->IsStandalone();

	LN_ENQUEUE_RENDER_COMMAND_2(
		end, m_manager,
		Driver::IRenderer*, m_internal,
		bool, isStandalone,
		{
			if (isStandalone)
			{
				m_internal->LeaveRenderState();
				m_internal->end();
			}
			else
			{
				m_internal->LeaveRenderState();
			}
		});
}

//------------------------------------------------------------------------------
void Renderer::setRenderState(const RenderState& state)
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
const RenderState& Renderer::getRenderState() const
{
	return m_currentRenderState;
}

//------------------------------------------------------------------------------
void Renderer::setDepthStencilState(const DepthStencilState& state)
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
const DepthStencilState& Renderer::getDepthStencilState() const
{
	return m_currentDepthStencilState;
}

//------------------------------------------------------------------------------
void Renderer::setRenderTarget(int index, Texture* texture)
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
Texture* Renderer::getRenderTarget(int index) const
{
	LN_THROW(0 <= index && index < Graphics::MaxMultiRenderTargets, ArgumentException);
	return m_currentRenderTargets[index];
}

//------------------------------------------------------------------------------
void Renderer::setDepthBuffer(DepthBuffer* depthBuffer)
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
DepthBuffer* Renderer::getDepthBuffer() const
{
	return m_currentDepthBuffer;
}

//------------------------------------------------------------------------------
//void Renderer::SetViewport(const RectI& rect)
//{
//	//LN_CALL_RENDERER_COMMAND(SetViewport, SetViewportCommand, rect);
//	m_currentViewport = rect;
//
//	LN_ENQUEUE_RENDER_COMMAND_2(
//		SetViewport, m_manager,
//		Driver::IRenderer*, m_internal,
//		RectI, rect,
//		{
//			m_internal->SetViewport(rect);
//		});
//}

////------------------------------------------------------------------------------
//const RectI& Renderer::GetViewport()
//{
//	return m_currentViewport;
//}

////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
//void Renderer::SetVertexBuffer(VertexBuffer* vertexBuffer)
//{
//	Driver::IVertexBuffer* t = (vertexBuffer != NULL) ? Helper::getDeviceObject(vertexBuffer) : NULL;
//	//LN_CALL_RENDERER_COMMAND(SetVertexBuffer, SetVertexBufferCommand, t);
//
//	LN_ENQUEUE_RENDER_COMMAND_2(
//		SetVertexBuffer, m_manager,
//		Driver::IRenderer*, m_internal,
//		RefPtr<Driver::IVertexBuffer>, t,
//		{
//			m_internal->SetVertexBuffer(t);
//		});
//}
//
////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
//void Renderer::SetIndexBuffer(IndexBuffer* indexBuffer)
//{
//	Driver::IIndexBuffer* t = (indexBuffer != NULL) ? Helper::getDeviceObject(indexBuffer) : NULL;
//	//LN_CALL_RENDERER_COMMAND(SetIndexBuffer, SetIndexBufferCommand, t);
//
//	LN_ENQUEUE_RENDER_COMMAND_2(
//		SetIndexBuffer, m_manager,
//		Driver::IRenderer*, m_internal,
//		RefPtr<Driver::IIndexBuffer>, t,
//		{
//			m_internal->SetIndexBuffer(t);
//		});
//}

//------------------------------------------------------------------------------
void Renderer::setShaderPass(ShaderPass* pass)
{
	if (pass != nullptr)
		pass->apply();
}

//------------------------------------------------------------------------------
void Renderer::clear(ClearFlags flags, const Color& color, float z, uint8_t stencil)
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
void Renderer::drawPrimitive(VertexDeclaration* vertexDeclaration, VertexBuffer* vertexBuffer, PrimitiveType primitive, int startVertex, int primitiveCount)
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
			m_internal->SetVertexDeclaration(decl);
			m_internal->SetVertexBuffer(0, vb);
			m_internal->drawPrimitive(primitive, startVertex, primitiveCount);
		});

	//LN_CALL_RENDERER_COMMAND(drawPrimitive, DrawPrimitiveCommand, primitive, startVertex, primitiveCount);
}

//------------------------------------------------------------------------------
void Renderer::drawPrimitiveIndexed(VertexDeclaration* vertexDeclaration, VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, PrimitiveType primitive, int startIndex, int primitiveCount)
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
			m_internal->SetVertexDeclaration(decl);
			m_internal->SetVertexBuffer(0, vb);
			m_internal->SetIndexBuffer(ib);
			m_internal->drawPrimitiveIndexed(primitive, startIndex, primitiveCount);
		});
	//LN_CALL_RENDERER_COMMAND(drawPrimitiveIndexed, DrawPrimitiveIndexedCommand, primitive, startIndex, primitiveCount);
}

//------------------------------------------------------------------------------
void Renderer::flushState(const detail::ContextState& state)
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
		//SetViewport(state.viewport);
		//SetVertexBuffer(state.vertexBuffer);
		//SetIndexBuffer(state.indexBuffer);
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
void Renderer::flush()
{

}

//------------------------------------------------------------------------------
void Renderer::onChangeDevice(Driver::IGraphicsDevice* device)
{
	if (device == NULL) {
	}
	else {
		m_internal = device->getRenderer();
	}
}

//------------------------------------------------------------------------------
void Renderer::presentCommandList(SwapChain* swapChain)
{

	m_primaryCommandList->addCommand<PresentCommand>(swapChain);
	
	auto* renderingThread = m_manager->getRenderingThread();
	renderingThread->pushRenderingCommand(m_primaryCommandList);

	// swapChain の持っているコマンドリストとスワップ。それをプライマリにする。
	RenderingCommandList* t = swapChain->m_commandList;
	swapChain->m_commandList = m_primaryCommandList;
	m_primaryCommandList = t;
}

//------------------------------------------------------------------------------
void Renderer::verifyFrameBuffers()
{
	// レンダリングターゲットと深度バッファのサイズが一致している必要がある。
	if (m_currentDepthBuffer != nullptr)
	{
		if (LN_CHECK_STATE(m_currentRenderTargets[0]->getSize() == m_currentDepthBuffer->getSize())) return;
	}
}

} // namespace Details
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
