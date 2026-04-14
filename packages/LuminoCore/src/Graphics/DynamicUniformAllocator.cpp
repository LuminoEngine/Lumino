#include <LuminoCore/Graphics/DynamicUniformAllocator.hpp>
#include <cstring>

namespace ln {

Result<std::unique_ptr<DynamicUniformAllocator>> DynamicUniformAllocator::create(
    rhi::Device* device,
    rhi::PipelineLayout* pipelineLayout,
    uint32_t setIndex,
    uint32_t binding,
    uint32_t elementSize,
    uint32_t framesInFlight) {

    auto alloc = std::unique_ptr<DynamicUniformAllocator>(new DynamicUniformAllocator());
    alloc->m_device = device;
    alloc->m_pipelineLayout = pipelineLayout;
    alloc->m_setIndex = setIndex;
    alloc->m_binding = binding;
    alloc->m_elementSize = elementSize;
    alloc->m_framesInFlight = framesInFlight;

    // Query alignment from device.
    auto limits = device->deviceLimits();
    uint32_t alignment = limits.minUniformBufferOffsetAlignment;
    alloc->m_alignedElementSize = ((elementSize + alignment - 1) / alignment) * alignment;
    alloc->m_maxElementsPerPage = limits.maxUniformBufferRange / alloc->m_alignedElementSize;
    alloc->m_pageByteSize = alloc->m_maxElementsPerPage * alloc->m_alignedElementSize;

    // Resize frame data to match the requested frames-in-flight count.
    alloc->m_frames.resize(framesInFlight);

    // Pre-create one page per frame slot.
    for (uint32_t f = 0; f < framesInFlight; ++f) {
        auto pageResult = alloc->createPage();
        if (!pageResult) return LN_FORWARD_ERROR(pageResult);
        alloc->m_frames[f].pages.push_back(std::move(*pageResult));
    }

    return alloc;
}

void DynamicUniformAllocator::beginFrame(uint32_t frameIndex) {
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
    uint32_t offset = page.usedElements * m_alignedElementSize;
    void* ptr = page.cpuShadow.data() + offset;
    ++page.usedElements;

    return {ptr, page.bindGroup.get(), offset};
}

VoidResult DynamicUniformAllocator::flushFrame() {
    auto& frame = m_frames[m_currentFrameSlot];
    for (auto& page : frame.pages) {
        if (page.usedElements > 0) {
            uint64_t usedBytes = static_cast<uint64_t>(page.usedElements) * m_alignedElementSize;
            auto result = m_device->writeBuffer(page.buffer.get(), 0, page.cpuShadow.data(), usedBytes);
            if (!result) return result;
        }
    }
    return LN_MAKE_SUCCESS();
}

Result<DynamicUniformAllocator::Page> DynamicUniformAllocator::createPage() {
    Page page;

    // Create a large uniform buffer.
    rhi::BufferDesc bufDesc;
    bufDesc.size = m_pageByteSize;
    bufDesc.usage = rhi::BufferUsage::Uniform;
    auto bufResult = m_device->createBuffer(bufDesc);
    if (!bufResult) return LN_FORWARD_ERROR(bufResult);
    page.buffer = std::move(*bufResult);

    // Allocate CPU shadow buffer.
    page.cpuShadow.resize(m_pageByteSize);

    // Create a BindGroup for this page via PipelineLayout.
    // The descriptor range is m_alignedElementSize (one element); actual offset is dynamic.
    std::vector<rhi::BindGroupEntry> entries = {
        {m_binding, page.buffer.get(), 0, m_alignedElementSize, nullptr, nullptr},
    };
    auto bgResult = m_pipelineLayout->createBindGroup(m_setIndex, entries);
    if (!bgResult) return LN_FORWARD_ERROR(bgResult);
    page.bindGroup = std::move(*bgResult);

    return page;
}

} // namespace ln
