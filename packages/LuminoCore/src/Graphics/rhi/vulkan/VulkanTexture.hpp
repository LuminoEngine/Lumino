#pragma once
#include "VulkanCommon.hpp"

namespace ln::rhi::vulkan {

class VulkanTexture final : public Texture {
public:
    VulkanTexture();
    VoidResult init(VulkanDevice* device, VkPhysicalDevice physicalDevice, const TextureDesc& desc);
    /** Wraps an externally-owned VkImage (e.g. swap chain image). */
    VoidResult initFromExternalImage(
        VulkanDevice* device,
        VkImage image,
        TextureFormat format,
        uint32_t width,
        uint32_t height);

    uint32_t width() const override { return m_width; }
    uint32_t height() const override { return m_height; }
    TextureFormat format() const override { return m_format; }
    VkImage handle() const { return m_image; }

protected:
    void finalize() override;

private:
    VulkanDevice* m_device;
    VkImage m_image = VK_NULL_HANDLE;
    VkDeviceMemory m_memory = VK_NULL_HANDLE;
    TextureFormat m_format = TextureFormat::RGBA8Unorm;
    uint32_t m_width = 0, m_height = 0;
    bool m_ownsImage = true;
};

} // namespace ln::rhi::vulkan
