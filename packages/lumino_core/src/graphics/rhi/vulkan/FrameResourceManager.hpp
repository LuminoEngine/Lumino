#pragma once

/// @file FrameResourceManager.hpp
/// Per-frame deferred cleanup queue.
///
/// Usage:
///   1. Call beginFrame(frameIndex) immediately after waiting for the frame's
///      in-flight fence.  This executes every cleanup fn queued for that index.
///   2. Call queueDelete(frameIndex, fn) to schedule a cleanup that must not
///      run until the GPU has finished processing that frame.
///
/// Double-buffering (MAX_FRAMES = 2) means a deletion is safe to execute
/// only after the *same* frame index appears again in beginFrame().

#include <lumino_base/Types.hpp>

#include <array>
#include <functional>
#include <vector>

namespace lumino::rhi::vulkan {

class FrameResourceManager {
public:
    static constexpr u32 MAX_FRAMES = 2;

    /// Execute all deferred deletions queued for `frameIndex`, then clear the
    /// queue.  Call this after the corresponding in-flight fence has been
    /// waited on (so the GPU is no longer using those resources).
    void beginFrame(u32 frameIndex) {
        auto& q = deleteQueues_[frameIndex % MAX_FRAMES];
        for (auto& fn : q) fn();
        q.clear();
    }

    /// Queue `fn` to run the next time beginFrame() is called with the same
    /// frame index.  Ownership of the callable is transferred.
    void queueDelete(u32 frameIndex, std::function<void()> fn) {
        deleteQueues_[frameIndex % MAX_FRAMES].push_back(std::move(fn));
    }

    /// Run every queued deletion immediately (used during device shutdown when
    /// the GPU is known to be idle and no more frames will be submitted).
    void flushAll() {
        for (auto& q : deleteQueues_) {
            for (auto& fn : q) fn();
            q.clear();
        }
    }

private:
    std::array<std::vector<std::function<void()>>, MAX_FRAMES> deleteQueues_;
};

} // namespace lumino::rhi::vulkan
