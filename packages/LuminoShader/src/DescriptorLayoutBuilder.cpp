// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#include "pch.hpp"
#include <LuminoShader/UnifiedShader.hpp>
#include "DescriptorLayoutBuilder.hpp"

namespace ln {
namespace shader {

DescriptorLayoutBuilder::DescriptorLayoutBuilder() {
}

VoidResult DescriptorLayoutBuilder::buildDescriptorLayout(UnifiedShader* shader) {
    const auto& globalShaderPasses = shader->globalShaderPasses();
    const auto& targetShaderPasses = shader->targetShaderPasses();

    for (const auto& globalShaderPass : globalShaderPasses) {
        for (TargetShaderPassId targetShaderPassId : globalShaderPass->targetShaderPassIds) {
            if (targetShaderPassId < 0) continue;
            TargetShaderPass* targetShaderPass = targetShaderPasses[targetShaderPassId].get();
            for (TargetBindingInfo& info : targetShaderPass->bindingLayout.bindings) {
                auto r = getOrCreateDescriptorLayoutEntry(
                    shader,
                    globalShaderPass.get(),
                    info,
                    &info.descriptorEntryCategory,
                    &info.descriptorEntryIndex);
                if (!r) return r;
            }
        }
    }

    return LNSHADER_OK();
}

VoidResult DescriptorLayoutBuilder::getOrCreateDescriptorLayoutEntry(
    UnifiedShader* shader,
    GlobalShaderPass* globalShaderPass,
    const TargetBindingInfo& bindingInfo,
    RegisterCategory* outCategory,
    int16_t* outIndex) {

    const std::string& name = bindingInfo.name;
    GlobalResourceLayout* globalResourceLayout = shader->globalResourceLayout();
    const std::vector<GlobalResourceSlotInfo>& globalBuffers = globalResourceLayout->buffers;
    const std::vector<GlobalResourceSlotInfo>& globalTextures = globalResourceLayout->textures;
    const std::vector<GlobalResourceSlotInfo>& globalSamplers = globalResourceLayout->samplers;
    const std::vector<GlobalResourceSlotInfo>& globalStorages = globalResourceLayout->storages;
    std::vector<int16_t>& buffers = globalShaderPass->descriptorLayout.buffers;
    std::vector<int16_t>& textures = globalShaderPass->descriptorLayout.textures;
    std::vector<int16_t>& samplers = globalShaderPass->descriptorLayout.samplers;
    std::vector<int16_t>& storages = globalShaderPass->descriptorLayout.storages;

    // Buffers
    if (bindingInfo.category == BindingResourceCategory_ConstantBuffer) {
        *outCategory = RegisterCategory_ConstantBuffer;
        for (int i = 0; i < static_cast<int>(globalBuffers.size()); i++) {
            const GlobalResourceSlotInfo& slotInfo = globalBuffers[i];
            if (slotInfo.name == name) {
                auto itr = std::find(buffers.begin(), buffers.end(), static_cast<int16_t>(i));
                if (itr == buffers.end()) {
                    buffers.push_back(static_cast<int16_t>(i));
                    *outIndex = static_cast<int16_t>(buffers.size() - 1);
                }
                else {
                    *outIndex = static_cast<int16_t>(itr - buffers.begin());
                }
                return LNSHADER_OK();
            }
        }
    }
    // Textures
    if (bindingInfo.category == BindingResourceCategory_TextureOrCombinedSampler) {
        *outCategory = RegisterCategory_TextureOrCombinedSampler;
        for (int i = 0; i < static_cast<int>(globalTextures.size()); i++) {
            const GlobalResourceSlotInfo& slotInfo = globalTextures[i];
            if (slotInfo.name == name) {
                auto itr = std::find(textures.begin(), textures.end(), static_cast<int16_t>(i));
                if (itr == textures.end()) {
                    textures.push_back(static_cast<int16_t>(i));
                    *outIndex = static_cast<int16_t>(textures.size() - 1);
                }
                else {
                    *outIndex = static_cast<int16_t>(itr - textures.begin());
                }
                return LNSHADER_OK();
            }
        }
    }
    // Textures (CombinedSampler)
    if (bindingInfo.category == BindingResourceCategory_SamplerState && !bindingInfo.combinedSamplerName.empty()) {
        *outCategory = RegisterCategory_TextureOrCombinedSampler;
        for (int i = 0; i < static_cast<int>(globalTextures.size()); i++) {
            const GlobalResourceSlotInfo& slotInfo = globalTextures[i];
            if (slotInfo.name == bindingInfo.combinedSamplerName) {
                auto itr = std::find(textures.begin(), textures.end(), static_cast<int16_t>(i));
                if (itr == textures.end()) {
                    textures.push_back(static_cast<int16_t>(i));
                    *outIndex = static_cast<int16_t>(textures.size() - 1);
                }
                else {
                    *outIndex = static_cast<int16_t>(itr - textures.begin());
                }
                return LNSHADER_OK();
            }
        }
    }
    // Samplers
    if (bindingInfo.category == BindingResourceCategory_SamplerState) {
        *outCategory = RegisterCategory_SamplerState;
        for (int i = 0; i < static_cast<int>(globalSamplers.size()); i++) {
            const GlobalResourceSlotInfo& slotInfo = globalSamplers[i];
            if (slotInfo.name == name) {
                auto itr = std::find(samplers.begin(), samplers.end(), static_cast<int16_t>(i));
                if (itr == samplers.end()) {
                    samplers.push_back(static_cast<int16_t>(i));
                    *outIndex = static_cast<int16_t>(samplers.size() - 1);
                }
                else {
                    *outIndex = static_cast<int16_t>(itr - samplers.begin());
                }
                return LNSHADER_OK();
            }
        }
    }
    // Storages
    if (bindingInfo.category == BindingResourceCategory_UnorderdAccess) {
        *outCategory = RegisterCategory_UnorderdAccess;
        for (int i = 0; i < static_cast<int>(globalStorages.size()); i++) {
            const GlobalResourceSlotInfo& slotInfo = globalStorages[i];
            if (slotInfo.name == name) {
                auto itr = std::find(storages.begin(), storages.end(), static_cast<int16_t>(i));
                if (itr == storages.end()) {
                    storages.push_back(static_cast<int16_t>(i));
                    *outIndex = static_cast<int16_t>(storages.size() - 1);
                }
                else {
                    *outIndex = static_cast<int16_t>(itr - storages.begin());
                }
                return LNSHADER_OK();
            }
        }
    }

    return LNSHADER_MAKE_ERROR("Descriptor layout entry not found for binding: " + name);
}

} // namespace shader
} // namespace ln
