
#include "../Internal.h"
#include "GraphicsManager.h"
#include <Lumino/Graphics/VertexDeclaration.h>
#include <Lumino/Graphics/SwapChain.h>
#include <Lumino/Graphics/Shader.h>
#include "CoreGraphicsRenderFeature.h"
#include "Internal.h"
#include "RenderingCommand.h"
#include "RenderingThread.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// Renderer
//==============================================================================

//------------------------------------------------------------------------------
static detail::CoreGraphicsRenderFeature* getRenderer()
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
CoreGraphicsRenderFeature::CoreGraphicsRenderFeature(detail::GraphicsManager* manager)
	: m_internal(manager->getGraphicsDevice()->getRenderer())
	, m_primaryCommandList(NULL)
	, m_lockPresentCommandList()
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
CoreGraphicsRenderFeature::~CoreGraphicsRenderFeature()
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
void CoreGraphicsRenderFeature::begin()
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
void CoreGraphicsRenderFeature::end()
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
void CoreGraphicsRenderFeature::setRenderState(const RenderState& state)
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
const RenderState& CoreGraphicsRenderFeature::getRenderState() const
{
	return m_currentRenderState;
}

//------------------------------------------------------------------------------
void CoreGraphicsRenderFeature::setDepthStencilState(const DepthStencilState& state)
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
const DepthStencilState& CoreGraphicsRenderFeature::getDepthStencilState() const
{
	return m_currentDepthStencilState;
}

//------------------------------------------------------------------------------
void CoreGraphicsRenderFeature::setRenderTarget(int index, Texture* texture)
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
Texture* CoreGraphicsRenderFeature::getRenderTarget(int index) const
{
	LN_THROW(0 <= index && index < Graphics::MaxMultiRenderTargets, ArgumentException);
	return m_currentRenderTargets[index];
}

//------------------------------------------------------------------------------
void CoreGraphicsRenderFeature::setDepthBuffer(DepthBuffer* depthBuffer)
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
DepthBuffer* CoreGraphicsRenderFeature::getDepthBuffer() const
{
	return m_currentDepthBuffer;
}

//------------------------------------------------------------------------------
//void CoreGraphicsRenderFeature::setViewport(const RectI& rect)
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
//const RectI& CoreGraphicsRenderFeature::getViewport()
//{
//	return m_currentViewport;
//}

////------------------------------------------------------------------------------
////
////------------------------------------------------------------------------------
//void CoreGraphicsRenderFeature::setVertexBuffer(VertexBuffer* vertexBuffer)
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
//void CoreGraphicsRenderFeature::setIndexBuffer(IndexBuffer* indexBuffer)
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
void CoreGraphicsRenderFeature::setShaderPass(ShaderPass* pass)
{
	if (pass != nullptr)
		pass->apply();
}

//------------------------------------------------------------------------------
void CoreGraphicsRenderFeature::clear(ClearFlags flags, const Color& color, float z, uint8_t stencil)
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
void CoreGraphicsRenderFeature::drawPrimitive(VertexDeclaration* vertexDeclaration, VertexBuffer* vertexBuffer, PrimitiveType primitive, int startVertex, int primitiveCount)
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
void CoreGraphicsRenderFeature::drawPrimitiveIndexed(VertexDeclaration* vertexDeclaration, VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, PrimitiveType primitive, int startIndex, int primitiveCount)
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
void CoreGraphicsRenderFeature::flushState(const detail::ContextState& state)
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
void CoreGraphicsRenderFeature::flush()
{

}

//------------------------------------------------------------------------------
void CoreGraphicsRenderFeature::onChangeDevice(Driver::IGraphicsDevice* device)
{
	if (device == NULL) {
	}
	else {
		m_internal = device->getRenderer();
	}
}

//------------------------------------------------------------------------------
void CoreGraphicsRenderFeature::presentSwapChain(SwapChain* swapChain)
{
	if (m_manager->getRenderingType() == GraphicsRenderingType::Immediate)
	{
		swapChain->PresentInternal();

		// コマンドは使っていないが一時メモリは使うので解放とかをやっておく
		m_primaryCommandList->clearCommands();
		//getPrimaryRenderingCommandList()->clearCommands();
	}
	else
	{
		// ごく稀に RenderingCommandList::Execute() でイテレータアクセス assert する問題があった。
		// この assert が発生する原因は、イテレート中に他のスレッドから Add とかされた時。
		// でも、パッと見原因になりそうなところが見つからなかったので、もしかしたら
		// キャッシュにリストのポインタが残っていたことが原因かもしれない。
		// 念のためここでキャッシュをフラッシュし、様子を見る。
		MutexScopedLock lock(m_lockPresentCommandList);

		// 前回この SwapChain から発行したコマンドリストがまだ処理中である。待ち状態になるまで待機する。
		swapChain->WaitForPresent();

		// 実行状態にする。present コマンドが実行された後、コマンドリストクラスから True がセットされる。
		// ※ presentCommandList() の前に false にしておかないとダメ。
		//    後で false にすると、presentCommandList() と同時に全部のコマンドが実行されて、描画スレッドから
		//    true がセットされるのに、その後 false をセットしてしまうことがある。
		swapChain->m_waiting.setFalse();

		m_manager->getGraphicsDevice()->flushResource();

		// Primary コマンドリストの末尾に present を追加し、キューへ追加する
		/*getRenderer()->*/presentCommandList(swapChain);
	}
}

//------------------------------------------------------------------------------
void CoreGraphicsRenderFeature::presentCommandList(SwapChain* swapChain)
{
	//m_primaryCommandList->addCommand<PresentCommand>(swapChain);

	auto* cur = m_primaryCommandList;

	// swapChain の持っているコマンドリストとスワップ。それをプライマリにする。
	auto* t = swapChain->m_commandList;
	swapChain->m_commandList = m_primaryCommandList;
	m_primaryCommandList = t;

	
	auto* renderingThread = m_manager->getRenderingThread();
	renderingThread->pushRenderingCommand(cur, swapChain);


	m_primaryCommandList->clearCommands();
}

//------------------------------------------------------------------------------
void CoreGraphicsRenderFeature::verifyFrameBuffers()
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
