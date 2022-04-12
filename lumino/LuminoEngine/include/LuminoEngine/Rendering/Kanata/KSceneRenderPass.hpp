#pragma once
#include <LuminoCore/Base/LinearAllocator.hpp>
#include <LuminoGraphics/RHI/ShaderInterfaceFramework.hpp>
#include "Common.hpp"

namespace ln {
namespace kanata {

class SceneRenderPass : public URefObject {
public:
    SceneRenderPass(detail::RenderingManager* manager, Shader* fallbackShader);

    void buildDrawCommands(
        detail::SceneRenderer* sceneRenderer,   // TODO: いらなくしたい
        const BatchCollector* batchList,
        GraphicsCommandList* descriptorAllocator,
        RenderPass* renderPass,
        const detail::RenderViewInfo& renderViewInfo,
        const detail::SceneInfo& sceneInfo,
        DrawCommandList* drawCommandList);

    Shader* fallbackShader() const { return m_fallbackShader; }

private:
    detail::RenderingManager* m_manager;
    Shader* m_fallbackShader;
};

} // namespace kanata
} // namespace ln

