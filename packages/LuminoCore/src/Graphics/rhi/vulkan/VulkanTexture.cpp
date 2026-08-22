#include "VulkanHelpers.hpp"
#include "VulkanDevice.hpp"
#include "VulkanTexture.hpp"

namespace ln::rhi::vulkan {

VulkanTexture::VulkanTexture() = default;

static bool hasStencilComponent(VkFormat format) {
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

VoidResult VulkanTexture::init(
    VulkanDevice* device,
    VkPhysicalDevice physicalDevice,
    const TextureDesc& desc) {
    m_device = device;
    m_format = desc.format;
    m_width = desc.width;
    m_height = desc.height;
    m_ownsImage = true;

    VkImageCreateInfo imgInfo{};
    imgInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imgInfo.imageType = VK_IMAGE_TYPE_2D;
    imgInfo.format = VulkanHelpers::toVkFormat(desc.format);
    imgInfo.extent = {desc.width, desc.height, 1};
    imgInfo.mipLevels = desc.mipLevels;
    imgInfo.arrayLayers = desc.depthOrArrayLayers;
    imgInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imgInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imgInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    // Vulkan VK_IMAGE_LAYOUT_UNDEFINED または VK_IMAGE_LAYOUT_PREINITIALIZED でなければならない。
    // (VUID-VkImageCreateInfo-initialLayout-00993)
    imgInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    auto usage = desc.usage;
    if (static_cast<uint32_t>(usage) & static_cast<uint32_t>(TextureUsage::Sampled))
        imgInfo.usage |= VK_IMAGE_USAGE_SAMPLED_BIT;
    if (static_cast<uint32_t>(usage) & static_cast<uint32_t>(TextureUsage::RenderTarget))
        imgInfo.usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    if (static_cast<uint32_t>(usage) & static_cast<uint32_t>(TextureUsage::DepthStencil)) {
        imgInfo.usage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    }
    if (static_cast<uint32_t>(usage) & static_cast<uint32_t>(TextureUsage::CopyDst))
        imgInfo.usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    if (static_cast<uint32_t>(usage) & static_cast<uint32_t>(TextureUsage::CopySrc))
        imgInfo.usage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

    VkDevice vkDevice = m_device->vkDevice();
    if (vkCreateImage(vkDevice, &imgInfo, nullptr, &m_image) != VK_SUCCESS) {
        return LN_MAKE_ERROR("vkCreateImage failed.");
    }

    VkMemoryRequirements memReqs;
    vkGetImageMemoryRequirements(vkDevice, m_image, &memReqs);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memReqs.size;
    allocInfo.memoryTypeIndex = VulkanHelpers::findMemoryType(
        physicalDevice,
        memReqs.memoryTypeBits,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    if (vkAllocateMemory(vkDevice, &allocInfo, nullptr, &m_memory) != VK_SUCCESS) {
        return LN_MAKE_ERROR("vkAllocateMemory failed.");
    }
    vkBindImageMemory(vkDevice, m_image, m_memory, 0);

    // VkImageCreateInfo::initialLayout でレイアウトを指定できそうなものだけど、
    // ↑のコメントの通り実際にはできないので、ここで明示的にレイアウト遷移しておく。
    const bool isRenderTarget = (static_cast<uint32_t>(usage) &
                                 static_cast<uint32_t>(TextureUsage::RenderTarget)) != 0;
    const bool isDepthBuffer = (static_cast<uint32_t>(usage) &
                                static_cast<uint32_t>(TextureUsage::DepthStencil)) != 0;
    if (isRenderTarget || isDepthBuffer) {
        auto result = m_device->beginSingleTimeCommands();
        if (!result) {
            return LN_FORWARD_ERROR(result);
        }
        VkCommandBuffer commandBuffer = *result;

        VkImageMemoryBarrier barrier = {};
        barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        if (isRenderTarget) {
            // RenderTarget は、RenderPass で描画中以外は
            // VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL (シェーダで読み取り可能) にしておく。
            // VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL (レンダーターゲット書き込み用) にしておく例もあるが、
            // どのみち使う時には VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL する処理をどこかに挟まないとならないので、
            // 逆に複雑になると考え、このようにしている。
            barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        }
        else {
            barrier.newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
            if (hasStencilComponent(imgInfo.format)) {
                barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
            }
        }
        barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
        barrier.image = m_image;
        barrier.subresourceRange.baseMipLevel = 0;
        barrier.subresourceRange.levelCount = 1;
        barrier.subresourceRange.baseArrayLayer = 0;
        barrier.subresourceRange.layerCount = 1;

        vkCmdPipelineBarrier(
            commandBuffer,
            VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
            VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, // Fragment Shader で使うまでにはレイアウト遷移しておく必要がある。
            0,
            0,
            nullptr,
            0,
            nullptr,
            1,
            &barrier);

        m_device->endSingleTimeCommands(commandBuffer);
    }
    return LN_MAKE_SUCCESS();
}

VoidResult VulkanTexture::initFromExternalImage(
    VulkanDevice* device,
    VkImage image,
    TextureFormat format,
    uint32_t width,
    uint32_t height) {
    m_device = device;
    m_image = image;
    m_format = format;
    m_width = width;
    m_height = height;
    m_ownsImage = false;
    return LN_MAKE_SUCCESS();
}

void VulkanTexture::finalize() {
    if (m_ownsImage) {
        VkDevice dev = m_device->vkDevice();
        VkImage img = m_image;
        VkDeviceMemory mem = m_memory;
        m_device->frameResources().queueDelete(m_device->currentFrameIndex(), [dev, img, mem]() {
            if (img) vkDestroyImage(dev, img, nullptr);
            if (mem) vkFreeMemory(dev, mem, nullptr);
        });
    }
    Texture::finalize();
}

} // namespace ln::rhi::vulkan
