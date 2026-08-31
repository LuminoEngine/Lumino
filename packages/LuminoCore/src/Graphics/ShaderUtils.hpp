#pragma once
#include <LuminoCore/Graphics/rhi/Rhi.hpp>
#include <LuminoShader/UnifiedShader2.hpp>
#include <string>

namespace ln {
namespace detail {

inline shader::ShaderTarget backendToShaderTarget(rhi::Backend backend) {
    switch (backend) {
    case rhi::Backend::Vulkan: return shader::ShaderTarget_SPIRV;
    case rhi::Backend::WebGPU: return shader::ShaderTarget_WGSL;
    case rhi::Backend::WebGL2: return shader::ShaderTarget_GLSL_ES300;
    }
    return shader::ShaderTarget_SPIRV;
}

inline rhi::BindingType mapElementKind(shader::ParameterBlockElementKind kind) {
    switch (kind) {
    case shader::ParameterBlockElementKind_ConstantBuffer: return rhi::BindingType::UniformBuffer;
    case shader::ParameterBlockElementKind_Texture:        return rhi::BindingType::SampledTexture;
    case shader::ParameterBlockElementKind_SamplerState:   return rhi::BindingType::Sampler;
    case shader::ParameterBlockElementKind_StorageBuffer:  return rhi::BindingType::StorageBuffer;
    default: return rhi::BindingType::UniformBuffer;
    }
}

inline rhi::BindGroupLayoutDesc buildBindGroupLayoutFromReflection(
    const shader::ParameterBlockLayout2& block,
    const shader::TargetBindingLayout2& mergedBindings) {
    rhi::BindGroupLayoutDesc desc;
    for (const auto& binding : mergedBindings.bindings) {
        if (binding.setIndex != block.setIndex) continue;
        rhi::BindGroupLayoutEntry entry;
        entry.binding = binding.bindingIndex;
        entry.type = mapElementKind(binding.kind);
        entry.visibility = static_cast<rhi::ShaderStage>(binding.used);
        desc.entries.push_back(entry);
    }
    return desc;
}

inline const shader::ParameterBlockLayout2* findParameterBlock(
    const shader::UnifiedShader2* shader, const std::string& name) {
    for (const auto& block : shader->parameterBlocks()) {
        if (block.name == name) return &block;
    }
    return nullptr;
}

inline int16_t findConstantBufferSize(const shader::ParameterBlockLayout2& block) {
    for (const auto& elem : block.elements) {
        if (elem.kind == shader::ParameterBlockElementKind_ConstantBuffer)
            return elem.constantBufferSize;
    }
    return -1;
}

} // namespace detail
} // namespace ln
