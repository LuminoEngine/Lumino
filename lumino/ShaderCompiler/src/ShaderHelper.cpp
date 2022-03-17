
#include "Internal.hpp"
#include <LuminoShaderCompiler/ShaderHelper.hpp>
#include "UnifiedShader.hpp"
#include "UnifiedShaderCompiler.hpp"
#include <LuminoShaderCompiler/detail/ShaderManager.hpp>

namespace ln {
namespace detail {

//==============================================================================
// ShaderTechniqueClass

void ShaderTechniqueClass::parseTechniqueClassString(const String& str, ShaderTechniqueClass* outClassSet)
{
    outClassSet->defaultTechnique = false;
    //outClassSet->phase = ShaderTechniqueClass_Phase::Forward;
    outClassSet->meshProcess = ShaderTechniqueClass_MeshProcess::StaticMesh;
    outClassSet->shadingModel = ShaderTechniqueClass_ShadingModel::Default;
    outClassSet->drawMode = ShaderTechniqueClass_DrawMode::Primitive;
    if (String::compare(str, _TT("Default"), CaseSensitivity::CaseInsensitive) == 0)
    {
        outClassSet->defaultTechnique = true;
    }
    else if (String::compare(str, _TT("LightDisc"), CaseSensitivity::CaseInsensitive) == 0)
    {
        outClassSet->phase = ShaderTechniqueClass_Phase::LightDisc;
    }
    else
    {
        // TODO: splitRef
        auto tokens = str.split(_TT("_"), StringSplitOptions::RemoveEmptyEntries);
        for (auto& token : tokens) {
            if (String::compare(token, _TT("SkinnedMesh"), CaseSensitivity::CaseInsensitive) == 0) {
                outClassSet->meshProcess = ShaderTechniqueClass_MeshProcess::SkinnedMesh;
            }
            else if (String::compare(token, _TT("UnLighting"), CaseSensitivity::CaseInsensitive) == 0) {
                outClassSet->shadingModel = ShaderTechniqueClass_ShadingModel::Unlit;
            }
            else if (String::compare(token, _TT("Instancing"), CaseSensitivity::CaseInsensitive) == 0) {
                outClassSet->drawMode = ShaderTechniqueClass_DrawMode::Instancing;
            }
        }
    }
}

bool ShaderTechniqueClass::equals(const ShaderTechniqueClass& a, const ShaderTechniqueClass& b)
{
    return a.phase == b.phase &&
        a.meshProcess == b.meshProcess &&
        a.shadingModel == b.shadingModel &&
        a.drawMode == b.drawMode &&
        a.normalClass == b.normalClass &&
        a.roughnessClass == b.roughnessClass;
}




//==============================================================================
// ShaderHelper

// https://www.khronos.org/registry/vulkan/specs/1.0-wsi_extensions/html/vkspec.html#interfaces-resources
// https://stackoverflow.com/questions/45638520/ubos-and-their-alignments-in-vulkan
// https://qiita.com/hoboaki/items/b188c4495f4708c19002
bool ShaderHelper::resolveStd140Layout(const ShaderUniformInfo& info, size_t* outAligndElemenSize)
{
    size_t aligndElemenSize = 0;
    switch (info.type)
    {
    case ln::detail::ShaderUniformType_Unknown:
        return false;
    case ln::detail::ShaderUniformType_Bool:
    case ln::detail::ShaderUniformType_Int:
    case ln::detail::ShaderUniformType_Float:
        aligndElemenSize = 4;
        if (info.arrayElements > 0) {   // 配列の場合は 16byte alignment
            aligndElemenSize = 16;
        }
        break;
    case ln::detail::ShaderUniformType_Vector:
        if (info.vectorElements == 2) {
            aligndElemenSize = 8;
        }
        else if (info.vectorElements == 3 || info.vectorElements == 4) {
            aligndElemenSize = 16;
        }
        else {
            LN_NOTIMPLEMENTED();
            return false;
        }
        if (info.arrayElements > 0) {   // 配列の場合は 16byte alignment
            aligndElemenSize = 16;
        }
        break;
    case ln::detail::ShaderUniformType_Matrix:
    {
        // https://www.khronos.org/opengl/wiki/Data_Type_(GLSL)
        // matNxM : N=columns, M=rows
        // major 方向 (vecX) のサイズにかかわらず、minor 方向は配列として扱われるので、16byte アライメントになる。
        size_t minorSize = 0;
        if (1) {  // row-major
            minorSize = info.matrixColumns;
        }
        else {      // column-major
            minorSize = info.matrixRows;
        }
        aligndElemenSize = minorSize * 16;
        break;
    }
    case ln::detail::ShaderUniformType_Texture:
        LN_UNREACHABLE();
        return false;
    default:
        LN_UNREACHABLE();
        return false;
    }

    *outAligndElemenSize = aligndElemenSize;
    return true;
}

bool ShaderHelper::buildShader(const ln::Path& inputFile, const ln::Path& outputFile, const ln::Path& exportDir)
{
    auto diag = ln::makeObject<ln::DiagnosticsManager>();

    auto result = ln::detail::ShaderHelper::generateShader(ShaderManager::instance(), inputFile, outputFile, exportDir, diag);

    diag->dumpToLog();

    bool result2 = ((!result) || diag->hasError());

    return result;
}

bool ShaderHelper::generateShader(ln::detail::ShaderManager* manager, const ln::Path& inputFile, const ln::Path& outputFile, const ln::Path& exportDir, ln::DiagnosticsManager* diag)
{
#ifdef LN_BUILD_EMBEDDED_SHADER_TRANSCOMPILER
    ln::Path inputFilePath = inputFile.canonicalize();
    ln::Path outputFilePath = outputFile;
    if (outputFilePath.isEmpty()) {
        outputFilePath = inputFilePath.replaceExtension(ln::detail::UnifiedShader::FileExt);
    }

    ln::List<ln::Path> includeDirectories = { inputFilePath.parent() };
    ln::List<ln::String> definitions;

    auto inputCodeBuffer = ln::FileSystem::readAllBytes(inputFilePath);
    char* inputCode = (char*)inputCodeBuffer.data();
    size_t inputCodeLength = inputCodeBuffer.size();


    ln::detail::UnifiedShaderCompiler compiler(manager, diag);
    if (!compiler.compile(inputCode, inputCodeLength, includeDirectories, definitions)) {
        return false;
    }
    if (!compiler.link()) {
        return false;
    }


    if (!compiler.unifiedShader()->save(outputFilePath)) {
        return false;
    }

    // dump intermediate codes.
    if (!exportDir.isEmpty()) {
        ln::FileSystem::createDirectory(exportDir);
        compiler.unifiedShader()->saveCodes(exportDir.str() + _TT("/"));
    }

    return true;
#else
    LN_NOTIMPLEMENTED();
    return false;
#endif
}

} // namespace detail
} // namespace ln
