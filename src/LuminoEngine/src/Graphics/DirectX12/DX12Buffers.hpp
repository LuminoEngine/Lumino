
#pragma once
#include "DX12Helper.hpp"

namespace ln {
namespace detail {

class DX12Buffer
	: public RefObject
{
public:
	DX12Buffer();
	bool init(DX12Device* device, size_t size, D3D12_HEAP_TYPE heapType, D3D12_RESOURCE_STATES initialState);
	void dispose();
	ID3D12Resource* dxResource() const { return m_dxResource.Get(); }
	size_t size() const { return m_size; }
	void* map();
	void unmap();
	void resourceBarrior(ID3D12GraphicsCommandList* commandList, D3D12_RESOURCE_STATES newState);

private:
	//DX12Device* m_device;
	//GraphicsResourceUsage m_usage;
	ComPtr<ID3D12Resource> m_dxResource;
	size_t m_size;
	D3D12_RESOURCE_STATES m_resourceState;
	bool m_mapped;
};

} // namespace detail
} // namespace ln
