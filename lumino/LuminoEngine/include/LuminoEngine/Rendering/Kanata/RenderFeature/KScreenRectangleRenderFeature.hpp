#pragma once
#include "../Common.hpp"

namespace ln {
namespace kanata {

class ScreenRectangleRenderFeature : public URefObject {
public:
    ScreenRectangleRenderFeature(detail::RenderingManager* manager);
    Result init();

    void drawScreenRectangle(BatchCollector* collector, Material* material) const;

private:
    detail::RenderingManager* m_manager;
    Ref<VertexLayout> m_vertexLayout;
    Ref<VertexBuffer> m_vertexBuffer;
};

} // namespace kanata
} // namespace ln

