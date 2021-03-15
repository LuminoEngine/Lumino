
#include "Internal.hpp"
#include <LuminoCore/Base/LinearAllocator.hpp>

namespace ln {
namespace detail {

size_t AlignUpWithMask(size_t value, size_t mask)
{
	return ((size_t)value + mask) & ~mask;
}

size_t AlignUp(size_t value, size_t alignment)
{
	return AlignUpWithMask(value, alignment - 1);
}

//=============================================================================
// HeapLinearAllocatorPage

HeapLinearAllocatorPage::HeapLinearAllocatorPage(size_t size)
{
	m_data = LN_OPERATOR_NEW(size);
	m_size = size;
}

HeapLinearAllocatorPage::~HeapLinearAllocatorPage()
{
	LN_OPERATOR_DELETE(m_data);
}

//=============================================================================
// AbstractLinearAllocatorPageManager

AbstractLinearAllocatorPageManager::AbstractLinearAllocatorPageManager(size_t pageSize)
	: m_pageSize((pageSize == 0) ? DefaultPageSize : pageSize)
	, m_mutex()
	, m_pagePool()
	, m_freePages()
{
}

AbstractLinearAllocatorPageManager::~AbstractLinearAllocatorPageManager()
{
	clear();
}

AbstractLinearAllocatorPage* AbstractLinearAllocatorPageManager::requestPage()
{
	std::lock_guard<std::mutex> lock(m_mutex);

	AbstractLinearAllocatorPage* resultPage = nullptr;

	//auto freePage = m_freePages.findIf([&](LinearAllocatorPage* page) { return page->size() >= requerSize; });
	//if (freePage) {
	//	resultPage = *freePage;
	//}

	if (!m_freePages.empty())
	{
        resultPage = m_freePages.front();
		m_freePages.pop_front();
	}

	if (!resultPage) {
		auto page = createNewPage(pageSize());
		m_pagePool.add(page);
		resultPage = page;
	}

	return resultPage;
}

void AbstractLinearAllocatorPageManager::discardPage(AbstractLinearAllocatorPage* page)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_freePages.push_back(page);
}

Ref<AbstractLinearAllocatorPage> AbstractLinearAllocatorPageManager::createNewPage(size_t size)
{
	return onCreateNewPage(size);
}

void AbstractLinearAllocatorPageManager::clear()
{
	m_pagePool.clear();
}

//=============================================================================
// LinearAllocatorPageManager

LinearAllocatorPageManager::LinearAllocatorPageManager(size_t pageSize)
	: AbstractLinearAllocatorPageManager(pageSize)
{
}

Ref<AbstractLinearAllocatorPage> LinearAllocatorPageManager::onCreateNewPage(size_t size)
{
	return makeRef<HeapLinearAllocatorPage>(size);
}

//=============================================================================
// AbstractLinearAllocator

AbstractLinearAllocator::AbstractLinearAllocator(LinearAllocatorPageManager* manager)
	: m_manager(manager)
	, m_usedOffset(0)
	, m_currentPage(nullptr)
	, m_maxAllocatedLargePageSize(0)
{
}

AbstractLinearAllocator::~AbstractLinearAllocator()
{
}

bool AbstractLinearAllocator::allocateCore(size_t size, size_t alignment, AbstractLinearAllocatorPage** outCurrentPage, size_t* outOffset)
{
	const size_t alignmentMask = alignment - 1;
	const size_t alignedSize = AlignUpWithMask(size, alignmentMask);


	if (alignedSize > m_manager->pageSize()) {
		*outCurrentPage = allocateLarge(alignedSize);
		*outOffset = 0;
		return true;
	}

	m_usedOffset = AlignUp(m_usedOffset, alignment);

	if (m_usedOffset + alignedSize > m_manager->pageSize())
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

	if (LN_REQUIRE(m_currentPage)) return false;

	//byte_t* ptr = static_cast<byte_t*>(m_currentPage->data()) + m_usedOffset;
	*outCurrentPage = m_currentPage;
	*outOffset = m_usedOffset;

	m_usedOffset += alignedSize;

	return true;
}

void AbstractLinearAllocator::cleanup()
{
    if (m_currentPage) {
        m_manager->discardPage(m_currentPage);
        m_currentPage = nullptr;
    }

	for (auto& page : m_retiredPages) {
		m_manager->discardPage(page);
	}
    m_retiredPages.clear();

	m_largePages.clear();

    m_usedOffset = 0;
	m_maxAllocatedLargePageSize = 0;
}

AbstractLinearAllocatorPage* AbstractLinearAllocator::allocateLarge(size_t size)
{
	m_maxAllocatedLargePageSize = std::max(m_maxAllocatedLargePageSize, size);
	auto page = m_manager->createNewPage(size);
	m_largePages.add(page);
	return page;
}

//=============================================================================
// LinearAllocator

LinearAllocator::LinearAllocator(LinearAllocatorPageManager* manager)
	: AbstractLinearAllocator(manager)
{
}

void* LinearAllocator::allocate(size_t size, size_t alignment)
{
	AbstractLinearAllocatorPage* page;
	size_t offset;
	if (allocateCore(size, alignment, &page, &offset)) {
		return static_cast<uint8_t*>(static_cast<HeapLinearAllocatorPage*>(page)->data()) + offset;
	}
	else {
		return nullptr;
	}
}

} // namespace detail
} // namespace ln

