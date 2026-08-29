#pragma once

/**
 * @file FrameResourceManager.hpp
 * フレーム単位の遅延破棄キューです。
 *
 * 使い方:
 *   1. フレームの in-flight フェンスを待った直後に beginFrame(frameIndex) を呼びます。
 *      そのインデックスにキューされた破棄処理をすべて実行します。
 *   2. GPU がそのフレームの処理を終えるまで実行してはならない破棄処理は
 *      queueDelete(frameIndex, fn) でスケジュールします。
 *
 * ダブルバッファリング (MAX_FRAMES = 2) のため、破棄を安全に実行できるのは
 * 同じフレームインデックスが再び beginFrame() に現れた後だけです。
 */

#include <LuminoBase/Types.hpp>

#include <array>
#include <functional>
#include <vector>

namespace ln::rhi::vulkan {

class FrameResourceManager {
    // NOTE: このようなリソース管理は RHI の外と中のどちらで行うべき？
    //   「いつ安全に破棄できるか」の判断はバックエンド固有です。
    //   - Vulkan: Fence シグナル後 / フレームインデックスのラップ
    //   - D3D12: Fence 値が特定値を超えた後（ほぼ同じ）
    //   - Metal: Command Buffer の completion handler
    //   - D3D11 ドライバが内部管理するため対策不要
    //   - WebGPU ランタイムが管理するため対策不要 
    //   RHI の外側で統一的に処理しようとすると、最も保守的な戦略（全バックエンドで N フレーム待つ等）を
    //   強制することになり、不要なレイテンシが生まれます。
public:
    static constexpr uint32_t MAX_FRAMES = 2;

    /**
     * `frameIndex` にキューされた遅延破棄をすべて実行し、キューをクリアします。
     * 対応する in-flight フェンスを待った後 (GPU がそれらのリソースを
     * 使っていない状態) に呼び出してください。
     */
    void beginFrame(uint32_t frameIndex) {
        auto& q = m_deleteQueues[frameIndex % MAX_FRAMES];
        for (auto& fn : q) fn();
        q.clear();
    }

    /**
     * 同じフレームインデックスで次に beginFrame() が呼ばれたときに実行されるよう
     * `fn` をキューします。呼び出し可能オブジェクトの所有権は移動します。
     */
    void queueDelete(uint32_t frameIndex, std::function<void()> fn) {
        m_deleteQueues[frameIndex % MAX_FRAMES].push_back(std::move(fn));
    }

    /**
     * キューされた破棄処理をすべて即座に実行します (GPU がアイドルで、
     * これ以上フレームが送信されないデバイス終了時に使います)。
     */
    void flushAll() {
        for (auto& q : m_deleteQueues) {
            for (auto& fn : q) fn();
            q.clear();
        }
    }

private:
    std::array<std::vector<std::function<void()>>, MAX_FRAMES> m_deleteQueues;
};

} // namespace ln::rhi::vulkan
