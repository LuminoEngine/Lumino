﻿#include <LuminoEngine/GPU/VertexLayout.hpp>
#include <LuminoEngine/GPU/VertexBuffer.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatch.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatchList.hpp>
#include <LuminoEngine/Rendering/Kanata/RenderFeature/KScreenRectangleRenderFeature.hpp>
#include <LuminoEngine/Rendering/detail/RenderingManager.hpp>

namespace ln {
namespace kanata {

ScreenRectangleRenderFeature::ScreenRectangleRenderFeature(detail::RenderingManager* manager)
    : m_manager(manager)
    , m_vertexLayout(nullptr)
    , m_vertexBuffer(nullptr) {
}

Result<> ScreenRectangleRenderFeature::init() {
    m_vertexLayout = m_manager->standardVertexDeclaration();

#ifdef LN_COORD_RH
    Vertex vertices[4] = {
        Vertex(Vector3(-1, 1, 0), Vector3::UnitZ, Vector2(0, 0), Color::White),
        Vertex(Vector3(-1, -1, 0), Vector3::UnitZ, Vector2(0, 1), Color::White),
        Vertex(Vector3(1, 1, 0), Vector3::UnitZ, Vector2(1, 0), Color::White),
        Vertex(Vector3(1, -1, 0), Vector3::UnitZ, Vector2(1, 1), Color::White),
    };
#else
    Vertex vertices[4] = {
        { Vector3(-1, 1, 0), Vector3::UnitZ, Vector2(0, 0), Color::White, Vector4(1, 0, 0, 1) },
        { Vector3(1, 1, 0), Vector3::UnitZ, Vector2(1, 0), Color::White, Vector4(1, 0, 0, 1) },
        { Vector3(-1, -1, 0), Vector3::UnitZ, Vector2(0, 1), Color::White, Vector4(1, 0, 0, 1) },
        { Vector3(1, -1, 0), Vector3::UnitZ, Vector2(1, 1), Color::White, Vector4(1, 0, 0, 1) },
    };
#endif
    m_vertexBuffer = makeObject_deprecated<VertexBuffer>(sizeof(vertices), vertices, GraphicsResourceUsage::Static);

    return ok();
}

void ScreenRectangleRenderFeature::drawScreenRectangle(BatchCollector* collector, Material* material) const {
    Batch* batch = collector->newBatch<Batch>(1, material);
    batch->elemets2[0].vertexBuffers[0] = m_vertexBuffer;
    batch->elemets2[0].primitiveCount = 2;
    batch->vertexLayout = m_vertexLayout;
    batch->primitiveTopology = PrimitiveTopology::TriangleStrip;
}

} // namespace kanata
} // namespace ln
