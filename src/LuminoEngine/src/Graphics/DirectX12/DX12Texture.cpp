
#include "Internal.hpp"
#include "DX12Buffers.hpp"
#include "DX12Texture.hpp"

#include <LuminoEngine/Graphics/Bitmap.hpp> // TODO: test

namespace ln {
namespace detail {

//==============================================================================
// DX12RenderTarget

DX12RenderTarget::DX12RenderTarget()
    : m_deviceContext(nullptr)
{
}

bool DX12RenderTarget::init(DX12Device* deviceContext, uint32_t width, uint32_t height, TextureFormat requestFormat, bool mipmap)
{
    LN_DCHECK(deviceContext);
    m_deviceContext = deviceContext;
    m_size.width = width;
    m_size.height = height;
    m_format = requestFormat;
    LN_NOTIMPLEMENTED();

    return true;
}

bool DX12RenderTarget::init(DX12Device* deviceContext, const ComPtr<ID3D12Resource>& dxRenderTarget)
{
    m_deviceContext = deviceContext;
    m_dxRenderTarget = dxRenderTarget;

    D3D12_RESOURCE_DESC desc = m_dxRenderTarget->GetDesc();
    m_size.width = desc.Width;
    m_size.height = desc.Height;
    m_format = DX12Helper::DXFormatToLNTextureFormat(desc.Format);
    m_dxFormat = desc.Format;
    m_currentState = D3D12_RESOURCE_STATE_PRESENT;
    if (LN_REQUIRE(m_format != TextureFormat::Unknown)) return false;

    return true;
}

void DX12RenderTarget::dispose()
{
    m_dxRenderTarget.Reset();
    DX12Texture::dispose();
}

void DX12RenderTarget::readData(void* outData)
{
    D3D12_PLACED_SUBRESOURCE_FOOTPRINT footprint;
    D3D12_RESOURCE_DESC textureDesc = m_dxRenderTarget->GetDesc();
    UINT64 totalSize;
    m_deviceContext->device()->GetCopyableFootprints(&textureDesc, 0, 1, 0, &footprint, nullptr, nullptr, &totalSize);


    // 読み取り用一時バッファ
    size_t size = totalSize;//m_size.width * m_size.height * DX12Helper::getFormatSize(m_dxFormat);
    size_t size2 = m_size.width * m_size.height * DX12Helper::getFormatSize(m_dxFormat);//footprint.Footprint.RowPitch * footprint.Footprint.Height;
    DX12Buffer buffer;
    if (!buffer.init(m_deviceContext, size, D3D12_HEAP_TYPE_READBACK, D3D12_RESOURCE_STATE_COPY_DEST)) {
        return;
    }



    ID3D12GraphicsCommandList* commandList = m_deviceContext->beginSingleTimeCommandList();
    if (!commandList) {
        return;
    }

    D3D12_TEXTURE_COPY_LOCATION src, dst;
    src.pResource = m_dxRenderTarget.Get();
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
    resourceBarrior(commandList, D3D12_RESOURCE_STATE_COPY_SOURCE);
    commandList->CopyTextureRegion(&dst, 0, 0, 0, &src, nullptr);
    resourceBarrior(commandList, D3D12_RESOURCE_STATE_GENERIC_READ);

    if (!m_deviceContext->endSingleTimeCommandList(commandList)) {
        return;
    }

    const void* data = buffer.map();

    auto w = footprint.Footprint.RowPitch / 4;
    auto bitmap = makeObject<Bitmap2D>(w, m_size.height, PixelFormat::RGBA8, data);
    bitmap->save(u"test2.png");



    memcpy(outData, data, size2);
    buffer.unmap();
}

//==============================================================================
// DX12DepthBuffer

DX12DepthBuffer::DX12DepthBuffer()
    : m_deviceContext(nullptr)
    , m_size(0, 0)
    , m_dxDepthBuffer()
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

    m_dxFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    m_currentState = D3D12_RESOURCE_STATE_DEPTH_READ;

    ID3D12Device* dxDevice = m_deviceContext->device();

    D3D12_HEAP_PROPERTIES props;
    props.Type = D3D12_HEAP_TYPE_DEFAULT;
    props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    props.CreationNodeMask = m_deviceContext->creationNodeMask();
    props.VisibleNodeMask = m_deviceContext->visibleNodeMask();

    D3D12_RESOURCE_DESC desc;
    desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    desc.Alignment = 0;
    desc.Width = m_size.width;
    desc.Height = m_size.height;
    desc.DepthOrArraySize = 1;  // This depth stencil view has only one texture.
    desc.MipLevels = 1;         // Use a single mipmap level.
    desc.Format = m_dxFormat;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

    D3D12_CLEAR_VALUE clearValue = {};
    clearValue.Format = m_dxFormat;
    clearValue.DepthStencil.Depth = 1.0f;
    clearValue.DepthStencil.Stencil = 0;

    if (FAILED(dxDevice->CreateCommittedResource(
        &props,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        D3D12_RESOURCE_STATE_DEPTH_WRITE,
        &clearValue,
        IID_PPV_ARGS(&m_dxDepthBuffer)))) {
        LN_ERROR("CreateCommittedResource failed.");
        return false;
    }

    return true;
}

void DX12DepthBuffer::dispose()
{
    m_dxDepthBuffer.Reset();
    IDepthBuffer::dispose();
}

void DX12DepthBuffer::resourceBarrior(ID3D12GraphicsCommandList* commandList, D3D12_RESOURCE_STATES newState)
{
    if (m_currentState == newState) return;

    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = m_dxDepthBuffer.Get();
    barrier.Transition.StateBefore = m_currentState;
    barrier.Transition.StateAfter = newState;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    commandList->ResourceBarrier(1, &barrier);
    m_currentState = newState;
}

} // namespace detail
} // namespace ln
