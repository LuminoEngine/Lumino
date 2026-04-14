#pragma once
#include "VulkanCommon.hpp"

namespace ln::rhi::vulkan {

class VulkanBuffer final : public Buffer {
public:
    VulkanBuffer();
    /**
     * @param deviceLocal  If true, allocates DEVICE_LOCAL memory (not CPU-mappable).
     *                     Vertex and Index buffers use this path; initial data is
     *                     uploaded later via StagingBufferPool::uploadImmediate().
     */
    VoidResult init(VulkanDevice* device, VkPhysicalDevice physicalDevice, const BufferDesc& desc,
                    bool deviceLocal = false);

    uint64_t size() const override { return m_size; }
    void* map() override;   ///< Returns nullptr for device-local buffers.
    void unmap() override;

    VkBuffer handle() const { return m_buffer; }
    bool isDeviceLocal() const { return m_deviceLocal; }

protected:
    void finalize() override;

private:
    VulkanDevice* m_device = nullptr;
    VkBuffer m_buffer = VK_NULL_HANDLE;
    VkDeviceMemory m_memory = VK_NULL_HANDLE;
    uint64_t m_size = 0;
    bool m_deviceLocal = false;
    void* m_mapped = nullptr;
};

} // namespace ln::rhi::vulkan
