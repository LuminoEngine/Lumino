
#pragma once
#ifdef LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER

#include <LuminoEngine/Graphics/RenderState.hpp>
#include <LuminoEngine/Engine/Diagnostics.hpp>

namespace ln {
class Token;

namespace detail {
class GraphicsManager;

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
    std::string surfaceShader;	// for Lumino HLSL
	std::string shadingModel;	// for Lumino HLSL
	std::string ligitingModel;	// for Lumino HLSL

    // RenderTargetBlendDesc
    Optional<bool> blendEnable;
    Optional<BlendFactor> sourceBlend;
    Optional<BlendFactor> destinationBlend;
    Optional<BlendOp> blendOp;
    Optional<BlendFactor> sourceBlendAlpha;
    Optional<BlendFactor> destinationBlendAlpha;
    Optional<BlendOp> blendOpAlpha;

    // RasterizerStateDesc
    Optional<FillMode> fillMode;
    Optional<CullingMode> cullMode;

    // DepthStencilStateDesc
    Optional<bool> depthTestEnabled;
    Optional<bool> depthWriteEnabled;

    // StencilOpDesc
    Optional<bool> stencilEnabled;
    Optional<uint8_t> stencilReferenceValue;
    Optional<StencilOp> stencilFailOp;
    Optional<StencilOp> stencilDepthFailOp;
    Optional<StencilOp> stencilPassOp;
    Optional<ComparisonFunc> stencilFunc;



    void save(BinaryWriter* w, int version);
    void load(BinaryReader* r, int version);
    //void save(JsonWriter* w, int version);
    //void load(JsonReader* r, int version);

};

struct HLSLTechnique
{
	std::string	name;
	std::vector<HLSLPass>	passes;
	size_t blockBegin = 0;	// "technique"
	size_t blockEnd = 0;	// next to "}"

    void save(BinaryWriter* w, int version);
    void load(BinaryReader* r, int version);
    //void save(JsonWriter* w, int version);
    //void load(JsonReader* r, int version);

    static void writeString(BinaryWriter* w, const std::string& str);
    static std::string readString(BinaryReader* r);

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

    template<typename TValue, typename TParser>
    bool parseStateValue(const std::string& token, Optional<TValue>* outValue, TParser parser)
    {
        TValue v;
        if (parser(token, &v)) {
            *outValue = v;
            return true;
        }
        else {
            m_diag->reportError(u"Parser error: " + String::fromStdString(token));
            return false;
        }
    }

    DiagnosticsManager* m_diag;
	const char* m_code;
	size_t m_codeLength;
	Ref<List<Token>> m_tokens;
	int m_current;
	bool m_isLuminoShader;
};

} // namespace detail
} // namespace ln

#endif // LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER
