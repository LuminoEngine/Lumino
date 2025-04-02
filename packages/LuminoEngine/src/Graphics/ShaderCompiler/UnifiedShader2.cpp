#include "Internal.hpp"
#include <LuminoEngine/Graphics/ShaderCompiler/ShaderHelper.hpp>
#include <LuminoEngine/Graphics/ShaderCompiler/UnifiedShader2.hpp>

namespace ln {
namespace kokage {

UnifiedShader2::UnifiedShader2() {

}

ModuleInfo* UnifiedShader2::addModuleInfo() {
    auto info = makeURef<ModuleInfo>();
    m_moduleInfos.push_back(std::move(info));
    return m_moduleInfos.back().get();
}

} // namespace kokage
} // namespace ln
