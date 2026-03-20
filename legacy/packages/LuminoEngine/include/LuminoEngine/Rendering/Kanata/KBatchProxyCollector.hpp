#pragma once
#include "Common.hpp"
#include "KBatchProxy.hpp"

namespace ln {
namespace kanata {
	
class BatchProxyCollector final : public URefObject {
public:
    BatchProxyCollector(detail::RenderingManager* manager);
    virtual ~BatchProxyCollector();
    void dispose();
    void clear();

    
    BatchProxyStateStackItem* primaryState() { return m_aliveStateStack.front(); }
    void pushState(bool reset); // ※ 単純に state を退避するための仕組みなので、OpenGL の push/pop Matrix のように transform の乗算などは行わない。
    void popState();



    detail::LinearAllocator* dataAllocator() const { return m_dataAllocator; }

    void addBatchProxy(BatchProxy* batchProxy);

    // UE4 と違って、Proxy は Scene の Node から作るだけではなく、
    // RenderingContext::drawXXXX() の中で動的に作ることもある。そのためのアロケータ。
    template<class TBatchProxy, class... TArgs>
    TBatchProxy* newSingleFrameBatchProxy(TArgs&&... args) {
        void* buffer = m_dataAllocator->allocate(sizeof(TBatchProxy));
        TBatchProxy* data = new (buffer) TBatchProxy(std::forward<TArgs>(args)...);
        addBatchProxy(data);
        addSingleFrameBatchProxyDestructList(data);
        return data;
    }

    // Not call destructor.
    template<class T, class... TArgs>
    T* newFrameRawData(TArgs&&... args) {
        void* buffer = m_dataAllocator->allocate(sizeof(T));
        T* data = new (buffer) T(std::forward<TArgs>(args)...);
        return data;
    }

    void resolveSingleFrameBatchProxies(BatchCollector* batchCollector);

private:
    //void addBatchProxy(BatchProxy* batchProxy);
    void addSingleFrameBatchProxyDestructList(SingleFrameBatchProxy* batchProxy);
    BatchProxyState* allocateBatchProxyState();
    void submitState(BatchProxy* batchProxy);

    detail::RenderingManager* m_manager;
    Ref<detail::LinearAllocator> m_dataAllocator;

    // Batch へ解決するための、BatchProxy 及び SingleFrameBatchProxy の List
    BatchProxy* m_headBatchProxy; // head of link list.
    BatchProxy* m_tailBatchProxy; // tail of link list.

    // SingleFrameBatchProxy のデストラクタを呼ぶための List
    SingleFrameBatchProxy* m_headSingleFrameBatchProxyDestructList; // head of link list.
    SingleFrameBatchProxy* m_tailSingleFrameBatchProxyDestructList; // tail of link list.

    ln::Array<URef<BatchProxyStateStackItem>> m_freeStateStack;
    ln::Array<URef<BatchProxyStateStackItem>> m_aliveStateStack; // size >= 1
    const BatchProxyState* m_lastBatchProxyState;

    // 検証用。clear() と resolveSingleFrameBatchProxies() は、1フレーム中に 1:1 で呼ばれなければならない。
    bool m_resolvedSingleFrameBatchProxies;

    // 検証用。1フレームの間に同じ BatchProxy は複数回追加してはならない。(state を持っているので)
    int32_t m_frameIndex;
};


} // namespace kanata
} // namespace ln

