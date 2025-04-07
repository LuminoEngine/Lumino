#include "Internal.hpp"
#include <LuminoEngine/Graphics/ShaderCompiler/ShaderHelper.hpp>
#include <LuminoEngine/Graphics/ShaderCompiler/UnifiedShader2.hpp>

namespace ln {
namespace kokage {

RegisterCategory GlobalResourceLayout::getRegisterCategoryByName(const std::string& name) const {
    for (const auto& slot : buffers) {
        if (slot.name == name) return RegisterCategory_UniformBuffer;
    }
    for (const auto& slot : textures) {
        if (slot.name == name) return RegisterCategory_TextureOrCombinedSampler;
    }
    for (const auto& slot : samplers) {
        if (slot.name == name) return RegisterCategory_SamplerState;
    }
    for (const auto& slot : storages) {
        if (slot.name == name) return RegisterCategory_UnorderdAccess;
    }
    return RegisterCategory_Unknown;
}

void GlobalShaderPass::getOrCreateDescriptorLayoutEntry(
    const TargetBindingInfo& bindingInfo, kokage::RegisterCategory* outCategory, int* outIndex) {
    const std::string& name = bindingInfo.name;
    GlobalResourceLayout* globalResourceLayout = m_owner->globalResourceLayout();
    const std::vector<GlobalResourceSlotInfo>& globalBuffers = globalResourceLayout->buffers;
    const std::vector<GlobalResourceSlotInfo>& globalTextures = globalResourceLayout->textures;
    const std::vector<GlobalResourceSlotInfo>& globalSamplers = globalResourceLayout->samplers;
    const std::vector<GlobalResourceSlotInfo>& globalStorages = globalResourceLayout->storages;
    std::vector<int>& buffers = descriptorLayout.buffers;
    std::vector<int>& textures = descriptorLayout.textures;
    std::vector<int>& samplers = descriptorLayout.samplers;
    std::vector<int>& storages = descriptorLayout.storages;

    // Buffers
    if (bindingInfo.category == BindingResourceCategory_UniformBuffer) {
        *outCategory = RegisterCategory_UniformBuffer;
        for (int i = 0; i < globalBuffers.size(); i++) {
            const GlobalResourceSlotInfo& slotInfo = globalBuffers[i];
            if (slotInfo.name == name) {
                auto itr = std::find(buffers.begin(), buffers.end(), i);
                if (itr == buffers.end()) {
                    buffers.push_back(i);
                    *outIndex = buffers.size() - 1;
                }
                else {
                    *outIndex = itr - buffers.begin();
                }
                return;
            }
        }
    }
    // Textures
    if (bindingInfo.category == BindingResourceCategory_TextureOrCombinedSampler) {
        *outCategory = RegisterCategory_TextureOrCombinedSampler;
        for (int i = 0; i < globalTextures.size(); i++) {
            const GlobalResourceSlotInfo& slotInfo = globalTextures[i];
            if (slotInfo.name == name) {
                auto itr = std::find(textures.begin(), textures.end(), i);
                if (itr == textures.end()) {
                    textures.push_back(i);
                    *outIndex = textures.size() - 1;
                }
                else {
                    *outIndex = itr - textures.begin();
                }
                return;
            }
        }
    }
    // Textures (CombinedSampler)
    if (bindingInfo.category == BindingResourceCategory_SamplerState &&
        !bindingInfo.combinedSamplerName.empty()) {
        *outCategory = RegisterCategory_TextureOrCombinedSampler;
        for (int i = 0; i < globalTextures.size(); i++) {
            const GlobalResourceSlotInfo& slotInfo = globalTextures[i];
            if (slotInfo.name == bindingInfo.combinedSamplerName) {
                auto itr = std::find(textures.begin(), textures.end(), i);
                if (itr == textures.end()) {
                    textures.push_back(i);
                    *outIndex = textures.size() - 1;
                }
                else {
                    *outIndex = itr - textures.begin();
                }
                return;
            }
        }
    }
    // Samplers
    if (bindingInfo.category == BindingResourceCategory_SamplerState) {
        *outCategory = RegisterCategory_SamplerState;
        for (int i = 0; i < globalSamplers.size(); i++) {
            const GlobalResourceSlotInfo& slotInfo = globalSamplers[i];
            if (slotInfo.name == name) {
                auto itr = std::find(samplers.begin(), samplers.end(), i);
                if (itr == samplers.end()) {
                    samplers.push_back(i);
                    *outIndex = samplers.size() - 1;
                }
                else {
                    *outIndex = itr - samplers.begin();
                }
                return;
            }
        }
    }
    // Storages
    if (bindingInfo.category == BindingResourceCategory_UnorderdAccess) {
        *outCategory = RegisterCategory_UnorderdAccess;
        for (int i = 0; i < globalStorages.size(); i++) {
            const GlobalResourceSlotInfo& slotInfo = globalStorages[i];
            if (slotInfo.name == name) {
                auto itr = std::find(storages.begin(), storages.end(), i);
                if (itr == storages.end()) {
                    storages.push_back(i);
                    *outIndex = storages.size() - 1;
                }
                else {
                    *outIndex = itr - storages.begin();
                }
                return;
            }
        }
    }
    LN_UNREACHABLE();
}

UnifiedShader2::UnifiedShader2()
    : m_globalResourceLayout(makeURef<GlobalResourceLayout>()) {
}

GlobalResourceLayout* UnifiedShader2::globalResourceLayout() const {
    return m_globalResourceLayout;
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

TargetInputResourceInfo* UnifiedShader2::createTargetInputResourceInfo() {
    int index = m_TargetInputResourceInfos.size();
    auto info = makeURef<TargetInputResourceInfo>();
    info->index = index;
    m_TargetInputResourceInfos.push_back(std::move(info));
    return m_TargetInputResourceInfos.back().get();
}

GlobalShaderPass* UnifiedShader2::createGlobalShaderPass() {
    int index = m_globalShaderPasses.size();
    auto pass = makeURef<GlobalShaderPass>(this);
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

MaybeResult UnifiedShader2::getOrCreateInputResourceWithVerify(
    const std::string& name,
    RegisterCategory category,
    int constantBufferSize,
    int arrayElementCount) {

    struct {
        RegisterCategory category;
        const std::vector<GlobalResourceSlotInfo>& slots;
    } categories[] = {
        { RegisterCategory_UniformBuffer, m_globalResourceLayout->buffers },
        { RegisterCategory_TextureOrCombinedSampler, m_globalResourceLayout->textures },
        { RegisterCategory_SamplerState, m_globalResourceLayout->samplers },
        { RegisterCategory_UnorderdAccess, m_globalResourceLayout->storages },
    };

    
    // まずは名前で検索する
    for (const auto& c : categories) {
        if (c.category == category) {
            auto itr = std::find_if(
                c.slots.begin(),
                c.slots.end(),
                [&name](const GlobalResourceSlotInfo& info) { return info.name == name; });
            if (itr != c.slots.end()) {
                // すでに存在する場合、同じ RegisterCategory かつ count が一致するか確認する
                if ((*itr).constantBufferSize == constantBufferSize) {
                    return LN_MAKE_SUCCESS();
                }
                else {
                    return LN_MAKE_ERROR(
                        "GlobalInputResourceInfo already exists with different typeinfo (%s)",
                        name.c_str());
                }
            }
        }
    }

    // 存在しない場合、新しい InputResourceInfo を作成する
    GlobalResourceSlotInfo slot = {};
    slot.name = name;
    slot.constantBufferSize = constantBufferSize;
    slot.arrayElementCount = arrayElementCount;
    switch (category) {
        case RegisterCategory_UniformBuffer:
            m_globalResourceLayout->buffers.push_back(slot);
            break;
        case RegisterCategory_TextureOrCombinedSampler:
            m_globalResourceLayout->textures.push_back(slot);
            break;
        case RegisterCategory_SamplerState:
            m_globalResourceLayout->samplers.push_back(slot);
            break;
        case RegisterCategory_UnorderdAccess:
            m_globalResourceLayout->storages.push_back(slot);
            break;
        default:
            return LN_MAKE_ERROR("Invalid category (%d)", category);
    }

    return LN_MAKE_SUCCESS();
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

MaybeResult UnifiedShader2::mergeTargetBindingLayoutInfo(
    TargetBindingLayoutInfo& target, const TargetBindingLayoutInfo& other, bool reset) {
    if (reset) {
        target.bindings.clear();
    }
    for (const TargetBindingInfo& otherBinding : other.bindings) {
        auto itr = std::find_if(
            target.bindings.begin(),
            target.bindings.end(),
            [&otherBinding](const TargetBindingInfo& b) {
                return b.name == otherBinding.name && b.category == otherBinding.category;
            });
        if (itr != target.bindings.end()) {
            TargetBindingInfo& existingBinding = *itr;

            // 同じ名前のバインディングが存在する場合、各プロパティを比較して整合性を確認する
            {
                //if (otherBinding.category != existingBinding.category) {
                //    return LN_MAKE_ERROR(
                //        "TargetBindingInfo already exists with different category (%s)",
                //        otherBinding.name.c_str());
                //}
                if (otherBinding.size != existingBinding.size) {
                    return LN_MAKE_ERROR(
                        "TargetBindingInfo already exists with different size (%s)",
                        otherBinding.name.c_str());
                }
                if (otherBinding.space != existingBinding.space) {
                    return LN_MAKE_ERROR(
                        "TargetBindingInfo already exists with different space (%s)",
                        otherBinding.name.c_str());
                }
                if (otherBinding.index != existingBinding.index) {
                    return LN_MAKE_ERROR(
                        "TargetBindingInfo already exists with different index (%s)",
                        otherBinding.name.c_str());
                }
                if (otherBinding.count != existingBinding.count) {
                    return LN_MAKE_ERROR(
                        "TargetBindingInfo already exists with different count (%s)",
                        otherBinding.name.c_str());
                }

                const auto& otherMembers = otherBinding.members;
                const auto& existingMembers = existingBinding.members;
                if (otherMembers.size() != existingMembers.size()) {
                    return LN_MAKE_ERROR(
                        "TargetBindingInfo already exists with different members size (%s)",
                        otherBinding.name.c_str());
                }

                for (size_t i = 0; i < otherMembers.size(); ++i) {
                    const auto& otherMember = otherMembers[i];
                    const auto& existingMember = existingMembers[i];
                    if (otherMember.name != existingMember.name) {
                        return LN_MAKE_ERROR(
                            "TargetBindingInfo already exists with different member name (%s)",
                            otherBinding.name.c_str());
                    }
                    if (otherMember.offset != existingMember.offset) {
                        return LN_MAKE_ERROR(
                            "TargetBindingInfo already exists with different member offset (%s)",
                            otherBinding.name.c_str());
                    }
                    if (otherMember.size != existingMember.size) {
                        return LN_MAKE_ERROR(
                            "TargetBindingInfo already exists with different member size (%s)",
                            otherBinding.name.c_str());
                    }
                }
            }

            // 既存のバインディングに対して、使用されるシェーダーステージをマージする
            existingBinding.used = static_cast<ShaderStageFlags>(
                existingBinding.used | otherBinding.used);
        }
        else {
            // 存在しない場合、新しいバインディングを追加する
            target.bindings.push_back(otherBinding);
        }
    }

    return LN_MAKE_SUCCESS();

}

MaybeResult UnifiedShader2::buildDescriptorLayout() {

    // GlobalShaderPass の DescriptorLayout を作成する。
    // 全ての子 TargetShaderPass を調べて、必要な情報を GlobalShaderPass に吸い出していく感じ。
    for (const auto& globalShaderPass : m_globalShaderPasses) {
        for (TargetShaderPassId targetShaderPassId : globalShaderPass->targetShaderPassIndices) {
            TargetShaderPass* targetShaderPass = m_targetShaderPasses[targetShaderPassId].get();
            for (TargetBindingInfo& info : targetShaderPass->bindingLayout.bindings) {
                globalShaderPass->getOrCreateDescriptorLayoutEntry(
                    info,
                    &info.descriptorEntryCategory,
                    &info.descriptorEntryIndex);
            }
        }
    }

    return LN_MAKE_SUCCESS();
}

} // namespace kokage
} // namespace ln
