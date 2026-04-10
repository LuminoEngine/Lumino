#pragma once
#include "VulkanCommon.hpp"

#include <vector>
#include <unordered_map>
#include <mutex>

#include "VulkanCacheKeys.hpp"
#include "DescriptorPoolManager.hpp"
#include "FrameResourceManager.hpp"
#include "StagingBufferPool.hpp"

namespace ln::rhi::vulkan {

class VulkanDevice final : public Device {
public:
    VulkanDevice();
    VoidResult init(const DeviceDesc& desc);

    bool isValid() const { return m_device != VK_NULL_HANDLE; }

    // RHI interface
    DeviceLimits deviceLimits() const override;
    Result<Ref<SwapChain>> createSwapChain(const SwapChainDesc& desc) override;
    Result<Ref<Buffer>> createBuffer(const BufferDesc& desc) override;
    Result<Ref<Texture>> createTexture(const TextureDesc& desc) override;
    Result<Ref<TextureView>> createTextureView(Texture* texture) override;
    Result<Ref<Sampler>> createSampler(const SamplerDesc& desc) override;
    Result<Ref<ShaderModule>> createShaderModule(const ShaderModuleDesc& desc) override;
    Result<Ref<PipelineLayout>> createPipelineLayout(const PipelineLayoutDesc& desc) override;
    Result<Ref<RenderPipeline>> createRenderPipeline(const RenderPipelineDesc& desc) override;
    VoidResult writeBuffer(Buffer* dst, u64 dstOffset, const void* data, u64 size) override;
    Result<std::vector<uint8_t>> readbackTexture(TextureView* view) override;
    void waitIdle() override;
    Backend backend() const override { return Backend::Vulkan; }

    // Internal accessors
    VkInstance instance() const { return m_instance; }
    VkDevice vkDevice() const { return m_device; }
    const VkAllocationCallbacks* vulkanAllocator() const { return nullptr; }// TODO: return m_allocator.vulkanAllocator();
    VkPhysicalDevice physicalDevice() const { return m_physicalDevice; }
    VkQueue graphicsQueue() const { return m_graphicsQueue; }
    uint32_t graphicsFamily() const { return m_graphicsQueuFamily; }
    VkCommandPool commandPool() const { return m_commandPool; }

    VkRenderPass getOrCreateRenderPass(const RenderPassKey& key);
    VkFramebuffer getOrCreateFramebuffer(const FramebufferKey& key);
    DescriptorPoolManager& descriptorPoolManager() { return m_descriptorPoolManager; }
    FrameResourceManager& frameResources() { return m_frameResources; }

    /**
     * Called at the start of each frame (after waiting for the in-flight fence)
     * to execute deferred cleanups for that frame index.
     */
    void beginFrame(u32 frameIndex) { m_frameResources.beginFrame(frameIndex); }

    void setActiveSwapChain(VulkanSwapChain* sc) { m_activeSwapChain = sc; }
    VulkanSwapChain* activeSwapChain() const { return m_activeSwapChain; }
    u32 currentFrameIndex() const;

    Result<Ref<VulkanCommandBuffer>> createCommandBuffer();

    Result<VkCommandBuffer> beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);

private:
    void finalize() override;
    static VoidResult lookupQueueFamilies(
        VkPhysicalDevice physicalDevice,
        uint32_t* outGraphicsQueuFamily);

    VkInstance m_instance = VK_NULL_HANDLE;
    VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;
    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
    VkDevice m_device = VK_NULL_HANDLE;

    uint32_t m_graphicsQueuFamily;
    VkQueue m_graphicsQueue = VK_NULL_HANDLE;
    VkCommandPool m_commandPool = VK_NULL_HANDLE;

    DescriptorPoolManager m_descriptorPoolManager;
    FrameResourceManager m_frameResources;
    StagingBufferPool m_stagingPool;

    VulkanSwapChain* m_activeSwapChain = nullptr;

    // Caches (hides Vulkan complexity from RHI users)
    std::mutex m_cacheMutex;
    std::unordered_map<RenderPassKey, VkRenderPass, RenderPassKeyHash> m_renderPassCache;
    std::unordered_map<FramebufferKey, VkFramebuffer, FramebufferKeyHash> m_framebufferCache;
};

} // namespace ln::rhi::vulkan
