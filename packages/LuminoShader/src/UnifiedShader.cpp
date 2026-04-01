// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#include "pch.hpp"
#include <LuminoShader/UnifiedShader.hpp>

namespace ln {
namespace shader {

RegisterCategory GlobalResourceLayout::getRegisterCategoryByName(const std::string& name) const {
    for (const auto& slot : buffers) {
        if (slot.name == name) return RegisterCategory_ConstantBuffer;
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

UnifiedShader::UnifiedShader()
    : m_globalResourceLayout(std::make_unique<GlobalResourceLayout>()) {
}

GlobalResourceLayout* UnifiedShader::globalResourceLayout() const {
    return m_globalResourceLayout.get();
}

const std::vector<std::unique_ptr<GlobalConstantBufferMember>>& UnifiedShader::globalConstantBufferMembers() const {
    return m_globalMembers;
}

const std::vector<std::unique_ptr<GlobalShaderPass>>& UnifiedShader::globalShaderPasses() const {
    return m_globalShaderPasses;
}

const std::vector<std::unique_ptr<TargetShaderPass>>& UnifiedShader::targetShaderPasses() const {
    return m_targetShaderPasses;
}

const std::vector<std::unique_ptr<TargetEntryPoint>>& UnifiedShader::targetEntryPoints() const {
    return m_targetEntryPoints;
}

TargetShaderPass* UnifiedShader::targetShaderPass(TargetShaderPassId id) const {
    return m_targetShaderPasses[id].get();
}

TargetEntryPoint* UnifiedShader::targetEntryPoint(TargetEntryPointId id) const {
    return m_targetEntryPoints[id].get();
}

Blob* UnifiedShader::blob(BlobId id) const {
    return m_blobs[id].get();
}

GlobalConstantBufferMember* UnifiedShader::createGlobalConstantBufferMember() {
    auto member = std::make_unique<GlobalConstantBufferMember>();
    member->id = static_cast<GlobalConstantBufferMemberId>(m_globalMembers.size());
    m_globalMembers.push_back(std::move(member));
    return m_globalMembers.back().get();
}

GlobalShaderPass* UnifiedShader::createGlobalShaderPass() {
    auto pass = std::make_unique<GlobalShaderPass>(this);
    pass->id = static_cast<GlobalShaderPassId>(m_globalShaderPasses.size());
    m_globalShaderPasses.push_back(std::move(pass));
    return m_globalShaderPasses.back().get();
}

TargetShaderPass* UnifiedShader::createTargetShaderPass() {
    auto pass = std::make_unique<TargetShaderPass>();
    pass->id = static_cast<TargetShaderPassId>(m_targetShaderPasses.size());
    m_targetShaderPasses.push_back(std::move(pass));
    return m_targetShaderPasses.back().get();
}

TargetEntryPoint* UnifiedShader::createEntryPoint() {
    auto entryPoint = std::make_unique<TargetEntryPoint>();
    entryPoint->id = static_cast<TargetEntryPointId>(m_targetEntryPoints.size());
    m_targetEntryPoints.push_back(std::move(entryPoint));
    return m_targetEntryPoints.back().get();
}

Blob* UnifiedShader::createBlob() {
    auto blob = std::make_unique<Blob>();
    blob->id = static_cast<BlobId>(m_blobs.size());
    m_blobs.push_back(std::move(blob));
    return m_blobs.back().get();
}

Result<GlobalConstantBufferMember*> UnifiedShader::getOrCreateGlobalMemberWithVerify(
    std::string name,
    ShaderGlobalMemberType type,
    ShaderGlobalMemberKind kind,
    int32_t arrayElements,
    int32_t vectorElements,
    int32_t matrixRows,
    int32_t matrixColumns) {
    auto itr = std::find_if(
        m_globalMembers.begin(),
        m_globalMembers.end(),
        [&name](const std::unique_ptr<GlobalConstantBufferMember>& info) { return info->name == name; });
    if (itr != m_globalMembers.end()) {
        GlobalConstantBufferMember* info = (*itr).get();
        if (info->type != type) {
            return LNSHADER_MAKE_ERROR(
                "GlobalConstantBufferMember already exists with different type (" + name + ")");
        }
        if (info->kind != kind) {
            return LNSHADER_MAKE_ERROR(
                "GlobalConstantBufferMember already exists with different kind (" + name + ")");
        }
        if (info->arrayElements != arrayElements) {
            return LNSHADER_MAKE_ERROR(
                "GlobalConstantBufferMember already exists with different arrayElements (" + name + ")");
        }
        if (info->vectorElements != vectorElements) {
            return LNSHADER_MAKE_ERROR(
                "GlobalConstantBufferMember already exists with different vectorElements (" + name + ")");
        }
        if (info->matrixRows != matrixRows) {
            return LNSHADER_MAKE_ERROR(
                "GlobalConstantBufferMember already exists with different matrixRows (" + name + ")");
        }
        if (info->matrixColumns != matrixColumns) {
            return LNSHADER_MAKE_ERROR(
                "GlobalConstantBufferMember already exists with different matrixColumns (" + name + ")");
        }
        return info;
    }

    GlobalConstantBufferMember* info = createGlobalConstantBufferMember();
    info->name = name;
    info->type = type;
    info->kind = kind;
    info->arrayElements = arrayElements;
    info->vectorElements = vectorElements;
    info->matrixRows = matrixRows;
    info->matrixColumns = matrixColumns;
    return m_globalMembers.back().get();
}

VoidResult UnifiedShader::getOrCreateInputResourceWithVerify(
    const std::string& name,
    RegisterCategory category,
    int constantBufferSize,
    int arrayElementCount) {

    struct {
        RegisterCategory category;
        const std::vector<GlobalResourceSlotInfo>& slots;
    } categories[] = {
        { RegisterCategory_ConstantBuffer, m_globalResourceLayout->buffers },
        { RegisterCategory_TextureOrCombinedSampler, m_globalResourceLayout->textures },
        { RegisterCategory_SamplerState, m_globalResourceLayout->samplers },
        { RegisterCategory_UnorderdAccess, m_globalResourceLayout->storages },
    };

    for (const auto& c : categories) {
        if (c.category == category) {
            auto itr = std::find_if(
                c.slots.begin(),
                c.slots.end(),
                [&name](const GlobalResourceSlotInfo& info) { return info.name == name; });
            if (itr != c.slots.end()) {
                if ((*itr).constantBufferSize == constantBufferSize) {
                    return LNSHADER_OK();
                }
                else {
                    return LNSHADER_MAKE_ERROR(
                        "GlobalInputResourceInfo already exists with different typeinfo (" + name + ")");
                }
            }
        }
    }

    GlobalResourceSlotInfo slot = {};
    slot.name = name;
    slot.constantBufferSize = constantBufferSize;
    slot.arrayElementCount = arrayElementCount;
    switch (category) {
        case RegisterCategory_ConstantBuffer:
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
            return LNSHADER_MAKE_ERROR("Invalid category");
    }

    return LNSHADER_OK();
}

Result<TargetEntryPoint*> UnifiedShader::getTargetEntryPoint(
    ShaderTarget target, const std::string& name) const {
    auto itr = std::find_if(
        m_targetEntryPoints.begin(),
        m_targetEntryPoints.end(),
        [&target, &name](const std::unique_ptr<TargetEntryPoint>& entryPoint) {
            return entryPoint->target == target && entryPoint->name == name;
        });
    if (itr == m_targetEntryPoints.end()) {
        return LNSHADER_MAKE_ERROR("TargetEntryPoint not found. (" + name + ")");
    }
    return (*itr).get();
}

VoidResult UnifiedShader::mergeTargetBindingLayoutInfo(
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

            if (otherBinding.size != existingBinding.size) {
                return LNSHADER_MAKE_ERROR(
                    "TargetBindingInfo already exists with different size (" + otherBinding.name + ")");
            }
            if (otherBinding.space != existingBinding.space) {
                return LNSHADER_MAKE_ERROR(
                    "TargetBindingInfo already exists with different space (" + otherBinding.name + ")");
            }
            if (otherBinding.index != existingBinding.index) {
                return LNSHADER_MAKE_ERROR(
                    "TargetBindingInfo already exists with different index (" + otherBinding.name + ")");
            }
            if (otherBinding.count != existingBinding.count) {
                return LNSHADER_MAKE_ERROR(
                    "TargetBindingInfo already exists with different count (" + otherBinding.name + ")");
            }

            const auto& otherMembers = otherBinding.members;
            const auto& existingMembers = existingBinding.members;
            if (otherMembers.size() != existingMembers.size()) {
                return LNSHADER_MAKE_ERROR(
                    "TargetBindingInfo already exists with different members size (" + otherBinding.name + ")");
            }

            for (size_t i = 0; i < otherMembers.size(); ++i) {
                const auto& otherMember = otherMembers[i];
                const auto& existingMember = existingMembers[i];
                if (otherMember.name != existingMember.name) {
                    return LNSHADER_MAKE_ERROR(
                        "TargetBindingInfo already exists with different member name (" + otherBinding.name + ")");
                }
                if (otherMember.offset != existingMember.offset) {
                    return LNSHADER_MAKE_ERROR(
                        "TargetBindingInfo already exists with different member offset (" + otherBinding.name + ")");
                }
                if (otherMember.size != existingMember.size) {
                    return LNSHADER_MAKE_ERROR(
                        "TargetBindingInfo already exists with different member size (" + otherBinding.name + ")");
                }
            }

            existingBinding.used = static_cast<ShaderStageFlags>(
                existingBinding.used | otherBinding.used);
        }
        else {
            target.bindings.push_back(otherBinding);
        }
    }

    return LNSHADER_OK();
}

} // namespace shader
} // namespace ln
