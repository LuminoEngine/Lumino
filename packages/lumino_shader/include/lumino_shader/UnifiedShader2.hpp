// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once

#include "Common.hpp"

namespace ln {
namespace shader {

using GlobalShaderPassId2 = int16_t;
using TargetShaderPassId2 = int16_t;
using TargetEntryPointId2 = int16_t;
using BlobId2 = int16_t;

class UnifiedShader2;

// Single element within a ParameterBlock (ConstantBuffer, Texture, Sampler, etc.)
struct ParameterBlockElement2 {
    std::string name;                    // "params", "baseTexture", "" (implicit CB)
    ParameterBlockElementKind kind;
    int16_t constantBufferSize;          // Valid only for ConstantBuffer kind; -1 otherwise
};

// ParameterBlock layout (target-independent)
struct ParameterBlockLayout2 {
    std::string name;                    // "viewData", "materialData", etc.
    int16_t setIndex;                    // Descriptor set index
    bool hasImplicitConstantBuffer;      // true when the struct contains only plain data fields
    std::vector<ParameterBlockElement2> elements;
};

// Per-target binding information
struct TargetBinding2 {
    std::string name;
    ParameterBlockElementKind kind;
    int16_t setIndex;
    int16_t bindingIndex;
    int16_t size;                        // CB: byte size, others: 0
    ShaderStageFlags used;
};

struct TargetBindingLayout2 {
    std::vector<TargetBinding2> bindings;
};

struct TargetEntryPoint2 {
    TargetEntryPointId2 id;
    ShaderTarget target;
    std::string name;
    BlobId2 codeBlobId;
    TargetBindingLayout2 bindingLayout;
    std::vector<VertexInputAttribute> inputAttributes;
};

struct TargetShaderPass2 {
    TargetShaderPassId2 id;
    TargetEntryPointId2 vertEntryPointId;
    TargetEntryPointId2 fragEntryPointId;
    TargetEntryPointId2 compEntryPointId;
    TargetBindingLayout2 bindingLayout;

    TargetShaderPass2()
        : id(-1), vertEntryPointId(-1), fragEntryPointId(-1), compEntryPointId(-1) {}
};

struct GlobalShaderPass2 {
    UnifiedShader2* m_owner;
    GlobalShaderPassId2 id;
    std::string name;
    std::string vertexEntryPoint;
    std::string fragmentEntryPoint;
    std::string computeEntryPoint;
    std::vector<TargetShaderPassId2> targetShaderPassIds;

    GlobalShaderPass2(UnifiedShader2* owner)
        : m_owner(owner)
        , id(-1)
        , targetShaderPassIds{-1, -1, -1, -1} {}

    TargetShaderPassId2 getTargetShaderPassId(ShaderTarget target) const {
        return targetShaderPassIds[target - 1];
    }
};

class UnifiedShader2 : public RefCounted {
public:
    UnifiedShader2();

    const std::vector<ParameterBlockLayout2>& parameterBlocks() const;
    const std::vector<std::unique_ptr<GlobalShaderPass2>>& globalShaderPasses() const;
    const std::vector<std::unique_ptr<TargetShaderPass2>>& targetShaderPasses() const;
    const std::vector<std::unique_ptr<TargetEntryPoint2>>& targetEntryPoints() const;

    TargetShaderPass2* targetShaderPass(TargetShaderPassId2 id) const;
    TargetEntryPoint2* targetEntryPoint(TargetEntryPointId2 id) const;
    Blob* blob(BlobId2 id) const;

    ParameterBlockLayout2* addParameterBlock(ParameterBlockLayout2&& layout);
    GlobalShaderPass2* createGlobalShaderPass();
    TargetShaderPass2* createTargetShaderPass();
    TargetEntryPoint2* createEntryPoint();
    Blob* createBlob();

    Result<TargetEntryPoint2*> getTargetEntryPoint(ShaderTarget target, const std::string& name) const;

    static VoidResult mergeTargetBindingLayout(
        TargetBindingLayout2& target, const TargetBindingLayout2& other, bool reset);

private:
    std::vector<ParameterBlockLayout2> m_parameterBlocks;
    std::vector<std::unique_ptr<GlobalShaderPass2>> m_globalShaderPasses;
    std::vector<std::unique_ptr<TargetShaderPass2>> m_targetShaderPasses;
    std::vector<std::unique_ptr<TargetEntryPoint2>> m_targetEntryPoints;
    std::vector<std::unique_ptr<Blob>> m_blobs;

    friend class UnifiedShaderSerializer2;
};

} // namespace shader
} // namespace ln
