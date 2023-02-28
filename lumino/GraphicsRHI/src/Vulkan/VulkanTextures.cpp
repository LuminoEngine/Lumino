#include <LuminoGraphicsRHI/RHIHelper.hpp>
#include "VulkanDeviceContext.hpp"
#include "VulkanBuffers.hpp"
#include "VulkanCommandList.hpp"
#include "VulkanTextures.hpp"

namespace ln {
namespace detail {

//==============================================================================
// VulkanTexture2D

VulkanTexture2D::VulkanTexture2D()
    : m_mipLevels(1) {
}

Result VulkanTexture2D::init(VulkanDevice* deviceContext, GraphicsResourceUsage usage, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData) {
    if (!VulkanTexture::initAsTexture2D(usage, width, height, requestFormat, mipmap)) return err();
    LN_DCHECK(deviceContext);
    m_deviceContext = deviceContext;
    //m_usage = usage;
    //   m_size.width = width;
    //   m_size.height = height;
    //   m_format = requestFormat;

    m_nativeFormat = VulkanHelper::LNFormatToVkFormat(requestFormat);
    VkDeviceSize imageSize = width * height * RHIHelper::getPixelSize(requestFormat);

    m_mipLevels = 1;
    if (mipmap) {
        m_mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;
    }

    if (!initialData) {
        LN_NOTIMPLEMENTED();
    }
    else {
        VkBufferUsageFlags usageFlags = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        if (mipmap) {
            // レベル 0 以外を生成するために、レベル 0 を転送元として扱う必要がある
            usageFlags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
        }

        VulkanBuffer stagingBuffer;
        if (!stagingBuffer.init(m_deviceContext, imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, nullptr)) {
            return err();
        }
        stagingBuffer.setData(0, initialData, imageSize);

        m_image.init(m_deviceContext, width, height, m_nativeFormat, m_mipLevels, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_TILING_OPTIMAL, usageFlags, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_COLOR_BIT);

        if (!m_deviceContext->transitionImageLayoutImmediately(m_image.vulkanImage(), m_nativeFormat, m_mipLevels, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)) {
            return err();
        }

        m_deviceContext->copyBufferToImageImmediately(stagingBuffer.nativeBuffer(), m_image.vulkanImage(), width, height);

        if (mipmap) {
            if (!generateMipmaps(m_image.vulkanImage(), m_nativeFormat, width, height, m_mipLevels)) {
                return err();
            }
            // transitioned to VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL while generating mipmaps
        }
        else {
            if (!m_deviceContext->transitionImageLayoutImmediately(m_image.vulkanImage(), m_nativeFormat, m_mipLevels, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)) {
                return err();
            }
        }

        stagingBuffer.dispose();
    }

    return ok();
}

void VulkanTexture2D::dispose() {
    m_image.dispose();
    VulkanTexture::dispose();
}

void VulkanTexture2D::setSubData(VulkanGraphicsContext* graphicsContext, int x, int y, int width, int height, const void* data, size_t dataSize) {
    // TODO:
    assert(x == 0);
    assert(y == 0);

    assert(m_mipLevels == 1); // TODO:

    // vkCmdCopyBufferToImage() の dstImageLayout は VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_SHARED_PRESENT_KHR の
    // いずれかでなければならない。https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/vkCmdCopyBufferToImage.html
    // 転送前にレイアウトを変更しておく。
    if (!m_deviceContext->transitionImageLayout(graphicsContext->recodingCommandBuffer()->vulkanCommandBuffer(), m_image.vulkanImage(), m_nativeFormat, 1, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)) {
        LN_ERROR();
        return;
    }

    VkBufferImageCopy region = {};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;
    region.imageOffset = { 0, 0, 0 };
    region.imageExtent = {
        static_cast<uint32_t>(width),
        static_cast<uint32_t>(height),
        1
    };
    VulkanBuffer* buffer = graphicsContext->recodingCommandBuffer()->cmdCopyBufferToImage(dataSize, region, &m_image);
    buffer->setData(0, data, dataSize);

    // レイアウトを元に戻す
    if (!m_deviceContext->transitionImageLayout(graphicsContext->recodingCommandBuffer()->vulkanCommandBuffer(), m_image.vulkanImage(), m_nativeFormat, 1, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)) {
        LN_ERROR();
        return;
    }
}

Result VulkanTexture2D::generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels) {
    VkPhysicalDevice physicalDevice = m_deviceContext->m_physicalDevice;

    // Check if image format supports linear blitting
    VkFormatProperties formatProperties;
    vkGetPhysicalDeviceFormatProperties(physicalDevice, imageFormat, &formatProperties);

    if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
        LN_ERROR("texture image format does not support linear blitting!");
        return err();
    }

    VkCommandBuffer commandBuffer = m_deviceContext->beginSingleTimeCommands();

    VkImageMemoryBarrier barrier = {};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.image = image;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    barrier.subresourceRange.levelCount = 1;

    int32_t mipWidth = texWidth;
    int32_t mipHeight = texHeight;

    for (uint32_t i = 1; i < mipLevels; i++) {
        barrier.subresourceRange.baseMipLevel = i - 1;
        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

        vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

        VkImageBlit blit = {};
        blit.srcOffsets[0] = { 0, 0, 0 };
        blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
        blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.srcSubresource.mipLevel = i - 1;
        blit.srcSubresource.baseArrayLayer = 0;
        blit.srcSubresource.layerCount = 1;
        blit.dstOffsets[0] = { 0, 0, 0 };
        blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
        blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        blit.dstSubresource.mipLevel = i;
        blit.dstSubresource.baseArrayLayer = 0;
        blit.dstSubresource.layerCount = 1;

        vkCmdBlitImage(commandBuffer, image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &blit, VK_FILTER_LINEAR);

        barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
        barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

        if (mipWidth > 1) mipWidth /= 2;
        if (mipHeight > 1) mipHeight /= 2;
    }

    barrier.subresourceRange.baseMipLevel = mipLevels - 1;
    barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &barrier);

    m_deviceContext->endSingleTimeCommands(commandBuffer);

    return ok();
}

//==============================================================================
// VulkanSwapchainRenderTargetTexture

VulkanRenderTarget::VulkanRenderTarget()
    : m_deviceContext(nullptr)
    , m_image(nullptr)
    , m_multisampleColorBuffer(nullptr)
    , m_swapchainImageAvailableSemaphoreRef(nullptr) {
}

Result VulkanRenderTarget::init(VulkanDevice* deviceContext, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, bool msaa) {
    if (!VulkanTexture::initAsRenderTarget(width, height, requestFormat, mipmap, msaa)) return err();
    LN_DCHECK(deviceContext);
    m_deviceContext = deviceContext;
    //m_size.width = width;
    //m_size.height = height;
    //m_format = requestFormat;

    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    LN_VK_CHECK(vkCreateSemaphore(m_deviceContext->vulkanDevice(), &semaphoreInfo, m_deviceContext->vulkanAllocator(), &m_renderFinishedSemaphore));

    {
        VkFormat nativeFormat = VulkanHelper::LNFormatToVkFormat(requestFormat);
        VkDeviceSize imageSize = width * height * RHIHelper::getPixelSize(requestFormat);

        VkImageUsageFlags usageFlags = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
        if (1) { // readData 可能にする
            usageFlags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
        }

        m_image = std::make_unique<VulkanImage>();
        if (!m_image->init(m_deviceContext, width, height, nativeFormat, 1, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_TILING_OPTIMAL, usageFlags, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_COLOR_BIT)) {
            return err();
        }

        if (!m_deviceContext->transitionImageLayoutImmediately(m_image->vulkanImage(), nativeFormat, 1, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)) {
            return err();
        }

        if (msaa) {
            m_multisampleColorBuffer = std::make_unique<VulkanImage>();
            if (!m_multisampleColorBuffer->init(
                    m_deviceContext, width, height, nativeFormat, 1, m_deviceContext->msaaSamples(), VK_IMAGE_TILING_OPTIMAL,
                    VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, // MultisampleColorBuffer は読み取り不要なので VK_IMAGE_USAGE_TRANSFER_SRC_BIT は不要
                    VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                    VK_IMAGE_ASPECT_COLOR_BIT)) {
                return err();
            }
        }
    }

    return ok();
}

Result VulkanRenderTarget::initFromSwapchainImage(VulkanDevice* deviceContext, uint32_t width, uint32_t height, VkFormat format, VkImage image, VkImageView imageView) {
    // TODO: SwapChain は BGRA フォーマットであることが多い。
    // ただ TextureFormat はそれに対応していないが、readData() で Bitmap をとるときにピクセルサイズが知りたい。
    // ここではダミーとして RGBA8 を与えて初期化してみる。
    if (!VulkanTexture::initAsRenderTarget(width, height, TextureFormat::RGBA8, false, false)) return err();
    LN_DCHECK(deviceContext);
    m_deviceContext = deviceContext;

    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    LN_VK_CHECK(vkCreateSemaphore(m_deviceContext->vulkanDevice(), &semaphoreInfo, m_deviceContext->vulkanAllocator(), &m_renderFinishedSemaphore));

    //m_size.width = width;
    //m_size.height = height;
    //m_format = VulkanHelper::VkFormatToLNFormat(format);

    m_image = std::make_unique<VulkanImage>();
    if (!m_image->initWrap(m_deviceContext, width, height, format, image, imageView)) {
        return err();
    }
    return ok();
}

void VulkanRenderTarget::dispose() {
    if (m_multisampleColorBuffer) {
        m_multisampleColorBuffer->dispose();
        m_multisampleColorBuffer = nullptr;
    }

    if (m_image) {
        m_image->dispose();
        m_image = nullptr;
    }

    if (m_deviceContext) {
        //m_deviceContext->framebufferCache()->invalidateRenderTarget(this);

        //if (m_imageAvailableSemaphore) {
        //    vkDestroySemaphore(m_deviceContext->vulkanDevice(), m_imageAvailableSemaphore, m_deviceContext->vulkanAllocator());
        //    m_imageAvailableSemaphore = VK_NULL_HANDLE;
        //}

        if (m_renderFinishedSemaphore) {
            vkDestroySemaphore(m_deviceContext->vulkanDevice(), m_renderFinishedSemaphore, m_deviceContext->vulkanAllocator());
            m_renderFinishedSemaphore = VK_NULL_HANDLE;
        }

        m_deviceContext = nullptr;
    }
    VulkanTexture::dispose();
}

RHIRef<RHIBitmap> VulkanRenderTarget::readData() {
    RHIRef<RHIBitmap> bitmap;

    // Flush
    //m_deviceContext->recodingCommandBuffer()->submit(imageAvailableSemaphore(), renderFinishedSemaphore());

    vkDeviceWaitIdle(m_deviceContext->vulkanDevice());

    VkImageLayout originalLayout;
    if (isSwapchainBackbuffer()) {
        originalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    }
    else {
        originalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    }

    uint32_t width = extentSize().width;
    uint32_t height = extentSize().height;
    VkDeviceSize size = width * height * 4; // TODO

    // 転送先として作成
    VulkanBuffer destBuffer;
    if (!destBuffer.init(m_deviceContext, size, VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, nullptr)) {
        LN_ERROR();
        goto Exit;
    }

    {
        VkCommandBuffer commandBuffer = m_deviceContext->beginSingleTimeCommands();

        // Swapchain の Backbuffer (originalLayout) から、転送ソース (VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL) へレイアウト変換
        {
            VkImageMemoryBarrier imageMemoryBarrier = {};
            imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            imageMemoryBarrier.pNext = nullptr;
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            imageMemoryBarrier.oldLayout = originalLayout;
            imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            imageMemoryBarrier.image = m_image->vulkanImage();
            imageMemoryBarrier.subresourceRange = VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
            vkCmdPipelineBarrier(
                commandBuffer,
                VK_PIPELINE_STAGE_TRANSFER_BIT,
                VK_PIPELINE_STAGE_TRANSFER_BIT,
                0,
                0,
                nullptr,
                0,
                nullptr,
                1,
                &imageMemoryBarrier);
        }

        // destBuffer へコピー
        {
            VkBufferImageCopy region = {};
            region.bufferOffset = 0;
            region.bufferRowLength = 0;
            region.bufferImageHeight = 0;
            region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            region.imageSubresource.mipLevel = 0;
            region.imageSubresource.baseArrayLayer = 0;
            region.imageSubresource.layerCount = 1;
            region.imageOffset = { 0, 0, 0 };
            region.imageExtent = { width, height, 1 };
            vkCmdCopyImageToBuffer(
                commandBuffer,
                m_image->vulkanImage(),
                VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
                destBuffer.nativeBuffer(),
                1,
                &region);
        }

        // レイアウトを元に戻す
        {
            VkImageMemoryBarrier imageMemoryBarrier = {};
            imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            imageMemoryBarrier.pNext = nullptr;
            imageMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
            imageMemoryBarrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
            imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
            imageMemoryBarrier.newLayout = originalLayout;
            imageMemoryBarrier.image = m_image->vulkanImage();
            imageMemoryBarrier.subresourceRange = VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };
            vkCmdPipelineBarrier(
                commandBuffer,
                VK_PIPELINE_STAGE_TRANSFER_BIT,
                VK_PIPELINE_STAGE_TRANSFER_BIT,
                0,
                0,
                nullptr,
                0,
                nullptr,
                1,
                &imageMemoryBarrier);
        }

        // Submit and Wait
        if (!m_deviceContext->endSingleTimeCommands(commandBuffer)) {
            LN_ERROR();
            goto Exit;
        }
    }

    bitmap = makeRHIRef<RHIBitmap>();
    if (!bitmap->init(4, width, height)) {
        return nullptr;
    }
    void* outData = bitmap->writableData();

    // Blit
    {
        void* rawData = destBuffer.map();

        // TODO: まだ Bitmap クラス側が BGRA の save に対応していないのでここで変換してしまう。
        if (m_image->vulkanFormat() == VK_FORMAT_B8G8R8A8_UNORM ||
            m_image->vulkanFormat() == VK_FORMAT_R8G8B8A8_UNORM) {

            if (m_image->vulkanFormat() == VK_FORMAT_B8G8R8A8_UNORM) {
                unsigned char* data = (unsigned char*)rawData;
                for (uint32_t y = 0; y < height; y++) {
                    unsigned char* row = data;
                    for (uint32_t x = 0; x < width; x++) {
                        std::swap(row[0], row[2]);
                        row += 4;
                    }
                    data += width * 4; //subResourceLayout.rowPitch;
                }
            }

            // V flip
            if (0) {
                for (uint32_t y = 0; y < height; y++) {
                    unsigned char* sr = static_cast<unsigned char*>(rawData) + ((y)*width) * 4;
                    unsigned char* dr = static_cast<unsigned char*>(outData) + ((height - y - 1) * width * 4);
                    memcpy(dr, sr, width * 4);
                }
            }
            else {
                memcpy(outData, rawData, height * width * 4);
            }
        }
        else if (m_image->vulkanFormat() == VK_FORMAT_R32_SINT) {
            memcpy(outData, rawData, height * width * 4);
        }
        else {
            LN_NOTIMPLEMENTED();
        }

        destBuffer.unmap();
    }

Exit:
    destBuffer.dispose();
    return bitmap;
}

//==============================================================================
// VulkanSwapchainRenderTargetTexture

//VulkanSwapchainRenderTargetTexture::VulkanSwapchainRenderTargetTexture()
//{
//}
//
//Result VulkanSwapchainRenderTargetTexture::init(VulkanDevice* deviceContext)
//{
//    LN_DCHECK(deviceContext);
//    m_deviceContext = deviceContext;
//    m_currentBufferIndex = 0;
//    return true;
//}
//
//void VulkanSwapchainRenderTargetTexture::dispose()
//{
//    if (m_deviceContext) {
//        m_deviceContext->framebufferCache()->invalidateRenderTarget(this);
//        m_deviceContext = nullptr;
//    }
//
//    clear();
//    VulkanTexture::dispose();
//}
//
//Result VulkanSwapchainRenderTargetTexture::reset(uint32_t width, uint32_t height, VkFormat format, const std::vector<VkImage>& images, const std::vector<VkImageView>& imageViews)
//{
//    LN_DCHECK(images.size() == imageViews.size());
//    clear();
//
//    m_size.width = width;
//    m_size.height = height;
//    m_format = VulkanHelper::VkFormatToLNFormat(format);
//
//    m_images.resize(images.size());
//    for (int i = 0; i < images.size(); i++) {
//        m_images[i] = std::make_shared<VulkanImage>();
//        if (!m_images[i]->init(m_deviceContext, format, images[i], imageViews[i])) {
//            return false;
//        }
//    }
//
//    return true;
//}
//
//void VulkanSwapchainRenderTargetTexture::clear()
//{
//    for (auto& image : m_images) {
//        image->dispose();
//    }
//    m_images.clear();
//}

//==============================================================================
// VulkanDepthBuffer

VulkanDepthBuffer::VulkanDepthBuffer() {
}

Result VulkanDepthBuffer::init(VulkanDevice* deviceContext, uint32_t width, uint32_t height) {
    LN_DCHECK(deviceContext);
    LN_TRY(RHIResource::initAsDepthBuffer(width, height, false));
    m_deviceContext = deviceContext;

    VkFormat depthFormat = m_deviceContext->findDepthFormat();

    if (!m_image.init(m_deviceContext, width, height, depthFormat, 1, VK_SAMPLE_COUNT_1_BIT, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_IMAGE_ASPECT_DEPTH_BIT)) {
        return err();
    }

    if (!m_deviceContext->transitionImageLayoutImmediately(m_image.vulkanImage(), depthFormat, 1, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)) {
        return err();
    }

    return ok();
}

void VulkanDepthBuffer::dispose() {
    if (m_deviceContext) {
        //m_deviceContext->framebufferCache()->invalidateDepthBuffer(this);
        m_deviceContext = nullptr;
    }

    m_image.dispose();
    RHIResource::dispose();
}

} // namespace detail
} // namespace ln
