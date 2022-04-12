#pragma once
#include <LuminoCore/Base/LinearAllocator.hpp>
#include <LuminoGraphics/Rendering/Vertex.hpp>
#include "Common.hpp"
#include "../../Graphics/src/RHI/StreamingBufferAllocator.hpp"

namespace ln {
namespace kanata {

struct MeshBufferView {
    VertexBuffer* vertexBuffer;
    IndexBuffer* indexBuffer;
    Vertex* vertexData;     // Mapped VertexBuffer (with offset)
    int32_t* indexData;     // Mapped IndexBuffer (with offset)
    int32_t firstIndex;     // for drawPrimitiveIndexed
    int32_t vertexOffset;   // for drawPrimitiveIndexed
};

// DrawElementList::newFrameData() で確保するデータのインターフェイス。
// 描画終了時にデストラクタを呼び出すために使用する。
// このクラスはフレーム終了時に開放されるため、自分で参照を管理する必要はないが、寿命に注意すること。
class IBatchFrameData {
protected:
    IBatchFrameData() : m_nextFrameData(nullptr) {}
    virtual ~IBatchFrameData() = default;

private:
    // next data of linked list.
    IBatchFrameData* m_nextFrameData;

    friend class BatchCollector;
};

// TODO: proxy のリストは分けたほうがいいかもしれない
class BatchCollector final : public URefObject {
    // FMeshElementCollector
public:
    class ScopedRenderPass {
    public:
        ScopedRenderPass(BatchCollector* parent, RenderPass* value)
            : m_owner(parent)
            , m_oldValue(m_owner->m_currentRenderPass) {
            m_owner->setRenderPass(value);
        }
        ~ScopedRenderPass() {
            m_owner->setRenderPass(m_oldValue);
        }

    private:
        BatchCollector* m_owner;
        RenderPass* m_oldValue;
    };

    BatchCollector(detail::RenderingManager* manager);
    ~BatchCollector();
    void clear();

    VertexLayout* standardVertexDeclaration() const;
    detail::LinearAllocator* dataAllocator() const { return m_dataAllocator; }
    const Array<Batch*>& batches() const { return m_batchList; }

    // 次の addBatch() で Batch に set する RenderPass.
    // 他にも Material や Skeleton など似たようなものがあるが、
    // RenderPass だけは blit というやや特殊なケースでだけ利用したい。
    // それ以外は Batch 作るときに、できるだけ直接指定する。変にオーバーヘッドを増やしたくない。
    void setRenderPass(RenderPass* value);

    const URef<ScreenRectangleRenderFeature>& screenRectangleRenderFeature() const { return m_screenRectangleRenderFeature; }
    const URef<PrimitiveMeshRenderer>& primitiveRenderer() const { return m_primitiveRenderer; }
    const URef<MeshRenderFeature>& meshRenderFeature() const { return m_meshRenderFeature; }


    // UE4 と違って、Proxy は Scene の Node から作るだけではなく、
    // RenderingContext::drawXXXX() の中で動的に作ることもある。そのためのアロケータ。
    template<class TBatchProxy, class... TArgs>
    TBatchProxy* newSingleFrameBatchProxy(TArgs&&... args) {
        void* buffer = m_dataAllocator->allocate(sizeof(TBatchProxy));
        TBatchProxy* data = new (buffer) TBatchProxy(std::forward<TArgs>(args)...);
        addSingleFrameBatchProxy(data);
        return data;
    }

    void resolveSingleFrameBatchProxies();
        
    // StaticMesh など、外部でキャッシュしている Batch を使うこともできる。
    void addBatch(Batch* batch);

    template<class TBatch, class... TArgs>
    TBatch* newBatch(TArgs&&... args) {
        void* buffer = m_dataAllocator->allocate(sizeof(TBatch));
        TBatch* data = new (buffer) TBatch(std::forward<TArgs>(args)...);
        addBatch(data);
        return data;
    }

    template<class T, class... TArgs>
    T* newFrameData(TArgs&&... args) {
        void* buffer = m_dataAllocator->allocate(sizeof(T));
        T* data = new (buffer) T(std::forward<TArgs>(args)...);
        addFrameData(data);
        return data;
    }

    // Not call destructor.
    template<class T, class... TArgs>
    T* newFrameRawData(TArgs&&... args) {
        void* buffer = m_dataAllocator->allocate(sizeof(T));
        T* data = new (buffer) T(std::forward<TArgs>(args)...);
        return data;
    }

    MeshBufferView allocateMeshBuffer(int32_t vertexCount, int32_t indexCount);

private:
    void addSingleFrameBatchProxy(SingleFrameBatchProxy* batchProxy);
    void addFrameData(IBatchFrameData* data);

    detail::RenderingManager* m_manager;
    Ref<detail::LinearAllocator> m_dataAllocator;
    SingleFrameBatchProxy* m_headSingleFrameBatchProxy; // head of link list.
    SingleFrameBatchProxy* m_tailSingleFrameBatchProxy; // tail of link list.
    IBatchFrameData* m_headFrameData;   // head of link list.
    IBatchFrameData* m_tailFrameData;   // tail of link list.
    Array<Batch*> m_batchList;  // TODO: LinkedList のほうがいいか？
    URef<detail::StreamingBufferAllocatorManager> m_vertexBufferAllocatorManager;
    URef<detail::StreamingBufferAllocatorManager> m_indexufferAllocatorManager;
    URef<detail::StreamingBufferAllocator> m_vertexBufferAllocator;
    URef<detail::StreamingBufferAllocator> m_indexBufferAllocator;

    RenderPass* m_currentRenderPass;

    URef<ScreenRectangleRenderFeature> m_screenRectangleRenderFeature;
    URef<PrimitiveMeshRenderer> m_primitiveRenderer;
    URef<MeshRenderFeature> m_meshRenderFeature;

    // 検証用。clear() と resolveSingleFrameBatchProxies() は、1フレーム中に 1:1 で呼ばれなければならない。
    bool m_resolvedSingleFrameBatchProxies;
};

} // namespace kanata
} // namespace ln

