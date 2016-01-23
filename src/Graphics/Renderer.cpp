
#pragma once
#include "../Internal.h"
#include <Lumino/Graphics/GraphicsManager.h>
#include <Lumino/Graphics/Renderer.h>
#include <Lumino/Graphics/SwapChain.h>
#include <Lumino/Graphics/Shader.h>
#include "RendererImpl.h"
#include "Internal.h"
#include "RenderingCommand.h"
#include "RenderingThread.h"
#include "GraphicsHelper.h"

LN_NAMESPACE_BEGIN

//=============================================================================
// Renderer
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
static Details::Renderer* GetRenderer()
{
	// TODO: 未初期化エラー
	return GraphicsManager::Instance->GetRenderer();
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Renderer::BeginRendering() { GetRenderer()->Begin(); }
void Renderer::EndRendering() { GetRenderer()->End(); }
void Renderer::SetRenderState(const RenderState& state) { GetRenderer()->SetRenderState(state); }
const RenderState& Renderer::GetRenderState() { return GetRenderer()->GetRenderState(); }
void Renderer::SetDepthStencilState(const DepthStencilState& state) { GetRenderer()->SetDepthStencilState(state); }
const DepthStencilState& Renderer::GetDepthStencilState() { return GetRenderer()->GetDepthStencilState(); }
void Renderer::SetRenderTarget(int index, Texture* texture) { GetRenderer()->SetRenderTarget(index, texture); }
Texture* Renderer::GetRenderTarget(int index) { return GetRenderer()->GetRenderTarget(index); }
void Renderer::SetDepthBuffer(Texture* depthBuffer) { GetRenderer()->SetDepthBuffer(depthBuffer); }
Texture* Renderer::GetDepthBuffer() { return GetRenderer()->GetDepthBuffer(); }
void Renderer::SetViewport(const Rect& rect) { GetRenderer()->SetViewport(rect); }
const Rect& Renderer::GetViewport() { return GetRenderer()->GetViewport(); }
void Renderer::SetVertexBuffer(VertexBuffer* vertexBuffer) { GetRenderer()->SetVertexBuffer(vertexBuffer); }
void Renderer::SetIndexBuffer(IndexBuffer* indexBuffer) { GetRenderer()->SetIndexBuffer(indexBuffer); }
// TODO: Get はいらない？
void Renderer::Clear(ClearFlags flags, const ColorF& color, float zf, uint8_t stencil) { GetRenderer()->Clear(flags, color, zf, stencil); }
void Renderer::DrawPrimitive(PrimitiveType primitive, int startVertex, int primitiveCount) { GetRenderer()->DrawPrimitive(primitive, startVertex, primitiveCount); }
void Renderer::DrawPrimitiveIndexed(PrimitiveType primitive, int startIndex, int primitiveCount) { GetRenderer()->DrawPrimitiveIndexed(primitive, startIndex, primitiveCount); }

//=============================================================================
// Details::Renderer
//=============================================================================

LN_NAMESPACE_GRAPHICS_BEGIN
namespace Details
{

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Renderer::Renderer(GraphicsManager* manager)
	: m_internal(manager->GetGraphicsDevice()->GetRenderer())
	, m_primaryCommandList(NULL)
	, m_currentRenderState()
	, m_currentDepthStencilState()
	, m_currentDepthBuffer(NULL)
	, m_currentViewport()
	, m_lockPresentCommandList()
{
	memset(m_currentRenderTargets, 0, sizeof(m_currentRenderTargets));
	GraphicsResourceObject::Initialize(manager);

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
void Renderer::Begin()
{
	if (m_manager->GetRenderingType() == RenderingType::Deferred) {
		m_primaryCommandList->AddCommand<Renderer_BeginCommand>();
	}
	else {
		if (m_manager->GetGraphicsDevice()->IsStandalone()) {
			m_internal->Begin();
			m_internal->EnterRenderState();
		}
		else {
			m_internal->EnterRenderState();
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Renderer::End()
{
	if (m_manager->GetRenderingType() == RenderingType::Deferred) {
		m_primaryCommandList->AddCommand<Renderer_EndCommand>();
	}
	else {
		if (m_manager->GetGraphicsDevice()->IsStandalone()) {
			m_internal->LeaveRenderState();
			m_internal->End();
		}
		else {
			m_internal->LeaveRenderState();
		}
	}
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
	Driver::ITexture* t = (texture != NULL) ? texture->GetDeviceObject() : NULL;
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
	Driver::ITexture* t = (depthBuffer != NULL) ? depthBuffer->GetDeviceObject() : NULL;
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
	Driver::IVertexBuffer* t = (vertexBuffer != NULL) ? Helper::GetDeviceObject(vertexBuffer) : NULL;
	LN_CALL_RENDERER_COMMAND(SetVertexBuffer, SetVertexBufferCommand, t);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Renderer::SetIndexBuffer(IndexBuffer* indexBuffer)
{
	Driver::IIndexBuffer* t = (indexBuffer != NULL) ? Helper::GetDeviceObject(indexBuffer) : NULL;
	LN_CALL_RENDERER_COMMAND(SetIndexBuffer, SetIndexBufferCommand, t);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Renderer::Clear(ClearFlags flags, const ColorF& color, float z, uint8_t stencil)
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
void Renderer::FlushState(const detail::ContextState& state)
{
	// TODO: 1つのコマンドで一括設定したい
	if (state.modifiedFlags.TestFlag(detail::ContextStateFlags::CommonState))
	{
		SetRenderState(state.renderState);
		SetDepthStencilState(state.depthStencilState);
		for (int i = 0; i < detail::ContextState::MaxMultiRenderTargets; ++i) {
			SetRenderTarget(i, state.GetRenderTarget(i));
		}
		SetDepthBuffer(state.depthBuffer);
		SetViewport(state.viewport);
		SetVertexBuffer(state.vertexBuffer);
		SetIndexBuffer(state.indexBuffer);
	}
	if (state.modifiedFlags.TestFlag(detail::ContextStateFlags::ShaderPass))
	{
		if (state.GetShaderPass() != nullptr)
		{
			state.GetShaderPass()->Apply();
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Renderer::Flush()
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Renderer::OnChangeDevice(Driver::IGraphicsDevice* device)
{
	if (device == NULL) {
	}
	else {
		m_internal = device->GetRenderer();
	}
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
	
	auto* renderingThread = m_manager->GetRenderingThread();
	renderingThread->PushRenderingCommand(m_primaryCommandList);

	// swapChain の持っているコマンドリストとスワップ。それをプライマリにする。
	RenderingCommandList* t = swapChain->m_commandList;
	swapChain->m_commandList = m_primaryCommandList;
	m_primaryCommandList = t;
}

} // namespace Details
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
