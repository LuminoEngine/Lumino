#pragma once

/// @file vulkan_backend.hpp
/// Internal Vulkan implementation of the RHI interfaces.
/// Pipeline caches, descriptor pools, and framebuffer management are
/// hidden here so the public RHI API stays clean.

#include <lumino_core/graphics/rhi/rhi.hpp>

#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include <mutex>

namespace lumino::rhi::vulkan {

// ─── Utilities ───────────────────────────────────────────────────────────

VkFormat toVkFormat(TextureFormat fmt);
VkFormat toVkVertexFormat(VertexFormat fmt);
u32 vertexFormatSize(VertexFormat fmt);

// ─── VulkanBuffer ────────────────────────────────────────────────────────

class VulkanBuffer final : public Buffer {
public:
    VulkanBuffer(VkDevice device, VkPhysicalDevice physDevice, const BufferDesc& desc);
    ~VulkanBuffer() override;

    u64 size() const override { return size_; }
    void* map() override;
    void unmap() override;

    VkBuffer handle() const { return buffer_; }

private:
    VkDevice device_;
    VkBuffer buffer_ = VK_NULL_HANDLE;
    VkDeviceMemory memory_ = VK_NULL_HANDLE;
    u64 size_ = 0;
    void* mapped_ = nullptr;
};

// ─── VulkanTexture ───────────────────────────────────────────────────────

class VulkanTexture final : public Texture {
public:
    VulkanTexture(VkDevice device, VkPhysicalDevice physDevice, const TextureDesc& desc);
    /// Wraps an externally-owned VkImage (e.g. swap chain image).
    VulkanTexture(VkDevice device, VkImage image, TextureFormat format, u32 width, u32 height);
    ~VulkanTexture() override;

    u32 width() const override { return width_; }
    u32 height() const override { return height_; }
    TextureFormat format() const override { return format_; }
    VkImage handle() const { return image_; }

private:
    VkDevice device_;
    VkImage image_ = VK_NULL_HANDLE;
    VkDeviceMemory memory_ = VK_NULL_HANDLE;
    TextureFormat format_;
    u32 width_, height_;
    bool ownsImage_ = true;
};

// ─── VulkanTextureView ───────────────────────────────────────────────────

class VulkanTextureView final : public TextureView {
public:
    VulkanTextureView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspect, u32 width, u32 height);
    ~VulkanTextureView() override;

    VkImageView handle() const { return view_; }
    VkFormat vkFormat() const { return format_; }
    u32 width() const { return width_; }
    u32 height() const { return height_; }

private:
    VkDevice device_;
    VkImageView view_ = VK_NULL_HANDLE;
    VkFormat format_;
    u32 width_, height_;
};

// ─── VulkanSampler ───────────────────────────────────────────────────────

class VulkanSampler final : public Sampler {
public:
    VulkanSampler(VkDevice device, const SamplerDesc& desc);
    ~VulkanSampler() override;
    VkSampler handle() const { return sampler_; }

private:
    VkDevice device_;
    VkSampler sampler_ = VK_NULL_HANDLE;
};

// ─── VulkanShaderModule ──────────────────────────────────────────────────

class VulkanShaderModule final : public ShaderModule {
public:
    VulkanShaderModule(VkDevice device, const ShaderModuleDesc& desc);
    ~VulkanShaderModule() override;
    VkShaderModule handle() const { return module_; }

private:
    VkDevice device_;
    VkShaderModule module_ = VK_NULL_HANDLE;
};

// ─── VulkanBindGroupLayout ───────────────────────────────────────────────

class VulkanBindGroupLayout final : public BindGroupLayout {
public:
    VulkanBindGroupLayout(VkDevice device, const BindGroupLayoutDesc& desc);
    ~VulkanBindGroupLayout() override;
    VkDescriptorSetLayout handle() const { return layout_; }

private:
    VkDevice device_;
    VkDescriptorSetLayout layout_ = VK_NULL_HANDLE;
};

// ─── VulkanBindGroup ─────────────────────────────────────────────────────

class VulkanBindGroup final : public BindGroup {
public:
    VulkanBindGroup(
        VkDevice device,
        VkDescriptorPool pool,
        VkDescriptorSetLayout layout,
        const BindGroupDesc& desc);
    ~VulkanBindGroup() override;
    VkDescriptorSet handle() const { return set_; }

private:
    VkDevice device_;
    VkDescriptorPool pool_;
    VkDescriptorSet set_ = VK_NULL_HANDLE;
};

// ─── VulkanPipelineLayout ────────────────────────────────────────────────

class VulkanPipelineLayout final : public PipelineLayout {
public:
    VulkanPipelineLayout(VkDevice device, const PipelineLayoutDesc& desc);
    ~VulkanPipelineLayout() override;
    VkPipelineLayout handle() const { return layout_; }

private:
    VkDevice device_;
    VkPipelineLayout layout_ = VK_NULL_HANDLE;
};

// ─── VulkanRenderPipeline ────────────────────────────────────────────────

class VulkanRenderPipeline final : public RenderPipeline {
public:
    VulkanRenderPipeline(VkDevice device, VkRenderPass renderPass, const RenderPipelineDesc& desc);
    ~VulkanRenderPipeline() override;
    VkPipeline handle() const { return pipeline_; }

private:
    VkDevice device_;
    VkPipeline pipeline_ = VK_NULL_HANDLE;
};

// ─── VulkanRenderPassEncoder ─────────────────────────────────────────────

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
    void setViewport(f32 x, f32 y, f32 w, f32 h, f32 minDepth, f32 maxDepth) override;
    void setScissorRect(u32 x, u32 y, u32 w, u32 h) override;
    void draw(u32 vertexCount, u32 instanceCount, u32 firstVertex, u32 firstInstance) override;
    void drawIndexed(u32 indexCount, u32 instanceCount, u32 firstIndex, i32 baseVertex, u32 firstInstance) override;
    void end() override;

private:
    VkCommandBuffer cmd_;
    VkPipelineLayout currentPipelineLayout_ = VK_NULL_HANDLE;
};

// ─── VulkanCommandBuffer ─────────────────────────────────────────────────

class VulkanDevice;

class VulkanCommandBuffer final : public CommandBuffer {
public:
    VulkanCommandBuffer(VulkanDevice* device, VkCommandBuffer cmd);
    ~VulkanCommandBuffer() override = default;

    RenderPassEncoder* beginRenderPass(const RenderPassDesc& desc) override;
    void submit() override;

private:
    VulkanDevice* device_;
    VkCommandBuffer cmd_;
    VulkanRenderPassEncoder* encoder_ = nullptr;
};

// ─── VulkanSwapChain ─────────────────────────────────────────────────────

class VulkanSwapChain final : public SwapChain {
public:
    VulkanSwapChain(VulkanDevice* device, const SwapChainDesc& desc);
    ~VulkanSwapChain() override;

    TextureView* acquireNextTexture() override;
    void present() override;
    u32 width() const override { return extent_.width; }
    u32 height() const override { return extent_.height; }
    TextureFormat format() const override { return format_; }

    VkSemaphore imageAvailableSemaphore() const;
    VkSemaphore renderFinishedSemaphore() const;
    VkFence inFlightFence() const;
    u32 currentImageIndex() const { return imageIndex_; }
    u32 currentFrame() const { return currentFrame_; }

private:
    void cleanup();

    VulkanDevice* device_;
    VkSwapchainKHR swapchain_ = VK_NULL_HANDLE;
    VkExtent2D extent_{};
    TextureFormat format_;

    std::vector<VkImage> images_;
    std::vector<Ref<VulkanTextureView>> views_;

    static constexpr u32 MAX_FRAMES = 2;
    VkSemaphore imageAvailable_[MAX_FRAMES]{};
    VkSemaphore renderFinished_[MAX_FRAMES]{};
    VkFence inFlightFences_[MAX_FRAMES]{};
    u32 currentFrame_ = 0;
    u32 imageIndex_ = 0;
};

// ─── RenderPass Cache ────────────────────────────────────────────────────

struct RenderPassKey {
    std::vector<VkFormat> colorFormats;
    VkFormat depthFormat = VK_FORMAT_UNDEFINED;
    std::vector<VkAttachmentLoadOp> loadOps;

    bool operator==(const RenderPassKey& o) const;
};

struct RenderPassKeyHash {
    size_t operator()(const RenderPassKey& key) const;
};

// ─── Framebuffer Cache ───────────────────────────────────────────────────

struct FramebufferKey {
    VkRenderPass renderPass = VK_NULL_HANDLE;
    std::vector<VkImageView> attachments;
    u32 width = 0, height = 0;

    bool operator==(const FramebufferKey& o) const;
};

struct FramebufferKeyHash {
    size_t operator()(const FramebufferKey& key) const;
};

// ─── VulkanDevice ────────────────────────────────────────────────────────

class VulkanDevice final : public Device {
public:
    VulkanDevice(const DeviceDesc& desc);
    ~VulkanDevice() override;

    bool isValid() const { return device_ != VK_NULL_HANDLE; }

    // RHI interface
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
    CommandBuffer* createCommandBuffer() override;
    void waitIdle() override;

    // Internal accessors
    VkInstance instance() const { return instance_; }
    VkDevice vkDevice() const { return device_; }
    VkPhysicalDevice physicalDevice() const { return physicalDevice_; }
    VkQueue graphicsQueue() const { return graphicsQueue_; }
    u32 graphicsFamily() const { return graphicsFamily_; }
    VkCommandPool commandPool() const { return commandPool_; }

    VkRenderPass getOrCreateRenderPass(const RenderPassKey& key);
    VkFramebuffer getOrCreateFramebuffer(const FramebufferKey& key);
    VkDescriptorPool descriptorPool() const { return descriptorPool_; }

    void setActiveSwapChain(VulkanSwapChain* sc) { activeSwapChain_ = sc; }
    VulkanSwapChain* activeSwapChain() const { return activeSwapChain_; }

private:
    VkInstance instance_ = VK_NULL_HANDLE;
    VkDebugUtilsMessengerEXT debugMessenger_ = VK_NULL_HANDLE;
    VkPhysicalDevice physicalDevice_ = VK_NULL_HANDLE;
    VkDevice device_ = VK_NULL_HANDLE;
    u32 graphicsFamily_ = 0;
    VkQueue graphicsQueue_ = VK_NULL_HANDLE;
    VkCommandPool commandPool_ = VK_NULL_HANDLE;
    VkDescriptorPool descriptorPool_ = VK_NULL_HANDLE;

    VulkanSwapChain* activeSwapChain_ = nullptr;

    // Caches (hides Vulkan complexity from RHI users)
    std::mutex cacheMutex_;
    std::unordered_map<RenderPassKey, VkRenderPass, RenderPassKeyHash> renderPassCache_;
    std::unordered_map<FramebufferKey, VkFramebuffer, FramebufferKeyHash> framebufferCache_;
};

} // namespace lumino::rhi::vulkan
