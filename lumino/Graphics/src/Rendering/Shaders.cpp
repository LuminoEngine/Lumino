
#include "Internal.hpp"
#include <LuminoGraphics/GPU/Shader.hpp>
#include <LuminoGraphics/Rendering/Shaders.hpp>
#include <LuminoGraphics/Rendering/detail/RenderingManager.hpp>

namespace ln {

//==============================================================================
// Shaders

Shader* Shaders::standard()
{
    return detail::RenderingManager::instance()->builtinShader(detail::BuiltinShader::ClusteredShadingDefault);
}

Shader* Shaders::unlit()
{
    return detail::RenderingManager::instance()->builtinShader(detail::BuiltinShader::Sprite);
}

} // namespace ln

