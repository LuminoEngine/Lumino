#include <LuminoEngine/GPU/VertexBuffer.hpp>
#include <LuminoEngine/GPU/IndexBuffer.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatch.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatchList.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatchProxy.hpp>
#include <LuminoEngine/Rendering/detail/RenderingManager.hpp>
#include "../../Mesh/MeshGeneraters/MeshGenerater.hpp"

#include <LuminoEngine/Rendering/Kanata/RenderFeature/KPrimitiveMeshRenderer.hpp>

namespace ln {
namespace kanata {

//==============================================================================
// BatchProxyState

void BatchProxyState::reset() {
    baseTransform = nullptr;
    transform = Matrix::Identity;
    renderPriority = 0;
    objectId = 0;

    m_blendMode = nullptr;
    m_cullingMode = nullptr;
    m_depthTestEnabled = nullptr;
    m_depthWriteEnabled = nullptr;
    shadingModel = nullptr;
    builtinEffectData = nullptr;
}

void BatchProxyState::copyFrom(const BatchProxyState* other) {
    baseTransform = other->baseTransform;
    transform = other->transform;
    renderPriority = other->renderPriority;
    objectId = other->objectId;

    m_blendMode = other->m_blendMode;
    m_cullingMode = other->m_cullingMode;
    m_depthTestEnabled = other->m_depthTestEnabled;
    m_depthWriteEnabled = other->m_depthWriteEnabled;
    shadingModel = other->shadingModel;
    builtinEffectData = other->builtinEffectData;
}

//==============================================================================
// BatchProxyStateStackItem

BatchProxyStateStackItem::BatchProxyStateStackItem() {
    reset();
}

void BatchProxyStateStackItem::reset() {
    s.reset();
    m_dirty = true;
}

void BatchProxyStateStackItem::copyFrom(const BatchProxyStateStackItem* other) {
    s.copyFrom(&other->s);
}

//==============================================================================
// SingleLineSingleFrameBatchProxy

void SingleLineSingleFrameBatchProxy::getBatch(BatchCollector* collector) {
    PrimitiveMeshRenderer* r = detail::RenderingManager::instance()->primitiveRenderer();
    LN_NOTIMPLEMENTED(); // TODO: material
    r->beginBatch(collector, nullptr);
    r->drawMeshGenerater(&data);
    r->endBatch(collector);
}

//==============================================================================
// RegularBoxSingleFrameBatchProxy

void RegularBoxSingleFrameBatchProxy::getBatch(BatchCollector* collector) {
    PrimitiveMeshRenderer* r = detail::RenderingManager::instance()->primitiveRenderer();
    LN_NOTIMPLEMENTED(); // TODO: material
    r->beginBatch(collector, nullptr);
    r->drawMeshGenerater(&data);
    r->endBatch(collector);
}

//==============================================================================
// BoxMeshBatchProxy

BoxMeshBatchProxy::BoxMeshBatchProxy()
    : m_size(1, 1, 1)
    , m_color(Color::Blue) {
}

void BoxMeshBatchProxy::getBatch(BatchCollector* collector) {
    if (!m_vertexBuffer) {

        detail::RegularBoxMeshFactory factory;
        //m_builder->setPrimitiveTopology(PrimitiveTopology::TriangleList);
        // TODO: box.center
        factory.m_size = m_size;
        factory.setColor(m_color);
        factory.setTransform(Matrix::Identity); // TODO:
        // TODO: bouding box

        int vertexCount = factory.vertexCount();
        int indexCount = factory.indexCount();
        m_vertexBuffer = makeObject_deprecated<VertexBuffer>(sizeof(Vertex) * vertexCount, GraphicsResourceUsage::Static);
        m_indexBuffer = makeObject_deprecated<IndexBuffer>(sizeof(uint16_t) * indexCount, IndexBufferFormat::UInt16, GraphicsResourceUsage::Static);
        
        Vertex* vertices = (Vertex*)m_vertexBuffer->writableData(0, vertexCount * sizeof(Vertex));
        uint16_t* indices = (uint16_t*)m_indexBuffer->map(MapMode::Write);
        detail::MeshGeneraterBuffer buffer(collector->dataAllocator());
        buffer.setBuffer(vertices, indices, IndexBufferFormat::UInt16, 0);
        buffer.generate(&factory);
    }

    LN_NOTIMPLEMENTED();    // TODO: material
    Batch* batch = collector->newBatch<Batch>(1, nullptr);
    batch->elemets2[0].vertexBuffers[0] = m_vertexBuffer;
    batch->elemets2[0].indexBuffer = m_indexBuffer;
    batch->elemets2[0].firstIndex = 0;
    batch->elemets2[0].firstVertex = 0;
    batch->elemets2[0].primitiveCount = 12;
    batch->elemets2[0].instanceCount = 0;
    batch->vertexLayout = collector->standardVertexDeclaration();
    batch->primitiveTopology = PrimitiveTopology::TriangleList;
}

} // namespace kanata
} // namespace ln

