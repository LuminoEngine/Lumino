
#include "Internal.hpp"
#include <LuminoShaderCompiler/detail/D3DCompilerAPI.hpp>
#include "DX12Helper.hpp"
#include "DX12Textures.hpp"
#include "DX12ShaderPass.hpp"
#include "DX12DescriptorPool.hpp"
#include "DX12RenderPass.hpp"
#include "DX12CommandList.hpp"
#include "DX12SwapChain.hpp"
#include "DX12DeviceContext.hpp"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

namespace ln {
namespace detail {

//==============================================================================
// DX12Device

DX12Device::DX12Device()
    : m_fenceValue(1) {
}

bool DX12Device::init(const Settings& settings, bool* outIsDriverSupported) {
    if (LN_REQUIRE(outIsDriverSupported)) return false;
    *outIsDriverSupported = true;

    if (!D3DCompilerAPI::Initialize()) {
        return false;
    }

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
            adapters.begin(), adapters.end(), [](const Adapter& a, const Adapter& b) { return a.desc.DedicatedVideoMemory > b.desc.DedicatedVideoMemory; });

        if (!settings.priorityAdapterName.empty()) {
            auto itr = std::find_if(adapters.begin(), adapters.end(), [&](const Adapter& a) { return settings.priorityAdapterName == a.desc.Description; });
            if (itr != adapters.end()) {
                Adapter t = *itr;
                adapters.erase(itr);
                adapters.insert(adapters.begin(), t);
            }
        }

        //if ()

        //if (!settings.useBasicRenderDriver) {
        //}
        //else {
        //    std::sort(
        //        adapters.begin(), adapters.end(),
        //        [](const Adapter& a, const Adapter& b) { return a.desc.DedicatedVideoMemory < b.desc.DedicatedVideoMemory; });
        //}

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

        LN_LOG_INFO("Adapter: ");
        LN_LOG_INFO(U"  Description: {}", String::fromCString(selected->desc.Description));
        LN_LOG_INFO("  DedicatedVideoMemory: {}", selected->desc.DedicatedVideoMemory);
        LN_LOG_INFO("  DedicatedSystemMemory: {}", selected->desc.DedicatedSystemMemory);
        LN_LOG_INFO("  SharedSystemMemory: {}", selected->desc.SharedSystemMemory);

        //{
        //    D3D12_FEATURE_DATA_D3D12_OPTIONS data;
        //    m_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS, &data, sizeof(data));
        //    printf("");
        //}
        //{
        //    D3D12_FEATURE_DATA_SHADER_MODEL data2;
        //    data2.HighestShaderModel = D3D_SHADER_MODEL_5_1;
        //    HRESULT hr = m_device->CheckFeatureSupport(D3D12_FEATURE_SHADER_MODEL, &data2, sizeof(data2));
        //    printf("");
        //}
    }

    // Check MSAA Sample count
    // FIXME: DX12 ではフォーマットフォトに値が変わるような API になっている。問題出るようならフォーマットごとに集計した方がいいかも。
    {
        const DXGI_FORMAT BufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
        UINT TargetSampleCount = 8;
        for (m_sampleCount = TargetSampleCount; m_sampleCount > 1; m_sampleCount--) {
            D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS levels = { BufferFormat, m_sampleCount };
            if (FAILED(m_device->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &levels, sizeof(levels)))) {
                continue;
            }

            if (levels.NumQualityLevels > 0) {
                break;
            }
        }

        if (m_sampleCount < 2) {
            LN_LOG_INFO("MSAA not supported.");
        }
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

    m_uploadBufferAllocatorManager = makeRef<DX12SingleFrameAllocatorPageManager>(
        this, DX12SingleFrameAllocatorPageManager::DefaultPageSize, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_GENERIC_READ);

    {

        // CreateRootSignature
        {
            // 線形補間 SamplerState
            D3D12_STATIC_SAMPLER_DESC samplerDesc = {};
            samplerDesc.Filter = D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT;
            samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
            samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
            samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
            samplerDesc.MipLODBias = 0.0f;
            samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
            samplerDesc.MinLOD = 0.0f;
            samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
            samplerDesc.MaxAnisotropy = 0;
            samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
            samplerDesc.ShaderRegister = 0;
            samplerDesc.RegisterSpace = 0;
            samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

            D3D12_DESCRIPTOR_RANGE srvCbvRanges[2];
            D3D12_ROOT_PARAMETER rootParameters[3];
            srvCbvRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
            srvCbvRanges[0].NumDescriptors = 1;
            srvCbvRanges[0].BaseShaderRegister = 0;
            srvCbvRanges[0].RegisterSpace = 0;
            srvCbvRanges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
            srvCbvRanges[1].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
            srvCbvRanges[1].NumDescriptors = 1;
            srvCbvRanges[1].BaseShaderRegister = 0;
            srvCbvRanges[1].RegisterSpace = 0;
            srvCbvRanges[1].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
            rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
            rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
            rootParameters[0].Constants.ShaderRegister = 0;
            rootParameters[0].Constants.RegisterSpace = 0;
            rootParameters[0].Constants.Num32BitValues = 3; // float2 + float
            rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
            rootParameters[1].DescriptorTable.NumDescriptorRanges = 1;
            rootParameters[1].DescriptorTable.pDescriptorRanges = &srvCbvRanges[0];
            rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
            rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
            rootParameters[2].DescriptorTable.NumDescriptorRanges = 1;
            rootParameters[2].DescriptorTable.pDescriptorRanges = &srvCbvRanges[1];

            ComPtr<ID3DBlob> signature;
            ComPtr<ID3DBlob> error;
            D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc;
            rootSignatureDesc.NumParameters = _countof(rootParameters);
            rootSignatureDesc.pParameters = rootParameters;
            rootSignatureDesc.NumStaticSamplers = 1;
            rootSignatureDesc.pStaticSamplers = &samplerDesc;
            rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

            HRESULT hr = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
            if (FAILED(hr)) {
                LN_ERROR("D3D12SerializeRootSignature failed.");
                return false;
            }

            hr = m_device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_mipmapRootSignature));
            if (FAILED(hr)) {
                LN_ERROR("CreateRootSignature failed.");
                return false;
            }
        }

        // CreateComputePipelineState
#include "../../Resource/GenerateMipMaps.hlsl.inl"
        D3D12_COMPUTE_PIPELINE_STATE_DESC psoDesc = {};
        psoDesc.pRootSignature = m_mipmapRootSignature.Get();
        psoDesc.CS = { g_GenerateMipMaps, LN_ARRAY_SIZE_OF(g_GenerateMipMaps) };

        HRESULT hr = m_device->CreateComputePipelineState(&psoDesc, IID_PPV_ARGS(&m_mipmapPipelineState));
        if (FAILED(hr)) {
            LN_ERROR("CreateComputePipelineState failed.");
            return false;
        }

        //#ifdef _DEBUG
        //        UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
        //#else
        //        UINT compileFlags = 0;
        //#endif
        //        ID3DBlob* errorBlob = nullptr;
        //        HRESULT hr = D3DCompilerAPI::D3DCompileFromFile(L"C:/Proj/LN/Lumino/src/LuminoEngine/src/Graphics/Resource/GenerateMipMaps.hlsl",
        //            nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
        //            "GenerateMipMaps", "cs_5_1",
        //            compileFlags,
        //            0, &m_generateMipMapsShader, &errorBlob);
        //        if (FAILED(hr)) {
        //            const char* err = (const char*)errorBlob->GetBufferPointer();
        //            LN_ERROR("D3DCompileFromFile failed.");
        //            return false;
        //        }
    }

    return true;
}

void DX12Device::dispose() {
    IGraphicsDevice::dispose();

    m_pipelineCache.reset();
    m_mipmapRootSignature.Reset();

    if (m_uploadBufferAllocatorManager) {
        m_uploadBufferAllocatorManager->clear();
        m_uploadBufferAllocatorManager = nullptr;
    }

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
}

ID3D12GraphicsCommandList* DX12Device::beginSingleTimeCommandList() {
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

bool DX12Device::endSingleTimeCommandList(ID3D12GraphicsCommandList* commandList) {
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

INativeGraphicsInterface* DX12Device::getNativeInterface() const {
    return nullptr;
}

void DX12Device::onGetCaps(GraphicsDeviceCaps* outCaps) {
    outCaps->requestedShaderTriple.target = "hlsl";
    outCaps->requestedShaderTriple.version = 5;
    outCaps->requestedShaderTriple.option = "";
    outCaps->uniformBufferOffsetAlignment = 256;
}

Ref<ISwapChain> DX12Device::onCreateSwapChain(PlatformWindow* window, const SizeI& backbufferSize) {
    auto ptr = makeRef<DX12SwapChain>();
    if (!ptr->init(this, window, backbufferSize)) {
        return nullptr;
    }
    return ptr;
}

Ref<ICommandList> DX12Device::onCreateCommandList() {
    auto ptr = makeRef<DX12GraphicsContext>();
    if (!ptr->init(this)) {
        return nullptr;
    }
    return ptr;
}

Ref<IRenderPass> DX12Device::onCreateRenderPass(const DeviceFramebufferState& buffers, ClearFlags clearFlags, const Color& clearColor, float clearDepth, uint8_t clearStencil) {
    auto ptr = makeRef<DX12RenderPass>();
    if (!ptr->init(this, buffers, clearFlags, clearColor, clearDepth, clearStencil)) {
        return nullptr;
    }
    return ptr;
}

Ref<IPipeline> DX12Device::onCreatePipeline(const DevicePipelineStateDesc& state) {
    auto ptr = makeRef<DX12Pipeline>();
    if (!ptr->init(this, state)) {
        return nullptr;
    }
    return ptr;
}

Ref<IVertexDeclaration> DX12Device::onCreateVertexDeclaration(const VertexElement* elements, int elementsCount) {
    auto ptr = makeRef<DX12VertexDeclaration>();
    if (!ptr->init(elements, elementsCount)) {
        return nullptr;
    }
    return ptr;
}

Ref<RHIResource> DX12Device::onCreateVertexBuffer(GraphicsResourceUsage usage, size_t bufferSize, const void* initialData) {
    auto ptr = makeRef<DX12VertexBuffer>();
    if (!ptr->init(this, usage, bufferSize, initialData)) {
        return nullptr;
    }
    return ptr;
}

Ref<RHIResource> DX12Device::onCreateIndexBuffer(GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData) {
    auto ptr = makeRef<DX12IndexBuffer>();
    if (!ptr->init(this, usage, format, indexCount, initialData)) {
        return nullptr;
    }
    return ptr;
}

Ref<RHIResource> DX12Device::onCreateTexture2D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, const void* initialData) {
    auto ptr = makeRef<DX12Texture2D>();
    if (!ptr->init(this, usage, width, height, requestFormat, mipmap, initialData)) {
        return nullptr;
    }
    return ptr;
}

Ref<RHIResource> DX12Device::onCreateTexture3D(GraphicsResourceUsage usage, uint32_t width, uint32_t height, uint32_t depth, TextureFormat requestFormat, bool mipmap, const void* initialData) {
    LN_NOTIMPLEMENTED();
    return nullptr;
}

Ref<RHIResource> DX12Device::onCreateRenderTarget(uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, bool msaa) {
    auto ptr = makeRef<DX12RenderTarget>();
    if (!ptr->init(this, width, height, requestFormat, mipmap, msaa)) {
        return nullptr;
    }
    return ptr;
}

Ref<RHIResource> DX12Device::onCreateDepthBuffer(uint32_t width, uint32_t height) {
    auto ptr = makeRef<DX12DepthBuffer>();
    if (!ptr->init(this, width, height)) {
        return nullptr;
    }
    return ptr;
}

Ref<ISamplerState> DX12Device::onCreateSamplerState(const SamplerStateData& desc) {
    auto ptr = makeRef<DX12SamplerState>();
    if (!ptr->init(this, desc)) {
        return nullptr;
    }
    return ptr;
}

Ref<IShaderPass> DX12Device::onCreateShaderPass(const ShaderPassCreateInfo& createInfo, ShaderCompilationDiag* diag) {
    auto ptr = makeRef<DX12ShaderPass>();
    if (!ptr->init(this, createInfo, diag)) {
        return nullptr;
    }
    return ptr;
}

Ref<RHIResource> DX12Device::onCreateUniformBuffer(uint32_t size) {
    auto ptr = makeRef<DX12UniformBuffer>();
    if (!ptr->init(this, size)) {
        return nullptr;
    }
    return ptr;
}

Ref<IDescriptorPool> DX12Device::onCreateDescriptorPool(IShaderPass* shaderPass) {
    auto ptr = makeRef<DX12DescriptorPool>();
    if (!ptr->init(this, static_cast<DX12ShaderPass*>(shaderPass))) {
        return nullptr;
    }
    return ptr;
}

void DX12Device::onSubmitCommandBuffer(ICommandList* context, RHIResource* affectRendreTarget) {
    DX12GraphicsContext* commandList = static_cast<DX12GraphicsContext*>(context);
    commandList->submit(m_fenceValue);
    m_fenceValue++;
}

ICommandQueue* DX12Device::getGraphicsCommandQueue() {
    LN_NOTIMPLEMENTED();
    return nullptr;
}

ICommandQueue* DX12Device::getComputeCommandQueue() {
    LN_NOTIMPLEMENTED();
    return nullptr;
}

void DX12Device::enableDebugLayer() const {
    ID3D12Debug* debugLayer = nullptr;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugLayer)))) {
        debugLayer->EnableDebugLayer();
        debugLayer->Release();
    }
}

//==============================================================================
// DX12Pipeline

DX12Pipeline::DX12Pipeline()
    : m_device(nullptr) {
}

bool DX12Pipeline::init(DX12Device* deviceContext, const DevicePipelineStateDesc& state) {
    LN_DCHECK(deviceContext);
    LN_DCHECK(state.renderPass);
    m_device = deviceContext;

    DX12ShaderPass* shaderPass = static_cast<DX12ShaderPass*>(state.shaderPass);

    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc;
    psoDesc.pRootSignature = shaderPass->rootSignature();

    psoDesc.VS = shaderPass->dxVSByteCode();
    psoDesc.PS = shaderPass->dxPSByteCode();
    psoDesc.DS = { nullptr, 0 };
    psoDesc.HS = { nullptr, 0 };
    psoDesc.GS = { nullptr, 0 };

    psoDesc.StreamOutput.pSODeclaration = nullptr;
    psoDesc.StreamOutput.NumEntries = 0;
    psoDesc.StreamOutput.pBufferStrides = nullptr;
    psoDesc.StreamOutput.NumStrides = 0;
    psoDesc.StreamOutput.RasterizedStream = 0;

    // BlendState
    {
        ZeroMemory(&psoDesc.BlendState, sizeof(psoDesc.BlendState));
        psoDesc.BlendState.AlphaToCoverageEnable = FALSE;
        psoDesc.BlendState.IndependentBlendEnable = state.blendState.independentBlendEnable;
        for (int i = 0; i < BlendStateDesc::MaxRenderTargets; i++) {
            D3D12_RENDER_TARGET_BLEND_DESC& dst = psoDesc.BlendState.RenderTarget[i];
            const RenderTargetBlendDesc& src = state.blendState.renderTargets[i];
            dst.BlendEnable = src.blendEnable;
            dst.LogicOpEnable = FALSE;
            dst.SrcBlend = DX12Helper::LNBlendFactorToDX12Blend(src.sourceBlend);
            dst.DestBlend = DX12Helper::LNBlendFactorToDX12Blend(src.destinationBlend);
            dst.BlendOp = DX12Helper::LNBlendOpToDX12Blend(src.blendOp);
            dst.SrcBlendAlpha = DX12Helper::LNBlendFactorToDX12Blend(src.sourceBlendAlpha);
            dst.DestBlendAlpha = DX12Helper::LNBlendFactorToDX12Blend(src.destinationBlendAlpha);
            dst.BlendOpAlpha = DX12Helper::LNBlendOpToDX12Blend(src.blendOpAlpha);
            dst.LogicOp = D3D12_LOGIC_OP_NOOP;
            ;
            dst.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

            if (!psoDesc.BlendState.IndependentBlendEnable) {
                // IndependentBlendEnable=FALSE なら、[0] だけ設定すればよい
                // https://docs.microsoft.com/en-us/windows/win32/api/d3d12/ns-d3d12-d3d12_blend_desc
                break;
            }
        }
    }

    psoDesc.SampleMask = UINT_MAX;

    // RasterizerState
    {
        psoDesc.RasterizerState.FillMode = DX12Helper::LNFillModeToDX12FillMode(state.rasterizerState.fillMode);
        psoDesc.RasterizerState.CullMode = DX12Helper::LNCullModeToDX12CullMode(state.rasterizerState.cullMode);
#ifdef LN_COORD_RH
        psoDesc.RasterizerState.FrontCounterClockwise = TRUE;
#else
        psoDesc.RasterizerState.FrontCounterClockwise = FALSE;
#endif
        psoDesc.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
        psoDesc.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
        psoDesc.RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
        psoDesc.RasterizerState.DepthClipEnable = TRUE;
        psoDesc.RasterizerState.MultisampleEnable = FALSE;
        psoDesc.RasterizerState.AntialiasedLineEnable = FALSE;
        psoDesc.RasterizerState.ForcedSampleCount = 0;
        psoDesc.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
    }

    // DepthStencilState
    {
        if (state.renderPass->hasDepthBuffer()) {
            psoDesc.DepthStencilState.DepthEnable = TRUE;
            psoDesc.DepthStencilState.DepthWriteMask = state.depthStencilState.depthWriteEnabled ? D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK_ZERO;
            psoDesc.DepthStencilState.DepthFunc = DX12Helper::LNComparisonFuncToDX12ComparisonFunc(state.depthStencilState.depthTestFunc);
            psoDesc.DepthStencilState.StencilEnable = state.depthStencilState.stencilEnabled;
            psoDesc.DepthStencilState.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
            psoDesc.DepthStencilState.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;
            psoDesc.DepthStencilState.FrontFace.StencilFailOp = DX12Helper::LNStencilOpToDX12StencilOp(state.depthStencilState.frontFace.stencilFailOp);
            psoDesc.DepthStencilState.FrontFace.StencilDepthFailOp = DX12Helper::LNStencilOpToDX12StencilOp(state.depthStencilState.frontFace.stencilDepthFailOp);
            psoDesc.DepthStencilState.FrontFace.StencilPassOp = DX12Helper::LNStencilOpToDX12StencilOp(state.depthStencilState.frontFace.stencilPassOp);
            psoDesc.DepthStencilState.FrontFace.StencilFunc = DX12Helper::LNComparisonFuncToDX12ComparisonFunc(state.depthStencilState.frontFace.stencilFunc);
            psoDesc.DepthStencilState.BackFace.StencilFailOp = DX12Helper::LNStencilOpToDX12StencilOp(state.depthStencilState.backFace.stencilFailOp);
            psoDesc.DepthStencilState.BackFace.StencilDepthFailOp = DX12Helper::LNStencilOpToDX12StencilOp(state.depthStencilState.backFace.stencilDepthFailOp);
            psoDesc.DepthStencilState.BackFace.StencilPassOp = DX12Helper::LNStencilOpToDX12StencilOp(state.depthStencilState.backFace.stencilPassOp);
            psoDesc.DepthStencilState.BackFace.StencilFunc = DX12Helper::LNComparisonFuncToDX12ComparisonFunc(state.depthStencilState.backFace.stencilFunc);
        }
        else {
            ZeroMemory(&psoDesc.DepthStencilState, sizeof(psoDesc.DepthStencilState));
        }
    }

    // InputLayout
    {
        DX12VertexDeclaration* layout = static_cast<DX12VertexDeclaration*>(state.vertexDeclaration);
        psoDesc.InputLayout.pInputElementDescs = layout->elements().data();
        psoDesc.InputLayout.NumElements = static_cast<UINT>(layout->elements().size());
    }

    psoDesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;

    switch (state.topology) {
        case PrimitiveTopology::TriangleList:
        case PrimitiveTopology::TriangleStrip:
            psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
            break;
        case PrimitiveTopology::LineList:
        case PrimitiveTopology::LineStrip:
            psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
            break;
        case PrimitiveTopology::PointList:
            psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
            break;
        default:
            LN_UNREACHABLE();
            return false;
    }

    // RenderPass
    {
        DX12RenderPass* renderPass = static_cast<DX12RenderPass*>(state.renderPass);

        psoDesc.NumRenderTargets = renderPass->getAvailableRenderTargetCount();
        UINT i = 0;
        for (; i < psoDesc.NumRenderTargets; i++) {
            psoDesc.RTVFormats[i] = renderPass->renderTarget(i)->dxFormat();
        }
        for (; i < LN_ARRAY_SIZE_OF(psoDesc.RTVFormats); i++) {
            psoDesc.RTVFormats[i] = DXGI_FORMAT_UNKNOWN;
        }

        if (renderPass->hasDepthBuffer()) {
            psoDesc.DSVFormat = renderPass->depthBuffer()->dxFormat();
        }
        else {
            psoDesc.DSVFormat = DXGI_FORMAT_UNKNOWN;
        }

        if (renderPass->isMultisample()) {
            psoDesc.SampleDesc.Count = m_device->msaaSampleCount();
            psoDesc.SampleDesc.Quality = m_device->msaaQualityLevel();
        }
        else {
            psoDesc.SampleDesc.Count = 1;
            psoDesc.SampleDesc.Quality = 0;
        }
    }

    psoDesc.NodeMask = 0;
    psoDesc.CachedPSO.pCachedBlob = nullptr;
    psoDesc.CachedPSO.CachedBlobSizeInBytes = 0;
    psoDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

    ID3D12Device* dxDevice = m_device->device();
    auto hr = dxDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState));
    if (FAILED(hr)) {
        LN_ERROR("CreateGraphicsPipelineState failed.");
        return false;
    }

    return true;
}

void DX12Pipeline::dispose() {
    m_pipelineState.Reset();
    IPipeline::dispose();
}

//==============================================================================
// DX12VertexDeclaration

DX12VertexDeclaration::DX12VertexDeclaration()
    : m_elements() {
}

// https://gist.github.com/SaschaWillems/428d15ed4b5d71ead462bc63adffa93a
Result DX12VertexDeclaration::init(const VertexElement* elements, int elementsCount) {
    LN_DCHECK(elements);

    //std::array<UINT, 16> offsets;

    std::fill(m_strides.begin(), m_strides.end(), 0);

    UINT offset = 0;
    for (int i = 0; i < elementsCount; i++) {
        const VertexElement& e = elements[i];
        D3D12_INPUT_ELEMENT_DESC desc = {};
        desc.SemanticName = DX12Helper::LNVertexElementUsageToSemanticName(e.Usage);
        desc.SemanticIndex = e.UsageIndex;
        desc.Format = DX12Helper::LNVertexElementTypeToDXFormat(e.Type);
        desc.InputSlot = e.StreamIndex;
        desc.AlignedByteOffset = m_strides[e.StreamIndex];
        if (e.rate == VertexInputRate::Vertex) {
            desc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA;
            desc.InstanceDataStepRate = 0;
        }
        else {
            desc.InputSlotClass = D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA;
            desc.InstanceDataStepRate = 1;
        }
        m_elements.push_back(desc);

        m_strides[e.StreamIndex] += static_cast<UINT>(GraphicsHelper::getVertexElementTypeSize(elements[i].Type));
    }

    return ok();
}

void DX12VertexDeclaration::dispose() {
    IVertexDeclaration::dispose();
}

//==============================================================================
// DX12SamplerState

DX12SamplerState::DX12SamplerState()
    : m_samplerDesc() {
}

Result DX12SamplerState::init(DX12Device* deviceContext, const SamplerStateData& desc) {
    LN_DCHECK(deviceContext);

    if (desc.filter == TextureFilterMode::Point) {
        m_samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
    }
    else {
        m_samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
    }

    if (desc.address == TextureAddressMode::Repeat) {
        m_samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
        m_samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
        m_samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    }
    else {
        m_samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
        m_samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
        m_samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
    }
    m_samplerDesc.MipLODBias = 0;
    m_samplerDesc.MaxAnisotropy = desc.anisotropy ? 16 : 0;
    m_samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    m_samplerDesc.BorderColor[0] = 0.0f;
    m_samplerDesc.BorderColor[1] = 0.0f;
    m_samplerDesc.BorderColor[2] = 0.0f;
    m_samplerDesc.BorderColor[3] = 0.0f;
    m_samplerDesc.MinLOD = 0.0f;
    m_samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;

    return ok();
}

void DX12SamplerState::dispose() {
    ISamplerState::dispose();
}

} // namespace detail
} // namespace ln
