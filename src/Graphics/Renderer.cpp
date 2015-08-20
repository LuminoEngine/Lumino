
#pragma once

#include "../Internal.h"
#include "../../include/Lumino/Graphics/GraphicsManager.h"
#include "../../include/Lumino/Graphics/Renderer.h"
#include "../../include/Lumino/Graphics/SwapChain.h"
#include "Internal.h"
#include "RenderingCommand.h"
#include "RenderingThread.h"
#include "GraphicsHelper.h"

#define LN_CALL_RENDERER_COMMAND(func, command, ...) \
	if (m_manager->GetRenderingType() == RenderingType::Deferred) { \
		m_primaryCommandList->AddCommand<command>(__VA_ARGS__); \
	} \
	else { \
		m_internal->func(__VA_ARGS__); \
	}

namespace Lumino
{
namespace Graphics
{

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Renderer::Renderer(GraphicsManager* manager)
	: m_manager(manager)
	, m_primaryCommandList(NULL)
	, m_currentDepthBuffer(NULL)
{
	memset(m_currentRenderTargets, 0, sizeof(m_currentRenderTargets));

	if (m_manager->GetRenderingType() == RenderingType::Deferred) {
		m_primaryCommandList = LN_NEW RenderingCommandList();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Renderer::~Renderer()
{
	if (m_primaryCommandList != NULL)
	{
		m_primaryCommandList->PostExecute();	// Present される前に解放されることの対策
		LN_SAFE_RELEASE(m_primaryCommandList);
	}

	for (int i = 0; i < MaxMultiRenderTargets; ++i) {
		LN_SAFE_RELEASE(m_currentRenderTargets[i]);
	}
	LN_SAFE_RELEASE(m_currentDepthBuffer);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Renderer::SetRenderState(const RenderState& state)
{
	LN_CALL_RENDERER_COMMAND(SetRenderState, SetRenderStateCommand, state);
	m_currentRenderState = state;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const RenderState& Renderer::GetRenderState() const
{
	return m_currentRenderState;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Renderer::SetDepthStencilState(const DepthStencilState& state)
{
	LN_CALL_RENDERER_COMMAND(SetDepthStencilState, SetDepthStencilStateCommand, state);
	m_currentDepthStencilState = state;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const DepthStencilState& Renderer::GetDepthStencilState() const
{
	return m_currentDepthStencilState;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Renderer::SetRenderTarget(int index, Texture* texture)
{
	Device::ITexture* t = (texture != NULL) ? Helper::GetDeviceObject(texture) : NULL;
	LN_CALL_RENDERER_COMMAND(SetRenderTarget, SetRenderTargetCommand, index, t);
	LN_REFOBJ_SET(m_currentRenderTargets[index], texture);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Texture* Renderer::GetRenderTarget(int index) const
{
	LN_THROW(0 <= index && index < MaxMultiRenderTargets, ArgumentException);
	return m_currentRenderTargets[index];
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Renderer::SetDepthBuffer(Texture* depthBuffer)
{
	Device::ITexture* t = (depthBuffer != NULL) ? Helper::GetDeviceObject(depthBuffer) : NULL;
	LN_CALL_RENDERER_COMMAND(SetDepthBuffer, SetDepthBufferCommand, t);
	LN_REFOBJ_SET(m_currentDepthBuffer, depthBuffer);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Texture* Renderer::GetDepthBuffer() const
{
	return m_currentDepthBuffer;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Renderer::SetViewport(const Rect& rect)
{
	LN_CALL_RENDERER_COMMAND(SetViewport, SetViewportCommand, rect);
	m_currentViewport = rect;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
const Rect& Renderer::GetViewport()
{
	return m_currentViewport;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Renderer::SetVertexBuffer(VertexBuffer* vertexBuffer)
{
	Device::IVertexBuffer* t = (vertexBuffer != NULL) ? Helper::GetDeviceObject(vertexBuffer) : NULL;
	LN_CALL_RENDERER_COMMAND(SetVertexBuffer, SetVertexBufferCommand, t);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Renderer::SetIndexBuffer(IndexBuffer* indexBuffer)
{
	Device::IIndexBuffer* t = (indexBuffer != NULL) ? Helper::GetDeviceObject(indexBuffer) : NULL;
	LN_CALL_RENDERER_COMMAND(SetIndexBuffer, SetIndexBufferCommand, t);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Renderer::Clear(Graphics::ClearFlags flags, const ColorF& color, float z, uint8_t stencil)
{
	LN_CALL_RENDERER_COMMAND(Clear, ClearCommand, flags, color, z, stencil);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Renderer::DrawPrimitive(PrimitiveType primitive, int startVertex, int primitiveCount)
{
	LN_CALL_RENDERER_COMMAND(DrawPrimitive, DrawPrimitiveCommand, primitive, startVertex, primitiveCount);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Renderer::DrawPrimitiveIndexed(PrimitiveType primitive, int startIndex, int primitiveCount)
{
	LN_CALL_RENDERER_COMMAND(DrawPrimitiveIndexed, DrawPrimitiveIndexedCommand, primitive, startIndex, primitiveCount);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Renderer::PresentCommandList(SwapChain* swapChain)
{
	// ごく稀に RenderingCommandList::Execute() でイテレータアクセス assert する問題があった。
	// この assert が発生する原因は、イテレート中に他のスレッドから Add とかされた時。
	// でも、パッと見原因になりそうなところが見つからなかったので、もしかしたら
	// キャッシュにリストのポインタが残っていたことが原因かもしれない。
	// 念のためここでキャッシュをフラッシュし、様子を見る。
	Threading::MutexScopedLock lock(m_lockPresentCommandList);

	m_primaryCommandList->AddCommand<PresentCommand>(swapChain);
	
	auto* renderingThread = Helper::GetRenderingThread(m_manager);
	renderingThread->PushRenderingCommand(m_primaryCommandList);

	// swapChain の持っているコマンドリストとスワップ。それをプライマリにする。
	RenderingCommandList* t = swapChain->m_commandList;
	swapChain->m_commandList = m_primaryCommandList;
	m_primaryCommandList = t;
}

} // namespace Graphics
} // namespace Lumino
