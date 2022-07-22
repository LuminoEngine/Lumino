
#pragma once
#ifdef LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER

#include <LuminoEngine/Engine/Diagnostics.hpp>
#include <LuminoGraphics/ShaderCompiler/ShaderHelper.hpp>
#include "Lexer/Token.hpp"

namespace YAML {
    class Node;
}

namespace ln {
class Token;

namespace kokage {

struct HLSLPass
{
    std::string name;
    std::string vertexShader;  // for Raw HLSL
    std::string pixelShader;   // for Raw HLSL
    //std::string surfaceShader; // for Lumino HLSL
    //std::string shadingModel;  // for Lumino HLSL
    //std::string ligitingModel; // for Lumino HLSL
    //std::string normal;         // for Lumino HLSL

    Ref<ShaderRenderState> renderState;

    //void save(BinaryWriter* w, int version);
    //void load(BinaryReader* r, int version);
    //void save(JsonWriter* w, int version);
    //void load(JsonReader* r, int version);
};

struct HLSLTechnique
{
    std::string name;
    ShaderTechniqueClass techniqueClass;
    std::vector<std::vector<std::string>> multiCompiles;
    std::vector<HLSLPass> passes;
    size_t blockBegin = 0; // "technique"
    size_t blockEnd = 0;   // next to "}"


    //void save(BinaryWriter* w, int version);
    //void load(BinaryReader* r, int version);
    //void save(JsonWriter* w, int version);
    //void load(JsonReader* r, int version);

    static void writeString(BinaryWriter* w, const std::string& str);
    static std::string readString(BinaryReader* r);
};

class HLSLMetadataParser
{
public:
    std::vector<HLSLTechnique> techniques;

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
    bool parseTechniqueMemberList(HLSLTechnique* tech, bool* outClosed);
    bool parseTechniqueMember(HLSLTechnique* tech);
    bool parsePass(HLSLPass* pass);
    bool parseRenderState(HLSLPass* pass);

    template<typename TValue, typename TParser>
    bool parseStateValue(const std::string& token, Optional_deprecated<TValue>* outValue, TParser parser)
    {
        TValue v;
        if (parser(token, &v)) {
            *outValue = v;
            return true;
        } else {
            m_diag->reportError(_TT("Parser error: ") + String::fromStdString(token));
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

/**
  @module ~ @end
 */
class ShaderModuleParser
{
public:
    int moduleBegin = 0;
    int moduleEnd = 0;

    std::vector<HLSLTechnique> techniques;
    static bool checkHasModuleBlock(const char* code, size_t length);
    bool parse(const char* code, size_t length, DiagnosticsManager* diag);

private:
    std::string findModuleTextRange(const char* code, size_t length, int* outBegin, int* outEnd);
    bool readTechniques(const YAML::Node& techniques);
    bool readTechnique(const std::string& name, const YAML::Node& technique);
    bool readPass(const std::string& techName, int passIndex, const YAML::Node& passItemNode, HLSLPass* outPass);

    DiagnosticsManager* m_diag;
};

} // namespace kokage
} // namespace ln

#endif // LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER
