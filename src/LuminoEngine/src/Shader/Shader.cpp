
#include "Internal.hpp"
#include <Lumino/Shader/Shader.hpp>

namespace ln {

Ref<Shader> Shader::create(const StringRef& vertexShaderFilePath, const StringRef& pixelShaderFilePath)
{
	return ln::newObject<Shader>(vertexShaderFilePath, pixelShaderFilePath, ShaderCodeType::RawGLSL);
}

Shader::Shader()
{

}

Shader::~Shader()
{

}

void Shader::initialize()
{
	GraphicsResource::initialize();
}

void Shader::initialize(const StringRef& vertexShaderFilePath, const StringRef& pixelShaderFilePath, ShaderCodeType codeType)
{
}

} // namespace ln
