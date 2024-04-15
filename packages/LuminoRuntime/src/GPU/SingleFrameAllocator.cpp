
#include "Internal.hpp"
#include <LuminoEngine/GPU/ConstantBuffer.hpp>
#include "SingleFrameAllocator.hpp"

namespace ln {
namespace detail {

//==============================================================================
// SingleFrameUniformBufferAllocatorPage

bool SingleFrameUniformBufferAllocatorPage::init(GraphicsContext* context, uint32_t size) {
    m_buffer = Ref<ConstantBuffer>(LN_NEW ConstantBuffer(), false);
    if (!m_buffer->init(context, size)) {
        return false;
    }

    //	makeObject_deprecated<ConstantBuffer>();
    // if (!m_buffer) {
    //	return false;
    //}
    return true;
}

SingleFrameUniformBufferAllocatorPage::~SingleFrameUniformBufferAllocatorPage() {
    if (m_buffer) {
        // m_buffer->dispose();
        m_buffer = nullptr;
    }
}

//==============================================================================
// SingleFrameUniformBufferAllocatorPage

SingleFrameUniformBufferAllocatorPageManager::SingleFrameUniformBufferAllocatorPageManager(
    GraphicsContext* context,
    size_t pageSize)
    : LinearAllocatorPageManager(pageSize)
    , m_context(context) {
}

Ref<AbstractLinearAllocatorPage> SingleFrameUniformBufferAllocatorPageManager::onCreateNewPage(size_t size) {
    auto page = makeRef<SingleFrameUniformBufferAllocatorPage>();
    if (!page->init(m_context, size)) {
        return nullptr;
    }
    return page;
}

//==============================================================================
// SingleFrameUniformBufferAllocator

SingleFrameUniformBufferAllocator::SingleFrameUniformBufferAllocator(SingleFrameUniformBufferAllocatorPageManager* manager)
    : AbstractLinearAllocator(manager) {
}

ConstantBufferView SingleFrameUniformBufferAllocator::allocate(size_t size, size_t alignment) {
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

void SingleFrameUniformBufferAllocator::unmap() {
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
