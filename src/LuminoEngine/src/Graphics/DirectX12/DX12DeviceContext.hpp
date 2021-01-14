
#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl/client.h>
#include "../GraphicsDeviceContext.hpp"
#include "DX12Helper.hpp"

using Microsoft::WRL::ComPtr;

namespace ln {
namespace detail {
class DX12ShaderDescriptorTable;

class DX12Device
	: public IGraphicsDevice
{
public:
	struct Settings
	{
		PlatformWindow* mainWindow = nullptr;
        bool debugMode = false;
	};

	DX12Device();
	bool init(const Settings& settings, bool* outIsDriverSupported);
	void dispose() override;

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
    void onFlushCommandBuffer(ICommandList* context, ITexture* affectRendreTarget) override;
    ICommandQueue* getGraphicsCommandQueue() override;
    ICommandQueue* getComputeCommandQueue() override;

private:
    void enableDebugLayer() const;

    ComPtr<IDXGIFactory6> m_dxgiFactory;
    ComPtr<IDXGIAdapter> m_adapter;
    ComPtr<ID3D12Device> m_device;

    ComPtr<ID3D12CommandAllocator> m_commandAllocator;
    ComPtr<ID3D12GraphicsCommandList> m_commandList;


    //IDXGIFactory6* _dxgiFactory = nullptr;
    //
    //ID3D12CommandQueue* _cmdQueue = nullptr;
    //IDXGISwapChain4* _swapchain = nullptr;
};

class DX12GraphicsContext
	: public ICommandList
{
public:
	DX12GraphicsContext();
	bool init(DX12Device* owner);
	void dispose();

protected:
    void onSaveExternalRenderState() override;
    void onRestoreExternalRenderState() override;
    void onBeginCommandRecoding() override;
    void onEndCommandRecoding() override;
    void onBeginRenderPass(IRenderPass* renderPass) override;
    void onEndRenderPass(IRenderPass* renderPass) override;
    void onSubmitStatus(const GraphicsContextState& state, uint32_t stateDirtyFlags, GraphicsContextSubmitSource submitSource, IPipeline* pipeline) override;
    void* onMapResource(IGraphicsRHIBuffer* resource, uint32_t offset, uint32_t size) override;
    void onUnmapResource(IGraphicsRHIBuffer* resource) override;
    void onSetSubData(IGraphicsRHIBuffer* resource, size_t offset, const void* data, size_t length) override;
    void onSetSubData2D(ITexture* resource, int x, int y, int width, int height, const void* data, size_t dataSize) override;
    void onSetSubData3D(ITexture* resource, int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) override;
    void onSetDescriptorTableData(IShaderDescriptorTable* resource, const ShaderDescriptorTableUpdateInfo* data) override;
    void onClearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil) override;
    void onDrawPrimitive(PrimitiveTopology primitive, int startVertex, int primitiveCount) override;
    void onDrawPrimitiveIndexed(PrimitiveTopology primitive, int startIndex, int primitiveCount, int instanceCount, int vertexOffset) override;
    void onDrawExtension(INativeGraphicsExtension* extension) override;

private:
	DX12Device* m_device;
};

class DX12SwapChain
	: public ISwapChain
{
public:
	DX12SwapChain();
	Result init(DX12Device* deviceContext, PlatformWindow* window, const SizeI& backbufferSize);
    void dispose() override;
    uint32_t getBackbufferCount() override;
    void acquireNextImage(int* outImageIndex) override;
	ITexture* getRenderTarget(int imageIndex) const override;
	Result resizeBackbuffer(uint32_t width, uint32_t height) override;
	void present() override;

private:
    DX12Device* m_device;
};

class DX12Framebuffer2;

class DX12RenderPass
	: public IRenderPass
{
public:
    DX12RenderPass();
	bool init(DX12Device* device, const DeviceFramebufferState& buffers, ClearFlags clearFlags, const Color& clearColor, float clearDepth, uint8_t clearStencil);
	void dispose() override;

	ClearFlags clearFlags() const { return m_clearFlags; }
	const Color& clearColor() const { return m_clearColor; }
	float clearDepth() const { return m_clearDepth; }
	uint8_t clearStencil() const { return m_clearStencil; }

private:
	DX12Device* m_device;
	ClearFlags m_clearFlags;
	Color m_clearColor;
	float m_clearDepth;
	uint8_t m_clearStencil;
};

class DX12Pipeline
	: public IPipeline
{
public:
	DX12Pipeline();
    bool init(DX12Device* deviceContext, const DevicePipelineStateDesc& state);
	void dispose() override;

private:
	DX12Device* m_device;
};

class DX12VertexDeclaration
    : public IVertexDeclaration
{
public:
    DX12VertexDeclaration();
    Result init(const VertexElement* elements, int elementsCount);
    void dispose() override;

    const std::vector<VertexElement>& elements() const { return m_elements; }

private:
    std::vector<VertexElement> m_elements;
};

class DX12VertexBuffer
    : public IVertexBuffer
{
public:
    DX12VertexBuffer();
    Result init(DX12Device* deviceContext, GraphicsResourceUsage usage, size_t bufferSize, const void* initialData);
    void dispose() override;
    size_t getBytesSize() override;
    GraphicsResourceUsage usage() const override;
    void* map() override;
    void unmap() override;

private:
    DX12Device* m_deviceContext;
    GraphicsResourceUsage m_usage;
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
    Result init(DX12Device* deviceContext, uint32_t size);
    void dispose() override;
    void* map() override;
    void unmap() override;

protected:
    DX12Device* m_deviceContext;
    uint32_t m_size;
};

class DX12Texture
    : public ITexture
{
public:
    //virtual const DX12Image* image() const = 0;
    virtual void setSubData(DX12GraphicsContext* graphicsContext, int x, int y, int width, int height, const void* data, size_t dataSize) = 0;
    virtual void setSubData3D(DX12GraphicsContext* graphicsContext, int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) = 0;

private:
};

class DX12Texture2D
	: public DX12Texture
{
public:
	DX12Texture2D();
	Result init(DX12Device* deviceContext, GraphicsResourceUsage usage, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData);
    virtual void dispose();
	virtual DeviceTextureType type() const { return DeviceTextureType::Texture2D; }
	virtual SizeI realSize() { return m_size; }
    virtual TextureFormat getTextureFormat() const { return m_format; }
	virtual GraphicsResourceUsage usage() const override { return m_usage; }
	virtual void readData(void* outData) { LN_UNREACHABLE(); }
    virtual void setSubData(DX12GraphicsContext* graphicsContext, int x, int y, int width, int height, const void* data, size_t dataSize) override;
	virtual void setSubData3D(DX12GraphicsContext* graphicsContext, int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) { LN_UNREACHABLE(); }

    //virtual const DX12Image* image() const override { return &m_image; }

private:
	DX12Device* m_deviceContext;
	GraphicsResourceUsage m_usage;
	SizeI m_size;
	TextureFormat m_format;
	uint32_t m_mipLevels;
};

class DX12RenderTarget
    : public DX12Texture
{
public:
    DX12RenderTarget();
    Result init(DX12Device* deviceContext, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap);
    virtual void dispose() override;
    virtual DeviceTextureType type() const { return DeviceTextureType::RenderTarget; }
    virtual SizeI realSize() { return m_size; }
    virtual TextureFormat getTextureFormat() const { return TextureFormat::RGBA8; }
    virtual GraphicsResourceUsage usage() const override { return GraphicsResourceUsage::Static; }
    virtual void readData(void* outData) override;
    virtual void setSubData(DX12GraphicsContext* graphicsContext, int x, int y, int width, int height, const void* data, size_t dataSize) {}
    virtual void setSubData3D(DX12GraphicsContext* graphicsContext, int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize) {}

    //virtual const DX12Image* image() const override { return m_image.get(); }

protected:
    DX12Device* m_deviceContext;
    SizeI m_size;
    TextureFormat m_format;
};

class DX12DepthBuffer
	: public IDepthBuffer
{
public:
    DX12DepthBuffer();
    Result init(DX12Device* deviceContext, uint32_t width, uint32_t height);
    void dispose();
    const SizeI& size() const { return m_size; }
    //const DX12Image* image() const { return &m_image; }

private:
    DX12Device* m_deviceContext;
    SizeI m_size;
};

class DX12SamplerState
	: public ISamplerState
{
public:
	DX12SamplerState();
	Result init(DX12Device* deviceContext, const SamplerStateData& desc);
	virtual void dispose() override;

private:
	DX12Device* m_deviceContext;
};

class DX12ShaderPass
    : public IShaderPass
{
public:
    DX12ShaderPass();
    Result init(DX12Device* deviceContext, const ShaderPassCreateInfo& createInfo, ShaderCompilationDiag* diag);
    void dispose();
    virtual IShaderDescriptorTable* descriptorTable() const;

private:
    DX12Device* m_deviceContext;
    Ref<DX12ShaderDescriptorTable> m_descriptorTable;
};

class DX12ShaderDescriptorTable
    : public IShaderDescriptorTable
{
public:
    DX12ShaderDescriptorTable();
    bool init(DX12Device* deviceContext, const DX12ShaderPass* ownerPass, const DescriptorLayout* descriptorLayout);
    void dispose() override;

private:

};

} // namespace detail
} // namespace ln
