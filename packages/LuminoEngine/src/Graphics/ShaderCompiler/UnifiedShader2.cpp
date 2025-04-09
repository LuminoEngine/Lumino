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

UnifiedShader2::UnifiedShader2()
    : m_globalResourceLayout(makeURef<GlobalResourceLayout>()) {
}

GlobalResourceLayout* UnifiedShader2::globalResourceLayout() const {
    return m_globalResourceLayout;
}

const std::vector<URef<TargetEntryPoint>>& UnifiedShader2::targetEntryPoints() const {
    return m_targetEntryPoints;
}

const std::vector<URef<GlobalShaderPass>>& UnifiedShader2::globalShaderPasses() const {
    return m_globalShaderPasses;
}

const std::vector<URef<TargetShaderPass>>& UnifiedShader2::targetShaderPasses() const {
    return m_targetShaderPasses;
}

TargetEntryPoint* UnifiedShader2::targetEntryPoint(TargetEntryPointId id) const {
    return m_targetEntryPoints[id].get();
}

TargetShaderPass* UnifiedShader2::targetShaderPass(TargetShaderPassId id) const {
    return m_targetShaderPasses[id].get();
}
Blob* UnifiedShader2::blob(BlobId id) const {
    return m_blobs[id].get();
}

GlobalShaderPass* UnifiedShader2::createGlobalShaderPass() {
    int id = m_globalShaderPasses.size();
    auto pass = makeURef<GlobalShaderPass>(this);
    pass->id = id;
    m_globalShaderPasses.push_back(std::move(pass));
    return m_globalShaderPasses.back().get();
}

TargetShaderPass* UnifiedShader2::createTargetShaderPass() {
    int id = m_targetShaderPasses.size();
    auto pass = makeURef<TargetShaderPass>();
    pass->id = id;
    m_targetShaderPasses.push_back(std::move(pass));
    return m_targetShaderPasses.back().get();
}

TargetEntryPoint* UnifiedShader2::createEntryPoint() {
    int id = m_targetEntryPoints.size();
    auto entryPoint = makeURef<TargetEntryPoint>();
    entryPoint->id = id;
    m_targetEntryPoints.push_back(std::move(entryPoint));
    return m_targetEntryPoints.back().get();
}

Blob* UnifiedShader2::createBlob() {
    int id = m_blobs.size();
    auto blob = makeURef<Blob>();
    blob->id = id;
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
    info->id = m_globalMembers.size();
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
                        "GlobalInputResourceInfo already exists with different typeinfo (%s, a:%d, b:%d)",
                        name.c_str(),
                        (*itr).constantBufferSize,
                        constantBufferSize);
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

Result<TargetEntryPoint*> UnifiedShader2::getTargetEntryPoint(
    ShaderTarget target, const std::string& name) const {
    auto itr = std::find_if(
        m_targetEntryPoints.begin(),
        m_targetEntryPoints.end(),
        [&target, &name](const URef<TargetEntryPoint>& entryPoint) {
            return entryPoint->target == target && entryPoint->name == name;
        });
    if (itr == m_targetEntryPoints.end()) {
        return LN_MAKE_ERROR("TargetEntryPoint not found. (%d:%s)", target, name.c_str());
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

} // namespace kokage
} // namespace ln
