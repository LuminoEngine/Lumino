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
        u32 width,
        u32 height);

    u32 width() const override { return m_width; }
    u32 height() const override { return m_height; }
    TextureFormat format() const override { return m_format; }
    VkImage handle() const { return m_image; }

protected:
    void finalize() override;

private:
    VulkanDevice* m_device;
    VkImage m_image = VK_NULL_HANDLE;
    VkDeviceMemory m_memory = VK_NULL_HANDLE;
    TextureFormat m_format = TextureFormat::RGBA8Unorm;
    u32 m_width = 0, m_height = 0;
    bool m_ownsImage = true;
};

} // namespace ln::rhi::vulkan
