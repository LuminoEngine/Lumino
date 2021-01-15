
#include "Internal.hpp"
#include "DX12DeviceContext.hpp"
#include "DX12VertexBuffer.hpp"

namespace ln {
namespace detail {
	
//==============================================================================
// DX12VertexBuffer

DX12VertexBuffer::DX12VertexBuffer()
    : m_usage(GraphicsResourceUsage::Static)
{
}

bool DX12VertexBuffer::init(DX12Device* device, GraphicsResourceUsage usage, size_t size, const void* initialData)
{
    LN_DCHECK(device);
    m_device = device;
    m_usage = usage;
    m_size = size;

    ID3D12Device* dx12Device = m_device->device();

    // NOTE: ↓ DirectX-Graphics-Samples, D3D12HelloTriangle.cpp より
    //   Note: using upload heaps to transfer static data like vert buffers is not 
    //   recommended. Every time the GPU needs it, the upload heap will be marshalled 
    //   over. Please read up on Default Heap usage. An upload heap is used here for 
    //   code simplicity and because there are very few verts to actually transfer.
    // Dynamic な時は D3D12_HEAP_TYPE_UPLOAD を使う、でよさそう。

    D3D12_HEAP_PROPERTIES props;
    props.Type = (m_usage == GraphicsResourceUsage::Dynamic) ? D3D12_HEAP_TYPE_UPLOAD : D3D12_HEAP_TYPE_DEFAULT;
    props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    props.CreationNodeMask = 1;
    props.VisibleNodeMask = 1;

    D3D12_RESOURCE_DESC desc;
    desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    desc.Alignment = 0;
    desc.Width = m_size;
    desc.Height = 1;
    desc.DepthOrArraySize = 1;
    desc.MipLevels = 1;
    DXGI_FORMAT Format = DXGI_FORMAT_UNKNOWN;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    desc.Flags = D3D12_RESOURCE_FLAG_NONE;

    if (FAILED(dx12Device->CreateCommittedResource(
        &props,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&m_vertexBuffer)))) {
        LN_ERROR("CreateCommittedResource failed.");
        return false;
    }

    if (initialData) {
        if (m_usage == GraphicsResourceUsage::Dynamic) {
            // Range(0,0) は、データの読み取りを行わないことを明示する。
            // null は読み取りの可能性を示すもので、最適化がされないことがある。
            // https://docs.microsoft.com/en-us/windows/win32/api/d3d12/nf-d3d12-id3d12resource-map
            D3D12_RANGE readRange = { 0, 0 };
            UINT8* data;
            if (FAILED(m_vertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&data)))) {
                LN_ERROR("Map failed.");
                return false;
            }

            memcpy(data, initialData, m_size);
            m_vertexBuffer->Unmap(0, nullptr);
        }
        else {
            LN_NOTIMPLEMENTED();
        }
    }

    return true;
}

void DX12VertexBuffer::dispose()
{
    m_vertexBuffer.Reset();
    IVertexBuffer::dispose();
}

size_t DX12VertexBuffer::getBytesSize()
{
    return m_size;
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

} // namespace detail
} // namespace ln
