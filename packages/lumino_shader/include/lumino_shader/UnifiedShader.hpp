// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once

#include "Common.hpp"

namespace lumino {
namespace shader {

using GlobalConstantBufferMemberId = int16_t;
using GlobalShaderPassId = int16_t;
using TargetShaderPassId = int16_t;
using TargetEntryPointId = int16_t;
using BlobId = int16_t;

class UnifiedShader;

struct GlobalResourceSlotInfo {
    std::string name;
    int16_t constantBufferSize;
    int16_t arrayElementCount;
};

class GlobalResourceLayout {
public:
    std::vector<GlobalResourceSlotInfo> buffers;
    std::vector<GlobalResourceSlotInfo> textures;
    std::vector<GlobalResourceSlotInfo> samplers;
    std::vector<GlobalResourceSlotInfo> storages;

    RegisterCategory getRegisterCategoryByName(const std::string& name) const;
};

struct TargetBindingConstantBufferMemberInfo {
    std::string name;
    int16_t offset;
    int16_t size;
};

struct TargetBindingInfo {
    std::string name;
    BindingResourceCategory category;
    int16_t size;
    int16_t space;
    int16_t index;
    int16_t count;
    ShaderStageFlags used;
    std::vector<TargetBindingConstantBufferMemberInfo> members;

    // Transient: not serialized.
    std::string combinedSamplerName;

    RegisterCategory descriptorEntryCategory = RegisterCategory_ConstantBuffer;
    int16_t descriptorEntryIndex = -1;
};

struct TargetBindingLayoutInfo {
    std::vector<TargetBindingInfo> bindings;
};

struct TargetEntryPoint {
    TargetEntryPointId id;
    ShaderTarget target;
    std::string name;
    BlobId codeBlobId;
    TargetBindingLayoutInfo bindingLayout;
    std::vector<VertexInputAttribute> inputAttributes;
};

struct GlobalShaderPass {
    struct DescriptorLayout {
        std::vector<int16_t> buffers;
        std::vector<int16_t> textures;
        std::vector<int16_t> samplers;
        std::vector<int16_t> storages;
    };

    UnifiedShader* m_owner;
    GlobalShaderPassId id;
    std::string name;
    std::string vertexEntryPoint;
    std::string fragmentEntryPoint;
    std::string computeEntryPoint;
    std::vector<TargetShaderPassId> targetShaderPassIds;
    DescriptorLayout descriptorLayout;

    GlobalShaderPass(UnifiedShader* owner)
        : m_owner(owner)
        , id(-1)
        , targetShaderPassIds{-1, -1, -1, -1} {}

    TargetShaderPassId getTargetShaderPassId(ShaderTarget target) const {
        return targetShaderPassIds[target - 1];
    }
};

struct TargetShaderPass {
    TargetShaderPassId id;
    TargetEntryPointId vertEntryPointId;
    TargetEntryPointId fragEntryPointId;
    TargetEntryPointId compEntryPointId;
    TargetBindingLayoutInfo bindingLayout;

    TargetShaderPass()
        : id(-1), vertEntryPointId(-1), fragEntryPointId(-1), compEntryPointId(-1) {}
};

struct Blob {
    BlobId id;
    std::vector<uint8_t> data;
};

struct GlobalConstantBufferMember {
    GlobalConstantBufferMemberId id;
    std::string name;
    ShaderGlobalMemberType type;
    ShaderGlobalMemberKind kind;
    int32_t arrayElements;
    int32_t vectorElements;
    int32_t matrixRows;
    int32_t matrixColumns;
};

class UnifiedShader : public RefCounted {
public:
    UnifiedShader();

    GlobalResourceLayout* globalResourceLayout() const;
    const std::vector<std::unique_ptr<GlobalConstantBufferMember>>& globalConstantBufferMembers() const;
    const std::vector<std::unique_ptr<GlobalShaderPass>>& globalShaderPasses() const;
    const std::vector<std::unique_ptr<TargetShaderPass>>& targetShaderPasses() const;
    const std::vector<std::unique_ptr<TargetEntryPoint>>& targetEntryPoints() const;

    TargetShaderPass* targetShaderPass(TargetShaderPassId id) const;
    TargetEntryPoint* targetEntryPoint(TargetEntryPointId id) const;
    Blob* blob(BlobId id) const;

    GlobalConstantBufferMember* createGlobalConstantBufferMember();
    GlobalShaderPass* createGlobalShaderPass();
    TargetShaderPass* createTargetShaderPass();
    TargetEntryPoint* createEntryPoint();
    Blob* createBlob();

    Result<GlobalConstantBufferMember*> getOrCreateGlobalMemberWithVerify(
        std::string name,
        ShaderGlobalMemberType type,
        ShaderGlobalMemberKind kind,
        int32_t arrayElements,
        int32_t vectorElements,
        int32_t matrixRows,
        int32_t matrixColumns);

    VoidResult getOrCreateInputResourceWithVerify(
        const std::string& name,
        RegisterCategory category,
        int constantBufferSize,
        int arrayElementCount);

    Result<TargetEntryPoint*> getTargetEntryPoint(ShaderTarget target, const std::string& name) const;

    static VoidResult mergeTargetBindingLayoutInfo(
        TargetBindingLayoutInfo& target, const TargetBindingLayoutInfo& other, bool reset);

private:
    std::unique_ptr<GlobalResourceLayout> m_globalResourceLayout;
    std::vector<std::unique_ptr<GlobalConstantBufferMember>> m_globalMembers;
    std::vector<std::unique_ptr<GlobalShaderPass>> m_globalShaderPasses;
    std::vector<std::unique_ptr<TargetShaderPass>> m_targetShaderPasses;
    std::vector<std::unique_ptr<TargetEntryPoint>> m_targetEntryPoints;
    std::vector<std::unique_ptr<Blob>> m_blobs;

    friend class UnifiedShaderSerializer;
};

} // namespace shader
} // namespace lumino
