#include <LuminoCore/graphics/DynamicUniformAllocator.hpp>
#include <cstring>

namespace ln {

DynamicUniformAllocator::~DynamicUniformAllocator() {
    // Unmap all persistently-mapped pages.
    for (auto& frame : m_frames) {
        for (auto& page : frame.pages) {
            if (page.mappedBase) {
                page.buffer->unmap();
                page.mappedBase = nullptr;
            }
        }
    }
}

Result<std::unique_ptr<DynamicUniformAllocator>> DynamicUniformAllocator::create(
    rhi::Device* device,
    rhi::BindGroupLayout* layout,
    u32 binding,
    u32 elementSize,
    u32 framesInFlight) {

    auto alloc = std::unique_ptr<DynamicUniformAllocator>(new DynamicUniformAllocator());
    alloc->m_device = device;
    alloc->m_layout = layout;
    alloc->m_binding = binding;
    alloc->m_elementSize = elementSize;
    alloc->m_framesInFlight = framesInFlight;

    // Query alignment from device.
    auto limits = device->deviceLimits();
    u32 alignment = limits.minUniformBufferOffsetAlignment;
    alloc->m_alignedElementSize = ((elementSize + alignment - 1) / alignment) * alignment;
    alloc->m_maxElementsPerPage = limits.maxUniformBufferRange / alloc->m_alignedElementSize;
    alloc->m_pageByteSize = alloc->m_maxElementsPerPage * alloc->m_alignedElementSize;

    // Pre-create one page per frame slot.
    for (u32 f = 0; f < framesInFlight; ++f) {
        auto pageResult = alloc->createPage();
        if (!pageResult) return tl::make_unexpected(pageResult.error());
        alloc->m_frames[f].pages.push_back(std::move(*pageResult));
    }

    return alloc;
}

void DynamicUniformAllocator::beginFrame(u32 frameIndex) {
    m_currentFrameSlot = frameIndex % m_framesInFlight;
    auto& frame = m_frames[m_currentFrameSlot];
    // Reset all pages for reuse.
    for (auto& page : frame.pages) {
        page.usedElements = 0;
    }
    frame.currentPage = 0;
}

DynamicUniformAllocation DynamicUniformAllocator::allocate() {
    auto& frame = m_frames[m_currentFrameSlot];

    // Advance to next page if current is full.
    while (frame.currentPage < frame.pages.size() &&
           frame.pages[frame.currentPage].usedElements >= m_maxElementsPerPage) {
        ++frame.currentPage;
    }

    // Create a new page if needed.
    if (frame.currentPage >= frame.pages.size()) {
        auto pageResult = createPage();
        // On failure, return a null allocation (caller should check cpuPtr).
        if (!pageResult) {
            return {nullptr, nullptr, 0};
        }
        frame.pages.push_back(std::move(*pageResult));
    }

    auto& page = frame.pages[frame.currentPage];
    u32 offset = page.usedElements * m_alignedElementSize;
    void* ptr = static_cast<u8*>(page.mappedBase) + offset;
    ++page.usedElements;

    return {ptr, page.bindGroup.get(), offset};
}

Result<DynamicUniformAllocator::Page> DynamicUniformAllocator::createPage() {
    Page page;

    // Create a large uniform buffer.
    rhi::BufferDesc bufDesc;
    bufDesc.size = m_pageByteSize;
    bufDesc.usage = rhi::BufferUsage::Uniform;
    auto bufResult = m_device->createBuffer(bufDesc);
    if (!bufResult) return tl::make_unexpected(bufResult.error());
    page.buffer = std::move(*bufResult);

    // Persistently map.
    page.mappedBase = page.buffer->map();
    if (!page.mappedBase) {
        return tl::make_unexpected(Error{ErrorCode::RuntimeError, "Failed to map dynamic UBO page"});
    }

    // Create a BindGroup for this page.
    // The descriptor range is m_alignedElementSize (one element); actual offset is dynamic.
    rhi::BindGroupDesc bgDesc;
    bgDesc.layout = m_layout;
    bgDesc.entries = {
        {m_binding, page.buffer.get(), 0, m_alignedElementSize, nullptr, nullptr},
    };
    auto bgResult = m_device->createBindGroup(bgDesc);
    if (!bgResult) return tl::make_unexpected(bgResult.error());
    page.bindGroup = std::move(*bgResult);

    return page;
}

} // namespace ln
