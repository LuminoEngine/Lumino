
#pragma once

#include "../Internal.h"
#include "../../include/Lumino/Graphics/GraphicsManager.h"
#include "../../include/Lumino/Graphics/Renderer.h"
#include "../../include/Lumino/Graphics/SwapChain.h"
#include "RenderingCommand.h"
#include "RenderingThread.h"

namespace Lumino
{
namespace Graphics
{

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Renderer::Renderer(GraphicsManager* manager)
	: m_manager(manager)
	, m_primaryCommandList(LN_NEW RenderingCommandList())
	, m_currentDepthBuffer(NULL)
{
	memset(m_currentRenderTargets, 0, sizeof(m_currentRenderTargets));
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Renderer::~Renderer()
{
	m_primaryCommandList->PostExecute();	// Present される前に解放されることの対策
	LN_SAFE_RELEASE(m_primaryCommandList);

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
	m_primaryCommandList->AddCommand<SetRenderStateCommand>(state);
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
void Renderer::SetRenderTarget(int index, Texture* texture)
{
	m_primaryCommandList->AddCommand<SetRenderTargetCommand>(index, texture);
	//SetRenderTargetCommand::AddCommand(m_primaryCommandList, index, texture);
	//m_primaryCommandList->SetRenderTarget(index, texture);
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
	m_primaryCommandList->AddCommand<SetDepthBufferCommand>(depthBuffer);
	//SetDepthBufferCommand::AddCommand(m_primaryCommandList, depthBuffer);
	//m_primaryCommandList->SetDepthBuffer(depthBuffer);
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
	m_primaryCommandList->AddCommand<SetViewportCommand>(rect);
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
	m_primaryCommandList->AddCommand<SetVertexBufferCommand>(vertexBuffer);
	//SetVertexBufferCommand::AddCommand(m_primaryCommandList, vertexBuffer);
	//m_primaryCommandList->SetVertexBuffer(vertexBuffer);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Renderer::SetIndexBuffer(IndexBuffer* indexBuffer)
{
	m_primaryCommandList->AddCommand<SetIndexBufferCommand>(indexBuffer);
	//SetIndexBufferCommand::AddCommand(m_primaryCommandList, indexBuffer);
	//m_primaryCommandList->SetIndexBuffer(indexBuffer);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Renderer::Clear(bool target, bool depth, const ColorF& color, float z)
{
	m_primaryCommandList->AddCommand<ClearCommand>(target, depth, color, z);
	//ClearCommand::AddCommand(m_primaryCommandList, target, depth, color, z);
	//m_primaryCommandList->Clear(target, depth, color, z);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Renderer::DrawPrimitive(PrimitiveType primitive, int startVertex, int primitiveCount)
{
	m_primaryCommandList->AddCommand<DrawPrimitiveCommand>(primitive, startVertex, primitiveCount);
	//DrawPrimitiveCommand::AddCommand(m_primaryCommandList, primitive, startVertex, primitiveCount);
	//m_primaryCommandList->DrawPrimitive(primitive, startVertex, primitiveCount);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Renderer::DrawPrimitiveIndexed(PrimitiveType primitive, int startIndex, int primitiveCount)
{
	m_primaryCommandList->AddCommand<DrawPrimitiveIndexedCommand>(primitive, startIndex, primitiveCount);
	//DrawPrimitiveIndexedCommand::AddCommand(m_primaryCommandList, primitive, startIndex, primitiveCount);
	//m_primaryCommandList->DrawPrimitiveIndexed(primitive, startIndex, primitiveCount);
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
	//PresentCommand::AddCommand(m_primaryCommandList, swapChain);
	//m_primaryCommandList->Present(swapChain);
	m_manager->m_renderingThread->PushRenderingCommand(m_primaryCommandList);

	// swapChain の持っているコマンドリストとスワップ。それをプライマリにする。
	RenderingCommandList* t = swapChain->m_commandList;
	swapChain->m_commandList = m_primaryCommandList;
	m_primaryCommandList = t;

	if (m_manager->m_renderingThread != NULL)
	{

	}
}

} // namespace Graphics
} // namespace Lumino
