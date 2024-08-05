#pragma once
#include <LuminoCore/Base/LinearAllocator.hpp>
#include <LuminoEngine/GPU/ShaderInterfaceFramework.hpp>
#include "Common.hpp"

namespace ln {
namespace kanata {

class SceneRenderPass : public Object {
public:
    SceneRenderPass(
        detail::RenderingManager* manager,
        Shader* fallbackShader,
        kokage::ShaderTechniqueClass_Phase phase);

    std::function<void(DrawEvent*)> overrideCommand;

    void buildDrawEvents(
        detail::SceneRenderer* sceneRenderer,   // TODO: いらなくしたい
        const BatchCollector* batchList,
        GraphicsCommandList* descriptorAllocator,
        RenderPass* renderPass,
        const detail::RenderViewInfo& renderViewInfo,
        const detail::SceneInfo& sceneInfo,
        DrawEventList* drawEventList);

    Shader* fallbackShader() const { return m_fallbackShader; }

    ShaderTechnique* getShaderTechnique(Shader* fallbackShader, const Batch* batch, const BatchElement* batchElement);

private:
    detail::RenderingManager* m_manager;
    Shader* m_fallbackShader;
    kokage::ShaderTechniqueClass_Phase m_phase;
};

} // namespace kanata
} // namespace ln

