
#include "Internal.hpp"
#include <LuminoEngine/Graphics/ConstantBuffer.hpp>
#include "SingleFrameAllocator.hpp"

namespace ln {
namespace detail {
	
//==============================================================================
// SingleFrameUniformBufferAllocatorPage

bool SingleFrameUniformBufferAllocatorPage::init(uint32_t size)
{
	m_buffer = makeObject<ConstantBuffer>(size);
	if (!m_buffer) {
		return false;
	}
	return true;
}

SingleFrameUniformBufferAllocatorPage::~SingleFrameUniformBufferAllocatorPage()
{
	if (m_buffer) {
		//m_buffer->dispose();
		m_buffer = nullptr;
	}
}

//==============================================================================
// SingleFrameUniformBufferAllocatorPage

SingleFrameUniformBufferAllocatorPageManager::SingleFrameUniformBufferAllocatorPageManager(size_t pageSize)
	: LinearAllocatorPageManager(pageSize)
{
}

Ref<AbstractLinearAllocatorPage> SingleFrameUniformBufferAllocatorPageManager::onCreateNewPage(size_t size)
{
	auto page = makeRef<SingleFrameUniformBufferAllocatorPage>();
	if (!page->init(size)) {
		return nullptr;
	}
	return page;
}

//==============================================================================
// SingleFrameUniformBufferAllocator

SingleFrameUniformBufferAllocator::SingleFrameUniformBufferAllocator(SingleFrameUniformBufferAllocatorPageManager* manager)
	: AbstractLinearAllocator(manager)
{
}

ConstantBufferView SingleFrameUniformBufferAllocator::allocate(size_t size, size_t alignment)
{
	ConstantBufferView info = { nullptr, 0 };

	AbstractLinearAllocatorPage* page;
	size_t offset;
	if (allocateCore(size, alignment, &page, &offset)) {
		auto* page2 = static_cast<SingleFrameUniformBufferAllocatorPage*>(page);
		info.buffer = page2->buffer();
		info.offset = offset;
		return info;
	}
	else {
		return info;
	}
}

void SingleFrameUniformBufferAllocator::unmap()
{
	if (auto* page = currentPage()) {
		auto* page2 = static_cast<SingleFrameUniformBufferAllocatorPage*>(page);
		page2->buffer()->unmap();
	}
	for (auto& page : retiredPages()) {
		auto* page2 = static_cast<SingleFrameUniformBufferAllocatorPage*>(page);
		page2->buffer()->unmap();
	}
	for (auto& page : largePages()) {
		auto* page2 = static_cast<SingleFrameUniformBufferAllocatorPage*>(page.get());
		page2->buffer()->unmap();
	}
}

} // namespace detail
} // namespace ln
