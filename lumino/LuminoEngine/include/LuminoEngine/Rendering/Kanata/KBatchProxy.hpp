#pragma once
#include "Common.hpp"
#include "../../../../src/Mesh/MeshGeneraters/MeshGenerater.hpp"

namespace ln {
namespace kanata {

// インスタンスを SceneNode で保持する用
class BatchProxy : public URefObject {
public:
    virtual void getBatch(BatchCollector* collector) = 0;

private:
};

// drawBox() などの動的描画用
class SingleFrameBatchProxy : public URefObject {
public:
    SingleFrameBatchProxy()
        : m_next(nullptr) {
    }

    virtual void getBatch(BatchCollector* collector) = 0;

private:
    SingleFrameBatchProxy* m_next;

    friend class BatchCollector;
};

class SingleLineSingleFrameBatchProxy : public SingleFrameBatchProxy {
public:
    detail::SingleLineGenerater data;

    void getBatch(BatchCollector* collector) override;
};

class RegularBoxSingleFrameBatchProxy : public SingleFrameBatchProxy {
public:
    detail::RegularBoxMeshFactory data;

    void getBatch(BatchCollector* collector) override;
};

class BoxMeshBatchProxy : public BatchProxy {
public:
    BoxMeshBatchProxy();
    void getBatch(BatchCollector* collector) override;

private:
    Vector3 m_size;
    Color m_color;
    Ref<VertexBuffer> m_vertexBuffer;
    Ref<IndexBuffer> m_indexBuffer;
};

} // namespace kanata
} // namespace ln

