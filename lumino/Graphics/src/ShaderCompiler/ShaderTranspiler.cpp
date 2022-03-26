
#ifdef LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER
#define ENABLE_HLSL
#include "Internal.hpp"
#include <sstream>
#include <glad/glad.h>
#include <glslang/Public/ShaderLang.h>
#include <glslang/Include/Types.h>
#include <glslang/Include/BaseTypes.h>
#include <glslang/Include/intermediate.h>
#include <glslang/MachineIndependent/localintermediate.h>
#include <glslang/Include/ResourceLimits.h>
#include <glslang/SPIRV/GlslangToSpv.h>
#include <spirv_cross/spirv_glsl.hpp>
#include "Lexer/CppLexer.hpp"
#include <LuminoGraphics/ShaderCompiler/ShaderHelper.hpp>
#include <LuminoGraphics/ShaderCompiler/detail/ShaderManager.hpp>
#include "ShaderTranspiler.hpp"

#ifdef _WIN32
#include <LuminoGraphics/ShaderCompiler/detail/D3DCompilerAPI.hpp>
#endif

namespace ln {
namespace kokage {

// from glslang: StanAalone/ResourceLimits.cpp

const TBuiltInResource DefaultTBuiltInResource = {
    /* .MaxLights = */ 32,
    /* .MaxClipPlanes = */ 6,
    /* .MaxTextureUnits = */ 32,
    /* .MaxTextureCoords = */ 32,
    /* .MaxVertexAttribs = */ 64,
    /* .MaxVertexUniformComponents = */ 4096,
    /* .MaxVaryingFloats = */ 64,
    /* .MaxVertexTextureImageUnits = */ 32,
    /* .MaxCombinedTextureImageUnits = */ 80,
    /* .MaxTextureImageUnits = */ 32,
    /* .MaxFragmentUniformComponents = */ 4096,
    /* .MaxDrawBuffers = */ 32,
    /* .MaxVertexUniformVectors = */ 128,
    /* .MaxVaryingVectors = */ 8,
    /* .MaxFragmentUniformVectors = */ 16,
    /* .MaxVertexOutputVectors = */ 16,
    /* .MaxFragmentInputVectors = */ 15,
    /* .MinProgramTexelOffset = */ -8,
    /* .MaxProgramTexelOffset = */ 7,
    /* .MaxClipDistances = */ 8,
    /* .MaxComputeWorkGroupCountX = */ 65535,
    /* .MaxComputeWorkGroupCountY = */ 65535,
    /* .MaxComputeWorkGroupCountZ = */ 65535,
    /* .MaxComputeWorkGroupSizeX = */ 1024,
    /* .MaxComputeWorkGroupSizeY = */ 1024,
    /* .MaxComputeWorkGroupSizeZ = */ 64,
    /* .MaxComputeUniformComponents = */ 1024,
    /* .MaxComputeTextureImageUnits = */ 16,
    /* .MaxComputeImageUniforms = */ 8,
    /* .MaxComputeAtomicCounters = */ 8,
    /* .MaxComputeAtomicCounterBuffers = */ 1,
    /* .MaxVaryingComponents = */ 60,
    /* .MaxVertexOutputComponents = */ 64,
    /* .MaxGeometryInputComponents = */ 64,
    /* .MaxGeometryOutputComponents = */ 128,
    /* .MaxFragmentInputComponents = */ 128,
    /* .MaxImageUnits = */ 8,
    /* .MaxCombinedImageUnitsAndFragmentOutputs = */ 8,
    /* .MaxCombinedShaderOutputResources = */ 8,
    /* .MaxImageSamples = */ 0,
    /* .MaxVertexImageUniforms = */ 0,
    /* .MaxTessControlImageUniforms = */ 0,
    /* .MaxTessEvaluationImageUniforms = */ 0,
    /* .MaxGeometryImageUniforms = */ 0,
    /* .MaxFragmentImageUniforms = */ 8,
    /* .MaxCombinedImageUniforms = */ 8,
    /* .MaxGeometryTextureImageUnits = */ 16,
    /* .MaxGeometryOutputVertices = */ 256,
    /* .MaxGeometryTotalOutputComponents = */ 1024,
    /* .MaxGeometryUniformComponents = */ 1024,
    /* .MaxGeometryVaryingComponents = */ 64,
    /* .MaxTessControlInputComponents = */ 128,
    /* .MaxTessControlOutputComponents = */ 128,
    /* .MaxTessControlTextureImageUnits = */ 16,
    /* .MaxTessControlUniformComponents = */ 1024,
    /* .MaxTessControlTotalOutputComponents = */ 4096,
    /* .MaxTessEvaluationInputComponents = */ 128,
    /* .MaxTessEvaluationOutputComponents = */ 128,
    /* .MaxTessEvaluationTextureImageUnits = */ 16,
    /* .MaxTessEvaluationUniformComponents = */ 1024,
    /* .MaxTessPatchComponents = */ 120,
    /* .MaxPatchVertices = */ 32,
    /* .MaxTessGenLevel = */ 64,
    /* .MaxViewports = */ 16,
    /* .MaxVertexAtomicCounters = */ 0,
    /* .MaxTessControlAtomicCounters = */ 0,
    /* .MaxTessEvaluationAtomicCounters = */ 0,
    /* .MaxGeometryAtomicCounters = */ 0,
    /* .MaxFragmentAtomicCounters = */ 8,
    /* .MaxCombinedAtomicCounters = */ 8,
    /* .MaxAtomicCounterBindings = */ 1,
    /* .MaxVertexAtomicCounterBuffers = */ 0,
    /* .MaxTessControlAtomicCounterBuffers = */ 0,
    /* .MaxTessEvaluationAtomicCounterBuffers = */ 0,
    /* .MaxGeometryAtomicCounterBuffers = */ 0,
    /* .MaxFragmentAtomicCounterBuffers = */ 1,
    /* .MaxCombinedAtomicCounterBuffers = */ 1,
    /* .MaxAtomicCounterBufferSize = */ 16384,
    /* .MaxTransformFeedbackBuffers = */ 4,
    /* .MaxTransformFeedbackInterleavedComponents = */ 64,
    /* .MaxCullDistances = */ 8,
    /* .MaxCombinedClipAndCullDistances = */ 8,
    /* .MaxSamples = */ 4,
    /* .maxMeshOutputVerticesNV = */ 256,
    /* .maxMeshOutputPrimitivesNV = */ 512,
    /* .maxMeshWorkGroupSizeX_NV = */ 32,
    /* .maxMeshWorkGroupSizeY_NV = */ 1,
    /* .maxMeshWorkGroupSizeZ_NV = */ 1,
    /* .maxTaskWorkGroupSizeX_NV = */ 32,
    /* .maxTaskWorkGroupSizeY_NV = */ 1,
    /* .maxTaskWorkGroupSizeZ_NV = */ 1,
    /* .maxMeshViewCountNV = */ 4,
    /* .maxDualSourceDrawBuffersEXT = */ 1,

    /* .limits = */ {
        /* .nonInductiveForLoops = */ 1,
        /* .whileLoops = */ 1,
        /* .doWhileLoops = */ 1,
        /* .generalUniformIndexing = */ 1,
        /* .generalAttributeMatrixVectorIndexing = */ 1,
        /* .generalVaryingIndexing = */ 1,
        /* .generalSamplerIndexing = */ 1,
        /* .generalVariableIndexing = */ 1,
        /* .generalConstantMatrixVectorIndexing = */ 1,
    }
};
// from glslang/StandAlone/StandAlone.cpp
// Add things like "#define ..." to a preamble to use in the beginning of the shader.
class TPreamble {
public:
    TPreamble() {
        // see Readme.md
        // GLSL に変換されたとき、GLSL 側がデフォルトのレイアウトになるようにする。
        // → Vukan で使ったとき、Lumino 側で転置する必要がなくなる。
        text.append("#pragma pack_matrix(row_major)\n");
    }

    bool isSet() const { return text.size() > 0; }
    const char* get() const { return text.c_str(); }
    const std::vector<std::string>& prepro() const { return processes; }

    // #define...
    void addDef(std::string def) {
        text.append("#define ");
        fixLine(def);

        processes.push_back("D");
        processes.back().append(def);

        // The first "=" needs to turn into a space
        const size_t equal = def.find_first_of("=");
        if (equal != def.npos)
            def[equal] = ' ';

        text.append(def);
        text.append("\n");
    }

    // #undef...
    void addUndef(std::string undef) {
        text.append("#undef ");
        fixLine(undef);

        processes.push_back("U");
        processes.back().append(undef);

        text.append(undef);
        text.append("\n");
    }

protected:
    void fixLine(std::string& line) {
        // Can't go past a newline in the line
        const size_t end = line.find_first_of("\n");
        if (end != line.npos)
            line = line.substr(0, end);
    }

    std::string text;                   // contents of preamble
    std::vector<std::string> processes; // what should be recorded by OpModuleProcessed, or equivalent
};

//==============================================================================
// LocalIncluder

class LocalIncluder
    : public glslang::TShader::Includer {
public:
    detail::ShaderManager* m_manager;
    const List<Path>* includeDirs;

    virtual IncludeResult* includeSystem(const char* headerName, const char* sourceName, size_t inclusionDepth) {
        for (auto& pair : m_manager->builtinShaderList()) {
            if (pair.first == headerName) {
                return new IncludeResult(headerName, pair.second.c_str(), pair.second.size(), nullptr);
            }
        }

        return nullptr;
    }

    virtual IncludeResult* includeLocal(const char* headerName, const char* sourceName, size_t inclusionDepth) {
        for (auto& dir : (*includeDirs)) {
            auto path = Path(dir, String::fromCString(headerName));
            if (FileSystem::existsFile(path)) {
                ByteBuffer* buf = new ByteBuffer(FileSystem::readAllBytes(path));
                return new IncludeResult(path.str().toStdString(), (const char*)buf->data(), buf->size(), buf);
            }
        }

        return nullptr;
    }

    virtual void releaseInclude(IncludeResult* result) {
        if (result) {
            if (result->userData) {
                delete reinterpret_cast<ByteBuffer*>(result->userData);
            }
            delete result;
        }
    }
};

//==============================================================================
// ShaderCodeTranspiler

struct OpenGLTypeConvertionItem {
    GLenum glType;
    ShaderUniformType lnType;
};
static const OpenGLTypeConvertionItem s_openGLTypeConvertionTable[] = {
    { GL_FLOAT, ShaderUniformType_Float },
    { GL_FLOAT_VEC2, ShaderUniformType_Vector },
    { GL_FLOAT_VEC3, ShaderUniformType_Vector },
    { GL_FLOAT_VEC4, ShaderUniformType_Vector },
    { GL_INT, ShaderUniformType_Int },
    { GL_BOOL, ShaderUniformType_Bool },
    { GL_FLOAT_MAT2, ShaderUniformType_Matrix },
    { GL_FLOAT_MAT3, ShaderUniformType_Matrix },
    { GL_FLOAT_MAT4, ShaderUniformType_Matrix },
    { GL_FLOAT_MAT2x3, ShaderUniformType_Matrix },
    { GL_FLOAT_MAT2x4, ShaderUniformType_Matrix },
    { GL_FLOAT_MAT3x2, ShaderUniformType_Matrix },
    { GL_FLOAT_MAT3x4, ShaderUniformType_Matrix },
    { GL_FLOAT_MAT4x2, ShaderUniformType_Matrix },
    { GL_FLOAT_MAT4x3, ShaderUniformType_Matrix },
    { GL_SAMPLER_2D, ShaderUniformType_Texture }, // HLSL で texture2D 型の uniform を定義すると、glslang からは GL_SAMPLER_XXXX として得られる
    { GL_SAMPLER_CUBE, ShaderUniformType_Texture },
    { GL_SAMPLER_3D, ShaderUniformType_Texture },
};
static bool GLTypeToLNUniformType(GLenum value, const glslang::TType* type, uint16_t* outType) {
    if (value == 0) {
        // see glslang\glslang\Include\Types.h TSampler::getString()
        if (type->getBasicType() == glslang::EbtSampler) {
            const glslang::TSampler& sampler = type->getSampler();
            if (sampler.sampler) {
                *outType = ShaderUniformType_SamplerState;
                return true;
            }
        }
    }
    else {
        for (int i = 0; i < LN_ARRAY_SIZE_OF(s_openGLTypeConvertionTable); i++) {
            if (s_openGLTypeConvertionTable[i].glType == value) {
                *outType = s_openGLTypeConvertionTable[i].lnType;
                return true;
            }
        }
    }
    return false;
}

static EShLanguage LNStageToEShLanguage(ShaderStage2 stage) {
    switch (stage) {
        case ShaderStage2_Vertex:
            return EShLanguage::EShLangVertex;
        case ShaderStage2_Fragment:
            return EShLanguage::EShLangFragment;
        case ShaderStage2_Compute:
            return EShLanguage::EShLangCompute;
        default:
            LN_NOTIMPLEMENTED();
            return EShLanguage::EShLangVertex;
    }
}

void ShaderCodeTranspiler::initializeGlobals() {
    glslang::InitializeProcess();
}

void ShaderCodeTranspiler::finalizeGlobals() {
    glslang::FinalizeProcess();
}

ShaderCodeTranspiler::ShaderCodeTranspiler(detail::ShaderManager* manager)
    : m_manager(manager)
    , m_filename("shadercode")
    , m_stage(ShaderStage2_Vertex) {
}

ShaderCodeTranspiler::~ShaderCodeTranspiler() {
    // この２つは開放順が重要。詳細は TProgram の ヘッダや Standalone.cpp CompileAndLinkShaderUnits の一番下に書いてある。
    m_program.reset();
    m_shader.reset();
}

bool ShaderCodeTranspiler::compileAndLinkFromHlsl(
    ShaderStage2 stage,
    const char* code,
    size_t length,
    const std::string& entryPoint,
    const List<Path>& includeDir,
    const List<String>* definitions,
    DiagnosticsManager* diag) {
    m_diag = diag;
    m_code = std::string(code, length);
    m_stage = stage;
    m_entryPoint = entryPoint;
    m_includeDirectories = includeDir;

    LocalIncluder includer;
    includer.m_manager = m_manager;
    includer.includeDirs = &m_includeDirectories;

    TPreamble preamble;
    if (definitions) {
        for (auto& def : *definitions) {
            const auto s = def.toStdString();
            preamble.addDef(s);
            m_definitions.push_back(s);
        }
    }

    // -d オプション
    //const int defaultVersion = Options & EOptionDefaultDesktop ? 110 : 100;
    const int defaultVersion = 110;

    glslang::EShSource sourceType = glslang::EShSourceHlsl;
    const int ClientInputSemanticsVersion = 410; //320;
    glslang::EshTargetClientVersion OpenGLClientVersion = glslang::EShTargetOpenGL_450;
    bool forwardCompatible = true;
    EShMessages messages = (EShMessages)(EShMsgSpvRules | EShMsgVulkanRules /* | EShMsgReadHlsl*/ | EShMsgHlslDX9Compatible);

    m_shader = std::make_unique<glslang::TShader>(LNStageToEShLanguage(m_stage));
    m_program = std::make_unique<glslang::TProgram>();

    m_shader->setEnvInput(sourceType, LNStageToEShLanguage(m_stage), glslang::EShClientOpenGL, ClientInputSemanticsVersion);
    m_shader->setEnvClient(glslang::EShClientOpenGL, OpenGLClientVersion);
    m_shader->setEntryPoint(entryPoint.c_str());

    // 実際に使われている UBO, Txture, SamplerState などに、自動的に binding 番号を振る。
    // false の場合は、すべて未指定 (-1) となる。
    // 値は m_program->getUniformBinding() で確認できる。
    m_shader->setAutoMapBindings(true);
    m_shader->setAutoMapLocations(true);
    m_shader->setHlslIoMapping(true);

    if (preamble.isSet()) {
        m_shader->setPreamble(preamble.get());
    }
    m_shader->addProcesses(preamble.prepro());

    /* Vulkan Rule
    shader->setEnvInput((Options & EOptionReadHlsl) ? glslang::EShSourceHlsl : glslang::EShSourceGlsl,
    compUnit.stage, glslang::EShClientVulkan, ClientInputSemanticsVersion);
    shader->setEnvClient(glslang::EShClientVulkan, VulkanClientVersion);
    */
    //m_shader->setShiftBinding(glslang::EResUbo, 0);
    //      m_shader->setShiftBinding(glslang::EResSampler, 1);	// sampler state
    //m_shader->setShiftBinding(glslang::EResTexture, 1);	// texture
    //m_shader->setShiftBinding(glslang::EResImage, 1);
    //	m_shader->setShiftBinding(glslang::EResUbo, 1);
    //		m_shader->setShiftBinding(glslang::EResSsbo, 1);
    //			m_shader->setShiftBinding(glslang::EResUav, 1);
    //m_shader->setShiftBindingForSet(glslang::EResUbo, 0, 0);
    //m_shader->setShiftBindingForSet(glslang::EResSampler, 1, 1);

    /*  FIXME: Preprocess と Parse を分けてみる。
        glslang のプリプロセッサに問題があるみたいで、HlslGrammar::acceptStructDeclarationList() の expected("member type"); に引っかかる。
        glslang はマクロ展開とパースを1パスで行っているが、その巻き戻しが上手くいってないようだ。
        ちょっと自分で追うには今は時間が無さすぎるか・・・。

        ``````````
        #define MY_DEF \
            float3 Pos : POSITION; \
            float2 UV : TEXCOORD0;

        struct VS_Input
        {
            MY_DEF; // ここで、"Pos は型じゃないよ" みたいなエラーになる
        };
        ``````````
        
        以下 Issue が少し関係してそう。
        https://github.com/KhronosGroup/glslang/issues/2233
    */
    if (0) {
        // preprocess
        std::string preprocessedCode;
        {
            const char* shaderCode[1] = { code };
            const int shaderLenght[1] = { static_cast<int>(length) };
            const char* shaderName[1] = { "preprocess" };
            glslang::TShader preprocessor(LNStageToEShLanguage(m_stage));
            preprocessor.setStringsWithLengthsAndNames(shaderCode, shaderLenght, shaderName, 1);

            if (!preprocessor.preprocess(&DefaultTBuiltInResource, defaultVersion, EProfile::ENoProfile, false, forwardCompatible, messages, &preprocessedCode, includer)) {
                if (!StringHelper::isNullOrEmpty(preprocessor.getInfoLog())) diag->reportError(String::fromCString(preprocessor.getInfoLog()));
                if (!StringHelper::isNullOrEmpty(preprocessor.getInfoDebugLog())) diag->reportError(String::fromCString(preprocessor.getInfoDebugLog()));
                return false;
            }
        }

        const char* shaderCode[1] = { preprocessedCode.c_str() };
        const int shaderLenght[1] = { static_cast<int>(preprocessedCode.length()) };
        const char* shaderName[1] = { m_filename.c_str() };
        m_shader->setStringsWithLengthsAndNames(shaderCode, shaderLenght, shaderName, 1);

        /* TODO: parse でメモリリークしてるぽい。EShLangFragment の時に発生する。
            Dumping objects ->
            {12053} normal block at 0x06EB1410, 8 bytes long.
                Data: <k       > 6B 0F 00 00 FF FF FF FF
        */
        if (!m_shader->parse(&DefaultTBuiltInResource, defaultVersion, forwardCompatible, messages, includer)) {
            if (!StringHelper::isNullOrEmpty(m_shader->getInfoLog())) diag->reportError(String::fromCString(m_shader->getInfoLog()));
            if (!StringHelper::isNullOrEmpty(m_shader->getInfoDebugLog())) diag->reportError(String::fromCString(m_shader->getInfoDebugLog()));
            return false;
        }
        else if (m_shader->getInfoLog()) {
            if (!StringHelper::isNullOrEmpty(m_shader->getInfoLog())) diag->reportWarning(String::fromCString(m_shader->getInfoLog()));
            if (!StringHelper::isNullOrEmpty(m_shader->getInfoDebugLog())) diag->reportWarning(String::fromCString(m_shader->getInfoDebugLog()));
        }
    }
    else {
        const char* shaderCode[1] = { code };
        const int shaderLenght[1] = { static_cast<int>(length) };
        const char* shaderName[1] = { m_filename.c_str() };
        m_shader->setStringsWithLengthsAndNames(shaderCode, shaderLenght, shaderName, 1);

        /* TODO: parse でメモリリークしてるぽい。EShLangFragment の時に発生する。
            Dumping objects ->
            {12053} normal block at 0x06EB1410, 8 bytes long.
                Data: <k       > 6B 0F 00 00 FF FF FF FF
        */
        if (!m_shader->parse(&DefaultTBuiltInResource, defaultVersion, forwardCompatible, messages, includer)) {
            if (!StringHelper::isNullOrEmpty(m_shader->getInfoLog())) diag->reportError(String::fromCString(m_shader->getInfoLog()));
            if (!StringHelper::isNullOrEmpty(m_shader->getInfoDebugLog())) diag->reportError(String::fromCString(m_shader->getInfoDebugLog()));
            return false;
        }
        else if (m_shader->getInfoLog()) {
            if (!StringHelper::isNullOrEmpty(m_shader->getInfoLog())) diag->reportWarning(String::fromCString(m_shader->getInfoLog()));
            if (!StringHelper::isNullOrEmpty(m_shader->getInfoDebugLog())) diag->reportWarning(String::fromCString(m_shader->getInfoDebugLog()));
        }
    }

    // link
    {
        m_program->addShader(m_shader.get());

        if (!m_program->link(messages)) {
            if (!StringHelper::isNullOrEmpty(m_shader->getInfoLog())) diag->reportError(String::fromCString(m_shader->getInfoLog()));
            if (!StringHelper::isNullOrEmpty(m_shader->getInfoDebugLog())) diag->reportError(String::fromCString(m_shader->getInfoDebugLog()));
            return false;
        }
        else if (m_shader->getInfoLog()) {
            if (!StringHelper::isNullOrEmpty(m_shader->getInfoLog())) diag->reportWarning(String::fromCString(m_shader->getInfoLog()));
            if (!StringHelper::isNullOrEmpty(m_shader->getInfoDebugLog())) diag->reportWarning(String::fromCString(m_shader->getInfoDebugLog()));
        }
    }

    ShaderStageFlags stageFlags = ShaderStageFlags_None;
    switch (stage) {
        case ShaderStage2_Vertex:
            stageFlags = ShaderStageFlags_Vertex;
            break;
        case ShaderStage2_Fragment:
            stageFlags = ShaderStageFlags_Pixel;
            break;
        case ShaderStage2_Compute:
            stageFlags = ShaderStageFlags_Compute;
            break;
        default:
            LN_UNREACHABLE();
            break;
    }

    m_program->buildReflection();

    // get input attributes
    {
        m_attributes.clear();

        for (int iAttr = 0; iAttr < m_program->getNumLiveAttributes(); iAttr++) {
            auto name = m_program->getAttributeName(iAttr);
            const glslang::TType* tt = m_program->getAttributeTType(iAttr);
            const glslang::TQualifier& qual = tt->getQualifier();

            VertexInputAttribute attr;
            int keywordLen = 0;
            if (StringHelper::compare(qual.semanticName, "POSITION", 8, ln::CaseSensitivity::CaseInsensitive) == 0) {
                attr.usage = AttributeUsage_Position;
                keywordLen = 8;
            }
            else if (StringHelper::compare(qual.semanticName, "BLENDWEIGHT", 11, ln::CaseSensitivity::CaseInsensitive) == 0) {
                attr.usage = AttributeUsage_BlendWeight;
                keywordLen = 11;
            }
            else if (StringHelper::compare(qual.semanticName, "BLENDINDICES", 12, ln::CaseSensitivity::CaseInsensitive) == 0) {
                attr.usage = AttributeUsage_BlendIndices;
                keywordLen = 12;
            }
            else if (StringHelper::compare(qual.semanticName, "NORMAL", 6, ln::CaseSensitivity::CaseInsensitive) == 0) {
                attr.usage = AttributeUsage_Normal;
                keywordLen = 6;
            }
            else if (StringHelper::compare(qual.semanticName, "TEXCOORD", 8, ln::CaseSensitivity::CaseInsensitive) == 0) {
                attr.usage = AttributeUsage_TexCoord;
                keywordLen = 8;
            }
            else if (StringHelper::compare(qual.semanticName, "TANGENT", 7, ln::CaseSensitivity::CaseInsensitive) == 0) {
                attr.usage = AttributeUsage_Tangent;
                keywordLen = 7;
            }
            else if (StringHelper::compare(qual.semanticName, "BINORMAL", 8, ln::CaseSensitivity::CaseInsensitive) == 0) {
                attr.usage = AttributeUsage_Binormal;
                keywordLen = 8;
            }
            else if (StringHelper::compare(qual.semanticName, "COLOR", 5, ln::CaseSensitivity::CaseInsensitive) == 0) {
                attr.usage = AttributeUsage_Color;
                keywordLen = 5;
            }
            else if (StringHelper::compare(qual.semanticName, "SV_INSTANCEID", 10, ln::CaseSensitivity::CaseInsensitive) == 0) {
                attr.usage = AttributeUsage_InstanceID;
                keywordLen = 10;
            }
            else {
                LN_NOTIMPLEMENTED();
            }

            attr.index = atoi(qual.semanticName + keywordLen);
            attr.layoutLocation = qual.layoutLocation;

            m_attributes.push_back(attr);
        }

        // UniformBlocks
        //   ComputeShader の RWStructuredBuffer, StructuredBuffer もこれに含まれる。
        for (int i = 0; i < m_program->getNumLiveUniformBlocks(); i++) {
            DescriptorLayoutItem item;
            item.name = m_program->getUniformBlockName(i);
            item.stageFlags = stageFlags;
            item.binding = -1;
            item.size = m_program->getUniformBlockSize(i);

            const glslang::TObjectReflection& block = m_program->getUniformBlock(i);
            const glslang::TType* type = block.getType();
            const glslang::TQualifier& qualifier = type->getQualifier();

            if (qualifier.declaredBuiltIn == glslang::TBuiltInVariable::EbvStructuredBuffer) {
                // StructuredBuffer は HLSL では t# 以外に指定するとエラーになるので、t# 前提としてかまわない
                descriptorLayout.textureRegister.push_back(std::move(item));
            }
            else if (qualifier.declaredBuiltIn == glslang::TBuiltInVariable::EbvRWStructuredBuffer) {
                descriptorLayout.unorderdRegister.push_back(std::move(item));
            }
            else if (qualifier.storage == glslang::TStorageQualifier::EvqUniform) {
                descriptorLayout.uniformBufferRegister.push_back(std::move(item));
            }
            else {
                LN_NOTIMPLEMENTED();
                return false;
            }
        }

        // ```
        // float4 g_color1;
        // cbuffer ConstBuff : register(b0) { float4 g_color2; };
        // ```
        // というコードがある場合、g_color1 は "g_color1"、"g_color2" は "g_color2" というように、そのままの名前で取得。
        //
        //
        // ```
        // cbuffer ConstBuff1 {
        //     float4 g_pos1;
        //     float4 g_pos2;
        // };
        // cbuffer ConstBuff2 {
        //     float4 g_pos3;
        //     float4 g_pos4;
        // };
        // vsMain() {
        //     return g_pos2;
        // }
        // ```
        // というように g_pos2 だけ使われている場合、
        // - ConstBuff1 は Live となる
        // - ConstBuff2 は Live ではないので getNumLiveUniformBlocks() には含まれない
        // - g_pos1 も Live ではないので getNumLiveUniformVariables には含まれない
        // - ConstBuff1 は size = 32(vec4 2つ分) で Live
        // - g_pos2 はバッファ先頭からのオフセット 16
        // 必要な分の最小限だけ Live になる。ただし、UniformBuffer の領域は維持される。
        //
        // また、これらはシェーダ単位で行われる。OpenGL でいうところの、VertexShader と PixelShader をリンクした後の program ではないので注意。
        //
        for (int i = 0; i < m_program->getNumLiveUniformVariables(); i++) {

            ShaderUniformInfo info;
            info.name = m_program->getUniformName(i);

            const glslang::TType* type = m_program->getUniformTType(i);
            GLenum gltype = m_program->getUniformType(i);
            if (!GLTypeToLNUniformType(gltype, type, &info.type)) {
                diag->reportError(_TT("Invalid type."));
                return false;
            }

            //if (type->isMatrix()) {
            //    auto t = type->getQualifier();
            //    const_cast<glslang::TQualifier&>(type->getQualifier()).layoutMatrix = glslang::ElmColumnMajor;
            //    printf("");
            //}

            info.offset = m_program->getUniformBufferOffset(i);

            info.vectorElements = type->getVectorSize();
            info.arrayElements = m_program->getUniformArraySize(i); // 実際に使っている範囲だけ出てくる。
            info.matrixRows = type->getMatrixRows();
            info.matrixColumns = type->getMatrixCols();

            LN_LOG_VERBOSE("Uniform[{}] : ", i);
            LN_LOG_VERBOSE("  name : {}", info.name);
            LN_LOG_VERBOSE("  type : {}", info.type);
            LN_LOG_VERBOSE("  basicType : {}", type->getBasicTypeString());
            LN_LOG_VERBOSE("  basicString : {}", type->getBasicString());
            LN_LOG_VERBOSE("  offset : {}", info.offset);
            LN_LOG_VERBOSE("  bindingIndex : {}", m_program->getUniformBinding(i)); // cbuffer Global : register(b3) のように書かれると、mapIO しなくても 3 がとれる。
            LN_LOG_VERBOSE("  vectorElements : {}", info.vectorElements);
            LN_LOG_VERBOSE("  arrayElements : {}", info.arrayElements);
            LN_LOG_VERBOSE("  matrixRows : {}", info.matrixRows);
            LN_LOG_VERBOSE("  matrixColumns : {}", info.matrixColumns);

            if (info.type == ShaderUniformType_Texture) {
                DescriptorLayoutItem item;
                item.name = info.name;
                item.stageFlags = stageFlags;
                item.binding = -1;

                // DX12 用の対応。CombinedSamper の場合は t レジスタに対応する s レジスタに SamperState を設定 `しなければならない`
                if (type->getSampler().isCombined()) {
                    descriptorLayout.samplerRegister.push_back(item);
                }

                descriptorLayout.textureRegister.push_back(std::move(item));
            }
            else if (info.type == ShaderUniformType_SamplerState) {
                DescriptorLayoutItem item;
                item.name = info.name;
                item.stageFlags = stageFlags;
                item.binding = -1;
                descriptorLayout.samplerRegister.push_back(std::move(item));
            }
            else {
                const int blockIndex = m_program->getUniformBlockIndex(i);
                if (blockIndex >= 0) {
                    const glslang::TObjectReflection& block = m_program->getUniformBlock(blockIndex);
                    const glslang::TType* type = block.getType();
                    const glslang::TQualifier& qualifier = type->getQualifier();
                    if (qualifier.storage == glslang::TStorageQualifier::EvqUniform) {
                        // 普通の UniformBuffer のみ取り出す
                        descriptorLayout.uniformBufferRegister[blockIndex].members.push_back(info);
                    }
                    else {
                        // RWStructuredBuffer 等に指定されている構造体のメンバ
                    }
                }
                else {
                    // TODO: texture など
                }
            }
        }
    }
    return true;
}

bool ShaderCodeTranspiler::mapIOAndGenerateSpirv(const DescriptorLayout& mergedDescriptorLayout, DiagnosticsManager* diag) {
    // Vulkan で必要となる uniform の set, binding は、HLSL で register を使って指定しないと常に 0 となる。
    // そのまま使うことはできないので、自前で調整を行う。
    //
    // Symbol をすべて traverse して、
    // - set 値を割り振る。
    // - mergedDescriptorLayout に含まれる binding 値をセットする。

    class IntermTraverser : public glslang::TIntermTraverser {
    public:
        const DescriptorLayout* mergedDescriptorLayout;

        virtual void visitSymbol(glslang::TIntermSymbol* symbol) {
            auto name1 = symbol->getName();
            if (name1 == "dstVertices") {
                printf("");
            }
            //std::cout << name1 << std::endl;

            //if (symbol->isMatrix()) {
            //    auto pare = getParentNode();
            //    auto parentS = pare->getAsSymbolNode();
            //    auto parentST = pare->getAsTyped();
            //    //auto t = pare->getBasicType();

            //    auto name1 = symbol->getName();
            //    //auto& name = symbol->getType().getTypeName();
            //    //symbol->getWritableType().getQualifier().layoutMatrix = glslang::ElmColumnMajor;
            //    printf("");

            //}
            //else
            if (symbol->getQualifier().declaredBuiltIn == glslang::EbvRWStructuredBuffer) {
                auto& name = symbol->getType().getTypeName();

                auto itr = std::find_if(
                    mergedDescriptorLayout->unorderdRegister.begin(), mergedDescriptorLayout->unorderdRegister.end(), [&](const DescriptorLayoutItem& x) { return strcmp(x.name.c_str(), name.c_str()) == 0; });
                if (itr != mergedDescriptorLayout->unorderdRegister.end()) {
                    symbol->getWritableType().getQualifier().layoutSet = DescriptorType_UnorderdAccess;
                    symbol->getWritableType().getQualifier().layoutBinding = itr->binding;
                }
                //symbol->getWritableType().getQualifier().layoutMatrix = glslang::ElmColumnMajor;
            }
            else if (symbol->getQualifier().declaredBuiltIn == glslang::EbvStructuredBuffer) {
                auto& name = symbol->getType().getTypeName();

                auto itr = std::find_if(
                    mergedDescriptorLayout->textureRegister.begin(), mergedDescriptorLayout->textureRegister.end(), [&](const DescriptorLayoutItem& x) { return strcmp(x.name.c_str(), name.c_str()) == 0; });
                if (itr != mergedDescriptorLayout->textureRegister.end()) {
                    symbol->getWritableType().getQualifier().layoutSet = DescriptorType_Texture;
                    symbol->getWritableType().getQualifier().layoutBinding = itr->binding;
                }
                //symbol->getWritableType().getQualifier().layoutMatrix = glslang::ElmColumnMajor;
            }
            else if (symbol->getBasicType() == glslang::EbtBlock) {
                auto& name = symbol->getType().getTypeName();

                auto itr = std::find_if(
                    mergedDescriptorLayout->uniformBufferRegister.begin(), mergedDescriptorLayout->uniformBufferRegister.end(), [&](const DescriptorLayoutItem& x) { return strcmp(x.name.c_str(), name.c_str()) == 0; });
                if (itr != mergedDescriptorLayout->uniformBufferRegister.end()) {
                    symbol->getWritableType().getQualifier().layoutSet = DescriptorType_UniformBuffer;
                    symbol->getWritableType().getQualifier().layoutBinding = itr->binding;
                }
                //symbol->getWritableType().getQualifier().layoutMatrix = glslang::ElmColumnMajor;
            }
            else if (symbol->getBasicType() == glslang::EbtSampler) {
                auto& name = symbol->getName();
                auto& sampler = symbol->getType().getSampler();
                if (sampler.type == glslang::EbtVoid && sampler.dim == glslang::EsdNone) {
                    // samplerState
                    auto itr = std::find_if(mergedDescriptorLayout->samplerRegister.begin(), mergedDescriptorLayout->samplerRegister.end(), [&](const DescriptorLayoutItem& x) { return strcmp(x.name.c_str(), name.c_str()) == 0; });
                    if (itr != mergedDescriptorLayout->samplerRegister.end()) {
                        symbol->getWritableType().getQualifier().layoutSet = DescriptorType_SamplerState;
                        symbol->getWritableType().getQualifier().layoutBinding = itr->binding;
                    }
                }
                else {
                    // texture
                    auto itr = std::find_if(mergedDescriptorLayout->textureRegister.begin(), mergedDescriptorLayout->textureRegister.end(), [&](const DescriptorLayoutItem& x) { return strcmp(x.name.c_str(), name.c_str()) == 0; });
                    if (itr != mergedDescriptorLayout->textureRegister.end()) {
                        symbol->getWritableType().getQualifier().layoutSet = DescriptorType_Texture;
                        symbol->getWritableType().getQualifier().layoutBinding = itr->binding;
                    }
                }
            }
        }

        // リテラル
        void visitConstantUnion(glslang::TIntermConstantUnion* node) override {
        }

        bool visitAggregate(glslang::TVisit visit, glslang::TIntermAggregate* node) override {
            //auto name = node->getName();
            //std::cout << name << std::endl;
            return TIntermTraverser::visitAggregate(visit, node);
        }
    };

    IntermTraverser localIntermTraverser;
    localIntermTraverser.mergedDescriptorLayout = &mergedDescriptorLayout;

    glslang::TIntermediate* intermediate = m_program->getIntermediate(LNStageToEShLanguage(m_stage));
    TIntermNode* root = intermediate->getTreeRoot();
    root->traverse(&localIntermTraverser);

    if (!m_program->mapIO()) {
        diag->reportError(String::fromCString(m_program->getInfoLog()));
        LN_ERROR();
        return false;
    }

    m_program->buildReflection();

    glslang::GlslangToSpv(*m_program->getIntermediate(LNStageToEShLanguage(m_stage)), m_spirvCode);

    //m_program->dumpReflection();
    //spirv_cross::CompilerGLSL glsl(m_spirvCode);
    //spirv_cross::ShaderResources resources = glsl.get_shader_resources();

    //{
    //    // UniformBuffers
    //    for (int i = 0; i < m_program->getNumLiveUniformBlocks(); i++)
    //    {
    //        LN_LOG_VERBOSE << "UniformBuffer[" << i << "] : ";
    //        LN_LOG_VERBOSE << "  name : " << m_program->getUniformBlockName(i);
    //        LN_LOG_VERBOSE << "  size : " << m_program->getUniformBlockSize(i);
    //        LN_LOG_VERBOSE << "  bindingIndex : " << m_program->getUniformBlockBinding(i);
    //    }

    //    for (int i = 0; i < m_program->getNumLiveUniformVariables(); i++)
    //    {
    //        const glslang::TType* type = m_program->getUniformTType(i);
    //        GLenum gltype = m_program->getUniformType(i);
    //        uint16_t lnType;
    //        if (!GLTypeToLNUniformType(gltype, type, &lnType)) {
    //            return false;
    //        }

    //        LN_LOG_VERBOSE << "Uniform[" << i << "] : ";
    //        LN_LOG_VERBOSE << "  name : " << m_program->getUniformName(i);
    //        LN_LOG_VERBOSE << "  type : " << lnType;
    //        LN_LOG_VERBOSE << "  basicType : " << type->getBasicTypeString();
    //        LN_LOG_VERBOSE << "  basicString : " << type->getBasicString();
    //        LN_LOG_VERBOSE << "  offset : " << m_program->getUniformBufferOffset(i);
    //        LN_LOG_VERBOSE << "  bindingIndex : " << m_program->getUniformBinding(i);
    //        LN_LOG_VERBOSE << "  vectorElements : " << type->getVectorSize();
    //        LN_LOG_VERBOSE << "  arrayElements : " << m_program->getUniformArraySize(i);
    //        LN_LOG_VERBOSE << "  matrixRows : " << type->getMatrixRows();
    //        LN_LOG_VERBOSE << "  matrixColumns : " << type->getMatrixCols();

    //        int ownerUiformBufferIndex = m_program->getUniformBlockIndex(i);
    //        if (ownerUiformBufferIndex >= 0)
    //        {
    //            LN_LOG_VERBOSE << "  ownerUiformBufferIndex : " << ownerUiformBufferIndex << "(" << m_program->getUniformBlockName(ownerUiformBufferIndex) << ")";
    //        }
    //        else {
    //            // TODO: texture など
    //        }
    //    }

    //}

    //glsl.build_combined_image_samplers();
    //for (auto& remap : glsl.get_combined_image_samplers()) {
    //    // ここで結合するキーワードにに _ を含めないこと。
    //    // 識別子内に連続する _ があると、SPIRV-Cross が内部でひとつの _ に変換するため、不整合が起こることがある。
    //    std::string name = (LN_CIS_PREFIX LN_TO_PREFIX) + glsl.get_name(remap.image_id) + LN_SO_PREFIX + glsl.get_name(remap.sampler_id);
    //    //glsl.set_name(remap.combined_id, name);
    //    //combinedImageSamplerNames.push_back(std::move(name));
    //}

    return true;
}

std::vector<byte_t> ShaderCodeTranspiler::spirvCode() const {
    auto begin = reinterpret_cast<const byte_t*>(m_spirvCode.data());
    auto end = begin + (m_spirvCode.size() * sizeof(uint32_t));
    return std::vector<byte_t>(begin, end);
}

std::vector<byte_t> ShaderCodeTranspiler::generateHlslByteCode() const {
#ifdef _WIN32
    std::vector<std::pair<std::string, std::string>> macroValues;
    std::vector<D3D_SHADER_MACRO> macros;
    macroValues.reserve(m_definitions.size());
    macros.reserve(m_definitions.size() + 1);
    for (const auto& text : m_definitions) {
        const size_t equal = text.find_first_of("=");
        if (equal != text.npos) {
            macroValues.push_back({ std::string(text.c_str(), equal), std::string(text.c_str() + equal + 1) });
        }
        else {
            macroValues.push_back({ text, std::string() });
        }
    }
    for (const auto& value : macroValues) {
        if (value.second.empty()) {
            macros.push_back({ value.first.c_str(), nullptr });
        }
        else {
            macros.push_back({ value.first.c_str(), value.second.c_str() });
        }
    }
    if (!macros.empty()) {
        macros.push_back({ nullptr, nullptr });
    }

    class Includer : public ID3DInclude {
    public:
        detail::ShaderManager* manager;
        const List<Path>* includeDirs;
        std::vector<Ref<ByteBuffer>> cache;

        STDMETHOD(Open)
        (D3D_INCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID* ppData, UINT* pBytes) override {
            switch (IncludeType) {
                case D3D_INCLUDE_SYSTEM: // #include <>
                    for (const auto& pair : manager->builtinShaderList()) {
                        if (pair.first == pFileName) {
                            *ppData = pair.second.c_str();
                            *pBytes = pair.second.size();
                            return S_OK;
                        }
                    }
                    break;
                case D3D_INCLUDE_LOCAL: // #include ""
                    for (const auto& dir : (*includeDirs)) {
                        auto path = Path(dir, String::fromCString(pFileName));
                        if (FileSystem::existsFile(path)) {
                            Ref<ByteBuffer> data(new ByteBuffer(FileSystem::readAllBytes(path)), false);
                            *ppData = data->data();
                            *pBytes = data->size();
                            cache.push_back(data);
                            return S_OK;
                        }
                    }
                    break;
            }
            return E_FAIL;
        }

        STDMETHOD(Close)
        (LPCVOID pData) override {
            return S_OK;
        }
    };
    Includer includer;
    includer.manager = m_manager;
    includer.includeDirs = &m_includeDirectories;

    const char targetVS[] = "vs_5_0";
    const char targetPS[] = "ps_5_0";
    const char targetCS[] = "cs_5_0";
    const char* target = nullptr;
    switch (m_stage) {
        case ShaderStage2_Vertex:
            target = targetVS;
            break;
        case ShaderStage2_Fragment:
            target = targetPS;
            break;
        case ShaderStage2_Compute:
            target = targetCS;
            break;
        default:
            LN_UNREACHABLE();
            return {};
    }

    // TODO: release
    ID3DBlob* shaderCode = nullptr;
    ID3DBlob* error = nullptr;
    UINT flags1 = D3DCOMPILE_PACK_MATRIX_ROW_MAJOR | D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY; // D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR;
    UINT flags2 = 0;
    //printf("D3DCompile2 s\n");
    HRESULT hr = m_manager->D3DCompile2(
        m_code.c_str(), m_code.length(), m_filename.c_str(), (macros.empty()) ? nullptr : macros.data(), &includer, m_entryPoint.c_str(), target, flags1, flags2, 0, nullptr, 0, &shaderCode, &error);
    //printf("D3DCompile2 e\n");

    const char* message = error ? (const char*)error->GetBufferPointer() : nullptr;

    if (FAILED(hr)) {
        if (message) {
            m_diag->reportError(String::fromCString(message));
        }
        else {
            m_diag->reportError(_TT("Unknown compilation error."));
        }
        return {};
    }
    else {
        if (message) {
            m_diag->reportWarning(String::fromCString(message));
        }
    }

    //ComPtr<ID3D12ShaderReflection> shaderReflection;
    //D3DCompilerAPI::D3DReflect(shaderCode->GetBufferPointer(), shaderCode->GetBufferSize(), IID_PPV_ARGS(&shaderReflection));

    //D3D12_SHADER_DESC desc{};
    //shaderReflection->GetDesc(&desc);

    //const auto cbCount = desc.ConstantBuffers;
    //for (auto i = 0; i < cbCount; ++i)
    //{
    //    D3D12_SHADER_BUFFER_DESC shaderBufDesc{};
    //    auto cbuffer = shaderReflection->GetConstantBufferByIndex(i);
    //    cbuffer->GetDesc(&shaderBufDesc);

    //    std::cout << "  " << shaderBufDesc.Name << std::endl;
    //}

    const uint8_t* d = static_cast<const uint8_t*>(shaderCode->GetBufferPointer());
    const size_t s = shaderCode->GetBufferSize();
    return std::vector<byte_t>(d, d + s);

#else
    return {};
#endif
}

std::vector<byte_t> ShaderCodeTranspiler::generateGlsl(uint32_t version, bool es) {
    spirv_cross::CompilerGLSL glsl(m_spirvCode);
    spirv_cross::ShaderResources resources = glsl.get_shader_resources();

    // Set some options.
    spirv_cross::CompilerGLSL::Options options;
    options.version = version;
    options.es = es;
    glsl.set_common_options(options);

    std::vector<std::string> combinedImageSamplerNames;

    // DescriptorSet は Vulkan 固有のものであるため、他のAPIがバインディングを理解できるように再マップする。
    // https://github.com/KhronosGroup/SPIRV-Cross#descriptor-sets-vulkan-glsl-for-backends-which-do-not-support-them-hlslglslmetal
    for (auto& resource : resources.sampled_images) {
        unsigned set = glsl.get_decoration(resource.id, spv::DecorationDescriptorSet);
        unsigned binding = glsl.get_decoration(resource.id, spv::DecorationBinding);

        glsl.unset_decoration(resource.id, spv::DecorationDescriptorSet);
        glsl.set_decoration(resource.id, spv::DecorationBinding, set * 16 + binding);

        combinedImageSamplerNames.push_back(resource.name.c_str());
    }

    // HLSL で register(b0) のような指定をすると binding の値が生成され、 GLSL に出力すると
    //   layout(binding = 0, std140) uniform ConstBuff
    // というようなコードが出力される。
    // しかし、macOS (Mojave) では binding には対応しておらず、
    //   unknown identifer 'binding' in layout
    // というエラーになってしまう。
    // そこで、binding の decoration は削除してしまう。
    // こうしておくと、glsl.compile() で binding 指定を含まない GLSL を生成することができる。
    for (size_t i = 0; i < resources.uniform_buffers.size(); i++) {
        glsl.unset_decoration(resources.uniform_buffers[i].id, spv::DecorationBinding);
    }
    for (size_t i = 0; i < resources.sampled_images.size(); i++) {
        glsl.unset_decoration(resources.sampled_images[i].id, spv::DecorationBinding);
    }

    // HLSL では Texture と SamplerState は独立しているが、GLSL では統合されている。
    // ここでは "キーワード + Texture名 + SamplerState名" というような名前を付けておく。
    // 実行時に GLSLShader の中で uniform を列挙してこの規則の uniform を見つけ、実際の Texture と SamplerState の対応付けを行う。
    {
        // From main.cpp
        // Builds a mapping for all combinations of images and samplers.
        glsl.build_combined_image_samplers();

        // Give the remapped combined samplers new names.
        // Here you can also set up decorations if you want (binding = #N).
        for (auto& remap : glsl.get_combined_image_samplers()) {
            // ここで結合するキーワードにに _ を含めないこと。
            // 識別子内に連続する _ があると、SPIRV-Cross が内部でひとつの _ に変換するため、不整合が起こることがある。
            std::string name = (LN_CIS_PREFIX LN_TO_PREFIX) + glsl.get_name(remap.image_id) + LN_SO_PREFIX + glsl.get_name(remap.sampler_id);
            glsl.set_name(remap.combined_id, name);
            combinedImageSamplerNames.push_back(std::move(name));
        }
    }

    // VertexShader から FragmentShader に渡す頂点データ (いわゆる昔の varying 変数) に同じ名前を付ける。
    {
        /*  TODO: 名前一致対応したい
			今のままだと、頂点シェーダの出力とピクセルシェーダの入力構造体のレイアウトが一致していなければ、
			glUseProgram が INVALID を返すみたいな原因のわかりにくい問題となる。
			glslang がセマンティクスまで理解していればいいが、そうでなければ構造体メンバの名前一致で指定できるようにしたい。
		*/

        if (m_stage == ShaderStage2_Vertex) {
            for (size_t i = 0; i < resources.stage_outputs.size(); i++) {
                std::stringstream s;
                s << "ln_varying_" << i;
                glsl.set_name(resources.stage_outputs[i].id, s.str());
                glsl.set_name(resources.stage_outputs[i].id, s.str());
            }
        }
        else if (m_stage == ShaderStage2_Fragment) {
            for (size_t i = 0; i < resources.stage_inputs.size(); i++) {
                std::stringstream s;
                s << "ln_varying_" << i;
                glsl.set_name(resources.stage_inputs[i].id, s.str());
            }
        }
    }

    // Generate GLSL code.
    std::string code = glsl.compile();

    // テクスチャサンプリング時にレンダリングターゲットであるかを判断し、上下反転するマクロコードを挿入する。
    {

        /*
         * Y-Flip について
         * ----------
         * OpenGL だけは他の API と異なり、テクスチャの左下を (0,0) と考えている。
         * これを他と合わせるアイデアは次の通り。
         * 
         * A. glClipControl(GL_UPPER_LEFT, GL_ZERO_TO_ONE)
         * B. glTexImage2D() で格納する画像データを反転する
         * C. gl_Position.y を反転する (spirv-cross の --flip-vert-y オプション)
         * D. シェーダでテクスチャをサンプリングする際の UV を反転する
         * 
         * A. は OpenGL 4.6 以降でサポートしている、最も新しい API。なので使えない環境もある。
         * というかそもそも WebGL ではサポートされていないようだ。
         * Emscripten でビルドすると、 Undefined reference になる。
         * 
         * B と C は、同じシェーダで通常テクスチャとレンダーターゲットテクスチャを使えるようにする際に障害となる。
         * 通常テクスチャよりはレンダーターゲットをまずは考えるべき。
         * 一番の問題は、Present のソースとなるバックバッファへのレンダリング時は、C による Y 反転は不正であるということ。
         * --flip-vert-y は全シェーダに対して一律有効化、は NG.
         * https://twitter.com/lriki8/status/1276738910517817344
         * レンダーターゲットは OpenGL デフォルトに素直に従うしかない。そしてそのデフォルトは左下を原点とする。
         * HelloTriangle したものを readImage してみると分かるが、Present された結果は ▲ であっても、readImage した結果は ▼ となっている。
         * このため B も NG.
         */
        /*
			DirectX に合わせたテクスチャ座標系(左上が原点)で OpenGL を使おうとすると、
			OpenGL のレンダリングターゲットをサンプリングするときに問題となる。

			普通のテクスチャは glTexImage などで画像を転送するときに上下反転するなど、自分で制御できる。
			しかし、レンダリングターゲットは OpenGL が書き込むため、上下の制御ができない。

			このためシェーダでは、レンダリングターゲットをサンプリングするときに限り上下反転する必要がある。

			bgfx の issue:
			https://github.com/bkaradzic/bgfx/issues/973

			TEXTURE COORDINATES – D3D VS. OPENGL:
			https://www.puredevsoftware.com/blog/2018/03/17/texture-coordinates-d3d-vs-opengl/
			通常テクスチャなら、glTexImage による反転と、サンプリング時の反転により正しく描画できる。

			Unity は内部的にうまいことやってくれているらしい:
			https://forum.unity.com/threads/fix-for-directx-flipping-vertical-screen-coordinates-in-image-effects-not-working.266455/
		*/
        Array<String> lines;
        int versionLineIndex = -1;
        int precisionLineIndex = -1;
        bool hasSamper2D = false;
        bool hasSamper3D = false;

        // 必要なキーワードを探しながら、行ごとの配列を作る
        StringReader reader(ln::String::fromUtf8(code));
        String line;
        int lineIndex = 0;
        while (reader.readLine(&line)) {
            lines.push(line);
            if (line.indexOf(U"#version") == 0) {
                versionLineIndex = lineIndex;
            }
            else if (line.indexOf(U"precision") == 0) {
                precisionLineIndex = lineIndex;
            }

            if (line.contains(U"sampler2D")) {
                hasSamper2D = true;
            }
            if (line.contains(U"sampler3D")) {
                hasSamper3D = true;
            }
            lineIndex++;
        }

        // マクロコードをどこに挿入したい？
        int insertionLineIndex = 0;
        if (precisionLineIndex >= 0) {
            insertionLineIndex = precisionLineIndex + 1;
        }
        else if (versionLineIndex >= 0) {
            insertionLineIndex = versionLineIndex + 1;
        }

        if (hasSamper2D) {
            lines.insert(insertionLineIndex, U"vec4 LN_xxTexture(sampler2D s, vec2 uv) { return texture(s, vec2(uv.x, (uv.y * -1.0) + 1.0)); }");
            insertionLineIndex++;
            lines.insert(insertionLineIndex, U"vec4 LN_xxTextureLod(sampler2D s, vec2 uv, float lod) { return textureLod(s, vec2(uv.x, (uv.y * -1.0) + 1.0), lod); }");
            insertionLineIndex++;
        }
        if (hasSamper3D) {
            lines.insert(insertionLineIndex, U"vec4 LN_xxTexture(sampler3D s, vec3 uv) { return texture(s, vec3(uv.x, (uv.y * -1.0) + 1.0, uv.z)); }");
            insertionLineIndex++;
        }
        lines.insert(insertionLineIndex, U"#define texture(s, uv) LN_xxTexture(s, uv)");
        insertionLineIndex++;
        lines.insert(insertionLineIndex, U"#define textureLod(s, uv, lod) LN_xxTextureLod(s, uv, lod)");
        insertionLineIndex++;

        StringWriter writer;
        for (const auto& line : lines) {
            writer.writeLine(line);
        }

        code = writer.toString().toUtf8();

        //// GLSL ES は言語仕様としては ## をサポートしていないので、ここでプリプロセッサを解決しておく。
        //// https://www.khronos.org/registry/OpenGL/specs/es/3.0/GLSL_ES_Specification_3.00.pdf
        //const char* shaderCode[1] = { code.c_str() };
        //const int shaderLenght[1] = { static_cast<int>(code.length()) };
        //const char* shaderName[1] = { "shadercode" };
        //auto shader = std::make_unique<glslang::TShader>(LNStageToEShLanguage(m_stage));
        //shader->setStringsWithLengthsAndNames(shaderCode, shaderLenght, shaderName, 1);
        //glslang::TShader::ForbidIncluder forbidIncluder;
        //std::string processedCode;
        //bool r = shader->preprocess(&DefaultTBuiltInResource, 300, EProfile::EEsProfile, true, false, EShMessages::EShMsgOnlyPreprocessor, &processedCode, forbidIncluder);
        //code = processedCode;

        //     } else {
        //         code = code.insert(13, declsIsRT + "\n" + "vec4 LN_xxTexture(int isRT, sampler2D s, vec2 uv) { return texture(s, vec2(uv.x, (uv.y * -1.0) + 1.0)); }\n"
        //                                                   "vec4 LN_xxTexture(int isRT, sampler3D s, vec3 uv) { return texture(s, vec3(uv.x, (uv.y * -1.0) + 1.0, uv.z)); }\n"
        //                                                   "#define texture(s, uv) LN_xxTexture(s, uv)\n"
        //                                                   "#line 1\n");
        //     }
    }

    return std::vector<byte_t>(code.begin(), code.end());
}

//==============================================================================
// ShaderUniformBufferInfo

void ShaderUniformBufferInfo::mergeFrom(const ShaderUniformBufferInfo& other) {
    LN_CHECK(name == other.name);
    LN_CHECK(size == other.size);

    for (auto& om : other.members) {
        auto itr = std::find_if(members.begin(), members.end(), [&](const ShaderUniformInfo& x) { return x.name == om.name; });
        if (itr != members.end()) {
            LN_CHECK(itr->name == om.name);
            LN_CHECK(itr->type == om.type);
            LN_CHECK(itr->offset == om.offset);
        }
        else {
            members.push_back(om);
        }
    }

    std::sort(members.begin(), members.end(), [](const ShaderUniformInfo& a, const ShaderUniformInfo& b) { return a.offset < b.offset; });
}

void ShaderUniformBufferInfo::mergeBuffers(
    const std::vector<ShaderUniformBufferInfo>& a,
    const std::vector<ShaderUniformBufferInfo>& b,
    std::vector<ShaderUniformBufferInfo>* out) {
    *out = a;

    for (auto& buffer : b) {
        auto itr = std::find_if(out->begin(), out->end(), [&](const ShaderUniformBufferInfo& x) { return x.name == buffer.name; });
        if (itr != out->end()) {
            itr->mergeFrom(buffer);
        }
        else {
            out->push_back(buffer);
        }
    }
}

} // namespace kokage
} // namespace ln

#endif // LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER
