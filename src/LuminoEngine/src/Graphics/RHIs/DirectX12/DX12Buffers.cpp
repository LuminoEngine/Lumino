
#include "Internal.hpp"
#include "DX12DeviceContext.hpp"
#include "DX12Buffers.hpp"

namespace ln {
namespace detail {
	
//==============================================================================
// DX12Buffer
    
DX12Buffer::DX12Buffer()
	: m_dxResource()
	, m_size(0)
	, m_resourceState(D3D12_RESOURCE_STATE_COMMON)
    , m_mapped(false)
{
}

bool DX12Buffer::init(DX12Device* device, size_t size, D3D12_HEAP_TYPE heapType, D3D12_RESOURCE_STATES initialState)
{
    m_size = size;
    m_resourceState = initialState;

    D3D12_HEAP_PROPERTIES props;
    props.Type = heapType;
    props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    props.CreationNodeMask = device->creationNodeMask();
    props.VisibleNodeMask = device->visibleNodeMask();

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

    HRESULT hr = device->device()->CreateCommittedResource(
        &props,
        D3D12_HEAP_FLAG_NONE,
        &desc,
        m_resourceState,
        nullptr,
        IID_PPV_ARGS(&m_dxResource));
    if (FAILED(hr)) {
        LN_ERROR("CreateCommittedResource failed.");
        return false;
    }

	return true;
}

void DX12Buffer::dispose()
{
    m_dxResource.Reset();
}

void* DX12Buffer::map()
{
    if (LN_REQUIRE(!m_mapped)) return nullptr;

    // Range(0,0) は、データの読み取りを行わないことを明示する。
    // null は読み取りの可能性を示すもので、最適化がされないことがある。
    // https://docs.microsoft.com/en-us/windows/win32/api/d3d12/nf-d3d12-id3d12resource-map
    D3D12_RANGE readRange = { 0, 0 };
    UINT8* data;
    if (FAILED(m_dxResource->Map(0, &readRange, reinterpret_cast<void**>(&data)))) {
        LN_ERROR("Map failed.");
        return nullptr;
    }
    m_mapped = true;
    return data;
}

void DX12Buffer::unmap()
{
    if (LN_REQUIRE(m_mapped)) return;
    //m_dxResource->Unmap(0, nullptr);
    m_mapped = false;
}

void DX12Buffer::resourceBarrior(ID3D12GraphicsCommandList* commandList, D3D12_RESOURCE_STATES newState)
{
    if (m_resourceState == newState) {
        return;
    }

    D3D12_RESOURCE_BARRIER barrier;
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barrier.Transition.pResource = m_dxResource.Get();
    barrier.Transition.StateBefore = m_resourceState;
    barrier.Transition.StateAfter = newState;
    barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    commandList->ResourceBarrier(1, &barrier);
    m_resourceState = newState;
}

//==============================================================================
// DX12VertexBuffer

DX12VertexBuffer::DX12VertexBuffer()
    : m_device(nullptr)
    , m_usage(GraphicsResourceUsage::Static)
    , m_buffer()
{
}

bool DX12VertexBuffer::init(DX12Device* device, GraphicsResourceUsage usage, size_t size, const void* initialData)
{
    m_device = device;
    m_usage = usage;

    // NOTE: ↓ DirectX-Graphics-Samples, D3D12HelloTriangle.cpp より
    //   Note: using upload heaps to transfer static data like vert buffers is not 
    //   recommended. Every time the GPU needs it, the upload heap will be marshalled 
    //   over. Please read up on Default Heap usage. An upload heap is used here for 
    //   code simplicity and because there are very few verts to actually transfer.
    // Dynamic な時は D3D12_HEAP_TYPE_UPLOAD を使う、でよさそう。

    // 組み合わせはこれが必須
    D3D12_HEAP_TYPE heapType;
    D3D12_RESOURCE_STATES resourceState;
    if (m_usage == GraphicsResourceUsage::Dynamic) {
        heapType = D3D12_HEAP_TYPE_UPLOAD;
        resourceState = D3D12_RESOURCE_STATE_GENERIC_READ;
    }
    else {
        heapType = D3D12_HEAP_TYPE_DEFAULT;
        resourceState = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
    }

    m_buffer = makeRHIRef<DX12Buffer>();
    if (!m_buffer->init(m_device, size, heapType, resourceState)) {
        return false;
    }




    //m_usage = usage;
    //m_size = size;

    //ID3D12Device* dx12Device = m_device->device();

    //D3D12_HEAP_PROPERTIES props;
    //props.Type = heapType;
    //props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    //props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    //props.CreationNodeMask = 1;
    //props.VisibleNodeMask = 1;

    //D3D12_RESOURCE_DESC desc;
    //desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    //desc.Alignment = 0;
    //desc.Width = m_size;
    //desc.Height = 1;
    //desc.DepthOrArraySize = 1;
    //desc.MipLevels = 1;
    //desc.Format = DXGI_FORMAT_UNKNOWN;
    //desc.SampleDesc.Count = 1;
    //desc.SampleDesc.Quality = 0;
    //desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    //desc.Flags = D3D12_RESOURCE_FLAG_NONE;

    //if (FAILED(dx12Device->CreateCommittedResource(
    //    &props,
    //    D3D12_HEAP_FLAG_NONE,
    //    &desc,
    //    resourceState,
    //    nullptr,
    //    IID_PPV_ARGS(&m_vertexBuffer)))) {
    //    LN_ERROR("CreateCommittedResource failed.");
    //    return false;
    //}

    if (initialData) {
        if (m_usage == GraphicsResourceUsage::Dynamic) {
            void* data = m_buffer->map();
            memcpy(data, initialData, size);
            m_buffer->unmap();
        }
        else {
            // D3D12_HEAP_TYPE_DEFAULT を指定して作成した Resource は Map/Unmap できない。
            // 転送用の一時バッファ (uploadBuffer) を作って、それ経由でデータを転送する。

            DX12Buffer uploadBuffer;
            if (!uploadBuffer.init(m_device, size, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_GENERIC_READ)) {
                return false;
            }
            void* data = uploadBuffer.map();
            memcpy(data, initialData, size);
            uploadBuffer.unmap();

            ID3D12GraphicsCommandList* commandList = m_device->beginSingleTimeCommandList();
            m_buffer->resourceBarrior(commandList, D3D12_RESOURCE_STATE_COPY_DEST);
            commandList->CopyResource(m_buffer->dxResource(), uploadBuffer.dxResource());
            m_buffer->resourceBarrior(commandList, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
            m_device->endSingleTimeCommandList(commandList);
        }
    }

    return true;
}

void DX12VertexBuffer::dispose()
{
    if (m_buffer) {
        m_buffer->dispose();
        m_buffer = nullptr;
    }
    IVertexBuffer::dispose();
}

size_t DX12VertexBuffer::getBytesSize()
{
    return m_buffer->size();
}

GraphicsResourceUsage DX12VertexBuffer::usage() const
{
    return m_usage;
}

//void* DX12VertexBuffer::map()
//{
//    LN_NOTIMPLEMENTED();
//    return 0;
//}
//
//void DX12VertexBuffer::unmap()
//{
//    LN_NOTIMPLEMENTED();
//}

//==============================================================================
// DX12IndexBuffer

DX12IndexBuffer::DX12IndexBuffer()
    : m_usage(GraphicsResourceUsage::Static)
    , m_indexFormat(DXGI_FORMAT_R16_UINT)
{
}

Result DX12IndexBuffer::init(DX12Device* device, GraphicsResourceUsage usage, IndexBufferFormat format, int indexCount, const void* initialData)
{
    m_device = device;
    m_usage = usage;

    // Buffer Size
    int stride = 0;
    if (format == IndexBufferFormat::UInt16) {
        m_indexFormat = DXGI_FORMAT_R16_UINT;
        stride = 2;
    }
    else {
        m_indexFormat = DXGI_FORMAT_R32_UINT;
        stride = 4;
    }
    size_t size = stride * indexCount;

    // Types
    D3D12_HEAP_TYPE heapType;
    D3D12_RESOURCE_STATES resourceState;
    if (m_usage == GraphicsResourceUsage::Dynamic) {
        heapType = D3D12_HEAP_TYPE_UPLOAD;
        resourceState = D3D12_RESOURCE_STATE_GENERIC_READ;
    }
    else {
        heapType = D3D12_HEAP_TYPE_DEFAULT;
        resourceState = D3D12_RESOURCE_STATE_INDEX_BUFFER;
    }

    m_buffer = makeRHIRef<DX12Buffer>();
    if (!m_buffer->init(m_device, size, heapType, resourceState)) {
        return false;
    }

    if (initialData) {
        if (m_usage == GraphicsResourceUsage::Dynamic) {
            void* data = m_buffer->map();
            memcpy(data, initialData, size);
            m_buffer->unmap();
        }
        else {
            // D3D12_HEAP_TYPE_DEFAULT を指定して作成した Resource は Map/Unmap できない。
            // 転送用の一時バッファ (uploadBuffer) を作って、それ経由でデータを転送する。

            DX12Buffer uploadBuffer;
            if (!uploadBuffer.init(m_device, size, D3D12_HEAP_TYPE_UPLOAD, D3D12_RESOURCE_STATE_GENERIC_READ)) {
                return false;
            }
            void* data = uploadBuffer.map();
            memcpy(data, initialData, size);
            uploadBuffer.unmap();

            ID3D12GraphicsCommandList* commandList = m_device->beginSingleTimeCommandList();
            m_buffer->resourceBarrior(commandList, D3D12_RESOURCE_STATE_COPY_DEST);
            commandList->CopyResource(m_buffer->dxResource(), uploadBuffer.dxResource());
            m_buffer->resourceBarrior(commandList, D3D12_RESOURCE_STATE_INDEX_BUFFER);
            m_device->endSingleTimeCommandList(commandList);
        }
    }

    return true;
}

void DX12IndexBuffer::dispose()
{
    if (m_buffer) {
        m_buffer->dispose();
        m_buffer = nullptr;
    }
    IIndexBuffer::dispose();
}

size_t DX12IndexBuffer::getBytesSize()
{
    return m_buffer->size();
}

GraphicsResourceUsage DX12IndexBuffer::usage() const
{
    return m_usage;
}

//void* DX12IndexBuffer::map()
//{
//    LN_NOTIMPLEMENTED();
//    return 0;
//}
//
//void DX12IndexBuffer::unmap()
//{
//    LN_NOTIMPLEMENTED();
//}

} // namespace detail
} // namespace ln
