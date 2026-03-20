#pragma once
#include "Common.hpp"

namespace ln {
namespace detail {

class WebGPUDevice : public IGraphicsDevice {
public:
    struct Settings {
        bool debugMode = false;
    };

    WebGPUDevice();
    bool init(const Settings& settings);
    void dispose() override;

    WGPUInstance nativeInstance() const { return m_instance; }
	WGPUDevice wgpuDevice() const { return m_device; }
    WGPUQueue wgpuQueue() const { return m_queue; }
    WGPUAdapter wgpuAdapter() const { return m_adapters[0].adapter; }
    WebGPUBindGroupCache* bindGroupCache() const { return m_bindGroupCache.get(); }
    const Ref<WebGPUSingleFrameAllocatorPageManager>& transferBufferSingleFrameAllocator() const {
        return m_transferBufferSingleFrameAllocator;
    }

protected:
    INativeGraphicsInterface* getNativeInterface() const override;
    void onGetDeviceProperties(GraphicsDeviceProperties* outCaps) override;
    Result<Ref<ISwapChain>> onCreateSwapChain(const SwapChainCreateInfo& createInfo) override;
    Result<Ref<ICommandList>> onCreateCommandList() override;
    Result<Ref<IRenderPass>> onCreateRenderPass(const RenderPassCreateInfo& createInfo) override;
    Result<Ref<IPipeline>> onCreatePipeline(const DevicePipelineCreateInfo& createInfo) override;
    Ref<IVertexDeclaration> onCreateVertexDeclaration(const VertexElement* elements, int elementsCount) override;
    Ref<RHIResource> onCreateVertexBuffer(GraphicsResourceUsage usage, size_t bufferSize, const void* initialData) override;
    Ref<RHIResource> onCreateIndexBuffer(GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData) override;
    Ref<RHIResource> onCreateTexture2D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData) override;
    Ref<RHIResource> onCreateTexture3D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, uint32_t depth, TextureFormat requestFormat, bool mipmap, const void* initialData) override;
    Ref<RHIResource> onCreateRenderTarget(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, bool msaa) override;
    Ref<RHIResource> onCreateWrappedRenderTarget(intptr_t nativeObject, uint32_t hintWidth, uint32_t hintHeight) override;
    Ref<RHIResource> onCreateDepthBuffer(uint32_t width, uint32_t height) override;
    Result<Ref<ISamplerState>> onCreateSamplerState(const SamplerStateData& desc) override;
    Ref<IShaderPass> onCreateShaderPass(
        const ShaderPassCreateInfo& createInfo,
        const ShaderPassCreateInfo2* createInfo2OrNull,
        ShaderCompilationDiag* diag) override;
    Ref<RHIResource> onCreateUniformBuffer(uint32_t size) override;
    Ref<IDescriptorPool> onCreateDescriptorPool(IShaderPass* shaderPass) override;
    void onQueueSubmit(ICommandList* context, RHIResource* affectRendreTarget) override;
    void onQueuePresent(ISwapChain* swapChain) override;

private:
    struct WebGPUAdapterEntry {
        WGPURequestAdapterStatus status;
        WGPUAdapter adapter;
    };

    Result_deprecated<> requestDevice(WGPUAdapter adapter, const WGPUDeviceDescriptor& descriptor);
	
    WGPUInstance m_instance;
    WGPUSurface m_surface;
    std::vector<WebGPUAdapterEntry> m_adapters;
    WGPUDevice m_device;
    WGPUQueue m_queue;
    std::unique_ptr<WebGPUBindGroupCache> m_bindGroupCache;
    Ref<WebGPUSingleFrameAllocatorPageManager> m_transferBufferSingleFrameAllocator;
};

} // namespace detail
} // namespace ln
