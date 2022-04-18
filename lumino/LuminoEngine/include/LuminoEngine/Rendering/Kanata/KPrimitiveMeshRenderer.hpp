#pragma once
#include <LuminoCore/Base/LinearAllocator.hpp>
#include "Common.hpp"
#include "KSceneRenderPass.hpp"

namespace ln {
namespace kanata {

class PrimitiveMeshRenderer : public URefObject {
public:
    PrimitiveMeshRenderer(detail::RenderingManager* manager);

    // NOTE: ステートフルとステートレスどちらがよいか？
    // TextRenderer では Atlas が一杯になったらどのみち flush が必要。
    // 他の Render と I/F を統一したければ、ステートフルで対応できるようにしなければならない。
    void beginBatch(BatchCollector* collector, Material* material);
    void endBatch(BatchCollector* collector);

    void drawMeshGenerater(detail::MeshGenerater* generater);
    void drawBox(const Box& box, const Color& color = Color::White, const Matrix& localTransform = Matrix());

private:
    BatchCollector* m_currentCollector;
    Material* m_material;
    Array<detail::MeshGenerater*> m_generators;
};

} // namespace kanata
} // namespace ln

