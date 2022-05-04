#pragma once
#include <LuminoCore/Base/LinearAllocator.hpp>
#include <LuminoGraphics/RHI/ShaderInterfaceFramework.hpp>
#include "Common.hpp"

namespace ln {
namespace kanata {

class SceneRenderPass : public URefObject {
public:
    SceneRenderPass(
        detail::RenderingManager* manager,
        Shader* fallbackShader,
        kokage::ShaderTechniqueClass_Phase phase);

    std::function<void(DrawCommand*)> overrideCommand;

    void buildDrawCommands(
        detail::SceneRenderer* sceneRenderer,   // TODO: いらなくしたい
        const BatchCollector* batchList,
        GraphicsCommandList* descriptorAllocator,
        RenderPass* renderPass,
        const detail::RenderViewInfo& renderViewInfo,
        const detail::SceneInfo& sceneInfo,
        DrawCommandList* drawCommandList);

    Shader* fallbackShader() const { return m_fallbackShader; }

    ShaderTechnique* getShaderTechnique(Shader* fallbackShader, const Batch* batch, const BatchElement* batchElement);

private:
    detail::RenderingManager* m_manager;
    Shader* m_fallbackShader;
    kokage::ShaderTechniqueClass_Phase m_phase;
};

} // namespace kanata
} // namespace ln

