
#pragma once
#include "VulkanHelper.hpp"

namespace ln {
namespace detail {
class VulkanGraphicsContext;

class VulkanTexture
    : public RHIResource {
public:
    virtual const VulkanImage* image() const = 0;
    virtual void setSubData(VulkanGraphicsContext* graphicsContext, int x, int y, int width, int height, const void* data, size_t dataSize) = 0;
    virtual void setSubData3D(VulkanGraphicsContext* graphicsContext, int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) = 0;

private:
};

class VulkanTexture2D
    : public VulkanTexture {
public:
    VulkanTexture2D();
    Result<> init(VulkanDevice* deviceContext, GraphicsResourceUsage usage, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData);
    void onDestroy() override;
    //virtual DeviceTextureType type() const { return DeviceTextureType::Texture2D; }
    RHIRef<RHIBitmap> readData() override {
        LN_UNREACHABLE();
        return nullptr;
    }
    void setSubData(VulkanGraphicsContext* graphicsContext, int x, int y, int width, int height, const void* data, size_t dataSize) override;
    void setSubData3D(VulkanGraphicsContext* graphicsContext, int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) override { LN_UNREACHABLE(); }

    virtual const VulkanImage* image() const override { return &m_image; }

private:
    Result<> generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);

    VulkanDevice* m_deviceContext;
    VulkanImage m_image;
    //GraphicsResourceUsage m_usage;
    //   RHISizeI m_size;
    //TextureFormat m_format;
    uint32_t m_mipLevels;
    VkFormat m_nativeFormat;
};

class VulkanRenderTarget
    : public VulkanTexture {
public:
    VulkanRenderTarget();
    Result<> init(VulkanDevice* deviceContext, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, bool msaa);
    Result<> initFromSwapchainImage(VulkanDevice* deviceContext, uint32_t width, uint32_t height, VkFormat format, VkImage image, VkImageView imageView);
    void onDestroy() override;
    //virtual DeviceTextureType type() const { return DeviceTextureType::RenderTarget; }
    RHIRef<RHIBitmap> readData() override;
    void setSubData(VulkanGraphicsContext* graphicsContext, int x, int y, int width, int height, const void* data, size_t dataSize) override {}
    void setSubData3D(VulkanGraphicsContext* graphicsContext, int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) override {}

    virtual const VulkanImage* image() const override { return m_image.get(); }
    const VulkanImage* multisampleColorBuffer() const { return m_multisampleColorBuffer.get(); }

    bool isSwapchainBackbuffer() const { return m_image->IsExternalManagement(); }
    //bool isMultisample() const override { return m_multisampleColorBuffer != nullptr; }
    VkSampleCountFlagBits msaaSamples() const { return (isMultisample()) ? m_deviceContext->msaaSamples() : VK_SAMPLE_COUNT_1_BIT; }
    void setSwapchainImageAvailableSemaphoreRef(VkSemaphore* semaphore) { m_swapchainImageAvailableSemaphoreRef = semaphore; }
    VkSemaphore swapchainImageAvailableSemaphore() const { return (m_swapchainImageAvailableSemaphoreRef) ? *m_swapchainImageAvailableSemaphoreRef : VK_NULL_HANDLE; }
    VkSemaphore renderFinishedSemaphore() const { return m_renderFinishedSemaphore; }

private:
    VulkanDevice* m_deviceContext;
    std::unique_ptr<VulkanImage> m_image;                  // MSAA 有効の時は、Resolve されたテクスチャデータ
    std::unique_ptr<VulkanImage> m_multisampleColorBuffer; // MSAA 有効の時に使う、マルチサンプル有効なレンダーターゲットバッファ。MSAA 無しのときは nullptr

    //   RHISizeI m_size;
    //TextureFormat m_format;

    // vkAcquireNextImageKHR により準備ができたかどうかを待つためのセマフォ。
    // この RenderTarget が Swapchain の Backbuffer である場合、そのポインタがセットされる。
    // present または readData による　CommandBuffer の Submit 時、これが nullptr でなければ待つようにしなければならない。
    VkSemaphore* m_swapchainImageAvailableSemaphoreRef;

    VkSemaphore m_renderFinishedSemaphore;
};

// ダブルバッファ・トリプルバッファを管理するため、通常の RenderTargetTexture とは別にする。
// IGraphicsDevice は DX11 相当の IF に合わせているので、ダブルバッファの実装はこちら側で隠蔽したい。
// 個別に公開するのは、特に readData の対象を外側で認識できるようにする必要があったりするのでちょっと大変。
//class VulkanSwapchainRenderTargetTexture
//    : public VulkanTexture
//{
//public:
//    VulkanSwapchainRenderTargetTexture();
//    Result<> init(VulkanDevice* deviceContext);
//    virtual void dispose();
//    virtual DeviceTextureType type() const { return DeviceTextureType::Texture2D; }
//    virtual SizeI realSize() { return m_size; }
//    virtual TextureFormat getTextureFormat() const { return m_format; }
//    virtual void readData(void* outData) { LN_UNREACHABLE(); }
//    virtual void setSubData(int x, int y, int width, int height, const void* data, size_t dataSize) { LN_NOTIMPLEMENTED(); }
//    virtual void setSubData3D(int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) { LN_UNREACHABLE(); }
//    virtual const VulkanImage* image() const override { return m_images[m_currentBufferIndex].get(); }
//
//    Result<> reset(uint32_t width, uint32_t height, VkFormat format, const std::vector<VkImage>& images, const std::vector<VkImageView>& imageViews);
//    void setCurrentBufferIndex(int index) { m_currentBufferIndex = index; }
//    uint32_t imageCount() const { return m_images.size(); }
//    VulkanImage* image(uint32_t index) const { return m_images[index].get(); }
//
//    //int prevBufferIndex() const { return (m_bufferIndex == 0) ? m_images.size() - 1 : m_bufferIndex - 1; }
//
//private:
//    void clear();
//
//    VulkanDevice* m_deviceContext;
//    std::vector<std::shared_ptr<VulkanImage>> m_images;
//    SizeI m_size;
//    TextureFormat m_format;
//    int m_currentBufferIndex;
//    //TextureDesc m_desc;
//    //std::vector<VkImage> m_images;
//    //std::vector<VkImageView> m_imageViews;
//    ////VkImageAspectFlags m_imageAspectFlags;
//    ////VkDeviceMemory m_deviceMemory;
//
//    //VkFormat m_vulkanFormat;
//};

class VulkanDepthBuffer : public RHIResource {
public:
    VulkanDepthBuffer();
    Result<> init(VulkanDevice* deviceContext, uint32_t width, uint32_t height);
    void onDestroy() override;
    const VulkanImage* image() const { return &m_image; }
    VkFormat nativeFormat() const { return m_deviceContext->findDepthFormat(); }

private:
    VulkanDevice* m_deviceContext;
    VulkanImage m_image;
};

} // namespace detail
} // namespace ln
