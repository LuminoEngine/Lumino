
#include "Internal.hpp"
#include <LuminoGraphics/RHI/Shader.hpp>
#include <LuminoEngine/Rendering/Shaders.hpp>
#include "RenderingManager.hpp"

namespace ln {

//==============================================================================
// Shaders

Shader* Shaders::standard()
{
	return detail::EngineDomain::renderingManager()->builtinShader(detail::BuiltinShader::ClusteredShadingDefault);
}

Shader* Shaders::unlit()
{
	return detail::EngineDomain::renderingManager()->builtinShader(detail::BuiltinShader::Sprite);
}

} // namespace ln

