﻿
#pragma once
#ifdef LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER

#include <LuminoEngine/Engine/Diagnostics.hpp>
#include "../Grammar/Token.hpp"

namespace ln {
class Token;

namespace detail {
class GraphicsManager;
class ShaderRenderState;

enum class ShaderCodeStage
{
	Vertex,
	Fragment,
};

// シェーダコード１つ分。
// HLSL 入力可能だが、technique 構文はあらかじめ取り除いておく必要がある。
class ShaderCodeTranspiler
{
public:
	static void initializeGlobals();
	static void finalizeGlobals();

    ShaderCodeTranspiler();

	bool parseAndGenerateSpirv(
		ShaderCodeStage stage, const char* code, size_t length, const std::string& entryPoint,
		const List<Path>& includeDir, const List<String>* definitions, DiagnosticsManager* diag);

	std::string generateGlsl(uint32_t version, bool es);


private:
	ShaderCodeStage m_stage;
	std::vector<uint32_t> m_spirvCode;
};

} // namespace detail
} // namespace ln

#endif // LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER