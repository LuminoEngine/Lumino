
#pragma once
#include <Lumino/Engine/Diagnostics.hpp>

namespace ln {
namespace detail {
class GraphicsManager;

enum class ShaderCodeStage
{
	Vertex,
	Fragment,
};

class ShaderManager
	: public RefObject
{
public:
	struct Settings
	{
		GraphicsManager* graphicsManager;
	};

	ShaderManager();
	virtual ~ShaderManager();

	void initialize(const Settings& settings);
	void dispose();

	GraphicsManager* graphicsManager() const { return m_graphicsManager; }

private:
	GraphicsManager * m_graphicsManager;
};

// シェーダコード１つ分。
// HLSL 入力可能だが、technique 構文はあらかじめ取り除いておく必要がある。
class ShaderCode
{
public:
	ShaderCode();

	bool parseAndGenerateSpirv(ShaderCodeStage stage, const char* code, size_t length, const std::string& entryPoint, DiagnosticsManager* diag);

	std::string generateGlsl();


private:
	std::vector<uint32_t> m_spirvCode;
};

} // namespace detail
} // namespace ln
