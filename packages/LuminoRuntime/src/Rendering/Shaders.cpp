
#include "Internal.hpp"
#include <LuminoEngine/GPU/Shader.hpp>
#include <LuminoEngine/Rendering/Shaders.hpp>
#include <LuminoEngine/Rendering/detail/RenderingManager.hpp>

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

