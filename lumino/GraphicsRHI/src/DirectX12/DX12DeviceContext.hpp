
#pragma once
#include "DX12Helper.hpp"
#include "DX12SingleFrameAllocator.hpp"

namespace ln {
namespace detail {
class DX12RenderTarget;
class DX12GraphicsContext;

// DX12すべきこととすべきでないこと
// https://developer.nvidia.com/dx12-dos-and-donts
//
class DX12Device
	: public IGraphicsDevice
{
public:
	struct Settings
	{
		PlatformWindow* mainWindow = nullptr;
        bool debugMode = false;
        std::wstring priorityAdapterName;
	};

    static const int BackBufferCount = 3;

	DX12Device();
	bool init(const Settings& settings, bool* outIsDriverSupported);
	void dispose() override;

    UINT creationNodeMask() const { return 1; }
    UINT visibleNodeMask() const { return 1; }
    IDXGIFactory6* dxgiFactory() const { return m_dxgiFactory.Get(); }
    ID3D12Device* device() const { return m_device.Get(); }
    ID3D12CommandQueue* dxCommandQueue() const { return m_commandQueue.Get(); }
    ID3D12GraphicsCommandList* beginSingleTimeCommandList();
    bool endSingleTimeCommandList(ID3D12GraphicsCommandList* commandList);
    UINT msaaSampleCount() const { return m_sampleCount; }
    UINT msaaQualityLevel() const { return 0; }
    const Ref<DX12SingleFrameAllocatorPageManager>& uploadBufferAllocatorManager() const { return m_uploadBufferAllocatorManager; }
    ID3D12RootSignature* mipmapRootSignature() const { return m_mipmapRootSignature.Get(); }
    ID3D12PipelineState* mipmapPipelineState() const { return m_mipmapPipelineState.Get(); }

protected:
    INativeGraphicsInterface* getNativeInterface() const override;
    void onGetCaps(GraphicsDeviceCaps* outCaps) override;
    URef<ISwapChain> onCreateSwapChain(PlatformWindow* window, const SizeI& backbufferSize) override;
    URef<ICommandList> onCreateCommandList() override;
    URef<IRenderPass> onCreateRenderPass(const DeviceFramebufferState& buffers, ClearFlags clearFlags, const Color& clearColor, float clearDepth, uint8_t clearStencil) override;
    URef<IPipeline> onCreatePipeline(const DevicePipelineStateDesc& state) override;
    URef<IVertexDeclaration> onCreateVertexDeclaration(const VertexElement* elements, int elementsCount) override;
    URef<RHIResource> onCreateVertexBuffer(GraphicsResourceUsage usage, size_t bufferSize, const void* initialData) override;
    URef<RHIResource> onCreateIndexBuffer(GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData) override;
    URef<RHIResource> onCreateTexture2D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData) override;
    URef<RHIResource> onCreateTexture3D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, uint32_t depth, TextureFormat requestFormat, bool mipmap, const void* initialData) override;
    URef<RHIResource> onCreateRenderTarget(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, bool msaa) override;
    URef<RHIResource> onCreateWrappedRenderTarget(intptr_t nativeObject, uint32_t hintWidth, uint32_t hintHeight) override { LN_NOTIMPLEMENTED(); return nullptr; }
    URef<RHIResource> onCreateDepthBuffer(uint32_t width, uint32_t height) override;
    URef<ISamplerState> onCreateSamplerState(const SamplerStateData& desc) override;
    URef<IShaderPass> onCreateShaderPass(const ShaderPassCreateInfo& createInfo, ShaderCompilationDiag* diag) override;
    URef<RHIResource> onCreateUniformBuffer(uint32_t size) override;
    URef<IDescriptorPool> onCreateDescriptorPool(IShaderPass* shaderPass) override;
    void onQueueSubmit(ICommandList* context, RHIResource* affectRendreTarget) override;
    void onQueuePresent(ISwapChain* swapChain) override;
    ICommandQueue* getGraphicsCommandQueue() override;
    ICommandQueue* getComputeCommandQueue() override;

public: // TODO:
    void enableDebugLayer() const;

    ComPtr<IDXGIFactory6> m_dxgiFactory;
    ComPtr<IDXGIAdapter> m_adapter;
    ComPtr<ID3D12Device> m_device;
    ComPtr<ID3D12CommandQueue> m_commandQueue;
    UINT64 m_fenceValue;
    UINT m_sampleCount;

    ComPtr<ID3D12CommandAllocator> m_singleTimeCommandAllocator;
    ComPtr<ID3D12GraphicsCommandList> m_singleTimeCommandList;
    ComPtr<ID3D12Fence> m_singleTimeCommandListFence;
    HANDLE m_singleTimeCommandListEvent;

    Ref<DX12SingleFrameAllocatorPageManager> m_uploadBufferAllocatorManager;
    ComPtr<ID3D12RootSignature> m_mipmapRootSignature;
    ComPtr<ID3D12PipelineState> m_mipmapPipelineState;
};

class DX12Framebuffer2;


class DX12Pipeline
	: public IPipeline
{
public:
	DX12Pipeline();
    bool init(DX12Device* deviceContext, const DevicePipelineStateDesc& state);
        void onDestroy() override;
    ID3D12PipelineState* dxPipelineState() const { return m_pipelineState.Get(); }

private:
	DX12Device* m_device;
    ComPtr<ID3D12PipelineState> m_pipelineState;
};

class DX12VertexDeclaration
    : public IVertexDeclaration
{
public:
    DX12VertexDeclaration();
    Result<> init(const VertexElement* elements, int elementsCount);
    void onDestroy() override;

    const std::vector<D3D12_INPUT_ELEMENT_DESC>& elements() const { return m_elements; }
    int32_t stride(int streamIndex) const { return m_strides[streamIndex]; }

private:
    std::vector<D3D12_INPUT_ELEMENT_DESC> m_elements;
    std::array<int32_t, MaxVertexStreams> m_strides;
};


class DX12SamplerState
	: public ISamplerState
{
public:
	DX12SamplerState();
    Result<> init(DX12Device* deviceContext, const SamplerStateData& desc);
    void onDestroy() override;

    const D3D12_SAMPLER_DESC& samplerDesc() const { return m_samplerDesc; }

protected:
    D3D12_SAMPLER_DESC m_samplerDesc;
};

} // namespace detail
} // namespace ln
