#pragma once
#include <LuminoCore/Graphics/rhi/Rhi.hpp>

#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include <mutex>

#include "DescriptorPoolManager.hpp"
#include "FrameResourceManager.hpp"
#include "StagingBufferPool.hpp"

namespace ln::rhi::vulkan {

// ------ RenderPass Cache --------------------------------------------------------------------------------------------------------

struct RenderPassKey {
    struct ColorAttachment {
        VkFormat format;
        VkAttachmentLoadOp loadOp;
        bool operator==(const ColorAttachment& other) const;
    };

    //std::array<ColorAttachment, kMaxMultiRenderTargets> colorAttachments;
    std::vector<ColorAttachment> colorAttachments;
    VkFormat depthFormat = VK_FORMAT_UNDEFINED;
    VkAttachmentLoadOp stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;

    bool operator==(const RenderPassKey& other) const;
};

struct RenderPassKeyHash {
    size_t operator()(const RenderPassKey& key) const;
};

// ------ Framebuffer Cache ------------------------------------------------------------------------------------------------------

struct FramebufferKey {
    VkRenderPass renderPass = VK_NULL_HANDLE;
    std::vector<VkImageView> attachments;
    u32 width = 0, height = 0;

    bool operator==(const FramebufferKey& o) const;
};

struct FramebufferKeyHash {
    size_t operator()(const FramebufferKey& key) const;
};

// ------ VulkanDevice ----------------------------------------------------------------------------------------------------------------

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
    Result<Ref<BindGroupLayout>> createBindGroupLayout(const BindGroupLayoutDesc& desc) override;
    Result<Ref<BindGroup>> createBindGroup(const BindGroupDesc& desc) override;
    Result<Ref<PipelineLayout>> createPipelineLayout(const PipelineLayoutDesc& desc) override;
    Result<Ref<RenderPass>> createRenderPass(const RenderPassLayoutDesc& desc) override;
    Result<Ref<RenderPipeline>> createRenderPipeline(const RenderPipelineDesc& desc) override;
    Result<std::vector<uint8_t>> readbackTexture(TextureView* view) override;
    void waitIdle() override;

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
    u32 currentFrameIndex() const {
        return m_activeSwapChain ? m_activeSwapChain->currentFrame() : 0u;
    }

    Result<Ref<VulkanCommandBuffer>> createCommandBuffer();

    Result<VkCommandBuffer> beginSingleTimeCommands();
    void endSingleTimeCommands(VkCommandBuffer commandBuffer);
    //VoidResult transitionImageLayoutImmediately(
    //    VkImage image,
    //    VkFormat format,
    //    uint32_t mipLevel,
    //    VkImageLayout oldLayout,
    //    VkImageLayout newLayout);

private:
    void finalize() override;
    static VoidResult lookupQueueFamilies(
        VkPhysicalDevice physicalDevice,
        uint32_t* outGraphicsQueuFamily);

    VkInstance m_instance = VK_NULL_HANDLE;
    VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;
    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
    VkDevice m_device = VK_NULL_HANDLE;

    //std::vector<VkQueueFamilyProperties> m_queueFamilyProps;
    //std::optional<uint32_t> m_graphicsFamily;
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
