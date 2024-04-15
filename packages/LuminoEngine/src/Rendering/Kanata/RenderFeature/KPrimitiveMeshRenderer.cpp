#include <LuminoEngine/Rendering/Kanata/KBatch.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatchList.hpp>
#include <LuminoEngine/Rendering/Kanata/RenderFeature/KPrimitiveMeshRenderer.hpp>
#include <LuminoEngine/Rendering/detail/RenderingManager.hpp>
#include <LuminoEngine/Mesh/detail/MeshGenerater.hpp>

namespace ln {
namespace kanata {

PrimitiveMeshRenderer::PrimitiveMeshRenderer(detail::RenderingManager* manager)
    : m_currentCollector(nullptr) {
}
void PrimitiveMeshRenderer::beginBatch(BatchCollector* collector, Material* material) {
    LN_DCHECK(!m_currentCollector);
    LN_DCHECK(collector);
    m_currentCollector = collector;
    m_material = material;
    m_generators.clear();
}

void PrimitiveMeshRenderer::endBatch(BatchCollector* collector) {
    LN_DCHECK(m_currentCollector);
    LN_DCHECK(m_currentCollector == collector);
    if (m_generators.length() > 0) {
        flush(m_generators.data(), m_generators.length());
    }
    m_currentCollector = nullptr;
    m_material = nullptr;
}

void PrimitiveMeshRenderer::drawMeshGenerater(detail::MeshGenerater* generater) {
    m_generators.push(generater);
}

void PrimitiveMeshRenderer::drawBox(const Box& box, const Color& color, const Matrix& localTransform) {
    auto* g = m_currentCollector->newFrameRawData<detail::RegularBoxMeshFactory>();

    // m_builder->setPrimitiveTopology(PrimitiveTopology::TriangleList);
    //  TODO: box.center
    g->m_size = Vector3(box.width, box.height, box.depth);
    g->setColor(color);
    g->setTransform(localTransform); // TODO:
                                    // TODO: bouding box

    m_generators.push(g);
}

void PrimitiveMeshRenderer::drawGeneratorsDirect(BatchCollector* collector, Material* material, detail::MeshGenerater** generators, int32_t count) {
    beginBatch(collector, material);
    flush(generators, count);
    endBatch(collector);
}

void PrimitiveMeshRenderer::flush(detail::MeshGenerater** generators, int32_t count) {
    // Prepare buffers
    int32_t vertexCount = 0;
    int32_t indexCount = 0;
    //for (auto* gen : m_generators) {
    for (int32_t i = 0; i < count; i++) {
        const auto* gen = generators[i];
        assert(gen->primitiveType() == PrimitiveTopology::TriangleList); // TODO: 今のところこれだけ対応
        vertexCount += gen->vertexCount();
        indexCount += gen->indexCount();
    }
    auto view = m_currentCollector->allocateMeshBuffer(vertexCount, indexCount);

    // Create Vertex and Index buffers
    //Vertex* vertexBuffer = (Vertex*)m_vertexBuffer->writableData(0, vertexCount * sizeof(Vertex));
    //uint16_t* indexBuffer = (uint16_t*)m_indexBuffer->map(MapMode::Write);
    detail::MeshGeneraterBuffer buffer(m_currentCollector->dataAllocator());
    size_t vertexOffset = 0;
    size_t indexOffset = 0;
    for (int32_t i = 0; i < count; i++) {
        auto* gen = generators[i];
        buffer.setBuffer(view.vertexData + vertexOffset, view.indexData + indexOffset, IndexBufferFormat::UInt32, vertexOffset);
        buffer.generate(gen);
        vertexOffset += gen->vertexCount();
        indexOffset += gen->indexCount();
    }

    Batch* batch = m_currentCollector->newBatch<Batch>(1, m_material);
    batch->elemets2[0].vertexBuffers[0] = view.vertexBuffer;
    batch->elemets2[0].indexBuffer = view.indexBuffer;
    batch->elemets2[0].firstIndex = view.firstIndex;
    batch->elemets2[0].firstVertex = view.vertexOffset;
    batch->elemets2[0].primitiveCount = indexCount / 3; // TODO: TriangleList only
    batch->vertexLayout = m_currentCollector->standardVertexDeclaration();
    batch->primitiveTopology = PrimitiveTopology::TriangleList;
}

} // namespace kanata
} // namespace ln

