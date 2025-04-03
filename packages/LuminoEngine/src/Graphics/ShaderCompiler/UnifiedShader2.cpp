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

EntryPoint* UnifiedShader2::createEntryPoint() {
    int index = m_entryPoints.size();
    auto entryPoint = makeURef<EntryPoint>();
    entryPoint->index = index;
    m_entryPoints.push_back(std::move(entryPoint));
    return m_entryPoints.back().get();
}

std::pair<int, ShaderPass*> UnifiedShader2::createShaderPass() {
    int index = m_shaderPasses.size();
    auto pass = makeURef<ShaderPass>();
    pass->index = index;
    m_shaderPasses.push_back(std::move(pass));
    return std::make_pair(index, m_shaderPasses.back().get());
}

Blob* UnifiedShader2::createBlob() {
    int index = m_blobs.size();
    auto blob = makeURef<Blob>();
    blob->index = index;
    m_blobs.push_back(std::move(blob));
    return m_blobs.back().get();
}

} // namespace kokage
} // namespace ln
