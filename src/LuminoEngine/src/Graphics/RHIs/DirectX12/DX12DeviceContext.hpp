
#pragma once
#include "DX12Helper.hpp"

namespace ln {
namespace detail {
class DX12RenderTarget;
class DX12GraphicsContext;

class DX12Device
	: public IGraphicsDevice
{
public:
	struct Settings
	{
		PlatformWindow* mainWindow = nullptr;
        bool debugMode = false;
        bool useBasicRenderDriver = true;
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
    UINT sampleCount() const { return m_sampleCount; }

protected:
    INativeGraphicsInterface* getNativeInterface() const override;
    void onGetCaps(GraphicsDeviceCaps* outCaps) override;
    Ref<ISwapChain> onCreateSwapChain(PlatformWindow* window, const SizeI& backbufferSize) override;
    Ref<ICommandList> onCreateCommandList() override;
    Ref<IRenderPass> onCreateRenderPass(const DeviceFramebufferState& buffers, ClearFlags clearFlags, const Color& clearColor, float clearDepth, uint8_t clearStencil) override;
    Ref<IPipeline> onCreatePipeline(const DevicePipelineStateDesc& state) override;
    Ref<IVertexDeclaration> onCreateVertexDeclaration(const VertexElement* elements, int elementsCount) override;
    Ref<IVertexBuffer> onCreateVertexBuffer(GraphicsResourceUsage usage, size_t bufferSize, const void* initialData) override;
    Ref<IIndexBuffer> onCreateIndexBuffer(GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData) override;
    Ref<ITexture> onCreateTexture2D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData) override;
    Ref<ITexture> onCreateTexture3D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, uint32_t depth, TextureFormat requestFormat, bool mipmap, const void* initialData) override;
    Ref<ITexture> onCreateRenderTarget(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, bool msaa) override;
    Ref<ITexture> onCreateWrappedRenderTarget(intptr_t nativeObject, uint32_t hintWidth, uint32_t hintHeight) override { LN_NOTIMPLEMENTED(); return nullptr; }
    Ref<IDepthBuffer> onCreateDepthBuffer(uint32_t width, uint32_t height) override;
    Ref<ISamplerState> onCreateSamplerState(const SamplerStateData& desc) override;
    Ref<IShaderPass> onCreateShaderPass(const ShaderPassCreateInfo& createInfo, ShaderCompilationDiag* diag) override;
    Ref<IUniformBuffer> onCreateUniformBuffer(uint32_t size) override;
    Ref<IDescriptorPool> onCreateDescriptorPool(IShaderPass* shaderPass) override;
    void onSubmitCommandBuffer(ICommandList* context, ITexture* affectRendreTarget) override;
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
};

class DX12Framebuffer2;


class DX12Pipeline
	: public IPipeline
{
public:
	DX12Pipeline();
    bool init(DX12Device* deviceContext, const DevicePipelineStateDesc& state);
	void dispose() override;
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
    Result init(const VertexElement* elements, int elementsCount);
    void dispose() override;

    const std::vector<D3D12_INPUT_ELEMENT_DESC>& elements() const { return m_elements; }
    int32_t stride(int streamIndex) const { return m_strides[streamIndex]; }

private:
    std::vector<D3D12_INPUT_ELEMENT_DESC> m_elements;
    std::array<int32_t, MaxVertexStreams> m_strides;
};

class DX12IndexBuffer
    : public IIndexBuffer
{
public:
    DX12IndexBuffer();
    Result init(DX12Device* deviceContext, GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData);
    void dispose() override;
    size_t getBytesSize() override;
    GraphicsResourceUsage usage() const override;
    void* map() override;
    void unmap() override;

protected:
    DX12Device* m_deviceContext;
    GraphicsResourceUsage m_usage;
};

class DX12UniformBuffer
    : public IUniformBuffer
{
public:
    DX12UniformBuffer();
    bool init(DX12Device* deviceContext, uint32_t size);
    void dispose() override;
    void* map() override;
    void unmap() override;
    ID3D12Resource* dxResource() const { return m_constantBuffer.Get(); }
    uint32_t size() const { return m_size; }

protected:
    DX12Device* m_deviceContext;
    uint32_t m_size;
    ComPtr<ID3D12Resource> m_constantBuffer;
    void* m_mappedBuffer;
};

class DX12Texture
    : public ITexture
{
public:
    DX12Texture()
        : m_currentState(D3D12_RESOURCE_STATE_COMMON)
    {}
    //virtual const DX12Image* image() const = 0;
    virtual void setSubData(DX12GraphicsContext* graphicsContext, int x, int y, int width, int height, const void* data, size_t dataSize) = 0;
    virtual void setSubData3D(DX12GraphicsContext* graphicsContext, int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) = 0;
   
    virtual ID3D12Resource* dxResource() const = 0;
    void resourceBarrior(ID3D12GraphicsCommandList* commandList, D3D12_RESOURCE_STATES newState);

protected:
    D3D12_RESOURCE_STATES m_currentState;
};

class DX12SamplerState
	: public ISamplerState
{
public:
	DX12SamplerState();
	Result init(DX12Device* deviceContext, const SamplerStateData& desc);
	virtual void dispose() override;

protected:
	DX12Device* m_deviceContext;

};

} // namespace detail
} // namespace ln
