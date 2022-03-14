#include "Internal.hpp"
#include <LuminoEngine/Base/ObjectPool.hpp>

namespace ln {

//=============================================================================
// AbstractObjectPoolManager

AbstractObjectPoolManager::AbstractObjectPoolManager(int32_t pageSize)
    : m_pageSize(pageSize) {
}

AbstractObjectPoolPage* AbstractObjectPoolManager::requestPage() {
    AbstractObjectPoolPage* resultPage = nullptr;

    if (!m_freePages.empty()) {
        resultPage = m_freePages.front();
        m_freePages.pop_front();
    }

    if (!resultPage) {
        auto page = createNewPage(pageSize());
        m_pagePool.push(page);
        resultPage = page;
    }

    return resultPage;
}

void AbstractObjectPoolManager::discardPage(AbstractObjectPoolPage* page) {
    m_freePages.push_back(page);
}

Ref<AbstractObjectPoolPage> AbstractObjectPoolManager::createNewPage(size_t size) {
    auto page = onCreateNewPage();
    page->onCreateObjects(size, &page->m_objects);
    return page;
}

//=============================================================================
// ObjectPoolManager

ObjectPoolManager::ObjectPoolManager(int32_t pageSize)
    : AbstractObjectPoolManager(pageSize) {
}

Ref<AbstractObjectPoolPage> ObjectPoolManager::onCreateNewPage() {
    return makeRef<ObjectPoolPage>(this);
}

//=============================================================================
// ObjectPool
    
ObjectPool::ObjectPool(AbstractObjectPoolManager* manager)
    : m_manager(manager)
    , m_usedCount(0)
    , m_currentPage(nullptr)
    , m_retiredPages() {
    LN_CHECK(manager->pageSize() > 0);
}

RefObject* ObjectPool::allocate() {
    if (m_usedCount + 1 > m_manager->pageSize()) {
        m_retiredPages.add(m_currentPage);
        m_currentPage = nullptr;
    }

    if (!m_currentPage) {
        m_currentPage = m_manager->requestPage();
        m_usedCount = 0;
    }

    RefObject* obj = m_currentPage->get(m_usedCount);
    m_usedCount++;
    return obj;
}

} // namespace ln
