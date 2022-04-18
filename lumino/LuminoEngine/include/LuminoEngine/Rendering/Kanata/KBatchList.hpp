#pragma once
#include <LuminoCore/Base/LinearAllocator.hpp>
#include <LuminoGraphics/RHI/ShaderInterfaceFramework.hpp>
#include <LuminoGraphics/Rendering/Vertex.hpp>
#include "Common.hpp"
#include "KBatch.hpp"
#include "../../Graphics/src/RHI/StreamingBufferAllocator.hpp"

namespace ln {
namespace kanata {

struct MeshBufferView {
    VertexBuffer* vertexBuffer;
    IndexBuffer* indexBuffer;
    Vertex* vertexData;     // Mapped VertexBuffer (with offset)
    uint32_t* indexData;    // Mapped IndexBuffer (with offset)
    uint32_t firstIndex;    // for drawPrimitiveIndexed
    uint32_t vertexOffset;  // for drawPrimitiveIndexed
    uint32_t vertexCount;
    uint32_t indexCount;
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

    const BatchProxyState* batchProxyState = nullptr;

    BatchCollector(detail::RenderingManager* manager);
    ~BatchCollector();
    void dispose();
    void clear(const RenderViewPoint* viewPoint);

    VertexLayout* standardVertexDeclaration() const;
    detail::LinearAllocator* dataAllocator() const { return m_dataAllocator; }
    const Array<Batch*>& batches() const { return m_batchList; }
    const RenderViewPoint* viewPoint() const { return m_viewPoint; }

    // 次の addBatch() で Batch に set する RenderPass.
    // 他にも Material や Skeleton など似たようなものがあるが、
    // RenderPass だけは blit というやや特殊なケースでだけ利用したい。
    // それ以外は Batch 作るときに、できるだけ直接指定する。変にオーバーヘッドを増やしたくない。
    void setRenderPass(RenderPass* value);

    const URef<ScreenRectangleRenderFeature>& screenRectangleRenderFeature() const { return m_screenRectangleRenderFeature; }
    const URef<PrimitiveMeshRenderer>& primitiveRenderer() const { return m_primitiveRenderer; }
    const URef<MeshRenderFeature>& meshRenderFeature() const { return m_meshRenderFeature; }
    const URef<SpriteRenderFeature>& spriteRenderFeature() const { return m_spriteRenderFeature; }
    const URef<ShapesRenderFeature>& shapesRenderFeature() const { return m_shapesRenderFeature; }
    const URef<SpriteTextRenderFeature>& spriteTextRenderFeature() const { return m_spriteTextRenderFeature; }
    const URef<FrameRectRenderFeature>& frameRectRenderFeature() const { return m_frameRectRenderFeature; }

    // StaticMesh など、外部でキャッシュしている Batch を使うこともできる。
    void addBatch(Batch* batch);

    template<class TBatch, class... TArgs>
    TBatch* newBatch(size_t elementCount, Material* material, TArgs && ... args) {
        void* buffer = m_dataAllocator->allocate(sizeof(TBatch));
        TBatch* data = new (buffer) TBatch(std::forward<TArgs>(args)...);
        addBatch(data);
        allocateBatchElements(data, elementCount);
        resolveBatchMaterial(data, material);
        return data;
    }

    void allocateBatchElements(Batch* batch, size_t count) {
        LN_DCHECK(count < 128);
        auto* elements = reinterpret_cast<BatchElement*>(m_dataAllocator->allocate(sizeof(BatchElement) * count));
        for (size_t i = 0; i < count; i++) {
            new (elements + i) BatchElement();
        }
        batch->elemets2 = elements;
        batch->elementsCount = count;
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
    void addFrameData(IBatchFrameData* data);
    void resolveBatchMaterial(Batch* batch, Material* material);

    detail::RenderingManager* m_manager;
    Ref<detail::LinearAllocator> m_dataAllocator;
    IBatchFrameData* m_headFrameData;   // head of link list.
    IBatchFrameData* m_tailFrameData;   // tail of link list.
    Array<Batch*> m_batchList;  // TODO: LinkedList のほうがいいか？
    URef<detail::StreamingBufferAllocatorManager> m_vertexBufferAllocatorManager;
    URef<detail::StreamingBufferAllocatorManager> m_indexufferAllocatorManager;
    URef<detail::StreamingBufferAllocator> m_vertexBufferAllocator;
    URef<detail::StreamingBufferAllocator> m_indexBufferAllocator;

    const RenderViewPoint* m_viewPoint;
    RenderPass* m_currentRenderPass;

    URef<ScreenRectangleRenderFeature> m_screenRectangleRenderFeature;
    URef<PrimitiveMeshRenderer> m_primitiveRenderer;
    URef<MeshRenderFeature> m_meshRenderFeature;
    URef<SpriteRenderFeature> m_spriteRenderFeature;
    URef<ShapesRenderFeature> m_shapesRenderFeature;
    URef<SpriteTextRenderFeature> m_spriteTextRenderFeature;
    URef<FrameRectRenderFeature> m_frameRectRenderFeature;
};

} // namespace kanata
} // namespace ln

