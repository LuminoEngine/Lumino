
#include "Internal.hpp"
#include <LuminoCore/Base/LinearAllocator.hpp>
#include <LuminoGraphics/GraphicsContext.hpp>
#include <LuminoGraphics/VertexLayout.hpp>
#include <LuminoGraphics/VertexBuffer.hpp>
#include <LuminoGraphics/IndexBuffer.hpp>
#include <LuminoGraphics/Texture.hpp>
#include <LuminoGraphics/DepthBuffer.hpp>
#include <LuminoGraphics/SwapChain.hpp>
#include <LuminoGraphics/GraphicsCommandBuffer.hpp>
#include <LuminoGraphics/RenderPass.hpp>
#include <LuminoGraphics/Shader.hpp>
#include <LuminoGraphics/ShaderDescriptor.hpp>
#include "GraphicsManager.hpp"
#include "RHIs/GraphicsDeviceContext.hpp"

namespace ln {

//==============================================================================
// GraphicsContext

GraphicsContext::GraphicsContext()
    : m_manager(nullptr)
    , m_commandList(nullptr)
    , m_rhiCommandList(nullptr)
    //, m_renderingType(RenderingType::Immediate)
    , m_staging()
    //, m_lastCommit()
    , m_dirtyFlags(DirtyFlags_All)
    , m_scopeState(ScopeState::Idle) {
}

GraphicsContext::~GraphicsContext() {
}

void GraphicsContext::init(/*RenderingType renderingType*/) {
    Object::init();
    //m_renderingType = renderingType;
    m_manager = detail::GraphicsManager::instance();
    // m_context = m_manager->deviceContext()->createCommandList();
    //m_recordingCommandList = makeRef<detail::RenderingCommandList>(m_manager->linearAllocatorPageManager());
    //m_executingCommandList = makeRef<detail::RenderingCommandList>(m_manager->linearAllocatorPageManager());
    //m_lastCommit.reset();
    resetState();
}

// void GraphicsContext::init(detail::ICommandList* context)
//{
//     LN_DCHECK(context);
//     Object::init();
//     m_manager = detail::EngineDomain::graphicsManager();
//     m_context = context;
//     m_recordingCommandList = makeRef<detail::RenderingCommandList>(m_manager->linearAllocatorPageManager());
//     m_executingCommandList = makeRef<detail::RenderingCommandList>(m_manager->linearAllocatorPageManager());
//     m_lastCommit.reset();
//     resetState();
// }

void GraphicsContext::resetCommandList(detail::GraphicsCommandList* commandList) {
    if (commandList) {
        // begin frame
        if (LN_REQUIRE(!m_commandList)) return;
        m_commandList = commandList;
        m_commandList->reset();
        m_rhiCommandList = m_commandList->rhiResource();
    }
    else {
        // end frame
        if (LN_REQUIRE(m_commandList)) return;
        m_commandList = nullptr;
        m_rhiCommandList = nullptr;
    }
}

//detail::RenderingCommandList* GraphicsContext::renderingCommandList() {
//    return m_recordingCommandList;
//}

void GraphicsContext::onDispose(bool explicitDisposing) {
    if (m_rhiCommandList) {
        endCommandRecoding();
        m_rhiCommandList = nullptr;
    }
    //m_lastCommit.reset();
    m_staging.reset();
    Object::onDispose(explicitDisposing);
}

void GraphicsContext::setShaderDescriptor(detail::ShaderSecondaryDescriptor* value) {
    m_staging.shaderDescriptor = value;
}

void GraphicsContext::resetState() {
    m_staging.reset();
    m_dirtyFlags = DirtyFlags_All;
}

// void GraphicsContext::beginRenderPass(RenderPass* value)
//{
// }
//
// void GraphicsContext::endRenderPass()
//{
// }

void GraphicsContext::setBlendState(const BlendStateDesc& value) {
    if (!BlendStateDesc::equals(m_staging.blendState, value)) {
        m_staging.blendState = value;
        m_dirtyFlags |= DirtyFlags_BlendState;
    }
}

void GraphicsContext::setRasterizerState(const RasterizerStateDesc& value) {
    if (!RasterizerStateDesc::equals(m_staging.rasterizerState, value)) {
        m_staging.rasterizerState = value;
        m_dirtyFlags |= DirtyFlags_RasterizerState;
    }
}

void GraphicsContext::setDepthStencilState(const DepthStencilStateDesc& value) {
    if (!DepthStencilStateDesc::equals(m_staging.depthStencilState, value)) {
        m_staging.depthStencilState = value;
        m_dirtyFlags |= DirtyFlags_DepthStencilState;
    }
}

// void GraphicsContext::setRenderTarget(int index, RenderTargetTexture* value)
//{
//     if (LN_REQUIRE_RANGE(index, 0, MaxMultiRenderTargets)) return;
//
//     if (m_staging.renderTargets[index] != value) {
//         m_staging.renderTargets[index] = value;
//         m_dirtyFlags |= DirtyFlags_Framebuffer;
//     }
//
//     if (index == 0 && value) {
//         auto rect = Rect(0, 0, value->width(), value->height());
//         setViewportRect(rect);
//         setScissorRect(rect);
//     }
// }
//
// RenderTargetTexture* GraphicsContext::renderTarget(int index) const
//{
//     if (LN_REQUIRE_RANGE(index, 0, MaxMultiRenderTargets)) return nullptr;
//     return m_staging.renderTargets[index];
// }
//
// void GraphicsContext::setDepthBuffer(DepthBuffer* value)
//{
//     if (m_staging.depthBuffer != value) {
//         m_staging.depthBuffer = value;
//         m_dirtyFlags |= DirtyFlags_Framebuffer;
//     }
// }
//
// DepthBuffer* GraphicsContext::depthBuffer() const
//{
//     return m_staging.depthBuffer;
// }

void GraphicsContext::setViewportRect(const Rect& value) {
    if (m_staging.viewportRect != value) {
        m_staging.viewportRect = value;
        m_dirtyFlags |= DirtyFlags_RegionRects;
    }
}

void GraphicsContext::setScissorRect(const Rect& value) {
    if (m_staging.scissorRect != value) {
        m_staging.scissorRect = value;
        m_dirtyFlags |= DirtyFlags_RegionRects;
    }
}

void GraphicsContext::setVertexLayout(VertexLayout* value) {
    if (m_staging.VertexLayout != value) {
        m_staging.VertexLayout = value;
        m_dirtyFlags |= DirtyFlags_PipelinePrimitiveState;
    }
}

VertexLayout* GraphicsContext::vertexLayout() const {
    return m_staging.VertexLayout;
}

void GraphicsContext::setPrimitiveTopology(PrimitiveTopology value) {
    if (m_staging.topology != value) {
        m_staging.topology = value;
        m_dirtyFlags |= DirtyFlags_PipelinePrimitiveState;
    }
}

void GraphicsContext::setVertexBuffer(int streamIndex, VertexBuffer* value) {
    if (LN_REQUIRE_RANGE(streamIndex, 0, MaxVertexStreams)) return;
    if (m_staging.vertexBuffers[streamIndex] != value) {
        m_staging.vertexBuffers[streamIndex] = value;
        m_dirtyFlags |= DirtyFlags_PrimitiveBuffers;
    }
}

VertexBuffer* GraphicsContext::vertexBuffer(int streamIndex) const {
    if (LN_REQUIRE_RANGE(streamIndex, 0, MaxVertexStreams)) return nullptr;
    return m_staging.vertexBuffers[streamIndex];
}

void GraphicsContext::setIndexBuffer(IndexBuffer* value) {
    if (m_staging.indexBuffer != value) {
        m_staging.indexBuffer = value;
        m_dirtyFlags |= DirtyFlags_PrimitiveBuffers;
    }
}

IndexBuffer* GraphicsContext::indexBuffer() const {
    return m_staging.indexBuffer;
}

void GraphicsContext::setShaderPass(ShaderPass* value) {
    if (m_staging.shaderPass != value) {
        if (value) {
            m_staging.shader = value->shader();
            m_staging.shaderPass = value;
        }
        else {
            m_staging.shader = nullptr;
            m_staging.shaderPass = nullptr;
        }
        m_dirtyFlags |= DirtyFlags_ShaderPass;
    }
}

ShaderPass* GraphicsContext::shaderPass() const {
    return m_staging.shaderPass;
}

// void GraphicsContext::setShaderDescriptor(ShaderDefaultDescriptor* value)
//{
//     if (m_staging.shaderDescriptor != value) {
//         m_staging.shaderDescriptor = value;
//         m_dirtyFlags |= DirtyFlags_ShaderDescriptor;
//     }
// }
//
// ShaderDefaultDescriptor* GraphicsContext::shaderDescriptor() const
//{
//     return m_staging.shaderDescriptor;
// }

// void GraphicsContext::setRenderPass(RenderPass* value)
//{
//	if (m_staging.renderPass != value) {
//		m_staging.renderPass = value;
//		m_dirtyFlags |= DirtyFlags_RenderPass;
//	}
//
//	if (value) {
//		if (auto target = value->renderTarget(0)) {
//			auto rect = Rect(0, 0, target->width(), target->height());
//			setViewportRect(rect);
//			setScissorRect(rect);
//		}
//	}
// }

void GraphicsContext::beginRenderPass(RenderPass* value) {
    if (LN_REQUIRE(value)) return;
    if (LN_REQUIRE(m_scopeState == ScopeState::RenderPassOutside)) return;
    if (LN_REQUIRE(!m_currentRenderPass)) return;

    m_currentRenderPass = value;
    m_currentRenderPass->m_active = true;
    m_scopeState = ScopeState::BeginRenderPassRequired;

    // 次の commitState() で実際に開始する。
    // 各リソースの実際の copy は commitState() まで遅延されるので、
    // RenderPass の being も遅延しておかないと、Vulkan の「RenderPass inside では copy できない」仕様に引っかかる。
#if 0
	// RenderPass
	{
		detail::IRenderPass* newRenderPass = nullptr;
		bool modified = false;
		newRenderPass = detail::GraphicsResourceInternal::resolveRHIObject<detail::IRenderPass>(this, m_currentRenderPass, &modified);

		//if (m_currentRHIRenderPass != newRenderPass || modified) {
		//	closeRenderPass();

			if (newRenderPass) {
				m_currentRHIRenderPass = newRenderPass;
				LN_ENQUEUE_RENDER_COMMAND_2(
					GraphicsContext_closeRenderPass, this,
					detail::ICommandList*, m_context,
					detail::IRenderPass*, m_currentRHIRenderPass,
					{
						m_context->beginRenderPass(m_currentRHIRenderPass);
					});
			}
		//}
	}
#endif
}

void GraphicsContext::endRenderPass() {
    if (LN_REQUIRE(m_currentRenderPass)) return;

    if (m_scopeState == ScopeState::BeginRenderPassRequired) {
        commitState();
    }

    if (m_scopeState == ScopeState::Active) {
        if (m_currentRHIRenderPass) {
            m_rhiCommandList->endRenderPass(m_currentRHIRenderPass);
            m_currentRHIRenderPass = nullptr;
        }
    }

    m_currentRenderPass->m_active = false;
    m_currentRenderPass = nullptr;
    m_scopeState = ScopeState::RenderPassOutside;
}

RenderPass* GraphicsContext::renderPass() const {
    return m_currentRenderPass;
}

void GraphicsContext::dispatch(int groupCountX, int groupCountY, int groupCountZ) {
    commitState();
    m_rhiCommandList->dispatch(groupCountX, groupCountY, groupCountZ);
}

void GraphicsContext::clear(ClearFlags flags, const Color& color, float z, uint8_t stencil) {
    if (!checkRenderPassInside()) return;
    commitState();
    m_rhiCommandList->clearBuffers(flags, color, z, stencil);
}

void GraphicsContext::drawPrimitive(int startVertex, int primitiveCount) {
    if (!checkRenderPassInside()) return;
    commitState();
    m_rhiCommandList->drawPrimitive(startVertex, primitiveCount);
    m_commandList->m_drawCall++;
}

void GraphicsContext::drawPrimitiveIndexed(int startIndex, int primitiveCount, int instanceCount, int vertexOffset) {
    if (!checkRenderPassInside()) return;
    commitState();
    m_rhiCommandList->drawPrimitiveIndexed(startIndex, primitiveCount, instanceCount, vertexOffset);
    m_commandList->m_drawCall++;
}

void GraphicsContext::drawExtension(INativeGraphicsExtension* extension) {
    if (!checkRenderPassInside()) return;
    commitState();
    m_rhiCommandList->drawExtension(extension);
}

detail::ShaderSecondaryDescriptor* GraphicsContext::allocateShaderDescriptor(ShaderPass* shaderPass) {
    detail::ShaderSecondaryDescriptor* d = m_commandList->acquireShaderDescriptor(shaderPass->shader());
    d->reset(m_commandList);
    return d;
}

// Vulkan の vkCmdCopyBuffer は、RenderPass の Outside でしか使用することができない。
// 今ところ Rendering モジュールがそのような仕組みに対応していないので、
// GraphicsCommandList としては RenderPass Outside でも setSubData できるようにしておきたい。
void GraphicsContext::interruptCurrentRenderPassFromResolveRHI() {
    if (m_scopeState == ScopeState::Active && m_currentRHIRenderPass) {
        m_rhiCommandList->endRenderPass(m_currentRHIRenderPass);
        m_currentRHIRenderPass = nullptr;
        m_scopeState = ScopeState::RenderPassInterrupted;
    }
}

void GraphicsContext::beginCommandRecoding() {
    if (LN_ASSERT(m_scopeState == ScopeState::Idle)) return;
    m_rhiCommandList->begin();
    m_scopeState = ScopeState::RenderPassOutside;
}

void GraphicsContext::endCommandRecoding() {
    if (LN_ASSERT(m_scopeState == ScopeState::RenderPassOutside)) return;
    m_rhiCommandList->end();
    m_scopeState = ScopeState::Idle;
}

//void GraphicsContext::flushCommandRecoding(RenderTargetTexture* affectRendreTarget) {
//    // Vulkan: CommandBuffer が空の状態で VkSubmitQueue するとエラーするので、一度もコマンドを作っていない場合は flush が呼ばれても何もしないようにする
//    if (m_recordingBegan) {
//        endCommandRecoding();
//    }
//}

// void GraphicsContext::closeRenderPass()
//{
//	if (m_currentRHIRenderPass) {
//		LN_ENQUEUE_RENDER_COMMAND_2(
//			GraphicsContext_closeRenderPass, this,
//			detail::ICommandList*, m_context,
//			detail::IRenderPass*, m_currentRHIRenderPass,
//			{
//				m_context->endRenderPass(m_currentRHIRenderPass);
//			});
//		m_currentRHIRenderPass = nullptr;
//	}
// }

// IGraphicsDevice の clear, draw 系の機能を呼び出したい場合はこの戻り値を使うこと。
// GraphicsContext は変更中のステートをキャッシュするが、それを確実に IGraphicsDevice へ送信した状態にする。
detail::ICommandList* GraphicsContext::commitState() {
    using VertexBufferArray = std::array<detail::RHIResource*, detail::MaxVertexStreams>;

    // ポインタとしては変わっていなくても、resolve は毎回呼び出す。
    // こうしておかないと、
    // 頂点バッファset > 描画 > 頂点バッファ更新 > 描画
    // といったように、同じオブジェクトを set したまま内容を更新した場合に反映されなくなる。

    bool resourceModified = false;

    detail::IShaderPass* shaderPassRHI = (m_staging.shaderPass) ? m_staging.shaderPass->resolveRHIObject(this, &resourceModified) : nullptr;
    if (m_staging.shaderPass && m_staging.shaderDescriptor) {
        assert(m_staging.shaderPass->shader() == m_staging.shaderDescriptor->shader());
        m_staging.shaderPass->submitShaderDescriptor2(this, m_staging.shaderDescriptor, &resourceModified);
    }
    else {
        m_commandList->rhiResource()->setDescriptor(nullptr);
    }

    bool vertexLayoutModified = false;
    detail::IVertexDeclaration* vertexLayoutRHI = detail::GraphicsResourceInternal::resolveRHIObject<detail::IVertexDeclaration>(this, m_staging.VertexLayout, &vertexLayoutModified);
    resourceModified |= vertexLayoutModified;

    bool primitiveBufferModified = false;
    VertexBufferArray vertexBuffersRHI;
    detail::RHIResource* indexBufferRHI;
    {
        bool modified = false;
        for (int i = 0; i < m_staging.vertexBuffers.size(); i++) {
            auto& value = m_staging.vertexBuffers[i];
            vertexBuffersRHI[i] = detail::GraphicsResourceInternal::resolveRHIObject<detail::RHIResource>(this, value, &modified);
            primitiveBufferModified |= modified;
        }

        indexBufferRHI = detail::GraphicsResourceInternal::resolveRHIObject<detail::RHIResource>(this, m_staging.indexBuffer, &modified);
        primitiveBufferModified |= modified;
        resourceModified |= primitiveBufferModified;
    }

    // BlendState
    if ((m_dirtyFlags & DirtyFlags_BlendState) != 0) {
        auto& blendState = m_staging.blendState;
        m_rhiCommandList->setBlendState(blendState);
        //m_lastCommit.blendState = m_staging.blendState;
    }

    // RasterizerState
    if ((m_dirtyFlags & DirtyFlags_RasterizerState) != 0) {
        auto& rasterizerState = m_staging.rasterizerState;
        m_rhiCommandList->setRasterizerState(rasterizerState);
        //m_lastCommit.rasterizerState = m_staging.rasterizerState;
    }

    // DepthStencilState
    if ((m_dirtyFlags & DirtyFlags_DepthStencilState) != 0) {
        auto& depthStencilState = m_staging.depthStencilState;
        m_rhiCommandList->setDepthStencilState(depthStencilState);
        //m_lastCommit.depthStencilState = m_staging.depthStencilState;
    }

    // RenderTarget, DepthBuffer
    //{
    //    bool anyModified = false;
    //    bool modified = false;

    //    using RenderTargetArray = std::array<detail::RHIResource*, detail::MaxMultiRenderTargets>;
    //    RenderTargetArray renderTargets;
    //    for (int i = 0; i < detail::MaxMultiRenderTargets; i++) {
    //        auto& value = m_staging.renderTargets[i];
    //        renderTargets[i] = detail::GraphicsResourceInternal::resolveRHIObject<detail::RHIResource>(this, value, &modified);
    //        anyModified |= modified;
    //    }

    //    detail::IDepthBuffer* depthBuffer = detail::GraphicsResourceInternal::resolveRHIObject<detail::IDepthBuffer>(this, m_staging.depthBuffer, &modified);
    //    anyModified |= modified;

    //    if ((m_dirtyFlags & DirtyFlags_Framebuffer) != 0 || anyModified) {
    //        LN_ENQUEUE_RENDER_COMMAND_3(
    //            GraphicsContext_setDepthBuffer, this,
    //            detail::ICommandList*, m_context,
    //            RenderTargetArray, renderTargets,
    //            detail::IDepthBuffer*, depthBuffer,
    //            {
    //                for (int i = 0; i < detail::MaxMultiRenderTargets; i++) {
    //                    m_context->setColorBuffer(i, renderTargets[i]);
    //                }
    //                m_context->setDepthBuffer(depthBuffer);
    //            });
    //    }
    //}

    // Viewport, Scissor
    if ((m_dirtyFlags & DirtyFlags_RegionRects) != 0) {
        RectI viewRect = RectI(0, 0, 0, 0);
        if (m_currentRenderPass) {
            viewRect.width = m_currentRenderPass->renderTarget(0)->width();
            viewRect.height = m_currentRenderPass->renderTarget(0)->height();
        }
        RectI viewportRect = RectI::fromFloatRect(m_staging.viewportRect);
        RectI scissorRect = RectI::fromFloatRect(m_staging.scissorRect);

        if (viewportRect.width < 0 || viewportRect.height < 0) {
            viewportRect.width = viewRect.width;
            viewportRect.height = viewRect.height;
        }
        if (scissorRect.width < 0 || scissorRect.height < 0) {
            scissorRect.width = viewRect.width;
            scissorRect.height = viewRect.height;
        }

        viewportRect.clip(viewRect);
        scissorRect.clip(viewRect);

        // TODO: Size 0 なら描画不要

        m_rhiCommandList->setViewportRect(viewportRect);
        m_rhiCommandList->setScissorRect(scissorRect);

        //m_lastCommit.viewportRect = m_staging.viewportRect;
        //m_lastCommit.scissorRect = m_staging.scissorRect;
    }

    // VertexLayout, Topology
    {
        // bool modified = false;
        // detail::IVertexDeclaration* vertexDeclaration = detail::GraphicsResourceInternal::resolveRHIObject<detail::IVertexDeclaration>(this, m_staging.VertexLayout, &modified);
        PrimitiveTopology topology = m_staging.topology;

        if ((m_dirtyFlags & DirtyFlags_PipelinePrimitiveState) != 0 || vertexLayoutModified) {
            m_rhiCommandList->setPrimitiveTopology(topology);
            m_rhiCommandList->setVertexDeclaration(vertexLayoutRHI);

            //m_lastCommit.VertexLayout = m_staging.VertexLayout;
            //m_lastCommit.topology = m_staging.topology;
        }
    }

    // VertexBuffer, IndexBuffer
    {
        // bool anyModified = false;
        // bool modified = false;

        // using VertexBufferArray = std::array<detail::RHIResource*, detail::MaxVertexStreams>;
        // VertexBufferArray vertexBuffers;
        // for (int i = 0; i < m_staging.vertexBuffers.size(); i++) {
        //     auto& value = m_staging.vertexBuffers[i];
        //     vertexBuffers[i] = detail::GraphicsResourceInternal::resolveRHIObject<detail::RHIResource>(this, value, &modified);
        //     anyModified |= modified;
        // }

        // detail::RHIResource* indexBuffer = detail::GraphicsResourceInternal::resolveRHIObject<detail::IIndexBuffer>(this, m_staging.indexBuffer, &modified);
        // anyModified |= modified;

        if ((m_dirtyFlags & DirtyFlags_PrimitiveBuffers) != 0 || primitiveBufferModified) {
            for (int i = 0; i < detail::MaxVertexStreams; i++) {
                m_rhiCommandList->setVertexBuffer(i, vertexBuffersRHI[i]);
            }
            m_rhiCommandList->setIndexBuffer(indexBufferRHI);

            //m_lastCommit.vertexBuffers = m_staging.vertexBuffers;
            //m_lastCommit.indexBuffer = m_staging.indexBuffer;
        }
    }

    // ShaderPass
    {

        if ((m_dirtyFlags & DirtyFlags_ShaderPass) != 0) {
            m_rhiCommandList->setShaderPass(shaderPassRHI);

            //m_lastCommit.shader = m_staging.shader;
            //m_lastCommit.shaderPass = m_staging.shaderPass;
        }
    }

    m_dirtyFlags = DirtyFlags_None;

    // RenderPass
    if (m_scopeState == ScopeState::BeginRenderPassRequired ||  // 普通に beginRenderPass した直後
        m_scopeState == ScopeState::RenderPassInterrupted)      // resolve でリソース更新したのでRenderPass中断した直後
    {
        detail::IRenderPass* newRenderPass = nullptr;
        bool modified = false;
        if (m_scopeState == ScopeState::BeginRenderPassRequired)
            newRenderPass = detail::GraphicsResourceInternal::resolveRHIObject<detail::IRenderPass>(this, m_currentRenderPass, &modified);
        else
            newRenderPass = m_currentRenderPass->resolveRHIObjectNoClear(this, &modified);

        if (newRenderPass) {
            m_currentRHIRenderPass = newRenderPass;
            m_rhiCommandList->beginRenderPass(m_currentRHIRenderPass);
        }
        //}
        m_scopeState = ScopeState::Active;
    }
    // else if (m_scopeState == ScopeState::Active && resourceModified) {
    //

    //    detail::IRenderPass* newRenderPass = nullptr;
    //    bool modified = false;
    //
    //    if (newRenderPass) {
    //        m_currentRHIRenderPass = newRenderPass;
    //        LN_ENQUEUE_RENDER_COMMAND_2(
    //            GraphicsContext_closeRenderPass, this,
    //            detail::ICommandList*, m_context,
    //            detail::IRenderPass*, m_currentRHIRenderPass,
    //            {
    //                m_context->beginRenderPass(m_currentRHIRenderPass);
    //            });
    //    }

    //}

    //{
    //    detail::DevicePipelineStateDesc pipelineDesc;
    //    pipelineDesc.blendState = m_staging.blendState;
    //    pipelineDesc.rasterizerState = m_staging.rasterizerState;
    //    pipelineDesc.depthStencilState = m_staging.depthStencilState;
    //    pipelineDesc.topology = m_staging.topology;
    //    pipelineDesc.vertexDeclaration = vertexLayoutRHI;
    //    pipelineDesc.shaderPass = shaderPassRHI;
    //    pipelineDesc.renderPass = ;
    //}

    return m_rhiCommandList;
}

// void GraphicsContext::submitCommandList()
//{
//     m_manager->submitCommandList(m_recordingCommandList);
// }


bool GraphicsContext::checkRenderPassInside() const {
    if (LN_ASSERT(
        m_scopeState == ScopeState::BeginRenderPassRequired ||
        m_scopeState == ScopeState::RenderPassInterrupted ||
        m_scopeState == ScopeState::Active)) {
        return false;
    }
    
    if (m_scopeState == ScopeState::BeginRenderPassRequired) {
        if (LN_ASSERT(m_currentRenderPass)) return false;
        if (LN_ASSERT(!m_currentRHIRenderPass)) return false;
        return true;
    }
    else if (m_scopeState == ScopeState::RenderPassInterrupted) {
        if (LN_ASSERT(m_currentRenderPass)) return false;
        if (LN_ASSERT(!m_currentRHIRenderPass)) return false;
        return true;
    }
    else if (m_scopeState == ScopeState::Active) {
        if (LN_ASSERT(m_currentRenderPass)) return false;
        if (LN_ASSERT(m_currentRHIRenderPass)) return false;
        return true;
    }
    return false;
}

void GraphicsContext::State::reset() {
    blendState = BlendStateDesc();
    rasterizerState = RasterizerStateDesc();
    depthStencilState = DepthStencilStateDesc();
    // renderTargets = {};
    // depthBuffer = nullptr;
    viewportRect = Rect(0, 0, -1, -1);
    scissorRect = Rect(0, 0, -1, -1);
    VertexLayout = nullptr;
    vertexBuffers = {};
    indexBuffer = nullptr;
    shader = nullptr;
    shaderPass = nullptr;
    shaderDescriptor = nullptr;
    topology = PrimitiveTopology::TriangleList;
    // renderPass = nullptr;
}

} // namespace ln
