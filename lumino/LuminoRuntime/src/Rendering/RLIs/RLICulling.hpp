#pragma once
#include <LuminoEngine/Rendering/Kanata/Common.hpp>
#include "../RenderStage.hpp"

namespace ln {
namespace detail {

// RenderPipeline のうち、カリング部分をモジュール化したもの。
// Unity の CullResults 相当。
class RLICulling {
public:
    RLICulling();
    void cull(
        const RenderView* renderView,
        RenderingContext* renderingContext,
        CommandListServer* commandListServer);

    const DynamicLightInfo* mainLight() const { return m_mainLight; }
    const std::vector<const DynamicLightInfo*>& visibleLights() const { return m_visibleLights; }
    const std::vector<RenderDrawElement*>& visibleElements(RenderPart part) const { return m_parts[(int)part].visibleElements; }
    const kanata::BatchCollector* batchCollector(RenderPart part) const { return m_parts[(int)part].batchCollector; }

private:
    struct Part {
        std::vector<RenderDrawElement*> visibleElements;
        const kanata::BatchCollector* batchCollector;
    };

    const DynamicLightInfo* m_mainLight;
    std::vector<const DynamicLightInfo*> m_visibleLights;
    std::array<Part, (int)RenderPart::_Count> m_parts;
    ZSortDistanceBase m_zSortDistanceBase;
};

} // namespace detail
} // namespace ln
