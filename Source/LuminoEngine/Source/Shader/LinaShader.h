
#pragma once

LN_NAMESPACE_BEGIN

// rawcode → IRCode + technique data
class LinaShaderIRGenerater
{
public:
	LinaShader();

	void loadRawHLSL(const std::string& code);

	std::string generateIRCode();

private:
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
