
#include "Internal.hpp"
#include "DX12DeviceContext.hpp"
#include "DX12Buffers.hpp"

namespace ln {
namespace detail {
	
//==============================================================================
// DX12VertexBuffer
    
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
    m_dxResource->Unmap(0, nullptr);
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

} // namespace detail
} // namespace ln
