#pragma once
#include <LuminoCore/Base/LinearAllocator.hpp>
#include "../Common.hpp"
#include "../KSceneRenderPass.hpp"

namespace ln {
namespace kanata {

class PrimitiveMeshRenderer : public URefObject {
public:
    PrimitiveMeshRenderer(detail::RenderingManager* manager);

    // NOTE: ステートフルとステートレスどちらがよいか？
    // TextRenderer では Atlas が一杯になったらどのみち flush が必要。
    // 他の Render と I/F を統一したければ、ステートフルで対応できるようにしなければならない。
    [[deprecated]]
    void beginBatch(BatchCollector* collector, Material* material);
    [[deprecated]]
    void endBatch(BatchCollector* collector);

    [[deprecated]]
    void drawMeshGenerater(detail::MeshGenerater* generater);
    [[deprecated]]
    void drawBox(const Box& box, const Color& color = Color::White, const Matrix& localTransform = Matrix());

    void drawGeneratorsDirect(BatchCollector* collector, Material* material, detail::MeshGenerater** generators, int32_t count);

private:
    void flush(detail::MeshGenerater** generators, int32_t count);

    BatchCollector* m_currentCollector;
    Material* m_material;
    Array<detail::MeshGenerater*> m_generators;
};

} // namespace kanata
} // namespace ln

