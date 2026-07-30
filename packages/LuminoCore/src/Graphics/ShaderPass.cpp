#include <LuminoCore/Graphics/ShaderPass.hpp>
#include <LuminoShader/UnifiedShader2.hpp>
#include <LuminoShader/UnifiedShaderSerializer2.hpp>
#include "ShaderUtils.hpp"
#include <algorithm>

namespace ln {

int32_t ShaderPass::s_liveCount = 0;

ShaderPass::ShaderPass() {
    ++s_liveCount;
}

ShaderPass::~ShaderPass() {
    --s_liveCount;
}

static shader::ShaderTarget backendToShaderTarget(rhi::Backend backend) {
    switch (backend) {
        case rhi::Backend::Vulkan: return shader::ShaderTarget_SPIRV;
        case rhi::Backend::WebGPU: return shader::ShaderTarget_WGSL;
    }
    return shader::ShaderTarget_SPIRV;
}

static rhi::ShaderCodeFormat backendToCodeFormat(rhi::Backend backend) {
    switch (backend) {
        case rhi::Backend::Vulkan: return rhi::ShaderCodeFormat::SPIRV;
        case rhi::Backend::WebGPU: return rhi::ShaderCodeFormat::WGSL;
    }
    return rhi::ShaderCodeFormat::SPIRV;
}

//------------------------------------------------------------------------------
// Sampler バインディングと、それが担当するテクスチャバインディングを対応付ける。
//
// Lumino のシェーダ規約ではテクスチャとサンプラーをペアで宣言するが、命名は 2 通り
// 存在する。
//   (a) <テクスチャ名> + "Sampler"   例: u_gbufferA   / u_gbufferASampler
//   (b) 共通接頭辞 + 役割サフィックス 例: u_baseTexture / u_baseSampler
// そのため、次の順で対応先を探す。
//   1. "Sampler" を除いた名前と完全一致するテクスチャ  -> (a)
//   2. "Sampler" を除いた名前を接頭辞に持つテクスチャが 1 つだけある -> (b)
//   3. 宣言順で直前にあるテクスチャ (規約から外れた命名へのフォールバック)
// いずれも見つからない場合は空文字列とし、マテリアル単位のサンプラー設定だけが効く。
static std::vector<std::string> resolveSamplerTextureNames(
    const rhi::BindGroupLayoutDesc& layoutDesc,
    const std::vector<std::string>& bindingNames) {

    static const std::string kSamplerSuffix = "Sampler";
    std::vector<std::string> result(layoutDesc.entries.size());

    for (size_t i = 0; i < layoutDesc.entries.size(); ++i) {
        if (layoutDesc.entries[i].type != rhi::BindingType::Sampler) continue;
        if (i >= bindingNames.size()) continue;
        const std::string& samplerName = bindingNames[i];

        if (samplerName.size() > kSamplerSuffix.size() &&
            samplerName.compare(samplerName.size() - kSamplerSuffix.size(),
                                kSamplerSuffix.size(), kSamplerSuffix) == 0) {
            std::string base = samplerName.substr(0, samplerName.size() - kSamplerSuffix.size());

            // 1. 完全一致
            bool matched = false;
            for (size_t j = 0; j < layoutDesc.entries.size() && j < bindingNames.size(); ++j) {
                if (layoutDesc.entries[j].type != rhi::BindingType::SampledTexture) continue;
                if (bindingNames[j] == base) {
                    result[i] = base;
                    matched = true;
                    break;
                }
            }
            if (matched) continue;

            // 2. 接頭辞一致 (候補が 1 つに絞れる場合のみ)
            const std::string* prefixMatch = nullptr;
            bool ambiguous = false;
            for (size_t j = 0; j < layoutDesc.entries.size() && j < bindingNames.size(); ++j) {
                if (layoutDesc.entries[j].type != rhi::BindingType::SampledTexture) continue;
                if (bindingNames[j].compare(0, base.size(), base) != 0) continue;
                if (prefixMatch) { ambiguous = true; break; }
                prefixMatch = &bindingNames[j];
            }
            if (prefixMatch && !ambiguous) {
                result[i] = *prefixMatch;
                continue;
            }
        }

        // 3. 直前のテクスチャへフォールバック
        for (size_t j = i; j > 0; --j) {
            if (layoutDesc.entries[j - 1].type != rhi::BindingType::SampledTexture) continue;
            if (j - 1 < bindingNames.size()) result[i] = bindingNames[j - 1];
            break;
        }
    }

    return result;
}

//------------------------------------------------------------------------------
// Internal: shared implementation used by createFromCompiledShader and createFromUnifiedShader
Result<Ref<ShaderPass>> ShaderPass::buildFromUnifiedShader(
    shader::UnifiedShader2* unifiedShader,
    rhi::Device* device,
    size_t passIndex) {

    // Select the requested pass for the backend's shader target.
    auto& globalPasses = unifiedShader->globalShaderPasses();
    if (globalPasses.empty()) {
        return LN_MAKE_ERROR("No shader passes found");
    }
    if (passIndex >= globalPasses.size()) {
        return LN_MAKE_ERROR("Shader pass index out of range");
    }

    auto shaderTarget = backendToShaderTarget(device->backend());
    auto codeFormat = backendToCodeFormat(device->backend());

    auto* globalPass = globalPasses[passIndex].get();
    auto targetPassId = globalPass->getTargetShaderPassId(shaderTarget);
    auto* targetPass = unifiedShader->targetShaderPass(targetPassId);
    if (!targetPass) {
        return LN_MAKE_ERROR("No target pass found for the current backend");
    }

    auto* vertEP = unifiedShader->targetEntryPoint(targetPass->vertEntryPointId);
    auto* fragEP = unifiedShader->targetEntryPoint(targetPass->fragEntryPointId);
    if (!vertEP || !fragEP) {
        return LN_MAKE_ERROR("Missing entry points");
    }

    auto* vertBlob = unifiedShader->blob(vertEP->codeBlobId);
    auto* fragBlob = unifiedShader->blob(fragEP->codeBlobId);

    // Create shader modules
    rhi::ShaderModuleDesc vsDesc;
    vsDesc.format = codeFormat;
    vsDesc.code = vertBlob->data.data();
    vsDesc.codeSizeBytes = vertBlob->data.size();
    auto vsResult = device->createShaderModule(vsDesc);
    if (!vsResult) {
        return LN_FORWARD_ERROR(vsResult);
    }

    rhi::ShaderModuleDesc fsDesc;
    fsDesc.format = codeFormat;
    fsDesc.code = fragBlob->data.data();
    fsDesc.codeSizeBytes = fragBlob->data.size();
    auto fsResult = device->createShaderModule(fsDesc);
    if (!fsResult) {
        return LN_FORWARD_ERROR(fsResult);
    }

    // Build material BindGroupLayout from the optional "$Material" ParameterBlock.
    //
    // $Material は「素の uniform / リソース」をまとめた合成ブロック。
    // フルスクリーン blit のような最小シェーダではマテリアル定数バッファを持たない
    // (テクスチャ+サンプラだけ、あるいは素のパラメータが何も無い) ことがある。
    // そのため $Material ブロックの有無も、その中の定数バッファの有無もどちらも任意とし、
    // 無い場合は materialSetIdx = -1 / cbSize = 0 として扱う。
    rhi::BindGroupLayoutDesc materialLayoutDesc;
    int16_t materialSetIdx = -1;
    int16_t cbSize = 0;

    auto* materialBlock = detail::findParameterBlock(unifiedShader, "$Material");
    if (materialBlock) {
        materialLayoutDesc = detail::buildBindGroupLayoutFromReflection(*materialBlock, targetPass->bindingLayout);
        materialSetIdx = materialBlock->setIndex;

        // 定数バッファは任意。見つからない場合は 0 (= マテリアル UBO 無し) とする。
        int16_t foundCbSize = detail::findConstantBufferSize(*materialBlock);
        if (foundCbSize > 0) {
            cbSize = foundCbSize;
        }
    }

    // Discover view/scene/object blocks from reflection
    auto* viewBlock = detail::findParameterBlock(unifiedShader, "viewData");
    auto* sceneBlock = detail::findParameterBlock(unifiedShader, "sceneData");
    auto* objectBlock = detail::findParameterBlock(unifiedShader, "objectData");
    if (!viewBlock || !sceneBlock || !objectBlock) {
        return LN_MAKE_ERROR("Missing required ParameterBlocks (viewData, sceneData, objectData)");
    }

    int16_t viewSetIndex = viewBlock->setIndex;
    int16_t sceneSetIndex = sceneBlock->setIndex;
    int16_t objectSetIndex = objectBlock->setIndex;

    // Build BindGroupLayoutDescs from reflection (with dynamic offset for UBOs)
    auto viewLayoutDesc = detail::buildBindGroupLayoutFromReflection(*viewBlock, targetPass->bindingLayout);
    for (auto& entry : viewLayoutDesc.entries) {
        if (entry.type == rhi::BindingType::UniformBuffer) entry.hasDynamicOffset = true;
    }

    auto sceneLayoutDesc = detail::buildBindGroupLayoutFromReflection(*sceneBlock, targetPass->bindingLayout);
    for (auto& entry : sceneLayoutDesc.entries) {
        if (entry.type == rhi::BindingType::UniformBuffer) entry.hasDynamicOffset = true;
    }

    auto objectLayoutDesc = detail::buildBindGroupLayoutFromReflection(*objectBlock, targetPass->bindingLayout);
    for (auto& entry : objectLayoutDesc.entries) {
        if (entry.type == rhi::BindingType::UniformBuffer) entry.hasDynamicOffset = true;
    }

    int16_t objectCBSize = detail::findConstantBufferSize(*objectBlock);
    if (objectCBSize <= 0) {
        return LN_MAKE_ERROR("Invalid object constant buffer size in reflection");
    }

    // Collect binding names for the material set (parallel to materialLayoutDesc entries)
    std::vector<std::string> materialBindingNames;
    for (const auto& binding : targetPass->bindingLayout.bindings) {
        if (binding.setIndex == materialSetIdx) {
            materialBindingNames.push_back(binding.name);
        }
    }

    // Determine the max set index to size the PipelineLayoutDesc properly
    int16_t maxSet = std::max({materialSetIdx, viewSetIndex, sceneSetIndex, objectSetIndex});

    // Assemble PipelineLayoutDesc using reflection-based set indices
    rhi::PipelineLayoutDesc plDesc;
    plDesc.setLayouts.resize(static_cast<size_t>(maxSet) + 1);
    if (materialSetIdx >= 0) {
        plDesc.setLayouts[materialSetIdx] = materialLayoutDesc;
    }
    plDesc.setLayouts[viewSetIndex] = viewLayoutDesc;
    plDesc.setLayouts[sceneSetIndex] = sceneLayoutDesc;
    plDesc.setLayouts[objectSetIndex] = objectLayoutDesc;

    // Convert GlobalMemberInfo to MaterialMemberInfo
    std::vector<MaterialMemberInfo> members;
    if (materialBlock) {
        members.reserve(materialBlock->members.size());
        for (const auto& gm : materialBlock->members) {
            members.push_back({gm.name, gm.offset, gm.size});
        }
    }

    // Create Instance
    auto plResult = device->createPipelineLayout(plDesc);
    if (!plResult) {
        return LN_FORWARD_ERROR(plResult);
    }

    auto sp = Ref<ShaderPass>::adopt(new ShaderPass());
    sp->m_passName = globalPass->name;
    sp->m_vertShader = std::move(*vsResult);
    sp->m_fragShader = std::move(*fsResult);
    // NOTE: エントリポイント名はコピーする。同一の UnifiedShader2 から複数のパスを
    // 構築する場合 (Shader が全パスを一度に構築する経路) に move してしまうと、
    // 2 つ目以降のパスがエントリポイント名を失うため。
    sp->m_vertEntry = vertEP->name;
    sp->m_fragEntry = fragEP->name;
    sp->m_pipelineLayout = std::move(*plResult);
    sp->m_materialParamBufferSize = static_cast<uint64_t>(cbSize);
    sp->m_materialSetIndex = materialSetIdx;
    sp->m_materialMembers = std::move(members);
    sp->m_materialLayoutDesc = materialLayoutDesc;
    sp->m_materialSamplerTextureNames =
        resolveSamplerTextureNames(materialLayoutDesc, materialBindingNames);
    sp->m_materialBindingNames = std::move(materialBindingNames);
    sp->m_viewSetIndex = viewSetIndex;
    sp->m_sceneSetIndex = sceneSetIndex;
    sp->m_objectSetIndex = objectSetIndex;
    sp->m_viewLayoutDesc = std::move(viewLayoutDesc);
    sp->m_sceneLayoutDesc = std::move(sceneLayoutDesc);
    sp->m_objectLayoutDesc = std::move(objectLayoutDesc);
    sp->m_objectUBOSize = static_cast<uint64_t>(objectCBSize);

    return sp;
}

//------------------------------------------------------------------------------
Result<Ref<ShaderPass>> ShaderPass::createFromCompiledShader(
    const void* data, size_t size,
    rhi::Device* device,
    size_t passIndex) {

    // Deserialize the unified shader from the binary blob.
    auto loadResult = shader::UnifiedShaderSerializer2::loadFromData(data, size);
    if (!loadResult) {
        return LN_FORWARD_ERROR(loadResult);
    }
    auto unifiedShader = std::move(*loadResult);

    return buildFromUnifiedShader(unifiedShader.get(), device, passIndex);
}

#ifdef LUMINO_USE_SLANG
//------------------------------------------------------------------------------
Result<Ref<ShaderPass>> ShaderPass::createFromUnifiedShader(
    shader::UnifiedShader2* unifiedShader,
    rhi::Device* device,
    size_t passIndex) {
    return buildFromUnifiedShader(unifiedShader, device, passIndex);
}
#endif // LUMINO_USE_SLANG

} // namespace ln
