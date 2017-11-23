
#pragma once

namespace fl { class AnalyzerContext; }
LN_NAMESPACE_BEGIN

//enum class LinaShaderTarget
//{
//	HLSL,
//	IR,
//};

class LinaShaderContext
{
public:
	LinaShaderContext();
	void initialize();

	const std::shared_ptr<fl::AnalyzerContext>& getFlContext() const { return m_flContext; }

	bool findBuiltinShaderCode(const char* pathBegin, const char* pathEnd, const char** codeBegin, const char** codeEnd);

private:

	std::shared_ptr<fl::AnalyzerContext> m_flContext;
};


// RawHLSLCode → IRCode + technique data
// LinaHLSLCode → IRCode + technique data
// ↑ここは自動判別。SurfaceShader の有無で。
class LinaShaderIRGenerater
{
public:
	LinaShaderIRGenerater();
	void initialize(LinaShaderContext* context);
	void finalize();

	// LuminoShader or LazyHLSL ->
	bool convert(const char* input, int len, std::string* outCode, std::string* log);

	bool convertFromRawHLSL(const char* input, int len, std::string* outCode, std::string* log);

	void convertRawHLSL_To_IncludeResolvedHLSLCode(const std::string& code);

	//std::string generateIncludeResolvedHLSLCode();
	//std::string generateIRCode();

private:
	LinaShaderContext* m_context;
};



class LinaShader
{
public:
	LinaShader();

	void loadRawHLSL(const std::string& code);

	// (RawIR:Intermediate Representation)
	// HLSL, GLSL などのコード範囲を示すタグ文字が埋め込まれたコード。
	void loadRawIR(const std::string& code);

	// DirectX API でコンパイル可能な HLSL コードを出力する
	std::string generateHLSLCode();

private:
};

LN_NAMESPACE_END
