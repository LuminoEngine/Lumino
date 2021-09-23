
#include "Internal.hpp"
#include "GraphicsManager.hpp"
#include "RHIs/GraphicsDeviceContext.hpp"
#include <LuminoEngine/Graphics/GraphicsCommandBuffer.hpp>
#include <LuminoEngine/Graphics/Shader.hpp>
#include <LuminoEngine/Graphics/ShaderDescriptor.hpp>
#include "SingleFrameAllocator.hpp"

namespace ln {
namespace detail {
    
//==============================================================================
// GraphicsCommandList

GraphicsCommandList::GraphicsCommandList()
    : m_drawCall(0)
{
}

void GraphicsCommandList::init(GraphicsManager* manager)
{
    m_rhiResource = manager->deviceContext()->createCommandList();
    m_allocator = makeRef<LinearAllocator>(manager->linearAllocatorPageManager());
	m_singleFrameUniformBufferAllocator = makeRef<detail::SingleFrameUniformBufferAllocator>(manager->singleFrameUniformBufferAllocatorPageManager());
	m_uniformBufferOffsetAlignment = manager->deviceContext()->caps().uniformBufferOffsetAlignment;
}

void GraphicsCommandList::dispose()
{
    reset();
    if (m_rhiResource) {
        m_rhiResource = nullptr;
    }
    //for (const auto& pair : m_usingDescriptorPools) {
    //    pair.descriptorPool->dispose();
    //}
    m_usingDescriptorPools.clear();
}

void GraphicsCommandList::reset()
{
    // 実行終了を待つ
    m_rhiResource->wait();

    m_allocator->cleanup();
	//m_singleFrameUniformBufferAllocator->cleanup();

    for (const auto& pair : m_usingDescriptorPools) {
        pair.shaderPass->releaseDescriptorSetsPool(pair.descriptorPool);
    }
    m_usingDescriptorPools.clear();

    m_drawCall = 0;
    m_vertexBufferDataTransferredSize = 0;
    m_indexBufferDataTransferredSize = 0;
    m_textureDataTransferredSize = 0;
}

detail::ConstantBufferView GraphicsCommandList::allocateUniformBuffer(size_t size)
{
	return m_singleFrameUniformBufferAllocator->allocate(size, m_uniformBufferOffsetAlignment);
}

ShaderSecondaryDescriptor* GraphicsCommandList::acquireShaderDescriptor(Shader* shader)
{
	return shader->acquireDescriptor();
}

IDescriptorPool* GraphicsCommandList::getDescriptorPool(ShaderPass* shaderPass)
{
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

} // namespace detail

#if 0
//==============================================================================
// GraphicsCommandBuffer

GraphicsCommandBuffer::GraphicsCommandBuffer()
    : m_manager(nullptr)
    , m_rhiObject()
{
}

GraphicsCommandBuffer::~GraphicsCommandBuffer()
{
}

void GraphicsCommandBuffer::init()
{
    Object::init();
    detail::GraphicsResourceInternal::initializeHelper_GraphicsResource(this, &m_manager);

    m_singleFrameUniformBufferAllocator = makeRef<detail::SingleFrameUniformBufferAllocator>(m_manager->singleFrameUniformBufferAllocatorPageManager());
}

void GraphicsCommandBuffer::onDispose(bool explicitDisposing)
{
    detail::GraphicsResourceInternal::finalizeHelper_GraphicsResource(this, &m_manager);
    Object::onDispose(explicitDisposing);
}

detail::UniformBufferView GraphicsCommandBuffer::allocateUniformBuffer(size_t size)
{
    return m_singleFrameUniformBufferAllocator->allocate(size);
}

void GraphicsCommandBuffer::onChangeDevice(detail::IGraphicsDevice* device)
{
}

detail::ICommandList* GraphicsCommandBuffer::resolveRHIObject(GraphicsContext* context, bool* outModified)
{
	return nullptr;
}
#endif

} // namespace ln
