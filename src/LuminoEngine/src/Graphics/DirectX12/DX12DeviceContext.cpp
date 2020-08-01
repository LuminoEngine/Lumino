
#include "Internal.hpp"
#include <LuminoEngine/Platform/PlatformWindow.hpp>
#include <LuminoEngine/Platform/PlatformSupport.hpp>
#include "DX12DeviceContext.hpp"

namespace ln {
namespace detail {

//==============================================================================
// DX12Device

DX12Device::DX12Device()
{
}

bool DX12Device::init(const Settings& settings, bool* outIsDriverSupported)
{
	if (LN_REQUIRE(outIsDriverSupported)) return false;
	*outIsDriverSupported = true;

	return true;
}

void DX12Device::dispose()
{
    IGraphicsDevice::dispose();
}

INativeGraphicsInterface* DX12Device::getNativeInterface() const
{
	return nullptr;
}

void DX12Device::onGetCaps(GraphicsDeviceCaps * outCaps)
{
    LN_NOTIMPLEMENTED();
    //outCaps->requestedShaderTriple.target = "spv";
    //outCaps->requestedShaderTriple.version = 110;
    //outCaps->requestedShaderTriple.option = "";
}

Ref<ISwapChain> DX12Device::onCreateSwapChain(PlatformWindow* window, const SizeI& backbufferSize)
{
	auto ptr = makeRef<DX12SwapChain>();
    if (!ptr->init(this, window, backbufferSize)) {
        return nullptr;
    }
	return ptr;
}

Ref<ICommandList> DX12Device::onCreateCommandList()
{
	auto ptr = makeRef<DX12GraphicsContext>();
	if (!ptr->init(this)) {
		return nullptr;
	}
	return ptr;
}

Ref<IRenderPass> DX12Device::onCreateRenderPass(const DeviceFramebufferState& buffers, ClearFlags clearFlags, const Color& clearColor, float clearDepth, uint8_t clearStencil)
{
	auto ptr = makeRef<DX12RenderPass>();
	if (!ptr->init(this, buffers, clearFlags, clearColor, clearDepth, clearStencil)) {
		return nullptr;
	}
	return ptr;
}

Ref<IPipeline> DX12Device::onCreatePipeline(const DevicePipelineStateDesc& state)
{
	auto ptr = makeRef<DX12Pipeline>();
	if (!ptr->init(this, state)) {
		return nullptr;
	}
	return ptr;
}

Ref<IVertexDeclaration> DX12Device::onCreateVertexDeclaration(const VertexElement* elements, int elementsCount)
{
    auto ptr = makeRef<DX12VertexDeclaration>();
    if (!ptr->init(elements, elementsCount)) {
        return nullptr;
    }
    return ptr;
}

Ref<IVertexBuffer> DX12Device::onCreateVertexBuffer(GraphicsResourceUsage usage, size_t bufferSize, const void* initialData)
{
    auto ptr = makeRef<DX12VertexBuffer>();
    if (!ptr->init(this, usage, bufferSize, initialData)) {
        return nullptr;
    }
	return ptr;
}

Ref<IIndexBuffer> DX12Device::onCreateIndexBuffer(GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData)
{
    auto ptr = makeRef<DX12IndexBuffer>();
    if (!ptr->init(this, usage, format, indexCount, initialData)) {
        return nullptr;
    }
	return ptr;
}

Ref<ITexture> DX12Device::onCreateTexture2D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData)
{
    auto ptr = makeRef<DX12Texture2D>();
    if (!ptr->init(this, usage, width, height, requestFormat, mipmap, initialData)) {
        return nullptr;
    }
	return ptr;
}

Ref<ITexture> DX12Device::onCreateTexture3D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, uint32_t depth, TextureFormat requestFormat, bool mipmap, const void* initialData)
{
	LN_NOTIMPLEMENTED();
	return nullptr;
}

Ref<ITexture> DX12Device::onCreateRenderTarget(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap)
{
    auto ptr = makeRef<DX12RenderTarget>();
    if (!ptr->init(this, width, height, requestFormat, mipmap)) {
        return nullptr;
    }
    return ptr;
}

Ref<IDepthBuffer> DX12Device::onCreateDepthBuffer(uint32_t width, uint32_t height)
{
	auto ptr = makeRef<DX12DepthBuffer>();
    if (!ptr->init(this, width, height)) {
        return nullptr;
    }
	return ptr;
}

Ref<ISamplerState> DX12Device::onCreateSamplerState(const SamplerStateData& desc)
{
	auto ptr = makeRef<DX12SamplerState>();
	if (!ptr->init(this, desc)) {
		return nullptr;
	}
	return ptr;
}

Ref<IShaderPass> DX12Device::onCreateShaderPass(const ShaderPassCreateInfo& createInfo, ShaderCompilationDiag* diag)
{
    auto ptr = makeRef<DX12ShaderPass>();
    if (!ptr->init(this, createInfo, diag)) {
        return nullptr;
    }
    return ptr;
}

void DX12Device::onFlushCommandBuffer(ICommandList* context, ITexture* affectRendreTarget)
{
    LN_NOTIMPLEMENTED();
}

ICommandQueue* DX12Device::getGraphicsCommandQueue()
{
	LN_NOTIMPLEMENTED();
	return nullptr;
}

ICommandQueue* DX12Device::getComputeCommandQueue()
{
	LN_NOTIMPLEMENTED();
	return nullptr;
}

//==============================================================================
// DX12GraphicsContext

DX12GraphicsContext::DX12GraphicsContext()
	: m_device(nullptr)
{
}

bool DX12GraphicsContext::init(DX12Device* owner)
{
	LN_CHECK(owner);
	ICommandList::init(owner);
	m_device = owner;

	return true;
}

void DX12GraphicsContext::dispose()
{
    ICommandList::dispose();
}

void DX12GraphicsContext::onSaveExternalRenderState()
{
}

void DX12GraphicsContext::onRestoreExternalRenderState()
{
}

void DX12GraphicsContext::onBeginCommandRecoding()
{
    LN_NOTIMPLEMENTED();
}

void DX12GraphicsContext::onEndCommandRecoding()
{
    LN_NOTIMPLEMENTED();
}

void DX12GraphicsContext::onBeginRenderPass(IRenderPass* renderPass_)
{
    LN_NOTIMPLEMENTED();
}

void DX12GraphicsContext::onEndRenderPass(IRenderPass* renderPass)
{
    LN_NOTIMPLEMENTED();
}

void DX12GraphicsContext::onSubmitStatus(const GraphicsContextState& state, uint32_t stateDirtyFlags, GraphicsContextSubmitSource submitSource, IPipeline* pipeline)
{
    LN_NOTIMPLEMENTED();
}

void* DX12GraphicsContext::onMapResource(IGraphicsRHIBuffer* resource, uint32_t offset, uint32_t size)
{
    LN_NOTIMPLEMENTED();
    return nullptr;
}

void DX12GraphicsContext::onUnmapResource(IGraphicsRHIBuffer* resource)
{
    LN_NOTIMPLEMENTED();
}

void DX12GraphicsContext::onSetSubData(IGraphicsRHIBuffer* resource, size_t offset, const void* data, size_t length)
{
    LN_NOTIMPLEMENTED();
}

void DX12GraphicsContext::onSetSubData2D(ITexture* resource, int x, int y, int width, int height, const void* data, size_t dataSize)
{
    LN_NOTIMPLEMENTED();
}

void DX12GraphicsContext::onSetSubData3D(ITexture* resource, int x, int y, int z, int width, int height, int depth, const void* data, size_t dataSize)
{
    LN_NOTIMPLEMENTED();
}

void DX12GraphicsContext::onSetDescriptorTableData(IShaderDescriptorTable* resource, const ShaderDescriptorTableUpdateInfo* data)
{
    LN_NOTIMPLEMENTED();
}

void DX12GraphicsContext::onClearBuffers(ClearFlags flags, const Color& color, float z, uint8_t stencil)
{
    LN_NOTIMPLEMENTED();
}

void DX12GraphicsContext::onDrawPrimitive(PrimitiveTopology primitive, int startVertex, int primitiveCount)
{
    LN_NOTIMPLEMENTED();
}

void DX12GraphicsContext::onDrawPrimitiveIndexed(PrimitiveTopology primitive, int startIndex, int primitiveCount, int instanceCount)
{
    LN_NOTIMPLEMENTED();
}

void DX12GraphicsContext::onDrawExtension(INativeGraphicsExtension* extension)
{
    LN_NOTIMPLEMENTED();
}

//==============================================================================
// DX12SwapChain

DX12SwapChain::DX12SwapChain()
{
}

Result DX12SwapChain::init(DX12Device* deviceContext, PlatformWindow* window, const SizeI& backbufferSize)
{
    LN_DCHECK(deviceContext);

    LN_NOTIMPLEMENTED();

	return true;
}

void DX12SwapChain::dispose()
{
    ISwapChain::dispose();
}

uint32_t DX12SwapChain::getBackbufferCount()
{
    LN_NOTIMPLEMENTED();
    return 0;
}

void DX12SwapChain::acquireNextImage(int* outIndex)
{
    LN_NOTIMPLEMENTED();
}

ITexture* DX12SwapChain::getRenderTarget(int imageIndex) const
{
    LN_NOTIMPLEMENTED();
	return nullptr;
}

Result DX12SwapChain::resizeBackbuffer(uint32_t width, uint32_t height)
{
    LN_NOTIMPLEMENTED();
	return true;
}

void DX12SwapChain::present()
{
    LN_NOTIMPLEMENTED();
}

//==============================================================================
// DX12RenderPass

DX12RenderPass::DX12RenderPass()
	: m_device(nullptr)
	, m_clearFlags(ClearFlags::None)
	, m_clearColor()
	, m_clearDepth(1.0f)
	, m_clearStencil(0x00)
{
}

bool DX12RenderPass::init(DX12Device* device, const DeviceFramebufferState& buffers, ClearFlags clearFlags, const Color& clearColor, float clearDepth, uint8_t clearStencil)
{
	LN_CHECK(device);
	m_device = device;
	m_clearFlags = clearFlags;
	m_clearColor = clearColor;
	m_clearDepth = clearDepth;
	m_clearStencil = clearStencil;

    LN_NOTIMPLEMENTED();
	return true;
}

void DX12RenderPass::dispose()
{
    LN_NOTIMPLEMENTED();
	m_device = nullptr;
	IRenderPass::dispose();
}

//==============================================================================
// DX12Pipeline

DX12Pipeline::DX12Pipeline()
	: m_device(nullptr)
{
}

bool DX12Pipeline::init(DX12Device* deviceContext, const DevicePipelineStateDesc& state)
{
	LN_DCHECK(deviceContext);
	LN_DCHECK(state.renderPass);
	m_device = deviceContext;
    LN_NOTIMPLEMENTED();
	return true;
}

void DX12Pipeline::dispose()
{
    LN_NOTIMPLEMENTED();
	IPipeline::dispose();
}

//==============================================================================
// DX12VertexDeclaration

DX12VertexDeclaration::DX12VertexDeclaration()
{
}

// https://gist.github.com/SaschaWillems/428d15ed4b5d71ead462bc63adffa93a
Result DX12VertexDeclaration::init(const VertexElement* elements, int elementsCount)
{
    LN_DCHECK(elements);
    LN_NOTIMPLEMENTED();

    return true;
}

void DX12VertexDeclaration::dispose()
{
    IVertexDeclaration::dispose();
}

//==============================================================================
// DX12VertexBuffer

DX12VertexBuffer::DX12VertexBuffer()
    : m_usage(GraphicsResourceUsage::Static)
{
}

Result DX12VertexBuffer::init(DX12Device* deviceContext, GraphicsResourceUsage usage, size_t bufferSize, const void* initialData)
{
    LN_DCHECK(deviceContext);
    m_deviceContext = deviceContext;

    LN_NOTIMPLEMENTED();

    m_usage = usage;

    return true;
}

void DX12VertexBuffer::dispose()
{
    LN_NOTIMPLEMENTED();
    IVertexBuffer::dispose();
}

size_t DX12VertexBuffer::getBytesSize()
{
    LN_NOTIMPLEMENTED();
    return 0;
}

GraphicsResourceUsage DX12VertexBuffer::usage() const
{
    return m_usage;
}

void* DX12VertexBuffer::map()
{
    LN_NOTIMPLEMENTED();
    return 0;
}

void DX12VertexBuffer::unmap()
{
    LN_NOTIMPLEMENTED();
}

//==============================================================================
// DX12IndexBuffer

DX12IndexBuffer::DX12IndexBuffer()
    : m_usage(GraphicsResourceUsage::Static)
{
}

Result DX12IndexBuffer::init(DX12Device* deviceContext, GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData)
{
    LN_DCHECK(deviceContext);
    m_deviceContext = deviceContext;
    LN_NOTIMPLEMENTED();

    m_usage = usage;

    return true;
}

void DX12IndexBuffer::dispose()
{
    LN_NOTIMPLEMENTED();
    IIndexBuffer::dispose();
}

size_t DX12IndexBuffer::getBytesSize()
{
    LN_NOTIMPLEMENTED();
    return 0;
}

GraphicsResourceUsage DX12IndexBuffer::usage() const
{
    return m_usage;
}

void* DX12IndexBuffer::map()
{
    LN_NOTIMPLEMENTED();
    return 0;
}

void DX12IndexBuffer::unmap()
{
    LN_NOTIMPLEMENTED();
}

//==============================================================================
// DX12Texture2D

DX12Texture2D::DX12Texture2D()
	: m_mipLevels(1)
{
}

Result DX12Texture2D::init(DX12Device* deviceContext, GraphicsResourceUsage usage, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData)
{
	LN_DCHECK(deviceContext);
	m_deviceContext = deviceContext;
	m_usage = usage;
    m_size.width = width;
    m_size.height = height;
    m_format = requestFormat;

    LN_NOTIMPLEMENTED();

	return true;
}

void DX12Texture2D::dispose()
{
    LN_NOTIMPLEMENTED();
    DX12Texture::dispose();
}

void DX12Texture2D::setSubData(DX12GraphicsContext* graphicsContext, int x, int y, int width, int height, const void* data, size_t dataSize)
{
    LN_NOTIMPLEMENTED();
}

//==============================================================================
// DX12SwapchainRenderTargetTexture

DX12RenderTarget::DX12RenderTarget()
    : m_deviceContext(nullptr)
{
}

Result DX12RenderTarget::init(DX12Device* deviceContext, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap)
{
    LN_DCHECK(deviceContext);
    m_deviceContext = deviceContext;
    m_size.width = width;
    m_size.height = height;
    m_format = requestFormat;
    LN_NOTIMPLEMENTED();

    return true;
}

void DX12RenderTarget::dispose()
{
    LN_NOTIMPLEMENTED();
    DX12Texture::dispose();
}

void DX12RenderTarget::readData(void* outData)
{
    LN_NOTIMPLEMENTED();
}

//==============================================================================
// DX12DepthBuffer

DX12DepthBuffer::DX12DepthBuffer()
{
}

Result DX12DepthBuffer::init(DX12Device* deviceContext, uint32_t width, uint32_t height)
{
    LN_DCHECK(deviceContext);
    if (LN_REQUIRE(width > 0)) return false;
    if (LN_REQUIRE(height > 0)) return false;
    m_deviceContext = deviceContext;
    m_size.width = width;
    m_size.height = height;
    LN_NOTIMPLEMENTED();

    return true;
}

void DX12DepthBuffer::dispose()
{
    LN_NOTIMPLEMENTED();
    IDepthBuffer::dispose();
}

//==============================================================================
// DX12SamplerState

DX12SamplerState::DX12SamplerState()
	: m_deviceContext(nullptr)
{
}

Result DX12SamplerState::init(DX12Device* deviceContext, const SamplerStateData& desc)
{
	LN_DCHECK(deviceContext);
	m_deviceContext = deviceContext;
    LN_NOTIMPLEMENTED();

	return true;
}

void DX12SamplerState::dispose()
{
    LN_NOTIMPLEMENTED();
	ISamplerState::dispose();
}

//==============================================================================
// DX12ShaderPass

DX12ShaderPass::DX12ShaderPass()
{
}

Result DX12ShaderPass::init(DX12Device* deviceContext, const ShaderPassCreateInfo& createInfo, ShaderCompilationDiag* diag)
{
    LN_DCHECK(deviceContext);
	if (!IShaderPass::init(createInfo)) {
		return false;
	}

    m_deviceContext = deviceContext;

    LN_NOTIMPLEMENTED();

    m_descriptorTable = makeRef<DX12ShaderDescriptorTable>();
    if (!m_descriptorTable->init(m_deviceContext, this, createInfo.descriptorLayout)) {
        return false;
    }

    return true;
}

void DX12ShaderPass::dispose()
{
    LN_NOTIMPLEMENTED();
    IShaderPass::dispose();
}

IShaderDescriptorTable* DX12ShaderPass::descriptorTable() const
{
    return m_descriptorTable;
}


//=============================================================================
// DX12ShaderDescriptorTable

DX12ShaderDescriptorTable::DX12ShaderDescriptorTable()
{
}

bool DX12ShaderDescriptorTable::init(DX12Device* deviceContext, const DX12ShaderPass* ownerPass, const DescriptorLayout* descriptorLayout)
{
    LN_NOTIMPLEMENTED();


    return true;
}

void DX12ShaderDescriptorTable::dispose()
{
    LN_NOTIMPLEMENTED();
    IShaderDescriptorTable::dispose();
}

} // namespace detail
} // namespace ln
