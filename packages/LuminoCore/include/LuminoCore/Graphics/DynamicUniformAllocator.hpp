#pragma once

#include <memory>
#include <vector>
#include <LuminoBase/Types.hpp>
#include <LuminoBase/Result.hpp>
#include <LuminoBase/RefObject.hpp>
#include <LuminoCore/Graphics/rhi/Rhi.hpp>

namespace ln {

/** 動的ユニフォームバッファのサブアロケーションの結果。 */
struct DynamicUniformAllocation {
    void* cpuPtr;                ///< シャドウバッファ内の CPU から書き込めるポインタ。
    rhi::BindGroup* bindGroup;   ///< バインドする BindGroup (ページごとに 1 つ)。
    uint32_t dynamicOffset;           ///< setBindGroup に渡す動的オフセット。
};

/**
 * 動的ユニフォームバッファ用の、フレームごとのリニアアロケータ。
 *
 * 大きなユニフォームバッファのページを管理する。毎フレーム、オブジェクトの UBO データは
 * リニアアロケータからサブアロケートされる。ページごとに 1 つの BindGroup を、
 * 異なる動的オフセットで繰り返しバインドする。
 *
 * 書き込みは CPU 側のシャドウバッファに集め、送信前に Device::writeBuffer() で
 * GPU へフラッシュする。
 *
 * ダブルバッファリング: フレームインデックスごとに独立したページ群を持つため、
 * フレーム N-1 がまだ GPU 上で実行中でもフレーム N が書き込める。
 */
class DynamicUniformAllocator {
public:
    ~DynamicUniformAllocator() = default;

    /**
     * 新しいアロケータを作成する。
     * @param device          RHI デバイス。
     * @param pipelineLayout  対象セットの BindGroupLayout を所有する PipelineLayout。
     * @param setIndex        PipelineLayout 内のディスクリプタセットのインデックス。
     * @param binding         レイアウト内の UBO のバインディングインデックス。
     * @param elementSize     要素 1 つのバイト数 (例: sizeof(ObjectParamsUBO))。
     * @param framesInFlight  インフライトフレーム数 (ダブルバッファリングなら 2)。
     */
    static Result<std::unique_ptr<DynamicUniformAllocator>> create(
        rhi::Device* device,
        rhi::PipelineLayout* pipelineLayout,
        uint32_t setIndex,
        uint32_t binding,
        uint32_t elementSize,
        uint32_t framesInFlight);

    /** 各フレームの開始時に呼び出し、アロケータを再利用のためにリセットする。 */
    void beginFrame(uint32_t frameIndex);

    /** 要素を 1 つサブアロケートする。CPU ポインタとバインド情報を返す。 */
    DynamicUniformAllocation allocate();

    /** 現在のフレームで書き込んだデータをすべて GPU へフラッシュする。submit の前に呼び出すこと。 */
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
