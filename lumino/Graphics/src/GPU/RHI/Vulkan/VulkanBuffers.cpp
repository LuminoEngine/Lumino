
#include "Internal.hpp"
#include "VulkanDeviceContext.hpp"
#include "VulkanBuffers.hpp"

namespace ln {
namespace detail {

//==============================================================================
// VulkanBuffer

VulkanBuffer::VulkanBuffer()
    : m_deviceContext(nullptr)
    , m_nativeBuffer(VK_NULL_HANDLE)
    , m_nativeBufferMemory(VK_NULL_HANDLE)
    , m_size(0)
    , m_allocator(nullptr) {
}

Result VulkanBuffer::init(VulkanDevice* deviceContext, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, const VkAllocationCallbacks* allocator) {
    if (LN_REQUIRE(deviceContext)) return err();
    dispose();

    m_deviceContext = deviceContext;
    m_size = size;
    allocator = (allocator) ? allocator : m_deviceContext->vulkanAllocator();
    m_allocator = allocator;

    auto device = m_deviceContext->vulkanDevice();

    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    LN_VK_CHECK(vkCreateBuffer(device, &bufferInfo, m_allocator, &m_nativeBuffer));

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, m_nativeBuffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size; // アライメントされたサイズ
    m_deviceContext->findMemoryType(memRequirements.memoryTypeBits, properties, &allocInfo.memoryTypeIndex);

    LN_VK_CHECK(vkAllocateMemory(device, &allocInfo, m_allocator, &m_nativeBufferMemory));
    LN_VK_CHECK(vkBindBufferMemory(device, m_nativeBuffer, m_nativeBufferMemory, 0));

    return ok();
}

void VulkanBuffer::dispose() {
    if (m_deviceContext) {
        auto device = m_deviceContext->vulkanDevice();
        const VkAllocationCallbacks* allocator = m_allocator ? m_allocator : m_deviceContext->vulkanAllocator();

        if (m_nativeBufferMemory) {
            vkFreeMemory(device, m_nativeBufferMemory, allocator);
            m_nativeBufferMemory = VK_NULL_HANDLE;
        }

        if (m_nativeBuffer) {
            vkDestroyBuffer(device, m_nativeBuffer, allocator);
            m_nativeBuffer = VK_NULL_HANDLE;
        }

        m_deviceContext = nullptr;
    }
}

void* VulkanBuffer::map() {
    void* mapped;
    if (vkMapMemory(m_deviceContext->vulkanDevice(), m_nativeBufferMemory, 0, m_size, 0, &mapped) != VK_SUCCESS) {
        LN_LOG_ERROR("Failed vkMapMemory");
        return nullptr;
    }
    return mapped;
}

void VulkanBuffer::unmap() {
    vkUnmapMemory(m_deviceContext->vulkanDevice(), m_nativeBufferMemory);
}

void VulkanBuffer::setData(size_t offset, const void* data, VkDeviceSize size) {
    if (LN_REQUIRE(data)) return;
    if (LN_REQUIRE((offset + size) <= m_size)) return;

    if (auto mapped = reinterpret_cast<unsigned char*>(map())) {
        memcpy(mapped + offset, data, size);
        unmap();
    }
}

//==============================================================================
// VulkanVertexBuffer

VulkanVertexBuffer::VulkanVertexBuffer()
    : m_buffer()
//, m_usage(GraphicsResourceUsage::Static)
{
}

Result VulkanVertexBuffer::init(VulkanDevice* deviceContext, GraphicsResourceUsage usage, size_t bufferSize, const void* initialData) {
    if (!RHIResource::initAsVertexBuffer(usage, bufferSize)) return err();

    LN_DCHECK(deviceContext);
    m_deviceContext = deviceContext;

    //m_usage = usage;

#if 1 // TODO: Dynamic という特別な状態を持たせる必要があるか、まだわからない。
    // VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT で十分なのか、VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT を使った方がいいのか、
    // 実際にパフォーマンス測定できる段になったら改めて調査する。
    if (!m_buffer.init(deviceContext, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, nullptr)) {
        return err();
    }

    if (initialData) {
        VulkanBuffer stagingBuffer;
        if (!stagingBuffer.init(m_buffer.deviceContext(), bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, nullptr)) {
            return err();
        }
        stagingBuffer.setData(0, initialData, bufferSize);
        m_buffer.deviceContext()->copyBufferImmediately(stagingBuffer.nativeBuffer(), vulkanBuffer(), bufferSize);
        stagingBuffer.dispose();
    }
#else
    VkMemoryPropertyFlags properties = 0;
    if (usage == GraphicsResourceUsage::Dynamic) {
        properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
    }
    else {
        properties = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    }

    if (!m_buffer.init(deviceContext, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, properties)) {
        return false;
    }

    if (initialData) {
        if (usage == GraphicsResourceUsage::Dynamic) {
            m_buffer.setData(0, initialData, bufferSize);
        }
        else {
            VulkanBuffer stagingBuffer;
            if (!stagingBuffer.init(m_buffer.deviceContext(), bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)) {
                return false;
            }
            stagingBuffer.setData(0, initialData, bufferSize);
            m_buffer.deviceContext()->copyBufferImmediately(stagingBuffer.vulkanBuffer(), vulkanBuffer(), bufferSize);
            stagingBuffer.dispose();
        }
    }
#endif

    return ok();
}

void VulkanVertexBuffer::dispose() {
    m_buffer.dispose();
    RHIResource::dispose();
}

//==============================================================================
// VulkanIndexBuffer

VulkanIndexBuffer::VulkanIndexBuffer()
    : m_buffer()
    , m_usage(GraphicsResourceUsage::Static) {
}

Result VulkanIndexBuffer::init(VulkanDevice* deviceContext, GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData) {
    if (!RHIResource::initAsIndexBuffer(usage, format, indexCount)) return err();
    LN_DCHECK(deviceContext);
    m_deviceContext = deviceContext;

    m_usage = usage;
    if (format == IndexBufferFormat::UInt16) {
        m_indexType = VK_INDEX_TYPE_UINT16;
    }
    else {
        m_indexType = VK_INDEX_TYPE_UINT32;
    }
    VkDeviceSize bufferSize = memorySize();

    if (!m_buffer.init(deviceContext, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, nullptr)) {
        return err();
    }

    if (initialData) {
        VulkanBuffer stagingBuffer;
        if (!stagingBuffer.init(m_buffer.deviceContext(), bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, nullptr)) {
            return err();
        }
        stagingBuffer.setData(0, initialData, bufferSize);
        m_buffer.deviceContext()->copyBufferImmediately(stagingBuffer.nativeBuffer(), vulkanBuffer(), bufferSize);
        stagingBuffer.dispose();
    }

    return ok();
}

void VulkanIndexBuffer::dispose() {
    m_buffer.dispose();
    RHIResource::dispose();
}

//==============================================================================
// VulkanUniformBuffer

VulkanUniformBuffer::VulkanUniformBuffer()
    : m_buffer() {
}

Result VulkanUniformBuffer::init(VulkanDevice* deviceContext, uint32_t size) {
    LN_TRY(RHIResource::initAsUniformBuffer(GraphicsResourceUsage::Dynamic, size));
    if (!m_buffer.init(deviceContext, size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, nullptr)) {
        return err();
    }
    return ok();
}

void VulkanUniformBuffer::dispose() {
    m_buffer.dispose();
    RHIResource::dispose();
}

void* VulkanUniformBuffer::map() {
    return m_buffer.map();
}

void VulkanUniformBuffer::unmap() {
    m_buffer.unmap();
}

} // namespace detail
} // namespace ln
