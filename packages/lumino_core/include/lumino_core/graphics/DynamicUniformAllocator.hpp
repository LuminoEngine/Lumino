#pragma once

#include <memory>
#include <vector>
#include <array>
#include <lumino_base/Types.hpp>
#include <lumino_base/Result.hpp>
#include <lumino_base/RefObject.hpp>
#include <lumino_core/graphics/rhi/Rhi.hpp>

namespace ln {

/** Result of a dynamic uniform buffer sub-allocation. */
struct DynamicUniformAllocation {
    void* cpuPtr;                ///< CPU-writable pointer (persistently mapped).
    rhi::BindGroup* bindGroup;   ///< BindGroup to bind (one per page).
    u32 dynamicOffset;           ///< Dynamic offset to pass to setBindGroup.
};

/**
 * Per-frame linear allocator for dynamic uniform buffers.
 *
 * Manages pages of large uniform buffers. Each frame, object UBO data is
 * sub-allocated from a linear allocator. A single VkDescriptorSet per page
 * is bound repeatedly with different dynamic offsets.
 *
 * Double-buffered: each frame index has its own set of pages so that
 * frame N can write while frame N-1 is still in flight on the GPU.
 */
class DynamicUniformAllocator {
public:
    ~DynamicUniformAllocator();

    /**
     * Create a new allocator.
     * @param device        RHI device.
     * @param layout        BindGroupLayout with hasDynamicOffset=true on the UBO binding.
     * @param binding       Binding index of the UBO within the layout.
     * @param elementSize   Size of one element in bytes (e.g. sizeof(ObjectParamsUBO)).
     * @param framesInFlight Number of frames in flight (default 2 for double-buffering).
     */
    static Result<std::unique_ptr<DynamicUniformAllocator>> create(
        rhi::Device* device,
        rhi::BindGroupLayout* layout,
        u32 binding,
        u32 elementSize,
        u32 framesInFlight = 2);

    /** Called at the start of each frame to reset the allocator for reuse. */
    void beginFrame(u32 frameIndex);

    /** Sub-allocate one element. Returns CPU pointer + bind info. */
    DynamicUniformAllocation allocate();

private:
    DynamicUniformAllocator() = default;

    struct Page {
        Ref<rhi::Buffer> buffer;
        Ref<rhi::BindGroup> bindGroup;
        void* mappedBase = nullptr;
        u32 usedElements = 0;
    };

    static constexpr u32 MAX_FRAMES = 2;

    struct FrameData {
        std::vector<Page> pages;
        u32 currentPage = 0;
    };

    rhi::Device* m_device = nullptr;
    rhi::BindGroupLayout* m_layout = nullptr;
    u32 m_binding = 0;
    u32 m_elementSize = 0;
    u32 m_alignedElementSize = 0;
    u32 m_maxElementsPerPage = 0;
    u32 m_pageByteSize = 0;
    u32 m_framesInFlight = 2;

    std::array<FrameData, MAX_FRAMES> m_frames;
    u32 m_currentFrameSlot = 0;

    Result<Page> createPage();
};

} // namespace ln
