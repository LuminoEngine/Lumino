
#pragma once
#include <Lumino/Engine/Diagnostics.hpp>

namespace ln {
class Token;

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
	ShaderCodeStage m_stage;
	std::vector<uint32_t> m_spirvCode;
};


struct HLSLPass
{
	std::string	name;
	std::string vertexShader;	// for Raw HLSL
	std::string pixelShader;	// for Raw HLSL
	std::string shadingModel;	// for Lumino HLSL
	std::string ligitingModel;	// for Lumino HLSL
	std::string surfaceShader;	// for Lumino HLSL
};

struct HLSLTechnique
{
	std::string	name;
	std::vector<HLSLPass>	passes;
};

class HLSLMetadataParser
{
public:
	std::vector<HLSLTechnique>	techniques;

	bool parse(const char* code, size_t length, DiagnosticsManager* diag);

	bool isLuminoShader() const { return m_isLuminoShader; }
	bool isLazyHLSLShader() const { return !isLuminoShader(); }

private:
	//Token& current();
	const Token& current() const;
	bool next();
	bool nextTo(char ch) { return nextTo(&ch, 1); }
	bool nextTo(const char* word, int len);
	bool isSpaceToken(const Token& token) const;
	bool isEof() const;
	bool equalString(const Token& token, const char* str, size_t len);
	std::string getString(const Token& token);

	bool parseCompileUnit();
	bool parseTechnique(HLSLTechnique* tech);
	bool parsePass(HLSLPass* pass);
	bool parseRenderState(HLSLPass* pass);

	const char* m_code;
	size_t m_codeLength;
	Ref<List<Token>> m_tokens;
	int m_current;
	bool m_isLuminoShader;
};

} // namespace detail
} // namespace ln
