#include "Internal.hpp"
#include <LuminoEngine/Graphics/ShaderCompiler/Common.hpp>
#include <LuminoEngine/Graphics/ShaderCompiler/UnifiedShader2.hpp>
#include "DescriptorLayoutBuilder.hpp"

namespace ln {
namespace kokage {

DescriptorLayoutBuilder::DescriptorLayoutBuilder() {
}

MaybeResult DescriptorLayoutBuilder::buildDescriptorLayout(UnifiedShader2* shader) {
    const std::vector<URef<GlobalShaderPass>>& globalShaderPasses = shader->globalShaderPasses();
    const std::vector<URef<TargetShaderPass>>& targetShaderPasses = shader->targetShaderPasses();

    // GlobalShaderPass の DescriptorLayout を作成する。
    // 全ての子 TargetShaderPass を調べて、必要な情報を GlobalShaderPass に吸い出していく感じ。
    for (const auto& globalShaderPass : globalShaderPasses) {
        for (TargetShaderPassId targetShaderPassId : globalShaderPass->targetShaderPassIds) {
            TargetShaderPass* targetShaderPass = targetShaderPasses[targetShaderPassId].get();
            for (TargetBindingInfo& info : targetShaderPass->bindingLayout.bindings) {
                getOrCreateDescriptorLayoutEntry(
                    shader,
                    globalShaderPass,
                    info,
                    &info.descriptorEntryCategory,
                    &info.descriptorEntryIndex);
            }
        }
    }

    return LN_MAKE_SUCCESS();
}

MaybeResult DescriptorLayoutBuilder::getOrCreateDescriptorLayoutEntry(
    UnifiedShader2* shader,
    GlobalShaderPass* globalShaderPass,
    const TargetBindingInfo& bindingInfo,
    kokage::RegisterCategory* outCategory,
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
                return LN_MAKE_SUCCESS();
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
                return LN_MAKE_SUCCESS();
            }
        }
    }
    // Textures (CombinedSampler)
    if (bindingInfo.category == BindingResourceCategory_SamplerState && !bindingInfo.combinedSamplerName.empty()) {
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
                return LN_MAKE_SUCCESS();
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
                return LN_MAKE_SUCCESS();
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
                return LN_MAKE_SUCCESS();
            }
        }
    }

    return LN_MAKE_ERROR_UNREACHABLE();
}

} // namespace kokage
} // namespace ln
