// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once

#include "Common.hpp"

#ifdef LUMINO_USE_SLANG
#include <filesystem>
#include <slang.h>
#include <slang-com-ptr.h>

namespace ln {
namespace shader {
namespace fs = std::filesystem;
class UnifiedShader2;
class WgslValidator;
struct GlobalShaderPass2;
struct TargetEntryPoint2;

class ShaderCompiler2 final {
public:
    static Result<std::unique_ptr<ShaderCompiler2>> create();

    VoidResult build(const fs::path& inputFilePath);
    UnifiedShader2* shader() const { return m_shader.get(); }

    void setDumpEnabled(bool enabled) { m_dump = enabled; }
    void addSearchPath(const fs::path& path) { m_searchPaths.push_back(path); }

    /**
     * 生成した WGSL を Dawn で検証するかどうか (既定: 検証可能なビルドでは有効)。
     *
     * Slang は WGSL 固有の制約 (textureSample を uniform control flow から呼ぶ等) を
     * 検査しないため、これを無効にすると WebGPU 実行時まで不正に気づけなくなる。
     * @see WgslValidator
     */
    void setWgslValidationEnabled(bool enabled) { m_validateWgsl = enabled; }
    bool isWgslValidationEnabled() const { return m_validateWgsl; }

    ~ShaderCompiler2();

private:
    ShaderCompiler2();
    VoidResult init();
    VoidResult buildModule();
    VoidResult buildParameterBlocks(int targetIndex);
    VoidResult buildTarget(ShaderTarget target, int targetIndex);
    VoidResult buildEntryPoint(ShaderTarget target, int targetIndex, int entryPointIndex);
    VoidResult buildTargetShaderPass(ShaderTarget target, GlobalShaderPass2* globalShaderPass);

    // Slang の GLSL 出力は Vulkan 風 GLSL で ESSL ではないため、GLSL ES 300 は Slang の
    // ターゲットとしては扱わず、ビルド済みの SPIRV ターゲットを SPIRV-Cross に通して作る。
    VoidResult buildGlslEs300Target();
    VoidResult buildGlslEs300EntryPoint(const TargetEntryPoint2* spirvEntryPoint);

    void dumpCode(ShaderTarget target, const std::string& name, const void* data, size_t size);

    void traverseVariableSemantic(
        slang::VariableLayoutReflection* var,
        const std::function<void(slang::VariableLayoutReflection* var)>& callback);

    VoidResult mergeTargetBindingLayouts();

    /**
     * 生成された WGSL を検証する。不正があればエラーを返す。
     * 検証器が使えないビルドでは何もせず成功を返す。
     */
    VoidResult validateWgsl(const std::string& entryPointName, const char* code, size_t length);

    /** 検証に失敗した WGSL をファイルへ書き出す。書き出せた場合はそのパスを返す。 */
    fs::path dumpFailedWgsl(const std::string& entryPointName, const char* code, size_t length);

    Result<VertexInputAttribute> makeVertexInputAttribute(
        const std::string& varName,
        const std::string& semanticName,
        int semanticIndex,
        int locationIndex);

    Slang::ComPtr<slang::IGlobalSession> m_globalSession;
    slang::IModule* m_module = nullptr;
    Slang::ComPtr<slang::IComponentType> m_program;
    fs::path m_inputFilePath;
    fs::path m_inputDirPath;
    fs::path m_dumpDirPath;
    bool m_dump;
    std::vector<fs::path> m_searchPaths;
    Ref<UnifiedShader2> m_shader;
    bool m_parameterBlocksBuilt = false;
    bool m_validateWgsl = true;
    std::unique_ptr<WgslValidator> m_wgslValidator;
};

} // namespace shader
} // namespace ln

#endif // LUMINO_USE_SLANG
