#pragma once
#include "../Common.hpp"
#include "../../../../../src/Rendering/RenderFeature/ShapesRenderFeature.hpp"

namespace ln {
namespace kanata {

class ShapesRenderFeature : public URefObject {
public:
    ShapesRenderFeature(detail::RenderingManager* manager);

    void beginBatch(BatchCollector* collector, Material* material);
    void endBatch(BatchCollector* collector);

    void drawCommandList(const detail::BoxElementShapeCommandList* commandList);

private:
    BatchCollector* m_currentCollector;
    Material* m_material;
    detail::BoxElementShapeBuilder3 m_shapeBuilder;
};

} // namespace kanata
} // namespace ln

