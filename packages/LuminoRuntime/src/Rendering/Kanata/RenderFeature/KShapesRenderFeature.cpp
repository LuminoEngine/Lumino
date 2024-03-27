#include <LuminoEngine/Rendering/Kanata/KBatch.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatchList.hpp>
#include <LuminoEngine/Rendering/Kanata/RenderFeature/KShapesRenderFeature.hpp>
using namespace ln::detail;

namespace ln {
namespace kanata {

ShapesRenderFeature::ShapesRenderFeature(detail::RenderingManager* manager)
    : m_currentCollector(nullptr)
    , m_material(nullptr) {
}

void ShapesRenderFeature::beginBatch(BatchCollector* collector, Material* material) {
    LN_DCHECK(!m_currentCollector);
    LN_DCHECK(collector);
    LN_DCHECK(material);
    m_currentCollector = collector;
    m_material = material;
}

void ShapesRenderFeature::endBatch(BatchCollector* collector) {
    LN_DCHECK(m_currentCollector);
    LN_DCHECK(m_currentCollector == collector);

    m_currentCollector = nullptr;
    m_material = nullptr;
}

void ShapesRenderFeature::drawCommandList(const detail::BoxElementShapeCommandList* commandList) {

    BoxElementShapeCommandList::ListNode* node = commandList->head;
    while (node) {
        switch (node->type) {
            case BoxElementShapeCommandList::Cmd_Reset: {
                m_shapeBuilder.reset();
                break;
            }
            case BoxElementShapeCommandList::Cmd_Base: {
                auto* cmd = reinterpret_cast<BoxElementShapeCommandList::BaseCommand*>(node);
                m_shapeBuilder.setBaseRect(cmd->style);
                break;
            }
            case BoxElementShapeCommandList::Cmd_Background: {
                auto* cmd = reinterpret_cast<BoxElementShapeCommandList::BackgroundCommand*>(node);
                m_shapeBuilder.setFillBox(cmd->style);
                break;
            }
            case BoxElementShapeCommandList::Cmd_Border: {
                auto* cmd = reinterpret_cast<BoxElementShapeCommandList::BorderCommand*>(node);
                m_shapeBuilder.setBoxBorderLine(cmd->style);
                break;
            }
            case BoxElementShapeCommandList::Cmd_Shadow: {
                auto* cmd = reinterpret_cast<BoxElementShapeCommandList::ShadowCommand*>(node);
                m_shapeBuilder.setBoxShadow(cmd->style);
                break;
            }
            case BoxElementShapeCommandList::Cmd_Submit: {
                m_shapeBuilder.build();
                int32_t vertexCount = m_shapeBuilder.vertexCount();
                int32_t indexCount = m_shapeBuilder.indexCount();
                auto view = m_currentCollector->allocateMeshBuffer(vertexCount, indexCount);
                if (view.vertexCount != vertexCount || view.indexCount != indexCount) {
                    LN_NOTIMPLEMENTED();
                }

                m_shapeBuilder.writeToBuffer32(view.vertexData, view.indexData);

                // Create batch
                {
                    Batch* batch = m_currentCollector->newBatch<Batch>(1, m_material);
                    batch->elemets2[0].vertexBuffers[0] = view.vertexBuffer;
                    batch->elemets2[0].indexBuffer = view.indexBuffer;
                    batch->elemets2[0].firstIndex = view.firstIndex;
                    batch->elemets2[0].firstVertex = view.vertexOffset;
                    batch->elemets2[0].primitiveCount = indexCount / 3;
                    batch->vertexLayout = m_currentCollector->standardVertexDeclaration();
                    batch->primitiveTopology = PrimitiveTopology::TriangleList;
                }

                break;
            }
            default:
                LN_UNREACHABLE();
                break;
        }

        node = node->next;
    }
}

} // namespace kanata
} // namespace ln

