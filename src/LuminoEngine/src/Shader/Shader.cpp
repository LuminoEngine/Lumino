
#include "Internal.hpp"
#include <Lumino/Shader/Shader.hpp>
#include "../Graphics/GraphicsDeviceContext.hpp"

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
	Shader::initialize();

	auto vsCode = FileSystem::readAllBytes(vertexShaderFilePath);
	auto psCode = FileSystem::readAllBytes(pixelShaderFilePath);

	ShaderCompilationDiag diag;
	auto pass = deviceContext()->createShaderPass(vsCode.data(), vsCode.size(), psCode.data(), psCode.size(), &diag);

	printf("");

}

} // namespace ln
