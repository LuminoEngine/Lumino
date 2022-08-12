#include "Internal.hpp"
#include "DX12Buffers.hpp"
#include "DX12Textures.hpp"

namespace ln {
namespace detail {

//==============================================================================
// DX12Image

DX12Image::DX12Image()
    : m_dxResource()
    , m_size{ 0, 0 }
    , m_dxFormat(DXGI_FORMAT_UNKNOWN)
    , m_currentState(D3D12_RESOURCE_STATE_COMMON)
    , m_uploadBufferSize(0)
    , m_footprint() {
}

bool DX12Image::init(DX12Device* device, uint32_t width, uint32_t height, uint32_t mipLevels, bool msaa, DXGI_FORMAT format, D3D12_RESOURCE_FLAGS flags, D3D12_RESOURCE_STATES initialState) {
    m_size.width = width;
    m_size.height = height;
    m_dxFormat = format;
    m_mipLevels = mipLevels;
    m_currentState = initialState;

    D3D12_HEAP_PROPERTIES props;
    props.Type = D3D12_HEAP_TYPE_DEFAULT;
    props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    props.CreationNodeMask = device->creationNodeMask();
    props.VisibleNodeMask = device->visibleNodeMask();

    D3D12_RESOURCE_DESC desc;
    desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    desc.Alignment = 0;
    desc.Width = width;
    desc.Height = height;
    desc.DepthOrArraySize = 1;
    desc.MipLevels = mipLevels;
    desc.Format = m_dxFormat;
    desc.SampleDesc.Count = (msaa) ? device->msaaSampleCount() : 1;
    desc.SampleDesc.Quality = (msaa) ? device->msaaQualityLevel() : 0;
    desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    desc.Flags = flags;

    bool useClearValue = false;
    D3D12_CLEAR_VALUE clearValue;
    if ((flags & D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET) != 0) {
        clearValue.Format = m_dxFormat;
        clearValue.Color[0] = 1.0f;
        clearValue.Color[1] = 1.0f;
        clearValue.Color[2] = 1.0f;
        clearValue.Color[3] = 1.0f;
        useClearValue = true;
    }
    else if ((flags & D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL) != 0) {
        clearValue.Format = format;
        clearValue.DepthStencil.Depth = 1.0f;
        clearValue.DepthStencil.Stencil = 0;
        useClearValue = true;
    }

    ID3D12Device* dxDevice = device->device();
    HRESULT hr = dxDevice->CreateCommittedResource(
        &props,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        m_currentState,
        (useClearValue) ? &clearValue : nullptr,
        IID_PPV_ARGS(&m_dxResource));
    if (FAILED(hr)) {
        LN_ERROR("CreateCommittedResource failed.");
        return false;
    }

    dxDevice->GetCopyableFootprints(&desc, 0, 1, 0, &m_footprint, nullptr, nullptr, &m_uploadBufferSize);

    return true;
}

bool DX12Image::init(DX12Device* device, const ComPtr<ID3D12Resource>& dxRenderTarget, D3D12_RESOURCE_STATES state) {
    m_dxResource = dxRenderTarget;

    D3D12_RESOURCE_DESC desc = m_dxResource->GetDesc();
    m_size.width = static_cast<int32_t>(desc.Width);
    m_size.height = static_cast<int32_t>(desc.Height);
    m_dxFormat = desc.Format;
    //m_format = DX12Helper::DXFormatToLNTextureFormat(desc.Format);
    //m_dxFormat = desc.Format;
    m_currentState = state;
    //if (LN_REQUIRE(m_format != TextureFormat::Unknown)) return false;

    return true;
}

void DX12Image::dispose() {
    m_dxResource.Reset();
}

void DX12Image::resourceBarrior(ID3D12GraphicsCommandList* commandList, D3D12_RESOURCE_STATES newState) {
    if (m_currentState == newState) return;

    D3D12_RESOURCE_BARRIER barrier;
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = dxResource();
    barrier.Transition.StateBefore = m_currentState;
    barrier.Transition.StateAfter = newState;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    commandList->ResourceBarrier(1, &barrier);

    m_currentState = newState;
}

//==============================================================================
// DX12Texture

DX12Texture::DX12Texture() {
}

//==============================================================================
// DX12Texture2D

DX12Texture2D::DX12Texture2D() {
}

Result DX12Texture2D::init(DX12Device* device, GraphicsResourceUsage usage, uint32_t width, uint32_t height, TextureFormat format, bool mipmap, const void* initialData) {
    if (!DX12Texture::initAsTexture2D(usage, width, height, format, mipmap)) return err();
    m_device = device;
    //m_usage = usage;
    //   m_size.width = width;
    //   m_size.height = height;

    UINT mipLevels = 1;
    if (mipmap) {
        mipLevels = static_cast<uint32_t>(std::floor(std::log2(std::max(width, height)))) + 1;
    }

    m_image = makeRHIRef<DX12Image>();
    if (!m_image->init(
            device, width, height, mipLevels, false, DX12Helper::LNTextureFormatToDXFormat(format), D3D12_RESOURCE_FLAG_NONE, (initialData) ? D3D12_RESOURCE_STATE_COPY_DEST : D3D12_RESOURCE_STATE_GENERIC_READ)) {
        return err();
    }

    if (initialData) {
        // Upload Buffer は サイズが足りていれば Format は問わない。
        // また D3D12_HEAP_TYPE_DEFAULT に作成されたテクスチャは width にアライメントがついていることがある。
        // そのため横幅の差を考慮して転送しないと画像がくずれる。
        DX12Buffer uploadBuffer;
        if (!uploadBuffer.init(m_device, m_image->uploadBufferSize(), D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_GENERIC_READ)) {
            return err();
        }
        const size_t pixelSize = GraphicsHelper::getPixelSize(format);
        void* data = uploadBuffer.map();
        RHIBitmap bmp1, bmp2;
        bmp1.initWrap(initialData, pixelSize, width, height);
        bmp2.initWritableWrap(data, pixelSize, m_image->dxFootprint().Footprint.RowPitch / pixelSize, height);
        bmp2.blit(&bmp1);
        uploadBuffer.unmap();

        D3D12_TEXTURE_COPY_LOCATION dst;
        dst.pResource = m_image->dxResource();
        dst.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
        dst.SubresourceIndex = 0;

        D3D12_TEXTURE_COPY_LOCATION src;
        src.pResource = uploadBuffer.dxResource();
        src.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
        src.PlacedFootprint = m_image->dxFootprint();

        ID3D12GraphicsCommandList* commandList = m_device->beginSingleTimeCommandList();
        commandList->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);
        m_image->resourceBarrior(commandList, D3D12_RESOURCE_STATE_GENERIC_READ);
        m_device->endSingleTimeCommandList(commandList);
    }

    if (mipLevels >= 2) {
        if (!generateMips()) {
            return err();
        }
    }

    return ok();
}

void DX12Texture2D::dispose() {
    if (m_image) {
        m_image->dispose();
        m_image = nullptr;
    }
    DX12Texture::dispose();
}

void DX12Texture2D::setSubData(DX12GraphicsContext* graphicsContext, int x, int y, int width, int height, const void* data, size_t dataSize) {
    LN_NOTIMPLEMENTED();
}

struct DWParam {
    DWParam(FLOAT f)
        : Float(f) {}
    DWParam(UINT u)
        : Uint(u) {}

    void operator=(FLOAT f) { Float = f; }
    void operator=(UINT u) { Uint = u; }

    union {
        FLOAT Float;
        UINT Uint;
    };
};

bool DX12Texture2D::generateMips() {
#if 1
    HRESULT hr;
    ID3D12Device* dxDevice = m_device->device();
    const uint32_t width = extentSize().width;
    const uint32_t height = extentSize().height;
    const uint32_t depth = 1;
    const uint32_t mipMaps = m_image->mipLevels();
    const uint32_t requiredHeapSize = mipMaps;
    ID3D12GraphicsCommandList* commandList = m_device->beginSingleTimeCommandList();

    D3D12_HEAP_PROPERTIES props;
    props.Type = D3D12_HEAP_TYPE_DEFAULT;
    props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    props.CreationNodeMask = m_device->creationNodeMask();
    props.VisibleNodeMask = m_device->visibleNodeMask();

    // 一時バッファ。
    // 最初に元テクスチャをここにコピーし、ComputeShader でダウンサンプリング、最後に書き戻す。
    D3D12_RESOURCE_DESC tempTextureDescriptor = {};
    tempTextureDescriptor.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    tempTextureDescriptor.Alignment = 0;
    tempTextureDescriptor.Width = width;
    tempTextureDescriptor.Height = height;
    tempTextureDescriptor.DepthOrArraySize = depth;
    tempTextureDescriptor.MipLevels = mipMaps;
    tempTextureDescriptor.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // TODO:
    tempTextureDescriptor.SampleDesc.Count = 1;
    tempTextureDescriptor.SampleDesc.Quality = 0;
    tempTextureDescriptor.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    tempTextureDescriptor.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
    ComPtr<ID3D12Resource> tmpTextureResource;
    dxDevice->CreateCommittedResource(&props, D3D12_HEAP_FLAG_NONE, &tempTextureDescriptor, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&tmpTextureResource));

    // 元テクスチャの内容を、Subresource 含めてすべてのコピーする
    //D3D12_RESOURCE_BARRIER srcTextureBarrier1 = {};
    //srcTextureBarrier1.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    //srcTextureBarrier1.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    //srcTextureBarrier1.Transition.pResource = tmpTextureResource;
    //srcTextureBarrier1.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
    //srcTextureBarrier1.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_SOURCE;
    //srcTextureBarrier1.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    //commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(texture->_resource, , ));
    m_image->resourceBarrior(commandList, D3D12_RESOURCE_STATE_COPY_SOURCE);
    commandList->CopyResource(tmpTextureResource.Get(), m_image->dxResource());

    // まずすべての Subresource を、UNORDERED_ACCESS にする
    D3D12_RESOURCE_BARRIER tmpTextureBarrier2 = {};
    tmpTextureBarrier2.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    tmpTextureBarrier2.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    tmpTextureBarrier2.Transition.pResource = tmpTextureResource.Get();
    tmpTextureBarrier2.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
    tmpTextureBarrier2.Transition.StateAfter = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
    tmpTextureBarrier2.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    commandList->ResourceBarrier(1, &tmpTextureBarrier2);

    // 0 番目の Subresource を D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE へ
    D3D12_RESOURCE_BARRIER uavToSrvBarrier = {};
    uavToSrvBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    uavToSrvBarrier.Transition.pResource = tmpTextureResource.Get();
    uavToSrvBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    uavToSrvBarrier.Transition.Subresource = 0;
    uavToSrvBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
    uavToSrvBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
    commandList->ResourceBarrier(1, &uavToSrvBarrier);

    // CreateDescriptorHeap
    ComPtr<ID3D12DescriptorHeap> descriptorHeap;
    UINT descriptorSize;
    {
        D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
        heapDesc.NumDescriptors = 2 * requiredHeapSize;
        heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

        hr = dxDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&descriptorHeap));
        if (FAILED(hr)) {
            LN_ERROR("CreateDescriptorHeap failed.");
            return false;
        }

        descriptorSize = dxDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    }

    commandList->SetComputeRootSignature(m_device->mipmapRootSignature());
    commandList->SetPipelineState(m_device->mipmapPipelineState());

    ID3D12DescriptorHeap* heaps[] = { descriptorHeap.Get() };
    commandList->SetDescriptorHeaps(1, heaps);

    D3D12_CPU_DESCRIPTOR_HANDLE currentCPUHandle = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
    D3D12_GPU_DESCRIPTOR_HANDLE currentGPUHandle = descriptorHeap->GetGPUDescriptorHandleForHeapStart();

    D3D12_SHADER_RESOURCE_VIEW_DESC srcTextureSRVDesc = {};
    srcTextureSRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
    srcTextureSRVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
    srcTextureSRVDesc.Format = tempTextureDescriptor.Format;

    D3D12_UNORDERED_ACCESS_VIEW_DESC destTextureUAVDesc = {};
    destTextureUAVDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
    destTextureUAVDesc.Format = tempTextureDescriptor.Format;

    for (uint32_t TopMip = 0; TopMip < mipMaps - 1; TopMip++) {
        uint32_t dstWidth = std::max(width >> (TopMip + 1), 1u);
        uint32_t dstHeight = std::max(height >> (TopMip + 1), 1u);

        // SrcTexture (D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE)
        srcTextureSRVDesc.Texture2D.MipLevels = 1;
        srcTextureSRVDesc.Texture2D.MostDetailedMip = TopMip;
        dxDevice->CreateShaderResourceView(tmpTextureResource.Get(), &srcTextureSRVDesc, currentCPUHandle);
        currentCPUHandle.ptr += descriptorSize;

        // DstTexture (D3D12_RESOURCE_STATE_UNORDERED_ACCESS)
        destTextureUAVDesc.Texture2D.MipSlice = TopMip + 1;
        dxDevice->CreateUnorderedAccessView(tmpTextureResource.Get(), nullptr, &destTextureUAVDesc, currentCPUHandle);
        currentCPUHandle.ptr += descriptorSize;

        // ConstantBuffer
        commandList->SetComputeRoot32BitConstant(0, DWParam(1.0f / dstWidth).Uint, 0);  // TexelSize.x
        commandList->SetComputeRoot32BitConstant(0, DWParam(1.0f / dstHeight).Uint, 1); // TexelSize.y
        commandList->SetComputeRoot32BitConstant(0, DWParam(1.0f).Uint, 2);             // GammaCurve TODO: Gamma

        // DescriptorTable
        commandList->SetComputeRootDescriptorTable(1, currentGPUHandle);
        currentGPUHandle.ptr += descriptorSize;
        commandList->SetComputeRootDescriptorTable(2, currentGPUHandle);
        currentGPUHandle.ptr += descriptorSize;

        // Dispatch
        commandList->Dispatch(std::max(dstWidth / 8, 1u), std::max(dstHeight / 8, 1u), 1);

        // UAV アクセス終了を待つ
        D3D12_RESOURCE_BARRIER tmpTextureBarrire3;
        tmpTextureBarrire3.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
        tmpTextureBarrire3.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        tmpTextureBarrire3.UAV.pResource = tmpTextureResource.Get();
        commandList->ResourceBarrier(1, &tmpTextureBarrire3);

        // D3D12_RESOURCE_STATE_UNORDERED_ACCESS -> D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE
        uavToSrvBarrier.Transition.Subresource = TopMip + 1;
        commandList->ResourceBarrier(1, &uavToSrvBarrier);
    }

    // 結果を元のテクスチャに戻す
    {
        m_image->resourceBarrior(commandList, D3D12_RESOURCE_STATE_COPY_DEST);

        D3D12_RESOURCE_BARRIER tmpTextureBarrier3 = {};
        tmpTextureBarrier3.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        tmpTextureBarrier3.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
        tmpTextureBarrier3.Transition.pResource = tmpTextureResource.Get();
        tmpTextureBarrier3.Transition.StateBefore = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
        tmpTextureBarrier3.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_SOURCE;
        tmpTextureBarrier3.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        commandList->ResourceBarrier(1, &tmpTextureBarrier3);

        commandList->CopyResource(m_image->dxResource(), tmpTextureResource.Get());

        m_image->resourceBarrior(commandList, D3D12_RESOURCE_STATE_GENERIC_READ);
    }

    m_device->endSingleTimeCommandList(commandList);
#endif
    return true;
}

//==============================================================================
// DX12RenderTarget

DX12RenderTarget::DX12RenderTarget()
    : m_device(nullptr) {
}

bool DX12RenderTarget::init(DX12Device* device, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap, bool msaa) {
    if (!DX12Texture::initAsRenderTarget(width, height, requestFormat, mipmap, msaa)) return false;
    m_device = device;
    //m_size.width = width;
    //m_size.height = height;
    //m_format = requestFormat;
    //m_currentState = D3D12_RESOURCE_STATE_GENERIC_READ;
    //m_dxFormat = DX12Helper::LNTextureFormatToDXFormat(m_format);
    //m_mipLevels = 1;

    m_image = makeRHIRef<DX12Image>();
    if (!m_image->init(
            device, width, height, 1, false, DX12Helper::LNTextureFormatToDXFormat(requestFormat), D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET, D3D12_RESOURCE_STATE_GENERIC_READ)) {
        return false;
    }

    if (msaa) {
        m_multisampleBuffer = makeRHIRef<DX12Image>();
        if (!m_multisampleBuffer->init(
                device, m_image->size().width, m_image->size().height, 1, true, m_image->dxFormat(), D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET, D3D12_RESOURCE_STATE_GENERIC_READ)) {
            return false;
        }
    }

    return true;
}

bool DX12RenderTarget::init(DX12Device* device, const ComPtr<ID3D12Resource>& dxRenderTarget) {
    m_device = device;

    m_image = makeRHIRef<DX12Image>();
    if (!m_image->init(device, dxRenderTarget, D3D12_RESOURCE_STATE_PRESENT)) {
        return false;
    }

    // TODO: SwapChain は BGRA フォーマットであることが多い。
    // ただ TextureFormat はそれに対応していないが、readData() で Bitmap をとるときにピクセルサイズが知りたい。
    // ここではダミーとして RGBA8 を与えて初期化してみる。
    if (!DX12Texture::initAsRenderTarget(m_image->size().width, m_image->size().height, TextureFormat::RGBA8, false, false)) return false;

#if 0
    m_multisampleBuffer = makeRHIRef<DX12Image>();
    if (!m_multisampleBuffer->init(
        device, m_image->size().width, m_image->size().height, 1, true,
        m_image->dxFormat(),
        D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET,
        D3D12_RESOURCE_STATE_GENERIC_READ)) {
        return false;
    }
#endif

    //m_size.width = m_image->size().width;
    //m_size.height = m_image->size().height;

    return true;
}

void DX12RenderTarget::dispose() {
    if (m_image) {
        m_image->dispose();
        m_image = nullptr;
    }
    if (m_multisampleBuffer) {
        m_multisampleBuffer->dispose();
        m_multisampleBuffer = nullptr;
    }
    DX12Texture::dispose();
}

RHIRef<RHIBitmap> DX12RenderTarget::readData() {
    D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint;
    D3D12_RESOURCE_DESC textureDesc = m_image->dxResource()->GetDesc();
    UINT64 totalSize;
    m_device->device()->GetCopyableFootprints(&textureDesc, 0, 1, 0, &footprint, nullptr, nullptr, &totalSize);

    // 読み取り用一時バッファ
    size_t size = totalSize;
    size_t size2 = extentSize().width * extentSize().height * DX12Helper::getFormatSize(m_image->dxFormat());
    DX12Buffer buffer;
    if (!buffer.init(m_device, size, D3D12_HEAP_TYPE_READBACK, D3D12_RESOURCE_STATE_COPY_DEST)) {
        return nullptr;
    }

    ID3D12GraphicsCommandList* commandList = m_device->beginSingleTimeCommandList();
    if (!commandList) {
        return nullptr;
    }

    D3D12_TEXTURE_COPY_LOCATION src, dst;
    src.pResource = m_image->dxResource();
    src.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
    src.SubresourceIndex = 0;
    dst.pResource = buffer.dxResource();
    dst.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
    dst.PlacedFootprint = footprint;

    //D3D12_BOX box;
    //box.left = 0;
    //box.top = 0;
    //box.front = 0;
    //box.right = m_size.width;
    //box.bottom = m_size.height;
    //box.back = 1;
    m_image->resourceBarrior(commandList, D3D12_RESOURCE_STATE_COPY_SOURCE);
    commandList->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);
    m_image->resourceBarrior(commandList, D3D12_RESOURCE_STATE_GENERIC_READ);

    if (!m_device->endSingleTimeCommandList(commandList)) {
        return nullptr;
    }

    const void* data = buffer.map();

    // width が 160 だと 192 に Alignment されることがあった。
    // 余分は切り捨てて返す。
    {
        auto bitmap1 = makeRHIRef<RHIBitmap>();
        if (!bitmap1->init(4, footprint.Footprint.RowPitch / 4, footprint.Footprint.Height)) {
            return nullptr;
        }
        bitmap1->copyRaw(data, totalSize);

        auto bitmap2 = makeRHIRef<RHIBitmap>();
        if (!bitmap2->init(4, extentSize().width, extentSize().height)) {
            return nullptr;
        }
        bitmap2->blit(bitmap1.get());

        return bitmap2;
    }

    //auto bitmap = makeObject_deprecated<Bitmap2D>(w, m_size.height, PixelFormat::RGBA8, data);
    //bitmap->save(u"test2.png");

    //memcpy(outData, data, size2);
    //buffer.unmap();
}

//==============================================================================
// DX12DepthBuffer

DX12DepthBuffer::DX12DepthBuffer()
    : m_deviceContext(nullptr) {
}

Result DX12DepthBuffer::init(DX12Device* device, uint32_t width, uint32_t height) {
    LN_DCHECK(device);
    LN_TRY(RHIResource::initAsDepthBuffer(width, height, true));
    m_deviceContext = device;

    m_image = makeRHIRef<DX12Image>();
    if (!m_image->init(
            device, width, height, 1, false, DXGI_FORMAT_D24_UNORM_S8_UINT, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL, D3D12_RESOURCE_STATE_GENERIC_READ)) {
        return err();
    }

    m_multisampleBuffer = makeRHIRef<DX12Image>();
    if (!m_multisampleBuffer->init(
            device, width, height, 1, true, DXGI_FORMAT_D24_UNORM_S8_UINT, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL, D3D12_RESOURCE_STATE_GENERIC_READ)) {
        return err();
    }

    return ok();
}

void DX12DepthBuffer::dispose() {
    if (m_image) {
        m_image->dispose();
        m_image = nullptr;
    }
    if (m_multisampleBuffer) {
        m_multisampleBuffer->dispose();
        m_multisampleBuffer = nullptr;
    }
    RHIResource::dispose();
}

} // namespace detail
} // namespace ln
