#pragma once
#include <mutex>
#include <deque>
#include <LuminoCore/Base/List.hpp>
#include <LuminoCore/Base/RefObject.hpp>

namespace ln {
namespace detail {

class AbstractLinearAllocatorPage
	: public RefObject
{
protected:
	AbstractLinearAllocatorPage() = default;
	virtual ~AbstractLinearAllocatorPage() = default;
};


class HeapLinearAllocatorPage
	: public AbstractLinearAllocatorPage
{
public:
	HeapLinearAllocatorPage(size_t size);
	virtual ~HeapLinearAllocatorPage();
	void* data() const { return m_data; }
	size_t size() const { return m_size; }

private:
	void* m_data;
	size_t m_size;
};



class AbstractLinearAllocatorPageManager
	: public RefObject
{
public:
	static constexpr size_t DefaultPageSize = 0x200000;	// 2MB

	AbstractLinearAllocatorPageManager(size_t pageSize = 0);
	virtual ~AbstractLinearAllocatorPageManager();
	void clear();

	size_t pageSize() const { return m_pageSize; }

	AbstractLinearAllocatorPage* requestPage();
	void discardPage(AbstractLinearAllocatorPage* page);
	Ref<AbstractLinearAllocatorPage> createNewPage(size_t size);

protected:
	virtual Ref<AbstractLinearAllocatorPage> onCreateNewPage(size_t size) = 0;

private:

	size_t m_pageSize;
	std::mutex m_mutex;
	List<Ref<AbstractLinearAllocatorPage>> m_pagePool;		// page instances
	std::deque<AbstractLinearAllocatorPage*> m_freePages;	// page references
};

class LinearAllocatorPageManager
	: public AbstractLinearAllocatorPageManager
{
public:
	LinearAllocatorPageManager(size_t pageSize = 0);

protected:
	Ref<AbstractLinearAllocatorPage> onCreateNewPage(size_t size) override;
};

//using LinearAllocatorPageManager = TLinearAllocatorPageManager<LinearAllocatorPage>;





/*
	ページベースのシンプルなアロケータ。

	任意の数 allocate() を呼び出してメモリ確保する。
	使い終わったら cleanup() で、確保したメモリをすべて一度に開放する。

	ページのサイズは LinearAllocator::PageSize。
	これを超えるメモリを確保しようとした場合は LinearAllocator::m_largePages にバッファの実態を持つ。

	通常のサイズのページはキャッシュ管理され、頻繁にメモリ確保は行われない。
	対し、LargePage は cleanup() 時点で解放される。
*/
class AbstractLinearAllocator
	: public RefObject
{
public:
	AbstractLinearAllocator(LinearAllocatorPageManager* manager);
	virtual ~AbstractLinearAllocator();

	void cleanup();

	size_t maxAllocatedLargePageSize() const { return m_maxAllocatedLargePageSize; }

protected:
	bool allocateCore(size_t size, size_t alignment, AbstractLinearAllocatorPage** outCurrentPage, size_t* outOffset);
	AbstractLinearAllocatorPage* currentPage() const { return m_currentPage; }
	const List<AbstractLinearAllocatorPage*>& retiredPages() const { return m_retiredPages; }
	const List<Ref<AbstractLinearAllocatorPage>>& largePages() const { return m_largePages; }

private:
	AbstractLinearAllocatorPage* allocateLarge(size_t size);

	LinearAllocatorPageManager* m_manager;
	size_t m_usedOffset;
	AbstractLinearAllocatorPage* m_currentPage;

	List<AbstractLinearAllocatorPage*> m_retiredPages;
	List<Ref<AbstractLinearAllocatorPage>> m_largePages;
	size_t m_maxAllocatedLargePageSize;
};

class LinearAllocator
	: public AbstractLinearAllocator
{
public:
	LinearAllocator(LinearAllocatorPageManager* manager);
	void* allocate(size_t size, size_t alignment = 64);

protected:
};

} // namespace detail
} // namespace ln

