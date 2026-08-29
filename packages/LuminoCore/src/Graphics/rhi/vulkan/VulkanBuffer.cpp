#include "VulkanHelpers.hpp"
#include "VulkanDevice.hpp"
#include "VulkanBuffer.hpp"

namespace ln::rhi::vulkan {

VulkanBuffer::VulkanBuffer() = default;

VoidResult VulkanBuffer::init(VulkanDevice* device, VkPhysicalDevice physicalDevice, const BufferDesc& desc,
                               bool deviceLocal) {
    m_device = device;
    m_size = desc.size;
    m_deviceLocal = deviceLocal;

    VkDevice dev = m_device->vkDevice();
    VkBufferCreateInfo bufInfo{};
    bufInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufInfo.size = desc.size;

    if (desc.usage & BufferUsage::Vertex)  bufInfo.usage |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    if (desc.usage & BufferUsage::Index)   bufInfo.usage |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    if (desc.usage & BufferUsage::Uniform) bufInfo.usage |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    if (desc.usage & BufferUsage::Storage) bufInfo.usage |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
    if (desc.usage & BufferUsage::CopySrc) bufInfo.usage |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    if (desc.usage & BufferUsage::CopyDst) bufInfo.usage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    // Device-local buffers need TRANSFER_DST so staging can copy into them.
    if (deviceLocal) bufInfo.usage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;

    bufInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    if (vkCreateBuffer(dev, &bufInfo, nullptr, &m_buffer) != VK_SUCCESS) {
        return LN_MAKE_ERROR("vkCreateBuffer failed.");
    }

    VkMemoryRequirements memReqs;
    vkGetBufferMemoryRequirements(dev, m_buffer, &memReqs);

    VkMemoryPropertyFlags memFlags = deviceLocal
        ? VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        : (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memReqs.size;
    allocInfo.memoryTypeIndex = VulkanHelpers::findMemoryType(physicalDevice, memReqs.memoryTypeBits, memFlags);
    if (vkAllocateMemory(dev, &allocInfo, nullptr, &m_memory) != VK_SUCCESS) {
        return LN_MAKE_ERROR("vkAllocateMemory failed.");
    }
    vkBindBufferMemory(dev, m_buffer, m_memory, 0);

    // Device-local buffers: initialData is uploaded by the caller (via StagingBufferPool).
    // Host-visible buffers: copy directly.
    if (!deviceLocal && desc.initialData) {
        void* mapped = map();
        if (mapped) {
            std::memcpy(mapped, desc.initialData, desc.size);
            unmap();
        }
    }
    return LN_MAKE_SUCCESS();
}

void VulkanBuffer::finalize() {
    VkDevice dev = m_device->vkDevice();
    VkBuffer buf = m_buffer;
    VkDeviceMemory mem = m_memory;
    m_device->frameResources().queueDelete(m_device->currentFrameIndex(), [dev, buf, mem]() {
        if (buf) vkDestroyBuffer(dev, buf, nullptr);
        if (mem) vkFreeMemory(dev, mem, nullptr);
    });
    Buffer::finalize();
}

void* VulkanBuffer::map() {
    if (m_deviceLocal) return nullptr;  // device-local memory cannot be CPU-mapped
    if (!m_mapped) vkMapMemory(m_device->vkDevice(), m_memory, 0, m_size, 0, &m_mapped);
    return m_mapped;
}

void VulkanBuffer::unmap() {
    // 永続マップのため何もしない。ホストビジブルなメモリは HOST_COHERENT で確保しているので
    // flush も不要で、書き込みは memcpy のみで完結する。
    // マップはバッファの寿命が尽きるまで保持し続ける。finalize() 側の追加処理は不要:
    // "If a memory object is mapped at the time it is freed, it is implicitly unmapped."
    // https://docs.vulkan.org/refpages/latest/refpages/source/vkFreeMemory.html
}

} // namespace ln::rhi::vulkan
