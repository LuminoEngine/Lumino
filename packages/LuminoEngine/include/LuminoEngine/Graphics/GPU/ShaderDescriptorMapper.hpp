// Copyright (c) 2019 lriki. Distributed under the MIT license.
#pragma once
#include "Common.hpp"
#include "../ShaderCompiler/UnifiedShader2.hpp"

namespace ln {

// Descripter → Native のマッピング (Native が Descripter から値を取得するときに使う)
class RHIBindingToDescriptorMapper final {
public:
    struct MapperEntry {
        kokage::RegisterCategory category; // Descripter 内のどの register であるか
        int index;                          // その register の何番目の要素から値を取得するか
    };

    RHIBindingToDescriptorMapper();
    ~RHIBindingToDescriptorMapper();
    void setup(kokage::TargetShaderPass* shaderPass);
    const MapperEntry& getEntry(int index) const;

private:
    // 要素数は kokage::TargetBindingLayoutInfo と一致する
    std::vector<MapperEntry> m_entries;
};

// Material(とSceneNode) → Descripter のマッピング
class StandardShaderDescriptorMapper final {
public:
    StandardShaderDescriptorMapper();
    ~StandardShaderDescriptorMapper();
    void setup(kokage::TargetShaderPass* shaderPass);

private:
    //std::array<detail::ConstantBufferView, detail::MaxDescriptorSlots> m_uniformBufferViews;
    //std::array<IGraphicsResource*, detail::MaxDescriptorSlots> m_textures;
    //std::array<SamplerState*, detail::MaxDescriptorSlots> m_samplers;
    //std::array<IGraphicsResource*, detail::MaxDescriptorSlots> m_storages;
    // Boolean flags BuiltinShaderParameters
    uint64_t m_hasBuiltinShaderParameters;

    //// Index of ShaderDescriptor::m_buffers
    //std::array<int, BuiltinShaderUniformBuffers__Count> m_builtinBufferSlotIndices;

    //// Index of ShaderDescriptor::m_textures
    //std::array<int, BuiltinShaderTextures__Count> m_builtinTextureSlotIndices;
};

} // namespace ln
