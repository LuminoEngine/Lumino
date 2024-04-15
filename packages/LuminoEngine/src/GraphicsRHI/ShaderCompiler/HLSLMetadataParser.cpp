
#ifdef LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER
#include "Internal.hpp"
#include <yaml-cpp/yaml.h>
#include "Lexer/CppLexer.hpp"
#include <LuminoEngine/Graphics/RenderState.hpp>
#include <LuminoEngine/GraphicsRHI/ShaderCompiler/ShaderHelper.hpp>
#include <LuminoEngine/GraphicsRHI/ShaderCompiler/detail/ShaderManager.hpp>
#include "HLSLMetadataParser.hpp"

namespace ln {
namespace kokage {

//==============================================================================
// HLSLPass

//void HLSLPass::save(BinaryWriter* w, int version)
//{
//    HLSLTechnique::writeString(w, name);
//    HLSLTechnique::writeString(w, vertexShader);
//    HLSLTechnique::writeString(w, pixelShader);
//    HLSLTechnique::writeString(w, surfaceShader);
//    HLSLTechnique::writeString(w, shadingModel);
//    HLSLTechnique::writeString(w, ligitingModel);
//    HLSLTechnique::writeString(w, normal);
//
//    //writeOptionalBool(w, renderState->blendEnable);
//    //writeOptionalUInt8(w, renderState->sourceBlend);
//    //writeOptionalUInt8(w, renderState->destinationBlend);
//    //writeOptionalUInt8(w, renderState->blendOp);
//    //writeOptionalUInt8(w, renderState->sourceBlendAlpha);
//    //writeOptionalUInt8(w, renderState->destinationBlendAlpha);
//    //writeOptionalUInt8(w, renderState->blendOpAlpha);
//
//    //writeOptionalUInt8(w, renderState->fillMode);
//    //writeOptionalUInt8(w, renderState->cullMode);
//
//    //writeOptionalUInt8(w, renderState->depthTestFunc);
//    //writeOptionalBool(w, renderState->depthWriteEnabled);
//
//    //writeOptionalBool(w, renderState->stencilEnabled);
//    //writeOptionalUInt8(w, renderState->stencilReferenceValue);
//    //writeOptionalUInt8(w, renderState->stencilFailOp);
//    //writeOptionalUInt8(w, renderState->stencilDepthFailOp);
//    //writeOptionalUInt8(w, renderState->stencilPassOp);
//    //writeOptionalUInt8(w, renderState->stencilFunc);
//}

//void HLSLPass::load(BinaryReader* r, int version)
//{
//    name = HLSLTechnique::readString(r);
//    vertexShader = HLSLTechnique::readString(r);
//    pixelShader = HLSLTechnique::readString(r);
//    surfaceShader = HLSLTechnique::readString(r);
//    shadingModel = HLSLTechnique::readString(r);
//    ligitingModel = HLSLTechnique::readString(r);
//    normal = HLSLTechnique::readString(r);
//
//    //readOptionalBool(r, &renderState->blendEnable);
//    //readOptionalUInt8(r, &renderState->sourceBlend);
//    //readOptionalUInt8(r, &renderState->destinationBlend);
//    //readOptionalUInt8(r, &renderState->blendOp);
//    //readOptionalUInt8(r, &renderState->sourceBlendAlpha);
//    //readOptionalUInt8(r, &renderState->destinationBlendAlpha);
//    //readOptionalUInt8(r, &renderState->blendOpAlpha);
//
//    //readOptionalUInt8(r, &renderState->fillMode);
//    //readOptionalUInt8(r, &renderState->cullMode);
//
//    //readOptionalUInt8(r, &renderState->depthTestFunc);
//    //readOptionalBool(r, &renderState->depthWriteEnabled);
//
//    //readOptionalBool(r, &renderState->stencilEnabled);
//    //readOptionalUInt8(r, &renderState->stencilReferenceValue);
//    //readOptionalUInt8(r, &renderState->stencilFailOp);
//    //readOptionalUInt8(r, &renderState->stencilDepthFailOp);
//    //readOptionalUInt8(r, &renderState->stencilPassOp);
//    //readOptionalUInt8(r, &renderState->stencilFunc);
//}

////==============================================================================
//// HLSLTechnique
//
//void HLSLTechnique::save(BinaryWriter* w, int version)
//{
//    LN_CHECK(passes.size() < 255);
//    writeString(w, name);
//
//    // passes
//    w->writeUInt8(passes.size());
//    for (auto& pass : passes) {
//        pass.save(w, version);
//    }
//}
//
//void HLSLTechnique::load(BinaryReader* r, int version)
//{
//    name = readString(r);
//
//    // passes
//    int count = r->readUInt8();
//    for (int i = 0; i < count; i++) {
//        HLSLPass pass;
//        pass.load(r, version);
//        passes.push_back(std::move(pass));
//    }
//}

void HLSLTechnique::writeString(BinaryWriter* w, const std::string& str) {
    LN_CHECK(str.length() < 255);
    w->writeUInt8(str.length());
    w->write(str.data(), str.length());
}

std::string HLSLTechnique::readString(BinaryReader* r) {
    char buf[255] = { 0 };
    uint8_t len = r->readUInt8();
    if (len == 0) {
        return std::string();
    }
    else {
        r->read(buf, len);
        return std::string(buf, len);
    }
}

//void save(JsonWriter* w, int version)
//{
//    w->writeStartObject();
//    w->writeString(name);
//    w->writeEndObject();
//}
//
//void load(JsonReader* r, int version)
//{
//}

//==============================================================================
// RenderStateParser

class RenderStateParser {
public:
    static bool equals(const std::string& str1, const char* str2, int str2len) {
        if (str1.size() != str2len) return false;
        return StringHelper::compare(str1.c_str(), str1.size(), str2, str2len, str2len, CaseSensitivity::CaseInsensitive) == 0;
    }

    static bool tryParseBool(const std::string& value, bool* outValue) {
        if (equals(value, "true", 4)) {
            *outValue = true;
            return true;
        }
        else if (equals(value, "false", 5)) {
            *outValue = false;
            return true;
        }
        return false;
    }

    static bool tryParseUInt8(const std::string& value, uint8_t* outValue) {
        NumberConversionResult result;
        const char* end;
        *outValue = StringHelper::toInt32(value.c_str(), value.length(), 10, &end, &result);
        return result == NumberConversionResult::Success;
    }

    template<typename TTable, typename TString, typename TValue>
    static bool findHelper(const TTable& table, const TString& value, TValue* outValue) {
        for (auto& i : table) {
            if (equals(value, i.name, i.len)) {
                *outValue = i.value;
                return true;
            }
        }
        return false;
    }

    static bool tryParseBlendOp(const std::string& value, BlendOp* outValue) {
        struct
        {
            const char* name;
            size_t len;
            BlendOp value;
        } table[] = {
            { "Add", 3, BlendOp::Add },
            { "Subtract", 8, BlendOp::Subtract },
            { "ReverseSubtract", 15, BlendOp::ReverseSubtract },
            { "Min", 3, BlendOp::Min },
            { "Max", 3, BlendOp::Max },
        };
        return findHelper(table, value, outValue);
    }

    static bool tryParseBlendFactor(const std::string& value, BlendFactor* outValue) {
        struct
        {
            const char* name;
            size_t len;
            BlendFactor value;
        } table[] = {
            { "Zero", 4, BlendFactor::Zero },
            { "One", 3, BlendFactor::One },
            { "SourceColor", 11, BlendFactor::SourceColor },
            { "InverseSourceColor", 18, BlendFactor::InverseSourceColor },
            { "SourceAlpha", 11, BlendFactor::SourceAlpha },
            { "InverseSourceAlpha", 18, BlendFactor::InverseSourceAlpha },
            { "DestinationColor", 16, BlendFactor::DestinationColor },
            { "InverseDestinationColor", 23, BlendFactor::InverseDestinationColor },
            { "DestinationAlpha", 16, BlendFactor::DestinationAlpha },
            { "InverseDestinationAlpha", 23, BlendFactor::InverseDestinationAlpha },
        };
        return findHelper(table, value, outValue);
    }

    static bool tryParseFillMode(const std::string& value, FillMode* outValue) {
        struct
        {
            const char* name;
            size_t len;
            FillMode value;
        } table[] = {
            { "Solid", 5, FillMode::Solid },
            { "Wireframe", 9, FillMode::Wireframe },
        };
        return findHelper(table, value, outValue);
    }

    static bool tryParseCullingMode(const std::string& value, CullMode* outValue) {
        struct
        {
            const char* name;
            size_t len;
            CullMode value;
        } table[] = {
            { "None", 4, CullMode::None },
            { "Front", 5, CullMode::Front },
            { "Back", 4, CullMode::Back },
        };
        return findHelper(table, value, outValue);
    }

    static bool tryParseComparisonFunc(const std::string& value, ComparisonFunc* outValue) {
        struct
        {
            const char* name;
            size_t len;
            ComparisonFunc value;
        } table[] = {
            { "Never", 5, ComparisonFunc::Never },
            { "Less", 4, ComparisonFunc::Less },
            { "LessEqual", 9, ComparisonFunc::LessEqual },
            { "Greater", 7, ComparisonFunc::Greater },
            { "GreaterEqual", 12, ComparisonFunc::GreaterEqual },
            { "Equal", 5, ComparisonFunc::Equal },
            { "NotEqual", 8, ComparisonFunc::NotEqual },
            { "Always", 6, ComparisonFunc::Always },
        };
        return findHelper(table, value, outValue);
    }

    static bool tryParseStencilOp(const std::string& value, StencilOp* outValue) {
        struct
        {
            const char* name;
            size_t len;
            StencilOp value;
        } table[] = {
            { "Keep", 4, StencilOp::Keep },
            { "Replace", 7, StencilOp::Replace },
        };
        return findHelper(table, value, outValue);
    }
};

//==============================================================================
// HLSLMetadataParser

bool HLSLMetadataParser::parse(const char* code, size_t length, DiagnosticsManager* diag) {
    m_diag = diag;
    m_code = code;
    m_codeLength = length;

    TextDiagnostics textdiag(diag);
    CppLexer lexer;
    lexer.setDiag(&textdiag);
    m_tokens = lexer.makeTokenList(code, length);

    m_current = 0;
    m_isLuminoShader = false;

    parseCompileUnit();

    return true;
}

//Token& HLSLMetadataParser::current()
//{
//	return m_tokens->at(m_current);
//}

const Token& HLSLMetadataParser::current() const {
    return m_tokens->at(m_current);
}

bool HLSLMetadataParser::next() {
    do {
        m_current++;

    } while (!isEof() && isSpaceToken(current()));

    return !isEof();
}

bool HLSLMetadataParser::nextTo(const char* word, int len) {
    do {
        m_current++;
    } while (isSpaceToken(current()));

    if (!equalString(current(), word, len) || isEof()) {
        m_diag->reportError(_TT("Expected ") + String::fromCString(word, len));
        return false;
    }

    return true;

    //} while (
    //    isSpaceToken(current()) ||
    //    !equalString(current(), word, len));

    //return !isEof();
}

bool HLSLMetadataParser::isSpaceToken(const Token& token) const {
    return token.group() == TokenGroup::SpaceSequence ||
           token.group() == TokenGroup::NewLine ||
           token.group() == TokenGroup::Comment;
}

bool HLSLMetadataParser::isEof() const {
    return m_current >= m_tokens->size();
    //return current().group() == TokenGroup::Eof;
}

bool HLSLMetadataParser::equalString(const Token& token, const char* str, size_t len) {
    if (token.length() != len) return false;
    const char* ts = CppLexer::getTokenString(token, m_code, m_codeLength);
    return StringHelper::compare(ts, token.length(), str, len, len, CaseSensitivity::CaseInsensitive) == 0;
    //return strncmp(ts, str, len) == 0;
}

std::string HLSLMetadataParser::getString(const Token& token) {
    const char* ts = CppLexer::getTokenString(token, m_code, m_codeLength);
    return std::string(ts, token.length());
}

bool HLSLMetadataParser::parseCompileUnit() {
    do {
        if (current().group() != TokenGroup::Unknown) {
            if (equalString(current(), "technique", 9)) {
                HLSLTechnique tech;
                if (!parseTechnique(&tech)) return false;
                techniques.push_back(std::move(tech));
            }
        }

    } while (next());

    return true;
}

bool HLSLMetadataParser::parseTechnique(HLSLTechnique* tech) {
    int begin = m_current;

    // 名前
    if (!next()) return false;
    if (current().group() == TokenGroup::Identifier) {
        tech->name = getString(current());
    }
    else {
        LN_NOTIMPLEMENTED();
        return false;
    }

    if (!nextTo('{')) return false;

    bool closed = false;
    while (next()) {
        if (!parseTechniqueMemberList(tech, &closed)) {
            return false;
        }
        if (closed) break;
    }
    if (!closed) return false;

    tech->blockBegin = m_tokens->at(begin).location();
    tech->blockEnd = current().location() + 1;

    return true;
}

bool HLSLMetadataParser::parseTechniqueMemberList(HLSLTechnique* tech, bool* outClosed) {
    if (equalString(current(), "pass", 4)) {
        HLSLPass pass;
        pass.renderState = makeRef<ShaderRenderState>();
        if (!parsePass(&pass)) return false;
        tech->passes.push_back(std::move(pass));
    }
    else if (equalString(current(), "}", 1)) {
        *outClosed = true;
    }
    else {
        if (!parseTechniqueMember(tech)) {
            return false;
        }
    }
    return true;
}

bool HLSLMetadataParser::parseTechniqueMember(HLSLTechnique* tech) {
    const Token& name = current();
    if (!nextTo('=')) return false;
    if (!next()) return false;

    if (equalString(name, "MultiCompile", 12)) {
        std::vector<std::string> tokens;
        while (1) {
            std::string value = getString(current());
            tokens.push_back(value);
            if (!next()) return false;

            if (equalString(current(), ",", 1)) {
                if (!next()) return false;
            }
            else if (equalString(current(), ";", 1)) {
                tech->multiCompiles.push_back(std::move(tokens));
                break;
            }
        }
    }
    else {
        std::string value = getString(current());

        if (equalString(name, "ShadingModel", 12)) {
            const struct {
                const char* name;
                size_t len;
                ShaderTechniqueClass_ShadingModel value;
            } table[] = {
                { "Default", 7, ShaderTechniqueClass_ShadingModel::Default },
                { "Unlit", 5, ShaderTechniqueClass_ShadingModel::Unlit },
            };
            if (!RenderStateParser::findHelper(table, value, &tech->techniqueClass.shadingModel)) {
                m_diag->reportError(_TT("Normal: Invalid value: ") + String::fromStdString(value));
                return false;
            }
        }
        else if (equalString(name, "Phase", 5)) {
            const struct {
                const char* name;
                size_t len;
                ShaderTechniqueClass_Phase value;
            } table[] = {
                { "Forward", 7, ShaderTechniqueClass_Phase::Forward },
                { "LightDisc", 9, ShaderTechniqueClass_Phase::LightDisc },
                { "ShadowCaster", 12, ShaderTechniqueClass_Phase::ShadowCaster },
                { "ForwardGBufferPrepass", 21, ShaderTechniqueClass_Phase::ForwardGBufferPrepass },
            };
            if (!RenderStateParser::findHelper(table, value, &tech->techniqueClass.phase)) {
                m_diag->reportError(_TT("Phase: Invalid value: ") + String::fromStdString(value));
                return false;
            }
        }
        else if (equalString(name, "VertexProcessing", 16)) {
            const struct {
                const char* name;
                size_t len;
                ShaderTechniqueClass_MeshProcess value;
            } table[] = {
                { "Static", 6, ShaderTechniqueClass_MeshProcess::StaticMesh },
                { "Skinned", 7, ShaderTechniqueClass_MeshProcess::SkinnedMesh },
            };
            if (!RenderStateParser::findHelper(table, value, &tech->techniqueClass.meshProcess)) {
                m_diag->reportError(_TT("Normal: Invalid value: ") + String::fromStdString(value));
                return false;
            }
        }
        else if (equalString(name, "Normal", 6)) {
            const struct {
                const char* name;
                size_t len;
                ShaderTechniqueClass_Normal value;
            } table[] = {
                { "Default", 7, ShaderTechniqueClass_Normal::Default },
                { "NormalMap", 9, ShaderTechniqueClass_Normal::NormalMap },
            };
            if (!RenderStateParser::findHelper(table, value, &tech->techniqueClass.normalClass)) {
                m_diag->reportError(_TT("Normal: Invalid value: ") + String::fromStdString(value));
                return false;
            }
        }
        else if (equalString(name, "Roughness", 9)) {
            const struct {
                const char* name;
                size_t len;
                ShaderTechniqueClass_Roughness value;
            } table[] = {
                { "Default", 7, ShaderTechniqueClass_Roughness::Default },
                { "RoughnessMap", 12, ShaderTechniqueClass_Roughness::RoughnessMap },
            };
            if (!RenderStateParser::findHelper(table, value, &tech->techniqueClass.roughnessClass)) {
                m_diag->reportError(_TT("Roughness: Invalid value: ") + String::fromStdString(value));
                return false;
            }
        }
        else if (equalString(name, "Instancing", 10)) {
            // とりあえず値は不問。フィールドがあればONにしてみる。
            tech->techniqueClass.drawMode = ShaderTechniqueClass_DrawMode::Instancing;
        }
        else {
            m_diag->reportError(_TT("Invalid technique parameter: ") + String::fromStdString(getString(name)));
            return false;
        }

        if (!nextTo(';')) return false;
    }


    return true;
}

bool HLSLMetadataParser::parsePass(HLSLPass* pass) {
    // 名前
    if (!next()) return false;
    if (current().group() == TokenGroup::Identifier) {
        pass->name = getString(current());
    }
    else {
        LN_NOTIMPLEMENTED();
        return false;
    }

    if (!nextTo('{')) return false;

    bool closed = false;
    while (next()) {
        if (current().group() == TokenGroup::Identifier) {
            if (!parseRenderState(pass)) return false;
        }
        else if (equalString(current(), "}", 1)) {
            closed = true;
            break;
        }
    }
    if (!closed) return false;

    return true;
}

bool HLSLMetadataParser::parseRenderState(HLSLPass* pass) {
    // TODO:  VertexShader　 = compile vs_3_0 VS_WriteLinearDepth(); とか間違えて書いてしまうことが多いので、警告したい。

    const Token& name = current();
    if (!nextTo('=')) return false;
    if (!next()) return false;

    std::string token = getString(current());

    //--------------------------------------------------
    // Shader code
    if (equalString(name, "VertexShader", 12)) {
        //next();	// skip "compile"
        //next();	// skip "vs_x_x"
        pass->vertexShader = token;
    }
    else if (equalString(name, "PixelShader", 11)) {
        //next();	// skip "compile"
        //next();	// skip "ps_x_x"
        pass->pixelShader = token;
    }
    //} else if (equalString(name, "ShadingModel", 12)) {
    //    pass->shadingModel = token;
    //    m_isLuminoShader = true;
    //} else if (equalString(name, "LigitingModel", 13)) {
    //    pass->ligitingModel = token;
    //    m_isLuminoShader = true;
    //} else if (equalString(name, "SurfaceShader", 13)) {
    //    pass->surfaceShader = token;
    //    m_isLuminoShader = true;
    //}
    //else if (equalString(name, "Normal", 6)) {
    //    pass->normal = token;
    //    m_isLuminoShader = true;
    //}
    //--------------------------------------------------
    // BlendStateDesc
    else if (equalString(name, "BlendEnable", 11)) {
        if (!parseStateValue(token, &pass->renderState->blendEnable, RenderStateParser::tryParseBool)) return false;
    }
    else if (equalString(name, "SourceBlend", 11)) {
        if (!parseStateValue(token, &pass->renderState->sourceBlend, RenderStateParser::tryParseBlendFactor)) return false;
    }
    else if (equalString(name, "DestinationBlend", 16)) {
        if (!parseStateValue(token, &pass->renderState->destinationBlend, RenderStateParser::tryParseBlendFactor)) return false;
    }
    else if (equalString(name, "BlendOp", 7)) {
        if (!parseStateValue(token, &pass->renderState->blendOp, RenderStateParser::tryParseBlendOp)) return false;
    }
    else if (equalString(name, "SourceBlendAlpha", 16)) {
        if (!parseStateValue(token, &pass->renderState->sourceBlendAlpha, RenderStateParser::tryParseBlendFactor)) return false;
    }
    else if (equalString(name, "DestinationBlendAlpha", 21)) {
        if (!parseStateValue(token, &pass->renderState->destinationBlendAlpha, RenderStateParser::tryParseBlendFactor)) return false;
    }
    else if (equalString(name, "BlendOpAlpha", 12)) {
        if (!parseStateValue(token, &pass->renderState->blendOpAlpha, RenderStateParser::tryParseBlendOp)) return false;
    }
    //--------------------------------------------------
    // RasterizerStateDesc
    else if (equalString(name, "FillMode", 8)) {
        if (!parseStateValue(token, &pass->renderState->fillMode, RenderStateParser::tryParseFillMode)) return false;
    }
    else if (equalString(name, "cullMode", 8)) {
        if (!parseStateValue(token, &pass->renderState->cullMode, RenderStateParser::tryParseCullingMode)) return false;
    }
    //--------------------------------------------------
    // DepthStencilStateDesc
    else if (equalString(name, "DepthTestFunc", 13)) {
        if (!parseStateValue(token, &pass->renderState->depthTestFunc, RenderStateParser::tryParseComparisonFunc)) return false;
    }
    else if (equalString(name, "DepthWriteEnabled", 17)) {
        if (!parseStateValue(token, &pass->renderState->depthWriteEnabled, RenderStateParser::tryParseBool)) return false;
    }
    //--------------------------------------------------
    // Stencil
    else if (equalString(name, "StencilEnabled", 14)) {
        if (!parseStateValue(token, &pass->renderState->stencilEnabled, RenderStateParser::tryParseBool)) return false;
    }
    else if (equalString(name, "StencilReferenceValue", 21)) {
        if (!parseStateValue(token, &pass->renderState->stencilReferenceValue, RenderStateParser::tryParseUInt8)) return false;
    }
    else if (equalString(name, "StencilFailOp", 13)) {
        if (!parseStateValue(token, &pass->renderState->stencilFailOp, RenderStateParser::tryParseStencilOp)) return false;
    }
    else if (equalString(name, "StencilDepthFailOp", 18)) {
        if (!parseStateValue(token, &pass->renderState->stencilDepthFailOp, RenderStateParser::tryParseStencilOp)) return false;
    }
    else if (equalString(name, "StencilPassOp", 13)) {
        if (!parseStateValue(token, &pass->renderState->stencilPassOp, RenderStateParser::tryParseStencilOp)) return false;
    }
    else if (equalString(name, "StencilFunc", 11)) {
        if (!parseStateValue(token, &pass->renderState->stencilFunc, RenderStateParser::tryParseComparisonFunc)) return false;
    }

    if (!nextTo(';')) return false;

    return true;
}

//==============================================================================
// ShaderModuleParser

bool ShaderModuleParser::checkHasModuleBlock(const char* code, size_t length) {
    return StringHelper::indexOf(code, length, "@module", 7) >= 0;
}

bool ShaderModuleParser::parse(const char* code, size_t length, DiagnosticsManager* diag) {
    m_diag = diag;
    std::string moduleText = findModuleTextRange(code, length, &moduleBegin, &moduleEnd);

    YAML::Node doc;
    try {
        doc = YAML::Load(moduleText);
    }
    catch (YAML::ParserException& e) {
        m_diag->reportError(ln::format(_TT("YAML: {0}:{1}:{2}"), e.mark.line, e.mark.column, String::fromStdString(e.msg)));
        return false;
    }

    auto techniques = doc["techniques"];
    if (techniques.IsDefined()) {
        if (!readTechniques(techniques)) {
            return false;
        }
    }
    else {
        diag->reportError(_TT("Undefiend 'techniques'."));
        return false;
    }

    return true;
}

std::string ShaderModuleParser::findModuleTextRange(const char* code, size_t length, int* outBegin, int* outEnd) {
    int begin = StringHelper::indexOf(code, length, "@module", 7);
    if (begin < 0) return std::string();

    int end = StringHelper::indexOf(code, length, "@end", 4, begin);
    if (end < 0) return std::string();

    *outBegin = begin;
    *outEnd = end + 4;
    return std::string(code, begin + 7, end - begin - 7);
}

bool ShaderModuleParser::readTechniques(const YAML::Node& techniques) {
    for (const auto& tech : techniques) {
        if (!readTechnique(tech.first.as<std::string>(), tech.second)) {
            return false;
        }
    }

    return true;
}

bool ShaderModuleParser::readTechnique(const std::string& name, const YAML::Node& technique) {
    if (!technique.IsMap()) {
        m_diag->reportError(ln::format(_TT("{0} is not map."), String::fromStdString(name)));
        return false;
    }

    HLSLTechnique tech;
    tech.name = name;

    auto passes = technique["passes"];
    if (!passes.IsDefined()) {
        m_diag->reportError(ln::format(_TT("{0} is don't have `passes`."), String::fromStdString(name)));
        return false;
    }
    if (!passes.IsSequence()) {
        m_diag->reportError(ln::format(_TT("{0}:passes is not sequence."), String::fromStdString(name)));
        return false;
    }

    int iPass = 0;
    for (const auto& passItemNode : passes) {
        HLSLPass passInfo;
        passInfo.renderState = makeRef<ShaderRenderState>();
        if (!readPass(name, iPass, passItemNode, &passInfo)) {
            return false;
        }

        auto itr = std::find_if(tech.passes.begin(), tech.passes.end(), [&](const auto& x) { return x.name == passInfo.name; });
        if (itr != tech.passes.end()) {
            m_diag->reportError(ln::format(_TT("{0} is duplicated name"), String::fromStdString(passInfo.name)));
            return false;
        }

        tech.passes.push_back(std::move(passInfo));
        iPass++;
    }

    techniques.push_back(std::move(tech));
    return true;
}

bool ShaderModuleParser::readPass(const std::string& techName, int passIndex, const YAML::Node& passItemNode, HLSLPass* outPass) {
    if (passItemNode.size() != 1) {
        m_diag->reportError(ln::format(_TT("Pass ({0}[{1}] is invalid pass item."), String::fromStdString(techName), passIndex));
        return false;
    }

    auto passName = passItemNode.begin()->first;
    auto passNode = passItemNode.begin()->second;

    auto vertexShader = passNode["vertexShader"];
    if (!vertexShader.IsDefined()) {
        m_diag->reportError(ln::format(_TT("Pass ({0}[{1}] is don't have `vertexShader`."), String::fromStdString(techName), passIndex));
        return false;
    }
    auto pixelShader = passNode["pixelShader"];
    if (!pixelShader.IsDefined()) {
        m_diag->reportError(ln::format(_TT("Pass ({0}[{1}] is don't have `pixelShader`."), String::fromStdString(techName), passIndex));
        return false;
    }

    outPass->name = passName.as<std::string>();
    outPass->vertexShader = vertexShader.as<std::string>();
    outPass->pixelShader = pixelShader.as<std::string>();

    return true;
}

} // namespace kokage
} // namespace ln

#endif // LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER
