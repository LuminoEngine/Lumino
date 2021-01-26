
#include "Internal.hpp"
#include <LuminoEngine/Platform/PlatformWindow.hpp>
#include <LuminoEngine/Platform/PlatformSupport.hpp>
#include "DX12Helper.hpp"
#include "DX12VertexBuffer.hpp"
#include "DX12Texture.hpp"
#include "DX12ShaderPass.hpp"
#include "DX12DescriptorPool.hpp"
#include "DX12CommandList.hpp"
#include "DX12DeviceContext.hpp"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

namespace ln {
namespace detail {

//==============================================================================
// DX12Device

DX12Device::DX12Device()
    : m_fenceValue(1)
{
}

bool DX12Device::init(const Settings& settings, bool* outIsDriverSupported)
{
	if (LN_REQUIRE(outIsDriverSupported)) return false;
	*outIsDriverSupported = true;

    // Create factory
    {
        if (settings.debugMode) {
            enableDebugLayer();
        }
        if (settings.debugMode) {
            CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&m_dxgiFactory));
        }
        if (!m_dxgiFactory) {
            if (FAILED(CreateDXGIFactory2(0, IID_PPV_ARGS(&m_dxgiFactory)))) {
                LN_ERROR("CreateDXGIFactory2 failed.");
                return false;
            }
        }
    }

    // Select adapter
    // Create device
    {
        struct Adapter {
            ComPtr<IDXGIAdapter> adapter;
            DXGI_ADAPTER_DESC desc;
        };

        std::vector<Adapter> adapters;
        ComPtr<IDXGIAdapter> adapter;
        for (int i = 0; m_dxgiFactory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND; ++i) {
            DXGI_ADAPTER_DESC desc;
            adapter->GetDesc(&desc);
            adapters.push_back({ adapter, desc });
        }
        if (adapters.empty()) {
            LN_ERROR("Adapter not found.");
            return false;
        }

        //一番よさそうなものを採用したいので、 VRAM 量でソート
        // ※ SystemMemory をチェックするのは NG. ソフトウェアドライバを採用してしまうことがある。
        std::sort(
            adapters.begin(), adapters.end(),
            [](const Adapter& a, const Adapter& b) { return a.desc.DedicatedVideoMemory > b.desc.DedicatedVideoMemory; });

        const Adapter* selected = nullptr;
        for (const auto& adapter : adapters) {
            const D3D_FEATURE_LEVEL levels[] = {
                D3D_FEATURE_LEVEL_12_1,
                D3D_FEATURE_LEVEL_12_0,
                D3D_FEATURE_LEVEL_11_1,
                D3D_FEATURE_LEVEL_11_0,
            };
            for (auto level : levels) {
                if (D3D12CreateDevice(adapter.adapter.Get(), level, IID_PPV_ARGS(&m_device)) == S_OK) {
                    selected = &adapter;
                    break;
                }
            }

            if (m_device) {
                m_adapter = adapter.adapter;
                break;
            }
        }

        if (!m_device) {
            LN_ERROR("CreateDevice failed.");
            return false;
        }

        const char* indent = "  ";
        LN_LOG_INFO << "Adapter: ";
        LN_LOG_INFO << indent << "Description: " << String::fromCString(selected->desc.Description);
        LN_LOG_INFO << indent << "DedicatedVideoMemory: " << selected->desc.DedicatedVideoMemory;
        LN_LOG_INFO << indent << "DedicatedSystemMemory: " << selected->desc.DedicatedSystemMemory;
        LN_LOG_INFO << indent << "SharedSystemMemory: " << selected->desc.SharedSystemMemory;
    }

    // Command Queue
    {
        D3D12_COMMAND_QUEUE_DESC desc = {};
        desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
        desc.Priority = 0;
        desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE; // Default command queue
        desc.NodeMask = 0;                          // 使用する GPU は 1 つだけであるため 0
        if (FAILED(m_device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_commandQueue)))) {
            LN_ERROR("CreateCommandQueue failed.");
            return false;
        }
    }

    // Single Time Command List
    {
        if (FAILED(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_singleTimeCommandAllocator)))) {
            LN_ERROR("CreateCommandAllocator failed.");
            return false;
        }

        if (FAILED(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_singleTimeCommandAllocator.Get(), nullptr, IID_PPV_ARGS(&m_singleTimeCommandList)))) {
            LN_ERROR("CreateCommandList failed.");
            return false;
        }
        // CommandList は初期状態が "記録中" になっている。この状態では CommandAllocator::Reset() がエラーを返す。
        // 記録開始時は状態を "記録終了" (非記録状態) にしておく方がコードがシンプルになるので、そうしておく。
        // https://docs.microsoft.com/en-us/windows/win32/api/d3d12/nf-d3d12-id3d12graphicscommandlist-close
        m_singleTimeCommandList->Close();

        if (FAILED(m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_singleTimeCommandListFence)))) {
            LN_ERROR("CreateFence failed.");
            return false;
        }

        m_singleTimeCommandListEvent = ::CreateEvent(0, 0, 0, 0);
        if (!m_singleTimeCommandListEvent) {
            LN_ERROR("CreateEvent failed.");
            return false;
        }
    }

	return true;
}

void DX12Device::dispose()
{
    if (m_singleTimeCommandListEvent) {
        ::CloseHandle(m_singleTimeCommandListEvent);
        m_singleTimeCommandListEvent = nullptr;
    }
    m_singleTimeCommandListFence.Reset();
    m_singleTimeCommandList.Reset();
    m_singleTimeCommandAllocator.Reset();
    m_commandQueue.Reset();
    m_device.Reset();
    m_adapter.Reset();
    m_dxgiFactory.Reset();

    IGraphicsDevice::dispose();
}

ID3D12GraphicsCommandList* DX12Device::beginSingleTimeCommandList()
{
    // https://docs.microsoft.com/ja-jp/windows/win32/direct3d12/recording-command-lists-and-bundles
    // 基本的な使い方 (1フレーム内で複数のコマンドリストをマルチスレッドで構築するようなケースではない) の場合、
    // ID3D12CommandAllocator と ID3D12GraphicsCommandList はワンセットと考えてよい。
    // 逆にひとつの Allocator から複数の CommandList を作ったとき、Allocator だけ Reset() してしまうと、
    // 生きている CommandList が使っているメモリが壊れてしまう。
    if (FAILED(m_singleTimeCommandAllocator->Reset())) {
        LN_ERROR("Reset failed.");
        return false;
    }
    if (FAILED(m_singleTimeCommandList->Reset(m_singleTimeCommandAllocator.Get(), nullptr))) {
        LN_ERROR("Reset failed.");
        return false;
    }

    return m_singleTimeCommandList.Get();
}

bool DX12Device::endSingleTimeCommandList(ID3D12GraphicsCommandList* commandList)
{
    if (LN_REQUIRE(commandList == m_singleTimeCommandList.Get())) return false;

    // 記録終了
    commandList->Close();

    // CommandList の実行開始前に、Fence の値を 0 にしておく
    if (FAILED(m_singleTimeCommandListFence->Signal(0))) {
        LN_ERROR("Signal(0) failed.");
        return false;
    }

    // 実行開始
    ID3D12CommandList* commandLists[1] = { commandList };
    m_commandQueue->ExecuteCommandLists(1, commandLists);

    // 実行完了であれば Fence の値が 1 になるようにしておく
    // ※ExecuteCommandLists() の直後、この Signal の呼び出し時点で完了している場合でも 1 にしてくれる
    if (FAILED(m_commandQueue->Signal(m_singleTimeCommandListFence.Get(), 1))) {
        LN_ERROR("Signal(0) failed.");
        return false;
    }

    // Fence の値が 1 になるまで待つ
    m_singleTimeCommandListFence->SetEventOnCompletion(1, m_singleTimeCommandListEvent);
    WaitForSingleObject(m_singleTimeCommandListEvent, INFINITE);

    return true;
}

INativeGraphicsInterface* DX12Device::getNativeInterface() const
{
	return nullptr;
}

void DX12Device::onGetCaps(GraphicsDeviceCaps * outCaps)
{
    outCaps->requestedShaderTriple.target = "hlsl";
    outCaps->requestedShaderTriple.version = 5;
    outCaps->requestedShaderTriple.option = "";
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

Ref<ITexture> DX12Device::onCreateRenderTarget(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, bool msaa)
{
    if (msaa) {
        LN_NOTIMPLEMENTED();
        return nullptr;
    }

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

Ref<IUniformBuffer> DX12Device::onCreateUniformBuffer(uint32_t size)
{
    auto ptr = makeRef<DX12UniformBuffer>();
    if (!ptr->init(this, size)) {
        return nullptr;
    }
    return ptr;
}

Ref<IDescriptorPool> DX12Device::onCreateDescriptorPool(IShaderPass* shaderPass)
{
    auto ptr = makeRef<DX12DescriptorPool>();
    if (!ptr->init(this, static_cast<DX12ShaderPass*>(shaderPass))) {
        return nullptr;
    }
    return ptr;
}

void DX12Device::onSubmitCommandBuffer(ICommandList* context, ITexture* affectRendreTarget)
{
    DX12GraphicsContext* commandList = static_cast<DX12GraphicsContext*>(context);

    commandList->submit(m_fenceValue);



    LN_NOTIMPLEMENTED();
    m_fenceValue++;
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


void DX12Device::enableDebugLayer() const
{
    ID3D12Debug* debugLayer = nullptr;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugLayer)))) {
        debugLayer->EnableDebugLayer();
        debugLayer->Release();
    }
}

//==============================================================================
// DX12SwapChain

DX12SwapChain::DX12SwapChain()
    : m_device(nullptr)
    , m_backbufferCount(0)
    , m_frameIndex(0)
{
}

Result DX12SwapChain::init(DX12Device* deviceContext, PlatformWindow* window, const SizeI& backbufferSize)
{
    LN_DCHECK(deviceContext);
    m_device = deviceContext;
    m_backbufferCount = DX12Device::BackBufferCount;

    HWND hWnd = (HWND)PlatformSupport::getWin32WindowHandle(window);
    HINSTANCE hInstance = ::GetModuleHandle(NULL);

    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
    swapChainDesc.BufferDesc.Width = backbufferSize.width;
    swapChainDesc.BufferDesc.Height = backbufferSize.height;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferCount = m_backbufferCount;
    swapChainDesc.OutputWindow = hWnd;
    swapChainDesc.Windowed = TRUE;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    IDXGIFactory6* dxgiFactory = m_device->dxgiFactory();
    ComPtr<IDXGISwapChain> swapChain;
    HRESULT hr = dxgiFactory->CreateSwapChain(m_device->dxCommandQueue(), &swapChainDesc, &swapChain);
    if (FAILED(hr)) {
        LN_ERROR("CreateSwapChain failed.");
        return false;
    }

    if (FAILED(swapChain.As(&m_dxgiSwapChain))) {
        LN_ERROR("Cast to IDXGISwapChain3 failed.");
        return false;
    }

    // Create a RTV for each frame.
    for (int i = 0; i < m_backbufferCount; i++)
    {
        ComPtr<ID3D12Resource> dxRenderTarget;
        if (FAILED(m_dxgiSwapChain->GetBuffer(i, IID_PPV_ARGS(&dxRenderTarget)))) {
            LN_ERROR("GetBuffer failed.");
            return false;
        }

        Ref<DX12RenderTarget> wrapper = makeRef<DX12RenderTarget>();
        if (!wrapper->init(m_device, dxRenderTarget)) {
            return false;
        }

        m_renderTargets.push_back(wrapper);
    }

    m_frameIndex = m_dxgiSwapChain->GetCurrentBackBufferIndex();

	return true;
}

void DX12SwapChain::dispose()
{
    for (auto& i : m_renderTargets) {
        i->dispose();
    }
    m_renderTargets.clear();
    m_dxgiSwapChain.Reset();
    ISwapChain::dispose();
}

uint32_t DX12SwapChain::getBackbufferCount()
{
    return m_backbufferCount;
}

void DX12SwapChain::acquireNextImage(int* outIndex)
{
    *outIndex = m_dxgiSwapChain->GetCurrentBackBufferIndex();
}

ITexture* DX12SwapChain::getRenderTarget(int imageIndex) const
{
	return m_renderTargets[imageIndex];
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
    : m_elements()
{
}

// https://gist.github.com/SaschaWillems/428d15ed4b5d71ead462bc63adffa93a
Result DX12VertexDeclaration::init(const VertexElement* elements, int elementsCount)
{
    LN_DCHECK(elements);

    std::array<UINT, 16> offsets;   // max=16 : https://docs.microsoft.com/en-us/windows/win32/api/d3d12/ns-d3d12-d3d12_input_element_desc
    UINT offset = 0;
    for (int i = 0; i < elementsCount; i++) {
        const VertexElement& e = elements[i];
        D3D12_INPUT_ELEMENT_DESC desc = {};
        desc.SemanticName = DX12Helper::LNVertexElementUsageToSemanticName(e.Usage);
        desc.SemanticIndex = e.UsageIndex;
        desc.Format = DX12Helper::LNVertexElementTypeToDXFormat(e.Type);
        desc.InputSlot = e.StreamIndex;
        desc.AlignedByteOffset = offsets[e.StreamIndex];
        if (e.rate == VertexInputRate::Vertex) {
            desc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
            desc.InstanceDataStepRate = 0;
        }
        else {
            desc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA;
            desc.InstanceDataStepRate = 1;
        }
        m_elements.push_back(desc);

        offsets[e.StreamIndex] += static_cast<UINT>(GraphicsHelper::getVertexElementTypeSize(elements[i].Type));
    }

    return true;
}

void DX12VertexDeclaration::dispose()
{
    IVertexDeclaration::dispose();
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
// DX12UniformBuffer

DX12UniformBuffer::DX12UniformBuffer()
    : m_deviceContext(nullptr)
    , m_size(0)
    , m_constantBuffer()
    , m_mappedBuffer(nullptr)
{
}

bool DX12UniformBuffer::init(DX12Device* deviceContext, uint32_t size)
{
    LN_DCHECK(deviceContext);
    m_deviceContext = deviceContext;
    m_size = size;

    ID3D12Device* device = m_deviceContext->device();

    // https://github.com/microsoft/DirectX-Graphics-Samples/blob/master/Samples/UWP/D3D12HelloWorld/src/HelloConstBuffers/d3dx12.h#L411
    D3D12_HEAP_PROPERTIES prop = {};
    prop.Type = D3D12_HEAP_TYPE_UPLOAD; // フレームごとに全体を書き換えるような運用をするため UPLOAD
    prop.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    prop.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    prop.CreationNodeMask = 1;
    prop.VisibleNodeMask = 1;

    D3D12_RESOURCE_DESC desc = {};
    desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    desc.Alignment = 0;
    desc.Width = m_size;
    desc.Height = 1;
    desc.DepthOrArraySize = 1;
    desc.MipLevels = 1;
    desc.Format = DXGI_FORMAT_UNKNOWN;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    desc.Flags = D3D12_RESOURCE_FLAG_NONE;

    if (FAILED(device->CreateCommittedResource(
        &prop,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_constantBuffer)))) {
        LN_ERROR("CreateCommittedResource failed.");
        return false;
    }

    // Map したままで OK
    if (FAILED(m_constantBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_mappedBuffer)))) {
        LN_ERROR("Map failed.");
        return false;
    }

    return true;
}

void DX12UniformBuffer::dispose()
{
    if (m_constantBuffer) {
        m_constantBuffer->Unmap(0, nullptr);
        m_mappedBuffer = nullptr;
        m_constantBuffer.Reset();
    }
    IUniformBuffer::dispose();
}

void* DX12UniformBuffer::map()
{
    return m_mappedBuffer;
}

void DX12UniformBuffer::unmap()
{
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

} // namespace detail
} // namespace ln
