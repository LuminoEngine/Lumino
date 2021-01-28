
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
    desc.Format = DXGI_FORMAT_UNKNOWN;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    desc.Flags = D3D12_RESOURCE_FLAG_NONE;

    if (FAILED(dx12Device->CreateCommittedResource(
        &props,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
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
            // D3D12_HEAP_TYPE_DEFAULT を指定して作成した Resource は Map/Unmap できない。
            // 転送用の一時バッファ (uploadBuffer) を作って、それ経由でデータを転送する。

            D3D12_HEAP_PROPERTIES uploadProps = props;
            uploadProps.Type = D3D12_HEAP_TYPE_UPLOAD;
            D3D12_RESOURCE_DESC uploadDesc = desc;

            ComPtr<ID3D12Resource> uploadBuffer;
            if (FAILED(dx12Device->CreateCommittedResource(
                &uploadProps,
                D3D12_HEAP_FLAG_NONE,
                &uploadDesc,
                D3D12_RESOURCE_STATE_GENERIC_READ,
                nullptr,
                IID_PPV_ARGS(&uploadBuffer)))) {
                LN_ERROR("CreateCommittedResource failed.");
                return false;
            }

            uint8_t* memory = nullptr;
            if (FAILED(uploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&memory)))) {
                LN_ERROR("Map failed.");
                return false;
            }
            std::memcpy(memory, initialData, m_size);
            uploadBuffer->Unmap(0, nullptr);

            ID3D12GraphicsCommandList* commandList = m_device->beginSingleTimeCommandList();


            D3D12_RESOURCE_BARRIER b1;
            b1.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
            b1.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
            b1.Transition.pResource = m_vertexBuffer.Get();
            b1.Transition.StateBefore = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
            b1.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;
            b1.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
            commandList->ResourceBarrier(1, &b1);

            commandList->CopyResource(m_vertexBuffer.Get(), uploadBuffer.Get());

            D3D12_RESOURCE_BARRIER b2;
            b2.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
            b2.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
            b2.Transition.pResource = m_vertexBuffer.Get();
            b2.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
            b2.Transition.StateAfter = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
            b2.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
            commandList->ResourceBarrier(1, &b2);

            m_device->endSingleTimeCommandList(commandList);
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
