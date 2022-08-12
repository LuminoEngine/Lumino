#include <LuminoEngine/Base/MixHash.hpp>
#include <LuminoGraphics/GPU/RenderPass.hpp>
#include "RenderPassCache.hpp"

namespace ln {
namespace detail {

//==============================================================================
// RenderPassCache

RenderPassCache::RenderPassCache()
    : m_items() {
}

RenderPassCache::~RenderPassCache() {
}

void RenderPassCache::clear() {
    for (auto& item : m_items) {
        item.renderPass->dispose();
    }
    m_items.clear();
}

RenderPass* RenderPassCache::getOrCreate(const FindKey& key) {
    uint64_t hash = computeHash(key);
    auto itr = m_items.findIf([hash](auto& item) { return item.key == hash; });
    if (itr) {
        itr->lifeFrames = HoldFrameCount;
        return itr->renderPass;
    }
    else {
        CacheItem item;
        item.key = hash;
        item.lifeFrames = HoldFrameCount;
        item.renderPass = makeObject_deprecated<RenderPass>();
        for (size_t i = 0; i < key.renderTargets.size(); i++) {
            item.renderPass->setRenderTarget(i, key.renderTargets[i]);
        }
        item.renderPass->setDepthBuffer(key.depthBuffer);
        item.renderPass->setClearFlags(key.clearFlags);
        item.renderPass->setClearColor(key.clearColor);
        item.renderPass->setClearDepth(key.clearDepth);
        item.renderPass->setClearStencil(key.clearStencil);
        item.renderPass->m_freezed = true;
        m_items.push(std::move(item));
        return m_items.back().renderPass;
    }
}

void RenderPassCache::collectGarbage() {
    for (auto it = m_items.begin(); it != m_items.end(); it++) {
        it->lifeFrames--;
        if (it->lifeFrames < 0) {
            it->renderPass->dispose();
            m_items.erase(it);
            break;  // 1フレームにまとめて delete が発生すると処理落ちの可能性があるため、ひとつずつ消す
        }
    }
}

uint64_t RenderPassCache::computeHash(const FindKey& key) {
    MixHash hash;
    hash.add(key);
    return hash.value();
}

} // namespace detail
} // namespace ln
