#include "DX12DeviceContext.hpp"
#include "DX12SingleFrameAllocator.hpp"

namespace ln {
namespace detail {
	
//==============================================================================
// DX12SingleFrameAllocatorPage

DX12SingleFrameAllocatorPage::~DX12SingleFrameAllocatorPage()
{
	if (m_buffer) {
		m_buffer->dispose();
		m_buffer = nullptr;
	}
}

bool DX12SingleFrameAllocatorPage::init(DX12Device* device, size_t size, D3D12_HEAP_TYPE heapType, D3D12_RESOURCE_STATES resourceState)
{
	m_buffer = makeRHIRef<DX12Buffer>();
	if (!m_buffer->init(device, size, heapType, resourceState)) {
		return false;
	}
	return true;
}

//==============================================================================
// DX12SingleFrameAllocatorPageManager

DX12SingleFrameAllocatorPageManager::DX12SingleFrameAllocatorPageManager(DX12Device* device, size_t pageSize, D3D12_HEAP_TYPE heapType, D3D12_RESOURCE_STATES resourceState)
	: LinearAllocatorPageManager(pageSize)
	, m_device(device)
	, m_heapType(heapType)
	, m_resourceState(resourceState)
{
}

Ref<AbstractLinearAllocatorPage> DX12SingleFrameAllocatorPageManager::onCreateNewPage(size_t size)
{
	auto page = makeRef<DX12SingleFrameAllocatorPage>();
	if (!page->init(m_device, size, m_heapType, m_resourceState)) {
		return nullptr;
	}
	return page;
}

//==============================================================================
// DX12SingleFrameAllocator

DX12SingleFrameAllocator::DX12SingleFrameAllocator(DX12SingleFrameAllocatorPageManager* manager)
	: AbstractLinearAllocator(manager)
{
}

DX12SingleFrameBufferView DX12SingleFrameAllocator::allocate(size_t size, size_t alignment)
{
	DX12SingleFrameBufferView info = { nullptr, 0 };

	AbstractLinearAllocatorPage* page;
	size_t offset;
	if (allocateCore(size, alignment, &page, &offset)) {
		auto* page2 = static_cast<DX12SingleFrameAllocatorPage*>(page);
		info.buffer = page2->buffer();
		info.offset = offset;
		return info;
	}
	else {
		return info;
	}
}

} // namespace detail
} // namespace ln

