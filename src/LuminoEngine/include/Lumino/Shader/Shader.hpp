
#pragma once
#include "Common.hpp"
#include "../Graphics/GraphicsResource.hpp"

namespace ln {

enum class ShaderCodeType
{
	RawGLSL,
};

class LN_API Shader
	: public GraphicsResource
{
public:
	static Ref<Shader> create(const StringRef& vertexShaderFilePath, const StringRef& pixelShaderFilePath);

LN_CONSTRUCT_ACCESS:
	Shader();
	virtual ~Shader();
	void initialize();
	void initialize(const StringRef& vertexShaderFilePath, const StringRef& pixelShaderFilePath, ShaderCodeType codeType);

private:
};

} // namespace ln
