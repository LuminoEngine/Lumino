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
    VoidResult writeBuffer(Buffer* dst, uint64_t dstOffset, const void* data, uint64_t size) override;
    Result<std::vector<uint8_t>> readbackTexture(TextureView* view) override;
    void waitIdle() override;
    Backend backend() const override { return Backend::Vulkan; }

    // Internal accessors
    VkInstance instance() const { return m_instance; }
    VkDevice vkDevice() const { return m_device; }
    const VkAllocationCallbacks* vulkanAllocator() const { return nullptr; }// TODO: return m_allocator.vulkanAllocator();
    VkPhysicalDevice physicalDevice() const { return m_physicalDevice; }
    VkQueue graphicsQueue() const { return m_graphicsQueue; }
    VkCommandPool commandPool() const { return m_commandPool; }

    VkRenderPass getOrCreateRenderPass(const RenderPassKey& key);
    VkFramebuffer getOrCreateFramebuffer(const FramebufferKey& key);
    DescriptorPoolManager& descriptorPoolManager() { return m_descriptorPoolManager; }
    FrameResourceManager& frameResources() { return m_frameResources; }

    /**
     * 各フレームの開始時 (in-flight フェンスを待った後) に呼び出し、
     * そのフレームインデックスの遅延破棄を実行する。
     */
    void beginFrame(uint32_t frameIndex) { m_frameResources.beginFrame(frameIndex); }

    void setActiveSwapChain(VulkanSwapChain* sc) { m_activeSwapChain = sc; }
    VulkanSwapChain* activeSwapChain() const { return m_activeSwapChain; }
    uint32_t currentFrameIndex() const;

    Result<Ref<VulkanCommandBuffer>> createCommandBuffer();

    Result<VkCommandBuffer> beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);

    /** VkResult を検査し、VK_ERROR_DEVICE_LOST であれば markDeviceLost する。r をそのまま返す。 */
    VkResult checkDeviceLost(VkResult r, const char* what);

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

    // キャッシュ (RHI 利用者から Vulkan の複雑さを隠す)
    std::mutex m_cacheMutex;
    std::unordered_map<RenderPassKey, VkRenderPass, RenderPassKeyHash> m_renderPassCache;
    std::unordered_map<FramebufferKey, VkFramebuffer, FramebufferKeyHash> m_framebufferCache;
};

} // namespace ln::rhi::vulkan
