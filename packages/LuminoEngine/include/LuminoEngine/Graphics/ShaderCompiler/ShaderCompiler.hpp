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
class ModuleInfo;

class ShaderCompiler : public URefObject {
public:
    static Result<URef<ShaderCompiler>> create();

    MaybeResult build(const fs::path& inputFilePath);

private:
    ShaderCompiler();
    ~ShaderCompiler() override;
    MaybeResult init();
    MaybeResult buildModule();
    MaybeResult buildTarget(slang::IComponentType* program, ShaderTarget target, int targetIndex);
    MaybeResult buildEntryPoint(slang::IComponentType* program, int targetIndex, int entryPointIndex);
    MaybeResult buildTargetInfoSPIRV(slang::ProgramLayout* layout, ModuleInfo* moduleInfo);

    Slang::ComPtr<slang::IGlobalSession> m_globalSession;
    slang::IModule* m_module;
    fs::path m_inputFilePath;
    fs::path m_inputDirPath;
    fs::path m_dumpDirPath;
    bool m_dump;
    URef<UnifiedShader2> m_shader;
};

} // namespace kokage
} // namespace ln

#endif // LN_USE_SLANG
