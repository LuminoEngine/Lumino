#pragma once
#include <LuminoCore/Base/LinearAllocator.hpp>
#include <LuminoEngine/Graphics/GPU/ShaderInterfaceFramework.hpp>
#include "Common.hpp"

namespace ln {
namespace kanata {

class DrawEventsEncoder : public Object {
public:
    DrawEventsEncoder(detail::RenderingManager* manager);

    std::function<void(DrawEvent*)> overrideCommand;

    void buildDrawEvents(
        const BatchCollector* batchList,
        GraphicsCommandList* descriptorAllocator,
        RenderPass* renderPass,
        const detail::RenderViewInfo& renderViewInfo,
        const detail::SceneInfo& sceneInfo,
        DrawEventList* drawEventList);

    Shader* fallbackShader() const { return m_fallbackShader; }

    ShaderTechnique* getShaderTechnique(Shader* fallbackShader, const Batch* batch, const BatchElement* batchElement);

    static void makeBlendMode(BlendMode mode, RenderTargetBlendDesc* state);

private:
    detail::RenderingManager* m_manager;
    Shader* m_fallbackShader;
    kokage::ShaderTechniqueClass_Phase m_phase;
};

} // namespace kanata
} // namespace ln
