
#include "Internal.hpp"
#include "GraphicsManager.hpp"
#include "RHIs/GraphicsDeviceContext.hpp"
#include <LuminoGraphics/GraphicsCommandBuffer.hpp>
#include <LuminoGraphics/Shader.hpp>
#include <LuminoGraphics/ShaderDescriptor.hpp>
#include "SingleFrameAllocator.hpp"

namespace ln {

//==============================================================================
// GraphicsCommandList

GraphicsCommandList::GraphicsCommandList()
    : m_drawCall(0) {
}

void GraphicsCommandList::init(detail::GraphicsManager* manager) {
    m_rhiResource = manager->deviceContext()->createCommandList();
    m_allocator = makeRef<detail::LinearAllocator>(manager->linearAllocatorPageManager());
    m_singleFrameUniformBufferAllocator = makeRef<detail::SingleFrameUniformBufferAllocator>(manager->singleFrameUniformBufferAllocatorPageManager());
    m_uniformBufferOffsetAlignment = manager->deviceContext()->caps().uniformBufferOffsetAlignment;
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

} // namespace ln
