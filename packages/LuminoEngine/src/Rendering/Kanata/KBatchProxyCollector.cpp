#include <LuminoEngine/Rendering/Kanata/KBatchList.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatchProxy.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatchProxyCollector.hpp>
#include <LuminoEngine/Rendering/detail/RenderingManager.hpp>

namespace ln {
namespace kanata {

//==============================================================================
// SingleLineSingleFrameBatchProxy

BatchProxyCollector::BatchProxyCollector(detail::RenderingManager* manager)
    : m_manager(nullptr)
    , m_dataAllocator(makeRef<detail::LinearAllocator>(manager->stageDataPageManager()))
    , m_headBatchProxy(nullptr)
    , m_tailBatchProxy(nullptr)
    , m_headSingleFrameBatchProxyDestructList(nullptr)
    , m_tailSingleFrameBatchProxyDestructList(nullptr)
    , m_resolvedSingleFrameBatchProxies(false)
    , m_frameIndex(0) {

    // いくつか事前に作っておく
    for (int i = 0; i < 10; i++) {
        m_freeStateStack.push(makeURef<BatchProxyStateStackItem>());
    }
}

BatchProxyCollector::~BatchProxyCollector() {
    LN_DCHECK(!m_headSingleFrameBatchProxyDestructList); // Not calling dispose
}

void BatchProxyCollector::dispose() {
    clear();
}
void BatchProxyCollector::clear() {
    {
        SingleFrameBatchProxy* p = m_headSingleFrameBatchProxyDestructList;
        while (p) {
            p->~SingleFrameBatchProxy();
            p = p->m_nextSingleFrame;
        }
        m_headSingleFrameBatchProxyDestructList = nullptr;
        m_tailSingleFrameBatchProxyDestructList = nullptr;
    }
    m_resolvedSingleFrameBatchProxies = false;

    m_headBatchProxy = nullptr;
    m_tailBatchProxy = nullptr;
    
    for (URef<BatchProxyStateStackItem>& s : m_aliveStateStack) {
        m_freeStateStack.push(std::move(s));
    }
    m_aliveStateStack.clear();
    pushState(true); // 1つスタックに積んでおく。コレがルートのステート
    m_lastBatchProxyState = nullptr;
    m_frameIndex++;
}

void BatchProxyCollector::pushState(bool reset) {
    URef<BatchProxyStateStackItem> state;
    if (m_freeStateStack.isEmpty()) {
        state = makeURef<BatchProxyStateStackItem>();
    }
    else {
        state = std::move(m_freeStateStack.back());
        m_freeStateStack.pop_back();
    }

    if (m_aliveStateStack.isEmpty()) {
        // 描画開始時に1つ積んでおくとき
    }
    else {
        // 現在のステートを保持
        state->copyFrom(primaryState());
    }

    m_aliveStateStack.push(std::move(state));

    if (reset) {
        primaryState()->reset();
    }
}

void BatchProxyCollector::popState() {
    URef<BatchProxyStateStackItem> state = std::move(m_aliveStateStack.back());
    m_aliveStateStack.pop_back();
    m_freeStateStack.push(std::move(state));
    primaryState()->m_dirty = true; // TODO: 内容比較して判断したほうがいいかも
}

void BatchProxyCollector::addBatchProxy(BatchProxy* batchProxy) {
    LN_CHECK(batchProxy);
    LN_CHECK(batchProxy->m_frameIndex != m_frameIndex);

    if (!m_headBatchProxy) {
        m_headBatchProxy = batchProxy;
    }
    else {
        m_tailBatchProxy->m_nextProxy = batchProxy;
    }
    m_tailBatchProxy = batchProxy;
    batchProxy->m_frameIndex = m_frameIndex;

    submitState(batchProxy);
}

void BatchProxyCollector::resolveSingleFrameBatchProxies(BatchCollector* batchCollector) {
    LN_CHECK(!m_resolvedSingleFrameBatchProxies);
    BatchProxy* p = m_headBatchProxy;
    while (p) {
        batchCollector->batchProxyState = p->state;
        p->getBatch(batchCollector);
        p = p->m_nextProxy;
    }
    batchCollector->batchProxyState = nullptr;
    m_resolvedSingleFrameBatchProxies = true;
}

void BatchProxyCollector::addSingleFrameBatchProxyDestructList(SingleFrameBatchProxy* batchProxy) {
    LN_CHECK(batchProxy);
    if (!m_headSingleFrameBatchProxyDestructList) {
        m_headSingleFrameBatchProxyDestructList = batchProxy;
    }
    else {
        m_tailSingleFrameBatchProxyDestructList->m_nextSingleFrame = batchProxy;
    }
    m_tailSingleFrameBatchProxyDestructList = batchProxy;
}

BatchProxyState* BatchProxyCollector::allocateBatchProxyState() {
    auto* buffer = reinterpret_cast<BatchElement*>(m_dataAllocator->allocate(sizeof(BatchProxyState)));
    return new (buffer) BatchProxyState();
}

void BatchProxyCollector::submitState(BatchProxy* batchProxy) {
    auto* stackItem = primaryState();

    if (!m_lastBatchProxyState || stackItem->m_dirty) {
        auto* state = allocateBatchProxyState();
        *state = stackItem->s;
        m_lastBatchProxyState = state;
        stackItem->clearDirty();
    }

    batchProxy->state = m_lastBatchProxyState;
}

} // namespace kanata
} // namespace ln

