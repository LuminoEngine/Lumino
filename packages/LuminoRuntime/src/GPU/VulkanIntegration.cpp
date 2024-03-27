﻿#ifdef LN_USE_VULKAN
#include <LuminoEngine/GraphicsRHI/Vulkan/VulkanHelper.hpp>
#include <LuminoEngine/GraphicsRHI/Vulkan/VulkanDeviceContext.hpp>
#include <LuminoEngine/GraphicsRHI/Vulkan/VulkanTextures.hpp>
#include <LuminoEngine/GPU/Texture.hpp>
#include <LuminoEngine/GPU/DepthBuffer.hpp>
#include <LuminoEngine/GPU/VulkanIntegration.hpp>

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