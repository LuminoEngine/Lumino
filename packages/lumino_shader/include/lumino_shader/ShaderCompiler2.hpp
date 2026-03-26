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
struct GlobalShaderPass2;

class ShaderCompiler2 final {
public:
    static Result<std::unique_ptr<ShaderCompiler2>> create();

    VoidResult build(const fs::path& inputFilePath);
    UnifiedShader2* shader() const { return m_shader.get(); }

    void setDumpEnabled(bool enabled) { m_dump = enabled; }
    void addSearchPath(const fs::path& path) { m_searchPaths.push_back(path); }

    ~ShaderCompiler2();

private:
    ShaderCompiler2();
    VoidResult init();
    VoidResult buildModule();
    VoidResult buildParameterBlocks(int targetIndex);
    VoidResult buildTarget(ShaderTarget target, int targetIndex);
    VoidResult buildEntryPoint(ShaderTarget target, int targetIndex, int entryPointIndex);
    VoidResult buildTargetShaderPass(
        ShaderTarget target, int targetIndex, GlobalShaderPass2* globalShaderPass);

    void traverseVariableSemantic(
        slang::VariableLayoutReflection* var,
        const std::function<void(slang::VariableLayoutReflection* var)>& callback);

    VoidResult mergeTargetBindingLayouts();

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
};

} // namespace shader
} // namespace ln

#endif // LUMINO_USE_SLANG
