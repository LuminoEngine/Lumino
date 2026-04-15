#pragma once

#include <memory>
#include <vector>
#include <LuminoBase/Types.hpp>
#include <LuminoBase/Result.hpp>
#include <LuminoBase/RefObject.hpp>
#include <LuminoCore/Graphics/rhi/Rhi.hpp>

namespace ln {

/** Result of a dynamic uniform buffer sub-allocation. */
struct DynamicUniformAllocation {
    void* cpuPtr;                ///< CPU-writable pointer into shadow buffer.
    rhi::BindGroup* bindGroup;   ///< BindGroup to bind (one per page).
    uint32_t dynamicOffset;           ///< Dynamic offset to pass to setBindGroup.
};

/**
 * Per-frame linear allocator for dynamic uniform buffers.
 *
 * Manages pages of large uniform buffers. Each frame, object UBO data is
 * sub-allocated from a linear allocator. A single BindGroup per page
 * is bound repeatedly with different dynamic offsets.
 *
 * Writes are collected in a CPU shadow buffer and flushed to the GPU
 * via Device::writeBuffer() before submission.
 *
 * Double-buffered: each frame index has its own set of pages so that
 * frame N can write while frame N-1 is still in flight on the GPU.
 */
class DynamicUniformAllocator {
public:
    ~DynamicUniformAllocator() = default;

    /**
     * Create a new allocator.
     * @param device          RHI device.
     * @param pipelineLayout  PipelineLayout that owns the BindGroupLayout for the target set.
     * @param setIndex        Descriptor set index within the PipelineLayout.
     * @param binding         Binding index of the UBO within the layout.
     * @param elementSize     Size of one element in bytes (e.g. sizeof(ObjectParamsUBO)).
     * @param framesInFlight  Number of frames in flight (default 2 for double-buffering).
     */
    static Result<std::unique_ptr<DynamicUniformAllocator>> create(
        rhi::Device* device,
        rhi::PipelineLayout* pipelineLayout,
        uint32_t setIndex,
        uint32_t binding,
        uint32_t elementSize,
        uint32_t framesInFlight);

    /** Called at the start of each frame to reset the allocator for reuse. */
    void beginFrame(uint32_t frameIndex);

    /** Sub-allocate one element. Returns CPU pointer + bind info. */
    DynamicUniformAllocation allocate();

    /** Flush all written data in the current frame to the GPU. Call before submit. */
    VoidResult flushFrame();

private:
    DynamicUniformAllocator() = default;

    struct Page {
        Ref<rhi::Buffer> buffer;
        Ref<rhi::BindGroup> bindGroup;
        std::vector<uint8_t> cpuShadow;
        uint32_t usedElements = 0;
    };

    struct FrameData {
        std::vector<Page> pages;
        uint32_t currentPage = 0;
    };

    rhi::Device* m_device = nullptr;
    rhi::PipelineLayout* m_pipelineLayout = nullptr;
    uint32_t m_setIndex = 0;
    uint32_t m_binding = 0;
    uint32_t m_elementSize = 0;
    uint32_t m_alignedElementSize = 0;
    uint32_t m_maxElementsPerPage = 0;
    uint32_t m_pageByteSize = 0;
    uint32_t m_framesInFlight = 2;

    std::vector<FrameData> m_frames;
    uint32_t m_currentFrameSlot = 0;

    Result<Page> createPage();
};

} // namespace ln
