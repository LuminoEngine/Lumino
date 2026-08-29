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

    // デバイスからアライメントを問い合わせる。
    auto limits = device->deviceLimits();
    uint32_t alignment = limits.minUniformBufferOffsetAlignment;
    alloc->m_alignedElementSize = ((elementSize + alignment - 1) / alignment) * alignment;
    alloc->m_maxElementsPerPage = limits.maxUniformBufferRange / alloc->m_alignedElementSize;
    alloc->m_pageByteSize = alloc->m_maxElementsPerPage * alloc->m_alignedElementSize;

    // 要求されたインフライトフレーム数に合わせてフレームデータをリサイズする。
    alloc->m_frames.resize(framesInFlight);

    // フレームスロットごとにページを 1 つ事前に作成する。
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
    // 再利用のため全ページをリセットする。
    for (auto& page : frame.pages) {
        page.usedElements = 0;
    }
    frame.currentPage = 0;
}

DynamicUniformAllocation DynamicUniformAllocator::allocate() {
    auto& frame = m_frames[m_currentFrameSlot];

    // 現在のページが満杯なら次のページへ進む。
    while (frame.currentPage < frame.pages.size() &&
           frame.pages[frame.currentPage].usedElements >= m_maxElementsPerPage) {
        ++frame.currentPage;
    }

    // 必要なら新しいページを作成する。
    if (frame.currentPage >= frame.pages.size()) {
        auto pageResult = createPage();
        // 失敗時は null のアロケーションを返す (呼び出し側は cpuPtr を確認すること)。
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

    // 大きなユニフォームバッファを作成する。
    rhi::BufferDesc bufDesc;
    bufDesc.size = m_pageByteSize;
    bufDesc.usage = rhi::BufferUsage::Uniform;
    auto bufResult = m_device->createBuffer(bufDesc);
    if (!bufResult) return LN_FORWARD_ERROR(bufResult);
    page.buffer = std::move(*bufResult);

    // CPU 側のシャドウバッファを確保する。
    page.cpuShadow.resize(m_pageByteSize);

    // PipelineLayout 経由でこのページ用の BindGroup を作成する。
    // ディスクリプタの範囲は m_alignedElementSize (要素 1 つ分)。実際のオフセットは動的に指定する。
    const rhi::BindGroupEntry entry{m_binding, page.buffer.get(), 0, m_alignedElementSize, nullptr, nullptr};
    auto bgResult = m_pipelineLayout->createBindGroup(m_setIndex, &entry, 1);
    if (!bgResult) return LN_FORWARD_ERROR(bgResult);
    page.bindGroup = std::move(*bgResult);

    return page;
}

} // namespace ln
