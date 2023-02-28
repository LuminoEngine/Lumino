#include "Internal.hpp"
#include <LuminoGraphicsRHI/RHIHelper.hpp>
#include <LuminoGraphics/GPU/VertexBuffer.hpp>
#include <LuminoGraphics/GPU/IndexBuffer.hpp>s
#include "StreamingBufferAllocator.hpp"

namespace ln {
namespace detail {

size_t AlignUpWithMask(size_t value, size_t mask);
size_t AlignUp(size_t value, size_t alignment);

//==============================================================================
// StreamingBufferPage

StreamingBufferPage::StreamingBufferPage()
    : m_resource() {
}

Result StreamingBufferPage::init(Type type, size_t elementSize, size_t count) {
    switch (type) {
        case ln::detail::StreamingBufferPage::Type::VertexBuffer:
            m_resource = makeObject_deprecated<VertexBuffer>(elementSize * count, GraphicsResourceUsage::Dynamic);
            break;
        case ln::detail::StreamingBufferPage::Type::IndexBuffer:
            m_resource = makeObject_deprecated<IndexBuffer>(count, RHIHelper::getIndexBufferFormat(elementSize), GraphicsResourceUsage::Dynamic);
            break;
        default:
            LN_UNREACHABLE();
            break;
    }
    return ok();
}

//==============================================================================
// StreamingBufferAllocatorManager

StreamingBufferAllocatorManager::StreamingBufferAllocatorManager(StreamingBufferPage::Type type, size_t elementSize)
    : m_type(type)
    , m_pageElementCount(50000)
    , m_elementSize(elementSize) {
}

StreamingBufferPage* StreamingBufferAllocatorManager::requestPage() {
    StreamingBufferPage* resultPage = nullptr;

    if (!m_freePages.empty()) {
        resultPage = m_freePages.front();
        m_freePages.pop_front();
    }

    if (!resultPage) {
        auto page = createNewPage();
        resultPage = page;
        m_pagePool.push(std::move(page));
    }

    return resultPage;
}

void StreamingBufferAllocatorManager::discardPage(StreamingBufferPage* page) {
    m_freePages.push_back(page);
}

URef<StreamingBufferPage> StreamingBufferAllocatorManager::createNewPage() {
    auto page = makeURef<StreamingBufferPage>();
    if (!page->init(m_type, m_elementSize, m_pageElementCount)) {
        return nullptr;
    }
    return page;
}

//==============================================================================
// StreamingBufferAllocator

StreamingBufferAllocator::StreamingBufferAllocator(StreamingBufferAllocatorManager* manager)
    : m_manager(manager)
    , m_currentPage(0)
    , m_retiredPages() {
}

void StreamingBufferAllocator::cleanup() {
    if (m_currentPage) {
        m_manager->discardPage(m_currentPage);
        m_currentPage = nullptr;
    }

    for (auto& page : m_retiredPages) {
        m_manager->discardPage(page);
    }
    m_retiredPages.clear();

    m_usedCount = 0;
}

StreamingBufferAllocator::View StreamingBufferAllocator::allocate(size_t elementCount) {
    if (elementCount > m_manager->pageElementCount()) {
        LN_ERROR();
        return {};
    }

    const size_t pageElementCount = m_manager->pageElementCount();

    if (m_usedCount >= pageElementCount) {
        m_retiredPages.push(m_currentPage);
        m_currentPage = nullptr;
    }

    if (!m_currentPage) {
        m_currentPage = m_manager->requestPage();
        m_usedCount = 0;
    }

    if (LN_REQUIRE(m_currentPage)) return {};

    size_t actualCount = std::min(elementCount, pageElementCount - m_usedCount);

    View view = {
        m_currentPage->resource(),
        m_usedCount,
        actualCount,
    };

    m_usedCount += elementCount;

    return view;
}

} // namespace detail
} // namespace ln
