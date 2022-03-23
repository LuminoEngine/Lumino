
#include "Internal.hpp"
#include "GraphicsManager.hpp"
#include "RHIs/GraphicsDeviceContext.hpp"
#include <LuminoGraphics/VertexLayout.hpp>
#include <LuminoGraphics/VertexBuffer.hpp>
#include <LuminoGraphics/IndexBuffer.hpp>
#include <LuminoGraphics/Texture.hpp>
#include <LuminoGraphics/DepthBuffer.hpp>
#include <LuminoGraphics/Shader.hpp>
#include <LuminoGraphics/ShaderDescriptor.hpp>
#include <LuminoGraphics/RenderPass.hpp>
#include <LuminoGraphics/GraphicsCommandBuffer.hpp>
#include "SingleFrameAllocator.hpp"

namespace ln {

//==============================================================================
// GraphicsCommandList

GraphicsCommandList::GraphicsCommandList()
    : m_manager(nullptr) 
    , m_drawCall(0) 
    , m_rhiResource(nullptr)
    , m_scopeState(ScopeState::Idle)
    , m_staging()
    , m_dirtyFlags(DirtyFlags_All)
    , m_currentRenderPass(nullptr)
    , m_currentRHIRenderPass(nullptr) {
}

void GraphicsCommandList::init(detail::GraphicsManager* manager) {
    m_manager = detail::GraphicsManager::instance();
    m_rhiResource = manager->deviceContext()->createCommandList();
    m_allocator = makeRef<detail::LinearAllocator>(manager->linearAllocatorPageManager());
    m_singleFrameUniformBufferAllocator = makeRef<detail::SingleFrameUniformBufferAllocator>(manager->singleFrameUniformBufferAllocatorPageManager());
    m_uniformBufferOffsetAlignment = manager->deviceContext()->caps().uniformBufferOffsetAlignment;
    resetState();
}

void GraphicsCommandList::dispose() {
    reset();
    if (m_rhiResource) {
        m_rhiResource = nullptr;
    }
    // for (const auto& pair : m_usingDescriptorPools) {
    //     pair.descriptorPool->dispose();
    // }
    m_usingDescriptorPools.clear();
}

void GraphicsCommandList::onDispose(bool explicitDisposing) {
    LN_ASSERT(m_scopeState == ScopeState::Idle);
    Object::onDispose(explicitDisposing);
}

void GraphicsCommandList::reset() {
    // 実行終了を待つ
    m_rhiResource->wait();

    m_allocator->cleanup();
    // m_singleFrameUniformBufferAllocator->cleanup();

    for (const auto& pair : m_usingDescriptorPools) {
        pair.shaderPass->releaseDescriptorSetsPool(pair.descriptorPool);
    }
    m_usingDescriptorPools.clear();

    m_drawCall = 0;
    m_vertexBufferDataTransferredSize = 0;
    m_indexBufferDataTransferredSize = 0;
    m_textureDataTransferredSize = 0;
}




void GraphicsCommandList::setShaderDescriptor(detail::ShaderSecondaryDescriptor* value) {
    m_staging.shaderDescriptor = value;
}

void GraphicsCommandList::resetState() {
    m_staging.reset();
    m_dirtyFlags = DirtyFlags_All;
}

void GraphicsCommandList::setBlendState(const BlendStateDesc& value) {
    if (!BlendStateDesc::equals(m_staging.blendState, value)) {
        m_staging.blendState = value;
        m_dirtyFlags |= DirtyFlags_BlendState;
    }
}

void GraphicsCommandList::setRasterizerState(const RasterizerStateDesc& value) {
    if (!RasterizerStateDesc::equals(m_staging.rasterizerState, value)) {
        m_staging.rasterizerState = value;
        m_dirtyFlags |= DirtyFlags_RasterizerState;
    }
}

void GraphicsCommandList::setDepthStencilState(const DepthStencilStateDesc& value) {
    if (!DepthStencilStateDesc::equals(m_staging.depthStencilState, value)) {
        m_staging.depthStencilState = value;
        m_dirtyFlags |= DirtyFlags_DepthStencilState;
    }
}

void GraphicsCommandList::setViewportRect(const Rect& value) {
    if (m_staging.viewportRect != value) {
        m_staging.viewportRect = value;
        m_dirtyFlags |= DirtyFlags_RegionRects;
    }
}

void GraphicsCommandList::setScissorRect(const Rect& value) {
    if (m_staging.scissorRect != value) {
        m_staging.scissorRect = value;
        m_dirtyFlags |= DirtyFlags_RegionRects;
    }
}

void GraphicsCommandList::setVertexLayout(VertexLayout* value) {
    if (m_staging.VertexLayout != value) {
        m_staging.VertexLayout = value;
        m_dirtyFlags |= DirtyFlags_PipelinePrimitiveState;
    }
}

VertexLayout* GraphicsCommandList::vertexLayout() const {
    return m_staging.VertexLayout;
}

void GraphicsCommandList::setPrimitiveTopology(PrimitiveTopology value) {
    if (m_staging.topology != value) {
        m_staging.topology = value;
        m_dirtyFlags |= DirtyFlags_PipelinePrimitiveState;
    }
}

void GraphicsCommandList::setVertexBuffer(int streamIndex, VertexBuffer* value) {
    if (LN_REQUIRE_RANGE(streamIndex, 0, MaxVertexStreams)) return;
    if (m_staging.vertexBuffers[streamIndex] != value) {
        m_staging.vertexBuffers[streamIndex] = value;
        m_dirtyFlags |= DirtyFlags_PrimitiveBuffers;
    }
}

VertexBuffer* GraphicsCommandList::vertexBuffer(int streamIndex) const {
    if (LN_REQUIRE_RANGE(streamIndex, 0, MaxVertexStreams)) return nullptr;
    return m_staging.vertexBuffers[streamIndex];
}

void GraphicsCommandList::setIndexBuffer(IndexBuffer* value) {
    if (m_staging.indexBuffer != value) {
        m_staging.indexBuffer = value;
        m_dirtyFlags |= DirtyFlags_PrimitiveBuffers;
    }
}

IndexBuffer* GraphicsCommandList::indexBuffer() const {
    return m_staging.indexBuffer;
}

void GraphicsCommandList::setShaderPass(ShaderPass* value) {
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

ShaderPass* GraphicsCommandList::shaderPass() const {
    return m_staging.shaderPass;
}

void GraphicsCommandList::beginRenderPass(RenderPass* value) {
    if (LN_REQUIRE(value)) return;
    if (LN_REQUIRE(m_scopeState == ScopeState::RenderPassOutside)) return;
    if (LN_REQUIRE(!m_currentRenderPass)) return;

    m_currentRenderPass = value;
    m_currentRenderPass->m_active = true;
    m_scopeState = ScopeState::BeginRenderPassRequired;

    // 次の commitState() で実際に開始する。
    // 各リソースの実際の copy は commitState() まで遅延されるので、
    // RenderPass の being も遅延しておかないと、Vulkan の「RenderPass inside では copy できない」仕様に引っかかる。
}

void GraphicsCommandList::endRenderPass() {
    if (LN_REQUIRE(m_currentRenderPass)) return;

    if (m_scopeState == ScopeState::BeginRenderPassRequired) {
        commitState();
    }

    if (m_scopeState == ScopeState::Active) {
        if (m_currentRHIRenderPass) {
            m_rhiResource->endRenderPass(m_currentRHIRenderPass);
            m_currentRHIRenderPass = nullptr;
        }
    }

    m_currentRenderPass->m_active = false;
    m_currentRenderPass = nullptr;
    m_scopeState = ScopeState::RenderPassOutside;
}

RenderPass* GraphicsCommandList::renderPass() const {
    return m_currentRenderPass;
}

void GraphicsCommandList::dispatch(int groupCountX, int groupCountY, int groupCountZ) {
    commitState();
    m_rhiResource->dispatch(groupCountX, groupCountY, groupCountZ);
}

void GraphicsCommandList::clear(ClearFlags flags, const Color& color, float z, uint8_t stencil) {
    if (!checkRenderPassInside()) return;
    commitState();
    m_rhiResource->clearBuffers(flags, color, z, stencil);
}

void GraphicsCommandList::drawPrimitive(int startVertex, int primitiveCount) {
    if (!checkRenderPassInside()) return;
    commitState();
    m_rhiResource->drawPrimitive(startVertex, primitiveCount);
    m_drawCall++;
}

void GraphicsCommandList::drawPrimitiveIndexed(int startIndex, int primitiveCount, int instanceCount, int vertexOffset) {
    if (!checkRenderPassInside()) return;
    commitState();
    m_rhiResource->drawPrimitiveIndexed(startIndex, primitiveCount, instanceCount, vertexOffset);
    m_drawCall++;
}

void GraphicsCommandList::drawExtension(INativeGraphicsExtension* extension) {
    if (!checkRenderPassInside()) return;
    commitState();
    m_rhiResource->drawExtension(extension);
}

detail::ShaderSecondaryDescriptor* GraphicsCommandList::allocateShaderDescriptor(ShaderPass* shaderPass) {
    detail::ShaderSecondaryDescriptor* d = acquireShaderDescriptor(shaderPass->shader());
    d->reset(this);
    return d;
}



// Vulkan の vkCmdCopyBuffer は、RenderPass の Outside でしか使用することができない。
// 今ところ Rendering モジュールがそのような仕組みに対応していないので、
// GraphicsCommandList としては RenderPass Outside でも setSubData できるようにしておきたい。
void GraphicsCommandList::interruptCurrentRenderPassFromResolveRHI() {
    if (m_scopeState == ScopeState::Active && m_currentRHIRenderPass) {
        m_rhiResource->endRenderPass(m_currentRHIRenderPass);
        m_currentRHIRenderPass = nullptr;
        m_scopeState = ScopeState::RenderPassInterrupted;
    }
}

void GraphicsCommandList::beginCommandRecoding() {
    if (LN_ASSERT(m_scopeState == ScopeState::Idle)) return;
    m_rhiResource->begin();
    m_scopeState = ScopeState::RenderPassOutside;
}

void GraphicsCommandList::endCommandRecoding() {
    if (LN_ASSERT(m_scopeState == ScopeState::RenderPassOutside)) return;
    m_rhiResource->end();
    m_scopeState = ScopeState::Idle;
}

// IGraphicsDevice の clear, draw 系の機能を呼び出したい場合はこの戻り値を使うこと。
// GraphicsCommandList は変更中のステートをキャッシュするが、それを確実に IGraphicsDevice へ送信した状態にする。
detail::ICommandList* GraphicsCommandList::commitState() {
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
        m_rhiResource->setDescriptor(nullptr);
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
        m_rhiResource->setBlendState(blendState);
    }

    // RasterizerState
    if ((m_dirtyFlags & DirtyFlags_RasterizerState) != 0) {
        auto& rasterizerState = m_staging.rasterizerState;
        m_rhiResource->setRasterizerState(rasterizerState);
    }

    // DepthStencilState
    if ((m_dirtyFlags & DirtyFlags_DepthStencilState) != 0) {
        auto& depthStencilState = m_staging.depthStencilState;
        m_rhiResource->setDepthStencilState(depthStencilState);
    }

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

        m_rhiResource->setViewportRect(viewportRect);
        m_rhiResource->setScissorRect(scissorRect);
    }

    // VertexLayout, Topology
    {
        // bool modified = false;
        // detail::IVertexDeclaration* vertexDeclaration = detail::GraphicsResourceInternal::resolveRHIObject<detail::IVertexDeclaration>(this, m_staging.VertexLayout, &modified);
        PrimitiveTopology topology = m_staging.topology;

        if ((m_dirtyFlags & DirtyFlags_PipelinePrimitiveState) != 0 || vertexLayoutModified) {
            m_rhiResource->setPrimitiveTopology(topology);
            m_rhiResource->setVertexDeclaration(vertexLayoutRHI);
        }
    }

    // VertexBuffer, IndexBuffer
    {
        if ((m_dirtyFlags & DirtyFlags_PrimitiveBuffers) != 0 || primitiveBufferModified) {
            for (int i = 0; i < detail::MaxVertexStreams; i++) {
                m_rhiResource->setVertexBuffer(i, vertexBuffersRHI[i]);
            }
            m_rhiResource->setIndexBuffer(indexBufferRHI);
        }
    }

    // ShaderPass
    {
        if ((m_dirtyFlags & DirtyFlags_ShaderPass) != 0) {
            m_rhiResource->setShaderPass(shaderPassRHI);
        }
    }

    m_dirtyFlags = DirtyFlags_None;

    // RenderPass
    if (m_scopeState == ScopeState::BeginRenderPassRequired || // 普通に beginRenderPass した直後
        m_scopeState == ScopeState::RenderPassInterrupted)     // resolve でリソース更新したのでRenderPass中断した直後
    {
        detail::IRenderPass* newRenderPass = nullptr;
        bool modified = false;
        if (m_scopeState == ScopeState::BeginRenderPassRequired)
            newRenderPass = detail::GraphicsResourceInternal::resolveRHIObject<detail::IRenderPass>(this, m_currentRenderPass, &modified);
        else
            newRenderPass = m_currentRenderPass->resolveRHIObjectNoClear(this, &modified);

        if (newRenderPass) {
            m_currentRHIRenderPass = newRenderPass;
            m_rhiResource->beginRenderPass(m_currentRHIRenderPass);
        }
        m_scopeState = ScopeState::Active;
    }

    return m_rhiResource;
}

bool GraphicsCommandList::checkRenderPassInside() const {
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



detail::ConstantBufferView GraphicsCommandList::allocateUniformBuffer(size_t size) {
    return m_singleFrameUniformBufferAllocator->allocate(size, m_uniformBufferOffsetAlignment);
}

detail::ShaderSecondaryDescriptor* GraphicsCommandList::acquireShaderDescriptor(Shader* shader) {
    return shader->acquireDescriptor();
}

detail::IDescriptorPool* GraphicsCommandList::getDescriptorPool(ShaderPass* shaderPass) {
    // このコマンド実行中に新たな ShaderPass が使われるたびに、新しく VulkanShaderPass から Pool を確保しようとする。
    // ただし、毎回やると重いので簡単なキャッシュを設ける。
    // 線形探索だけど、ShaderPass が1フレームに 100 も 200 も使われることはそうないだろう。

    int usingShaderPass = -1;
    for (int i = 0; i < m_usingDescriptorPools.size(); i++) {
        if (m_usingDescriptorPools[i].shaderPass == shaderPass) {
            usingShaderPass = i;
        }
    }

    if (usingShaderPass == -1) {
        auto pool = shaderPass->getDescriptorSetsPool();

        ShaderPassDescriptorPair pair;
        pair.shaderPass = shaderPass;
        pair.descriptorPool = pool;
        m_usingDescriptorPools.push_back(pair);

        usingShaderPass = m_usingDescriptorPools.size() - 1;
    }

    return m_usingDescriptorPools[usingShaderPass].descriptorPool;
}

void* GraphicsCommandList::allocateBulkData(size_t size) {
    return m_allocator->allocate(size);
}

void GraphicsCommandList::enterRenderState() {
    m_rhiResource->enterRenderState();
}

void GraphicsCommandList::leaveRenderState() {
    m_rhiResource->leaveRenderState();
}

void GraphicsCommandList::State::reset() {
    blendState = BlendStateDesc();
    rasterizerState = RasterizerStateDesc();
    depthStencilState = DepthStencilStateDesc();
    viewportRect = Rect(0, 0, -1, -1);
    scissorRect = Rect(0, 0, -1, -1);
    VertexLayout = nullptr;
    vertexBuffers = {};
    indexBuffer = nullptr;
    shader = nullptr;
    shaderPass = nullptr;
    shaderDescriptor = nullptr;
    topology = PrimitiveTopology::TriangleList;
}

} // namespace ln
