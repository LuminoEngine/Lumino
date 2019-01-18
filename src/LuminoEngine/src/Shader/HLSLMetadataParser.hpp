
#pragma once
#ifdef LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER

#include <LuminoEngine/Engine/Diagnostics.hpp>
#include "../Grammar/Token.hpp"

namespace ln {
class Token;

namespace detail {
class GraphicsManager;
class ShaderRenderState;

struct HLSLPass
{
    std::string name;
    std::string vertexShader;  // for Raw HLSL
    std::string pixelShader;   // for Raw HLSL
    std::string surfaceShader; // for Lumino HLSL
    std::string shadingModel;  // for Lumino HLSL
    std::string ligitingModel; // for Lumino HLSL

    Ref<ShaderRenderState> renderState;

    void save(BinaryWriter* w, int version);
    void load(BinaryReader* r, int version);
    //void save(JsonWriter* w, int version);
    //void load(JsonReader* r, int version);
};

struct HLSLTechnique
{
    std::string name;
    std::vector<HLSLPass> passes;
    size_t blockBegin = 0; // "technique"
    size_t blockEnd = 0;   // next to "}"

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
    bool parsePass(HLSLPass* pass);
    bool parseRenderState(HLSLPass* pass);

    template<typename TValue, typename TParser>
    bool parseStateValue(const std::string& token, Optional<TValue>* outValue, TParser parser)
    {
        TValue v;
        if (parser(token, &v)) {
            *outValue = v;
            return true;
        } else {
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
