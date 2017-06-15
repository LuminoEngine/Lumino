
#pragma once
#include <Fluorite/Token.h>

LN_NAMESPACE_BEGIN
namespace detail {

// lnfx コードを受け取り、各グラフィックスAPI用のシェーダコードを出力する。
class ShaderAnalyzer
{
public:
	void analyzeLNFX(const char* code, int len);
	void analyzeLNFXC(const char* code, int len);

	std::vector<char> makeHLSLCode() const;

private:
	void parseSimpleShaderMacros(const fl::TokenList* tokenList);

	enum class ShaderCodeType
	{
		SurfaceVertexShader,
		SurfacePixelShader,
	};

	struct WrappedMacroInfo
	{
		int				lastTechniqueKWLoc;
		StringA			funcName;
		ShaderCodeType	type;
	};

	const char*				m_code;
	int						m_codeLength;
	List<WrappedMacroInfo>	m_wrapedShaderFuncList;
};

} // namespace detail
LN_NAMESPACE_END
