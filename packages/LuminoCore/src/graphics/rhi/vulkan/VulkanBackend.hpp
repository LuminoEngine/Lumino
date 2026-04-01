#pragma once

/**
 * @file vulkan_backend.hpp
 * Internal Vulkan implementation of the RHI interfaces.
 * Pipeline caches, descriptor pools, and framebuffer management are
 * hidden here so the public RHI API stays clean.
 */

#include <LuminoCore/graphics/rhi/Rhi.hpp>

#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include <mutex>

#include "DescriptorPoolManager.hpp"
#include "FrameResourceManager.hpp"
#include "StagingBufferPool.hpp"

namespace ln::rhi::vulkan {

// ------ Utilities ----------------------------------------------------------------------------------------------------------------------

VkFormat toVkFormat(TextureFormat fmt);
VkFormat toVkVertexFormat(VertexFormat fmt);
u32 vertexFormatSize(VertexFormat fmt);

// Forward declaration so Buffer/Sampler/BindGroup/Pipeline can hold VulkanDevice*
class VulkanDevice;

// ------ VulkanBuffer ----------------------------------------------------------------------------------------------------------------

class VulkanBuffer final : public Buffer {
public:
    VulkanBuffer();
    /**
     * @param deviceLocal  If true, allocates DEVICE_LOCAL memory (not CPU-mappable).
     *                     Vertex and Index buffers use this path; initial data is
     *                     uploaded later via StagingBufferPool::uploadImmediate().
     */
    VoidResult init(VulkanDevice* device, VkPhysicalDevice physicalDevice, const BufferDesc& desc,
                    bool deviceLocal = false);

    u64 size() const override { return m_size; }
    void* map() override;   ///< Returns nullptr for device-local buffers.
    void unmap() override;

    VkBuffer handle() const { return m_buffer; }

protected:
    void finalize() override;

private:
    VulkanDevice* m_device = nullptr;
    VkBuffer m_buffer = VK_NULL_HANDLE;
    VkDeviceMemory m_memory = VK_NULL_HANDLE;
    u64 m_size = 0;
    bool m_deviceLocal = false;
    void* m_mapped = nullptr;
};

// ------ VulkanTexture --------------------------------------------------------------------------------------------------------------

class VulkanTexture final : public Texture {
public:
    VulkanTexture();
    VoidResult init(VkDevice device, VkPhysicalDevice physicalDevice, const TextureDesc& desc);
    /** Wraps an externally-owned VkImage (e.g. swap chain image). */
    VoidResult initFromExternalImage(VkDevice device, VkImage image, TextureFormat format, u32 width, u32 height);

    u32 width() const override { return m_width; }
    u32 height() const override { return m_height; }
    TextureFormat format() const override { return m_format; }
    VkImage handle() const { return m_image; }

protected:
    void finalize() override;

private:
    VkDevice m_device = VK_NULL_HANDLE;
    VkImage m_image = VK_NULL_HANDLE;
    VkDeviceMemory m_memory = VK_NULL_HANDLE;
    TextureFormat m_format = TextureFormat::RGBA8Unorm;
    u32 m_width = 0, m_height = 0;
    bool m_ownsImage = true;
};

// ------ VulkanTextureView ------------------------------------------------------------------------------------------------------

class VulkanTextureView final : public TextureView {
public:
    VulkanTextureView();
    VoidResult init(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspect, u32 width, u32 height);

    VkImageView handle() const { return m_view; }
    VkFormat vkFormat() const { return m_format; }
    u32 width() const { return m_width; }
    u32 height() const { return m_height; }

protected:
    void finalize() override;

private:
    VkDevice m_device = VK_NULL_HANDLE;
    VkImageView m_view = VK_NULL_HANDLE;
    VkFormat m_format = VK_FORMAT_UNDEFINED;
    u32 m_width = 0, m_height = 0;
};

// ------ VulkanSampler --------------------------------------------------------------------------------------------------------------

class VulkanSampler final : public Sampler {
public:
    VulkanSampler();
    VoidResult init(VulkanDevice* device, const SamplerDesc& desc);
    VkSampler handle() const { return m_sampler; }

protected:
    void finalize() override;

private:
    VulkanDevice* m_device = nullptr;
    VkSampler m_sampler = VK_NULL_HANDLE;
};

// ------ VulkanShaderModule ----------------------------------------------------------------------------------------------------

class VulkanShaderModule final : public ShaderModule {
public:
    VulkanShaderModule();
    VoidResult init(VkDevice device, const ShaderModuleDesc& desc);
    VkShaderModule handle() const { return m_module; }

protected:
    void finalize() override;

private:
    VkDevice m_device = VK_NULL_HANDLE;
    VkShaderModule m_module = VK_NULL_HANDLE;
};

// ------ VulkanBindGroupLayout ----------------------------------------------------------------------------------------------

class VulkanBindGroupLayout final : public BindGroupLayout {
public:
    VulkanBindGroupLayout();
    VoidResult init(VkDevice device, const BindGroupLayoutDesc& desc);
    VkDescriptorSetLayout handle() const { return m_layout; }

    /** Per-binding dynamic offset flags (parallel to desc.entries). */
    const std::vector<bool>& dynamicFlags() const { return m_dynamicFlags; }

protected:
    void finalize() override;

private:
    VkDevice m_device = VK_NULL_HANDLE;
    VkDescriptorSetLayout m_layout = VK_NULL_HANDLE;
    std::vector<bool> m_dynamicFlags;
};

// ------ VulkanBindGroup ----------------------------------------------------------------------------------------------------------

class VulkanBindGroup final : public BindGroup {
public:
    VulkanBindGroup();
    VoidResult init(VulkanDevice* device, DescriptorPoolManager& poolManager,
                    VulkanBindGroupLayout* layout, const BindGroupDesc& desc);
    VkDescriptorSet handle() const { return m_set; }

protected:
    void finalize() override;

private:
    VulkanDevice* m_device = nullptr;
    VkDescriptorPool m_pool = VK_NULL_HANDLE; ///< Pool that owns m_set; used for vkFreeDescriptorSets.
    VkDescriptorSet m_set = VK_NULL_HANDLE;
};

// ------ VulkanPipelineLayout ------------------------------------------------------------------------------------------------

class VulkanPipelineLayout final : public PipelineLayout {
public:
    VulkanPipelineLayout();
    VoidResult init(VkDevice device, const PipelineLayoutDesc& desc);
    VkPipelineLayout handle() const { return m_layout; }

protected:
    void finalize() override;

private:
    VkDevice m_device = VK_NULL_HANDLE;
    VkPipelineLayout m_layout = VK_NULL_HANDLE;
};

// ------ VulkanRenderPipeline ------------------------------------------------------------------------------------------------

class VulkanRenderPipeline final : public RenderPipeline {
public:
    VulkanRenderPipeline();
    VoidResult init(VulkanDevice* device, VkRenderPass renderPass, const RenderPipelineDesc& desc);
    VkPipeline handle() const { return m_pipeline; }
    VkPipelineLayout layoutHandle() const { return m_layout; }

protected:
    void finalize() override;

private:
    VulkanDevice* m_device = nullptr;
    VkPipeline m_pipeline = VK_NULL_HANDLE;
    VkPipelineLayout m_layout = VK_NULL_HANDLE;
};

// ------ VulkanRenderPassEncoder ------------------------------------------------------------------------------------------

class VulkanRenderPassEncoder final : public RenderPassEncoder {
public:
    VulkanRenderPassEncoder(VkCommandBuffer cmd, VkRenderPass renderPass,
                            VkFramebuffer framebuffer, VkExtent2D extent,
                            const RenderPassDesc& desc);
    ~VulkanRenderPassEncoder() override = default;

    void setPipeline(RenderPipeline* pipeline) override;
    void setVertexBuffer(u32 slot, Buffer* buffer, u64 offset) override;
    void setIndexBuffer(Buffer* buffer, IndexFormat format, u64 offset) override;
    void setBindGroup(u32 index, BindGroup* group) override;
    void setBindGroup(u32 index, BindGroup* group,
                      const u32* dynamicOffsets, u32 dynamicOffsetCount) override;
    void setViewport(f32 x, f32 y, f32 w, f32 h, f32 minDepth, f32 maxDepth) override;
    void setScissorRect(u32 x, u32 y, u32 w, u32 h) override;
    void draw(u32 vertexCount, u32 instanceCount, u32 firstVertex, u32 firstInstance) override;
    void drawIndexed(u32 indexCount, u32 instanceCount, u32 firstIndex, i32 baseVertex, u32 firstInstance) override;
    void end() override;

private:
    VkCommandBuffer m_cmd;
    VkPipelineLayout m_currentPipelineLayout = VK_NULL_HANDLE;
};

// ------ VulkanCommandBuffer --------------------------------------------------------------------------------------------------

class VulkanCommandBuffer final : public CommandBuffer {
public:
    VulkanCommandBuffer();
    VoidResult init(VulkanDevice* device, VkCommandBuffer cmd);

    void dispose();

    VoidResult begin();
    RenderPassEncoder* beginRenderPass(const RenderPassDesc& desc) override;
    void submit() override;

protected:
    void finalize() override;

private:
    VulkanDevice* m_device = nullptr;
    VkCommandBuffer m_cmd = VK_NULL_HANDLE;
    VulkanRenderPassEncoder* m_encoder = nullptr;
    VkFence m_inFlightFences = VK_NULL_HANDLE;
    /** Frame index recorded at submit() time; used to schedule deferred cleanup. */
    u32 m_submittedFrame = 0;
    bool m_submitted = false;
};

// ------ VulkanSwapChain ----------------------------------------------------------------------------------------------------------

class VulkanSwapChain final : public SwapChain {
public:
    VulkanSwapChain();
    VoidResult init(VulkanDevice* device, const SwapChainDesc& desc);

    TextureView* acquireNextTexture() override;
    void present() override;
    u32 width() const override { return m_extent.width; }
    u32 height() const override { return m_extent.height; }
    //TextureFormat format() const override { return m_format; }

    VkSemaphore imageAvailableSemaphore() const;
    VkSemaphore renderFinishedSemaphore() const;
    u32 currentImageIndex() const { return m_imageIndex; }
    u32 currentFrame() const { return m_currentFrame; }

public:
    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;

        VkSurfaceTransformFlagBitsKHR preTransform;
        VkCompositeAlphaFlagBitsKHR compositeAlpha;
    };
    static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
    static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);

    void finalize() override;

    VulkanDevice* m_device;
    VkSurfaceKHR m_surface = VK_NULL_HANDLE;
    VkSwapchainKHR m_swapchain = VK_NULL_HANDLE;
    VkExtent2D m_extent{};
    //TextureFormat m_format;

    int m_maxFrames;
    std::vector<VkImage> m_images;
    std::vector<Ref<VulkanTextureView>> m_views;
    std::vector<Ref<VulkanCommandBuffer>> m_commandBuffers;
    // NOTE: なぜ SwapChain に CommandBuffer を持たせるのか？
    //   https://webgpufundamentals.org/webgpu/lessons/ja/webgpu-multiple-canvases.html
    //   コチラを見ると、 WebGPU ではひとつの CommandEncoder を複数の SwapChain で共有できるるように見える。
    //   ただしこれをやろうとすると、 CommandBuffer に RenderTarget として使われた Context を、
    //   次の present 対象として覚えておく必要があるなど、管理が複雑になる。

    std::vector<VkSemaphore> m_imageAvailableSemaphores;
    std::vector<VkSemaphore> m_renderFinished;
    u32 m_currentFrame = 0;
    u32 m_imageIndex = 0;
};

// ------ RenderPass Cache --------------------------------------------------------------------------------------------------------

struct RenderPassKey {
    std::vector<VkFormat> colorFormats;
    VkFormat depthFormat = VK_FORMAT_UNDEFINED;
    std::vector<VkAttachmentLoadOp> loadOps;

    bool operator==(const RenderPassKey& o) const;
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
    Result<Ref<RenderPipeline>> createRenderPipeline(const RenderPipelineDesc& desc) override;
    void waitIdle() override;

    // Internal accessors
    VkInstance instance() const { return m_instance; }
    VkDevice vkDevice() const { return m_device; }
    const VkAllocationCallbacks* vulkanAllocator() const { return nullptr; }// TODO: return m_allocator.vulkanAllocator();
    VkPhysicalDevice physicalDevice() const { return m_physicalDevice; }
    VkQueue graphicsQueue() const { return m_graphicsQueue; }
    u32 graphicsFamily() const { return m_graphicsFamily; }
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
    CommandBuffer* getCommandBuffer() override;

protected:
    void finalize() override;

private:
    VkInstance m_instance = VK_NULL_HANDLE;
    VkDebugUtilsMessengerEXT m_debugMessenger = VK_NULL_HANDLE;
    VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
    VkDevice m_device = VK_NULL_HANDLE;
    u32 m_graphicsFamily = 0;
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
