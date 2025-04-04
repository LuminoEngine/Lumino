#include "Internal.hpp"
#include <LuminoEngine/Graphics/ShaderCompiler/ShaderHelper.hpp>
#include <LuminoEngine/Graphics/ShaderCompiler/UnifiedShader2.hpp>

namespace ln {
namespace kokage {

UnifiedShader2::UnifiedShader2() {
}

InputResourceInfo* UnifiedShader2::createInputResourceInfo() {
    int index = m_inputResourceInfos.size();
    auto info = makeURef<InputResourceInfo>();
    info->index = index;
    m_inputResourceInfos.push_back(std::move(info));
    return m_inputResourceInfos.back().get();
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

TargetShaderPass* UnifiedShader2::createTargetShaderPass() {
    int index = m_targetShaderPasses.size();
    auto pass = makeURef<TargetShaderPass>();
    pass->index = index;
    m_targetShaderPasses.push_back(std::move(pass));
    return m_targetShaderPasses.back().get();
}

GlobalShaderPass* UnifiedShader2::createGlobalShaderPass() {
    int index = m_globalShaderPasses.size();
    auto pass = makeURef<GlobalShaderPass>();
    pass->index = index;
    m_globalShaderPasses.push_back(std::move(pass));
    return m_globalShaderPasses.back().get();
}

Blob* UnifiedShader2::createBlob() {
    int index = m_blobs.size();
    auto blob = makeURef<Blob>();
    blob->index = index;
    m_blobs.push_back(std::move(blob));
    return m_blobs.back().get();
}

Result<GlobalMemberInfo*> UnifiedShader2::getOrCreateGlobalMemberWithVerify(
    std::string name,
    ShaderGlobalMemberType type,
    ShaderGlobalMemberKind kind,
    int32_t arrayElements,
    int32_t vectorElements,
    int32_t matrixRows,
    int32_t matrixColumns) {
    // まずは名前で検索する
    auto itr = std::find_if(
        m_globalMembers.begin(),
        m_globalMembers.end(),
        [&name](const URef<GlobalMemberInfo>& info) { return info->name == name; });
    if (itr != m_globalMembers.end()) {
        GlobalMemberInfo* info = (*itr).get();
        if (info->type != type) {
            return LN_MAKE_ERROR(
                "GlobalMemberInfo already exists with different type (%s)",
                name.c_str());
        }
        if (info->kind != kind) {
            return LN_MAKE_ERROR(
                "GlobalMemberInfo already exists with different kind (%s)",
                name.c_str());
        }
        if (info->arrayElements != arrayElements) {
            return LN_MAKE_ERROR(
                "GlobalMemberInfo already exists with different arrayElements (%s)",
                name.c_str());
        }
        if (info->vectorElements != vectorElements) {
            return LN_MAKE_ERROR(
                "GlobalMemberInfo already exists with different vectorElements (%s)",
                name.c_str());
        }
        if (info->matrixRows != matrixRows) {
            return LN_MAKE_ERROR(
                "GlobalMemberInfo already exists with different matrixRows (%s)",
                name.c_str());
        }
        if (info->matrixColumns != matrixColumns) {
            return LN_MAKE_ERROR(
                "GlobalMemberInfo already exists with different matrixColumns (%s)",
                name.c_str());
        }
        return info;
    }

    // 存在しない場合、新しい GlobalMemberInfo を作成する
    auto info = makeURef<GlobalMemberInfo>();
    info->index = m_globalMembers.size();
    info->name = name;
    info->type = type;
    info->kind = kind;
    info->arrayElements = arrayElements;
    info->vectorElements = vectorElements;
    info->matrixRows = matrixRows;
    info->matrixColumns = matrixColumns;
    m_globalMembers.push_back(std::move(info));
    return m_globalMembers.back().get();
}

Result<InputResourceInfo*> UnifiedShader2::getOrCreateInputResourceWithVerify(
    const std::string& name,
    RegisterCategory category,
    int constantBufferSize,
    int arrayElementCount) {

    // まずは名前で検索する
    auto itr = std::find_if(
        m_inputResourceInfos.begin(),
        m_inputResourceInfos.end(),
        [&name](const URef<InputResourceInfo>& info) { return info->name == name; });
    if (itr != m_inputResourceInfos.end()) {
        // すでに存在する場合、同じ RegisterCategory かつ count が一致するか確認する
        if ((*itr)->category == category) {
            return (*itr).get();
        }
        else {
            return LN_MAKE_ERROR(
                "InputResourceInfo already exists with different typeinfo (%s)",
                name.c_str());
        }
    }

    // 存在しない場合、新しい InputResourceInfo を作成する
    auto info = makeURef<InputResourceInfo>();
    info->name = name;
    info->category = category;
    info->index = m_inputResourceInfos.size();
    m_inputResourceInfos.push_back(std::move(info));
    return m_inputResourceInfos.back().get();
}

Result<EntryPoint*> UnifiedShader2::getEntryPoint(
    ShaderTarget target, const std::string& name) const {
    auto itr = std::find_if(
        m_entryPoints.begin(),
        m_entryPoints.end(),
        [&target, &name](const URef<EntryPoint>& entryPoint) {
            return entryPoint->target == target && entryPoint->name == name;
        });
    if (itr == m_entryPoints.end()) {
        return LN_MAKE_ERROR("EntryPoint not found. (%d:%s)", target, name.c_str());
    }
    return (*itr).get();
}

} // namespace kokage
} // namespace ln
