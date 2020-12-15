
#include "Internal.hpp"
#include "SingleFrameAllocator.hpp"

namespace ln {
namespace detail {
	
//==============================================================================
// SingleFrameUniformBufferAllocatorPage

bool SingleFrameUniformBufferAllocatorPage::init(IGraphicsDevice* device, uint32_t size)
{
	m_buffer = device->createUniformBuffer(size);
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

SingleFrameUniformBufferAllocatorPageManager::SingleFrameUniformBufferAllocatorPageManager(IGraphicsDevice* device, size_t pageSize)
	: LinearAllocatorPageManager(pageSize)
	, m_device(device)
{
}

Ref<AbstractLinearAllocatorPage> SingleFrameUniformBufferAllocatorPageManager::onCreateNewPage(size_t size)
{
	auto page = makeRef<SingleFrameUniformBufferAllocatorPage>();
	if (!page->init(m_device, size)) {
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

UniformBufferView SingleFrameUniformBufferAllocator::allocate(size_t size, size_t alignment)
{
	UniformBufferView info = { nullptr, 0 };

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

} // namespace detail
} // namespace ln
