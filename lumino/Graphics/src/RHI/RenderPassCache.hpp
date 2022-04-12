#pragma once
#include <unordered_map>
#include <LuminoGraphics/RHI/Texture.hpp>
#include <LuminoGraphics/RHI/DepthBuffer.hpp>

namespace ln {
namespace detail {

class RenderPassCache : public URefObject {
public:
    struct FindKey {
        std::array<RenderTargetTexture*, MaxMultiRenderTargets> renderTargets = {};
        DepthBuffer* depthBuffer = nullptr;
        ClearFlags clearFlags = ClearFlags::None;
        Color clearColor = Color(0, 0, 0, 0);
        float clearDepth = 1.0f;
        uint8_t clearStencil = 0x00;
    };

    RenderPassCache();
    ~RenderPassCache() override;
    void clear();
    RenderPass* getOrCreate(const FindKey& key);
    void collectGarbage();
    static uint64_t computeHash(const FindKey& key);

private:
    static const int HoldFrameCount = 60;

    struct CacheItem {
        uint64_t key;
        int lifeFrames;
        Ref<RenderPass> renderPass;
    };

    // GC のために毎フレーム for 出まわすうえ、要素が 100 も 200 も増えることは稀なので、
    // map/unorderd_map よりも vector の方が全体的に効率が良い。
    Array<CacheItem> m_items;
};

} // namespace detail
} // namespace ln
