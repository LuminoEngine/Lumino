#pragma once
#include "Common.hpp"

#ifdef LN_USE_SLANG
#include <filesystem>
#include "../../../../../../vcpkg/packages/shader-slang_x64-windows/include/slang.h"
#include "../../../../../../vcpkg/packages/shader-slang_x64-windows/include/slang-com-ptr.h"

namespace ln {
namespace kokage {
namespace fs = std::filesystem;

struct Parameter {};


struct Reflection {

};

struct Component {
    std::vector<uint8_t> code;

};

class ShaderCompiler : public URefObject {
public:
    static Result<URef<ShaderCompiler>> create();

    MaybeResult build(const fs::path& inputFilePath);

private:
    ShaderCompiler();
    ~ShaderCompiler() override;
    MaybeResult init();
    MaybeResult buildTarget(const std::string& inputFilePath, SlangCompileTarget target);
    MaybeResult buildSingle(
        slang::IComponentType* linkedProgram,
        int entryPointIndex,
        std::vector<uint8_t>* code);
    
    Slang::ComPtr<slang::IGlobalSession> m_globalSession;
    fs::path m_inputDirPath;
    fs::path m_dumpDirPath;
    bool m_dump;
};

} // namespace kokage
} // namespace ln

#endif // LN_USE_SLANG
