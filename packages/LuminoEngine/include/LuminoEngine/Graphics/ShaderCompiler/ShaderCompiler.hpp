#pragma once
#include "Common.hpp"

#ifdef LN_USE_SLANG
#include <filesystem>
#include "../../../../../../vcpkg/packages/shader-slang_x64-windows/include/slang.h"
#include "../../../../../../vcpkg/packages/shader-slang_x64-windows/include/slang-com-ptr.h"

namespace ln {
namespace kokage {
namespace fs = std::filesystem;
class UnifiedShader2;
class GlobalShaderPass;

class ShaderCompiler final : public URefObject {
public:
    static Result<URef<ShaderCompiler>> create();

    MaybeResult build(const fs::path& inputFilePath);
    const URef<UnifiedShader2>& shader() const { return m_shader; }

private:
    ShaderCompiler();
    ~ShaderCompiler() override;
    MaybeResult init();
    MaybeResult buildModule();
    MaybeResult buildInputResources(int targetIndex);
    MaybeResult buildTarget(ShaderTarget target, int targetIndex);
    MaybeResult buildEntryPoint(ShaderTarget target, int targetIndex, int entryPointIndex);
    MaybeResult buildTargetShaderPass(
        ShaderTarget target, int targetIndex, GlobalShaderPass* globalShaderPass);

    MaybeResult getBindingResourceInfo(
        slang::VariableLayoutReflection* parameter,
        std::string* outName,
        RegisterCategory* outRegisterCategory,
        int* outConstantBufferSize,
        int* outArrayElementCount);

    //MaybeResult buildTargetInputResources(
    //    ShaderTarget target,
    //    int targetIndex,
    //    TargetShaderPass* ownerShaderPass,
    //    ShaderStage2 stage);
    void traverseVariableSemaintic(
        slang::VariableLayoutReflection* var,
        const std::function<void(slang::VariableLayoutReflection* var)>& callback);


    Slang::ComPtr<slang::IGlobalSession> m_globalSession;
    slang::IModule* m_module;
    Slang::ComPtr<slang::IComponentType> m_program;
    fs::path m_inputFilePath;
    fs::path m_inputDirPath;
    fs::path m_dumpDirPath;
    bool m_dump;
    URef<UnifiedShader2> m_shader;
};

} // namespace kokage
} // namespace ln

#endif // LN_USE_SLANG
