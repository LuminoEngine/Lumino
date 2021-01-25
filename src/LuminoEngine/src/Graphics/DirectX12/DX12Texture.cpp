
#include "Internal.hpp"
#include "DX12Texture.hpp"

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
    m_dxRenderTarget = dxRenderTarget;

    D3D12_RESOURCE_DESC desc = m_dxRenderTarget->GetDesc();
    m_size.width = desc.Width;
    m_size.height = desc.Height;
    m_format = DX12Helper::DXFormatToLNTextureFormat(desc.Format);
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
    LN_NOTIMPLEMENTED();
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

    const DXGI_FORMAT format = DXGI_FORMAT_D24_UNORM_S8_UINT;

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
    desc.Format = format;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    desc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

    D3D12_CLEAR_VALUE clearValue = {};
    clearValue.Format = format;
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

} // namespace detail
} // namespace ln
