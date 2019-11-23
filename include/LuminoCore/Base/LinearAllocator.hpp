#pragma once
#include <mutex>
#include <deque>
#include <LuminoCore/Base/List.hpp>
#include <LuminoCore/Base/RefObject.hpp>

namespace ln {
namespace detail {

class LinearAllocatorPage
	: public RefObject
{
public:
	LinearAllocatorPage(size_t size);
	virtual ~LinearAllocatorPage();
	void* data() const { return m_data; }
	size_t size() const { return m_size; }

private:
	void* m_data;
	size_t m_size;
};

class LinearAllocatorPageManager
	: public RefObject
{
public:
	static const size_t DefaultPageSize = 0x200000;	// 2MB

	LinearAllocatorPageManager(size_t pageSize = 0);
	virtual ~LinearAllocatorPageManager();

	size_t pageSize() const { return m_pageSize; }

	LinearAllocatorPage* requestPage();
	void discardPage(LinearAllocatorPage* page);

	static Ref<LinearAllocatorPage> createNewPage(size_t size);

private:
	void clear();

	size_t m_pageSize;
	std::mutex m_mutex;
	List<Ref<LinearAllocatorPage>> m_pagePool;		// page instances
	std::deque<LinearAllocatorPage*> m_freePages;	// page references
};

/*
	ページベースのシンプルなアロケータ。

	任意の数 allocate() を呼び出してメモリ確保する。
	使い終わったら cleanup() で、確保したメモリをすべて一度に開放する。

	ページのサイズは LinearAllocator::PageSize。
	これを超えるメモリを確保しようとした場合は LinearAllocator::m_largePages にバッファの実態を持つ。

	通常のサイズのページはキャッシュ管理され、頻繁にメモリ確保は行われない。
	対し、LargePage は cleanup() 時点で解放される。
*/
class LinearAllocator
	: public RefObject
{
public:
	LinearAllocator(LinearAllocatorPageManager* manager);
	virtual ~LinearAllocator();

	void* allocate(size_t size, size_t alignment = 64);
	void cleanup();

	size_t maxAllocatedLargePageSize() const { return m_maxAllocatedLargePageSize; }

private:
	void* allocateLarge(size_t size);

	LinearAllocatorPageManager* m_manager;
	size_t m_usedOffset;
	LinearAllocatorPage* m_currentPage;

	List<LinearAllocatorPage*> m_retiredPages;
	List<Ref<LinearAllocatorPage>> m_largePages;
	size_t m_maxAllocatedLargePageSize;
};

} // namespace detail
} // namespace ln

