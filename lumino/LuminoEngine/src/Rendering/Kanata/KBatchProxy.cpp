#include <LuminoGraphics/RHI/VertexBuffer.hpp>
#include <LuminoGraphics/RHI/IndexBuffer.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatch.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatchList.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatchProxy.hpp>
#include "../RenderingManager.hpp"
#include "../../Mesh/MeshGeneraters/MeshGenerater.hpp"

#include <LuminoEngine/Rendering/Kanata/KPrimitiveMeshRenderer.hpp>

namespace ln {
namespace kanata {

    
//==============================================================================
// SingleLineSingleFrameBatchProxy

void SingleLineSingleFrameBatchProxy::getBatch(BatchCollector* collector) {
    PrimitiveMeshRenderer* r = collector->primitiveRenderer();
    r->beginBatch(collector);
    r->drawMeshGenerater(&data);
    r->endBatch(collector);
}

//==============================================================================
// RegularBoxSingleFrameBatchProxy

void RegularBoxSingleFrameBatchProxy::getBatch(BatchCollector* collector) {
    PrimitiveMeshRenderer* r = collector->primitiveRenderer();
    r->beginBatch(collector);
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
        m_vertexBuffer = makeObject<VertexBuffer>(sizeof(Vertex) * vertexCount, GraphicsResourceUsage::Static);
        m_indexBuffer = makeObject<IndexBuffer>(sizeof(uint16_t) * indexCount, IndexBufferFormat::UInt16, GraphicsResourceUsage::Static);
        
        Vertex* vertices = (Vertex*)m_vertexBuffer->writableData(0, vertexCount * sizeof(Vertex));
        uint16_t* indices = (uint16_t*)m_indexBuffer->map(MapMode::Write);
        detail::MeshGeneraterBuffer buffer(collector->dataAllocator());
        buffer.setBuffer(vertices, indices, IndexBufferFormat::UInt16, 0);
        buffer.generate(&factory);
    }

    Batch* batch = collector->newBatch<Batch>();
    BatchElement batchElement;
    batchElement.vertexBuffers = {};
    batchElement.vertexBuffers[0] = m_vertexBuffer;
    batchElement.indexBuffer = m_indexBuffer;
    batchElement.firstIndex = 0;
    batchElement.firstVertex = 0;
    batchElement.primitiveCount = 12;
    batchElement.instanceCount = 0;
    batch->elemets.push(batchElement);
    batch->vertexLayout = collector->standardVertexDeclaration();
    batch->primitiveTopology = PrimitiveTopology::TriangleList;
}

} // namespace kanata
} // namespace ln

