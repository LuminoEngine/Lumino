#pragma once
#include "Common.hpp"

namespace ln {
namespace detail {

class WebGPUDevice : public IGraphicsDevice {
public:
    struct Settings {
        PlatformWindow* mainWindow = nullptr;
        bool debugMode = false;
    };

    WebGPUDevice();
    bool init(const Settings& settings);
    void dispose() override;

	WGPUDevice wgpuDevice() const { return m_device; }
    WGPUAdapter wgpuAdapter() const { return m_adapters[0].adapter; }
	
	WGPUSurface getWGPUSurface(PlatformWindow* window) const;

protected:
    INativeGraphicsInterface* getNativeInterface() const override;
    void onGetCaps(GraphicsDeviceCaps* outCaps) override;
    Ref<ISwapChain> onCreateSwapChain(PlatformWindow* window, const SizeI& backbufferSize) override;
    Ref<ICommandList> onCreateCommandList() override;
    Ref<IRenderPass> onCreateRenderPass(const DeviceFramebufferState& buffers, ClearFlags clearFlags, const Color& clearColor, float clearDepth, uint8_t clearStencil) override;
    Ref<IPipeline> onCreatePipeline(const DevicePipelineStateDesc& state) override;
    Ref<IVertexDeclaration> onCreateVertexDeclaration(const VertexElement* elements, int elementsCount) override;
    Ref<RHIResource> onCreateVertexBuffer(GraphicsResourceUsage usage, size_t bufferSize, const void* initialData) override;
    Ref<RHIResource> onCreateIndexBuffer(GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData) override;
    Ref<RHIResource> onCreateTexture2D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData) override;
    Ref<RHIResource> onCreateTexture3D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, uint32_t depth, TextureFormat requestFormat, bool mipmap, const void* initialData) override;
    Ref<RHIResource> onCreateRenderTarget(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, bool msaa) override;
    Ref<RHIResource> onCreateWrappedRenderTarget(intptr_t nativeObject, uint32_t hintWidth, uint32_t hintHeight) override;
    Ref<RHIResource> onCreateDepthBuffer(uint32_t width, uint32_t height) override;
    Ref<ISamplerState> onCreateSamplerState(const SamplerStateData& desc) override;
    Ref<IShaderPass> onCreateShaderPass(const ShaderPassCreateInfo& createInfo, ShaderCompilationDiag* diag) override;
    Ref<RHIResource> onCreateUniformBuffer(uint32_t size) override;
    Ref<IDescriptorPool> onCreateDescriptorPool(IShaderPass* shaderPass) override;
    void onQueueSubmit(ICommandList* context, RHIResource* affectRendreTarget) override;
    void onQueuePresent(ISwapChain* swapChain) override;

private:
    struct WebGPUAdapterEntry {
        WGPURequestAdapterStatus status;
        WGPUAdapter adapter;
    };

    Result<> requestDevice(WGPUAdapter adapter, const WGPUDeviceDescriptor& descriptor);
	
    WGPUInstance m_instance;
    WGPUSurface m_surface;
    std::vector<WebGPUAdapterEntry> m_adapters;
    WGPUDevice m_device;
    WGPUQueue m_queue;
};

} // namespace detail
} // namespace ln
