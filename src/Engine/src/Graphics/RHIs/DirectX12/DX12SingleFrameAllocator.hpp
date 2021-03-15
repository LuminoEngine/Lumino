
#pragma once
#include <LuminoCore/Base/LinearAllocator.hpp>
#include "DX12Helper.hpp"
#include "DX12Buffers.hpp"

namespace ln {
namespace detail {
class DX12Device;

struct DX12SingleFrameBufferView
{
	DX12Buffer* buffer;
	uint32_t offset;
};

class DX12SingleFrameAllocatorPage
	: public AbstractLinearAllocatorPage
{
public:
	virtual ~DX12SingleFrameAllocatorPage();
	bool init(DX12Device* device, size_t size, D3D12_HEAP_TYPE heapType, D3D12_RESOURCE_STATES resourceState);
	DX12Buffer* buffer() const { return m_buffer.get(); }

private:
	RHIRef<DX12Buffer> m_buffer;
};

class DX12SingleFrameAllocatorPageManager
	: public LinearAllocatorPageManager
{
public:
	DX12SingleFrameAllocatorPageManager(DX12Device* device, size_t pageSize, D3D12_HEAP_TYPE heapType, D3D12_RESOURCE_STATES resourceState);

protected:
	Ref<AbstractLinearAllocatorPage> onCreateNewPage(size_t size) override;

private:
	DX12Device* m_device;
	D3D12_HEAP_TYPE m_heapType;
	D3D12_RESOURCE_STATES m_resourceState;
};

class DX12SingleFrameAllocator
	: public AbstractLinearAllocator
{
public:
	DX12SingleFrameAllocator(DX12SingleFrameAllocatorPageManager* manager);
	DX12SingleFrameBufferView allocate(size_t size, size_t alignment);

protected:
};

} // namespace detail
} // namespace ln
