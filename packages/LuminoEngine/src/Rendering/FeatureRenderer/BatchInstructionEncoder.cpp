#include <LuminoEngine/Rendering/Kanata/KBatchList.hpp>
#include <LuminoEngine/Rendering/FeatureRenderer/BatchInstructionEncoder.hpp>
#include <LuminoEngine/Mesh/detail/MeshGenerater.hpp>
#include <LuminoEngine/Rendering/detail/RenderingManager.hpp>

namespace ln {
namespace detail {

BatchInstructionEncoder::BatchInstructionEncoder(RenderingManager* manager)
    : m_manager(manager)
{
}

void BatchInstructionEncoder::dispatchList(
    kanata::BatchCollector* collector,
    Material* material,
    BatchInstruction* first) {
    BatchInstruction* currentFirst = nullptr;
    BatchInstruction* currentLast = nullptr;
    BatchInstruction* current = first;
    while (current) {
        if (currentFirst != nullptr && current->type != currentFirst->type) {
            flush(collector, material, currentFirst, currentLast);
            currentFirst = nullptr;
            currentLast = nullptr;
        }

        if (currentFirst == nullptr) {
            currentFirst = current;
        }
        currentLast = current;

        current = current->next;
    }

    if (currentFirst != nullptr && currentLast != nullptr) {
        flush(collector, material, currentFirst, currentLast);
    }
}

void BatchInstructionEncoder::flush(
    kanata::BatchCollector* collector,
    Material* material,
    BatchInstruction* currentFirst,
    BatchInstruction* currentLast) {
    switch (currentFirst->type) {
        case BatchInstructionType::StandardMesh: {
            dispatchStandardMesh(
                collector,
                material,
                static_cast<MeshGenerater*>(currentFirst),
                static_cast<MeshGenerater*>(currentLast));
            break;
        }
        default:
            break;
    }
}

void BatchInstructionEncoder::dispatchStandardMesh(
    kanata::BatchCollector* collector,
    Material* material,
    MeshGenerater* currentFirst,
    MeshGenerater* currentLast) {

    // Prepare buffers.
    int32_t vertexCount = 0;
    int32_t indexCount = 0;
    {
        MeshGenerater* current = currentFirst;
        while (current) {
            assert(current->primitiveType() == PrimitiveTopology::TriangleList); // TODO: 今のところこれだけ対応
            vertexCount += current->vertexCount();
            indexCount += current->indexCount();
            if (current == currentLast) break;
            current = static_cast<MeshGenerater*>(current->next);
        }
    }
    kanata::MeshBufferView view = collector->allocateMeshBuffer(vertexCount, indexCount);

    // Create Vertex and Index buffers.
    {
        detail::MeshGeneraterBuffer buffer(collector->dataAllocator());
        int32_t vertexOffset = 0;
        int32_t indexOffset = 0;
        MeshGenerater* current = currentFirst;
        while (current) {
            buffer.setBuffer(
                view.vertexData + vertexOffset, view.indexData + indexOffset, IndexBufferFormat::UInt32, vertexOffset);
            buffer.generate(current);
            vertexOffset += current->vertexCount();
            indexOffset += current->indexCount();
            if (current == currentLast) break;
            current = static_cast<MeshGenerater*>(current->next);
        }
    }

    // Create Batch.
    kanata::Batch* batch = collector->newBatch<kanata::Batch>(1, material);
    batch->elemets2[0].vertexBuffers[0] = view.vertexBuffer;
    batch->elemets2[0].indexBuffer = view.indexBuffer;
    batch->elemets2[0].firstIndex = view.firstIndex;
    batch->elemets2[0].firstVertex = view.vertexOffset;
    batch->elemets2[0].primitiveCount = indexCount / 3; // TODO: TriangleList only
    batch->vertexLayout = collector->standardVertexDeclaration();
    batch->primitiveTopology = PrimitiveTopology::TriangleList;
}

} // namespace detail
} // namespace ln

