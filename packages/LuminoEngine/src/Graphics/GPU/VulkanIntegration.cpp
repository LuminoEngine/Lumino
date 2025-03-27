#ifdef LN_USE_VULKAN
#include <LuminoEngine/Graphics/GraphicsRHI/Vulkan/VulkanHelper.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/Vulkan/VulkanDeviceContext.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/Vulkan/VulkanTextures.hpp>
#include <LuminoEngine/Graphics/GPU/Texture.hpp>
#include <LuminoEngine/Graphics/GPU/DepthBuffer.hpp>
#include <LuminoEngine/Graphics/GPU/VulkanIntegration.hpp>

namespace ln {

void VulkanIntegration::getImageInfo(GraphicsCommandList* graphicsContext, RenderTargetTexture* texture, VkImage* outImage, VkImageView* outImageView, VkFormat* outFormat, int* outWidth, int* outHeight) {
    auto vulkanTexture = static_cast<detail::VulkanRenderTarget*>(detail::GraphicsResourceInternal::resolveRHIObject<detail::RHIResource>(graphicsContext, texture, nullptr));
    auto image = vulkanTexture->image();
    *outImage = image->vulkanImage();
    *outImageView = image->vulkanImageView();
    *outFormat = image->vulkanFormat();
    *outWidth = vulkanTexture->extentSize().width;
    *outHeight = vulkanTexture->extentSize().height;
}

void VulkanIntegration::getImageInfo(GraphicsCommandList* graphicsContext, DepthBuffer* texture, VkImage* outImage, VkImageView* outImageView, VkFormat* outFormat, int* outWidth, int* outHeight) {
    auto vulkanTexture = static_cast<detail::VulkanDepthBuffer*>(detail::GraphicsResourceInternal::resolveRHIObject<detail::RHIResource>(graphicsContext, texture, nullptr));
    auto image = vulkanTexture->image();
    *outImage = image->vulkanImage();
    *outImageView = image->vulkanImageView();
    *outFormat = image->vulkanFormat();
    *outWidth = vulkanTexture->extentSize().width;
    *outHeight = vulkanTexture->extentSize().height;
}

} // namespace ln

#endif
