
#include "Internal.hpp"
#include "LinearAllocator.hpp"

namespace ln {
namespace detail {

//=============================================================================
// LinearAllocatorPageManager

LinearAllocatorPage::LinearAllocatorPage(size_t size)
{
	m_data = LN_OPERATOR_NEW(size);
	m_size = size;
}

LinearAllocatorPage::~LinearAllocatorPage()
{
	LN_OPERATOR_DELETE(m_data);
}

//=============================================================================
// LinearAllocatorPageManager

LinearAllocatorPageManager::LinearAllocatorPageManager()
{
}

LinearAllocatorPageManager::~LinearAllocatorPageManager()
{
	clear();
}

LinearAllocatorPage* LinearAllocatorPageManager::requestPage()
{
	std::lock_guard<std::mutex> lock(m_mutex);

	LinearAllocatorPage* resultPage = nullptr;

	//auto freePage = m_freePages.findIf([&](LinearAllocatorPage* page) { return page->size() >= requerSize; });
	//if (freePage) {
	//	resultPage = *freePage;
	//}

	LinearAllocatorPage* freePage = nullptr;
	if (!m_freePages.empty())
	{
		freePage = m_freePages.front();
		m_freePages.pop_front();
	}

	if (!resultPage) {
		auto page = createNewPage(PageSize);
		m_pagePool.add(page);
		resultPage = page;
	}

	return resultPage;
}

void LinearAllocatorPageManager::discardPage(LinearAllocatorPage* page)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_freePages.push_back(page);
}

void LinearAllocatorPageManager::clear()
{
	m_pagePool.clear();
}

Ref<LinearAllocatorPage> LinearAllocatorPageManager::createNewPage(size_t size)
{
	return makeRef<LinearAllocatorPage>(size);
}

//=============================================================================
// LinearAllocator

LinearAllocator::LinearAllocator(LinearAllocatorPageManager* manager)
	: m_manager(manager)
	, m_usedOffset(0)
	, m_currentPage(nullptr)
{
}

LinearAllocator::~LinearAllocator()
{
}

void* LinearAllocator::allocate(size_t size)
{
	if (size > LinearAllocatorPageManager::PageSize) {
		return allocateLarge(size);
	}

	if (m_usedOffset + size > LinearAllocatorPageManager::PageSize)
	{
		assert(m_currentPage);
		m_retiredPages.add(m_currentPage);
		m_currentPage = nullptr;
	}

	if (!m_currentPage)
	{
		m_currentPage = m_manager->requestPage();
		m_usedOffset = 0;
	}

	byte_t* ptr = static_cast<byte_t*>(m_currentPage->data()) + m_usedOffset;

	m_usedOffset += size;

	return ptr;
}

void LinearAllocator::cleanup()
{
    if (m_currentPage) {
        m_manager->discardPage(m_currentPage);
    }

	for (auto& page : m_retiredPages) {
		m_manager->discardPage(page);
	}

	m_largePages.clear();
}

void* LinearAllocator::allocateLarge(size_t size)
{
	auto page = LinearAllocatorPageManager::createNewPage(size);
	m_largePages.add(page);
	return page->data();
}

} // namespace detail
} // namespace ln

