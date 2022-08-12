#include <LuminoGraphics/GPU/Shader.hpp>
#include <LuminoGraphics/Rendering/Kanata/KUnlitRenderPass.hpp>
#include <LuminoGraphics/Rendering/detail/RenderingManager.hpp>

namespace ln {
namespace kanata {

UnlitRenderPass::UnlitRenderPass(detail::RenderingManager* manager)
    : SceneRenderPass(manager, manager->builtinShader(detail::BuiltinShader::Sprite), kokage::ShaderTechniqueClass_Phase::Forward) {
}

} // namespace kanata
} // namespace ln

