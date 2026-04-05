#include <LuminoCore/Graphics/ShaderPass.hpp>
#include <LuminoShader/UnifiedShader2.hpp>
#include <LuminoShader/UnifiedShaderSerializer2.hpp>
#include "ShaderUtils.hpp"
#include <algorithm>

namespace ln {

using detail::buildBindGroupLayoutFromReflection;
using detail::findParameterBlock;
using detail::findConstantBufferSize;

//------------------------------------------------------------------------------
Result<Ref<ShaderPass>> ShaderPass::create(
    Ref<rhi::ShaderModule> vertShader,
    Ref<rhi::ShaderModule> fragShader,
    std::string vertEntry,
    std::string fragEntry,
    rhi::PipelineLayoutDesc pipelineLayoutDesc,
    u64 materialParamBufferSize,
    int16_t materialSetIndex,
    std::vector<MaterialMemberInfo> materialMembers,
    rhi::Device* device) {

    auto plResult = device->createPipelineLayout(pipelineLayoutDesc);
    if (!plResult) return tl::make_unexpected(plResult.error());

    auto sp = Ref<ShaderPass>::adopt(new ShaderPass());
    sp->m_vertShader = std::move(vertShader);
    sp->m_fragShader = std::move(fragShader);
    sp->m_vertEntry = std::move(vertEntry);
    sp->m_fragEntry = std::move(fragEntry);
    sp->m_pipelineLayout = std::move(*plResult);
    sp->m_materialParamBufferSize = materialParamBufferSize;
    sp->m_materialSetIndex = materialSetIndex;
    sp->m_materialMembers = std::move(materialMembers);
    return sp;
}

//------------------------------------------------------------------------------
Result<Ref<ShaderPass>> ShaderPass::createFromCompiledShader(
    const void* data, size_t size,
    const rhi::BindGroupLayoutDesc& viewLayoutDesc,
    const rhi::BindGroupLayoutDesc& sceneLayoutDesc,
    const rhi::BindGroupLayoutDesc& objectLayoutDesc,
    int16_t viewSetIndex,
    int16_t sceneSetIndex,
    int16_t objectSetIndex,
    rhi::Device* device) {

    // Deserialize the unified shader from the binary blob.
    auto loadResult = shader::UnifiedShaderSerializer2::loadFromData(data, size);
    if (!loadResult) return tl::make_unexpected(loadResult.error());
    auto unifiedShader = std::move(*loadResult);

    // Find the first pass for SPIR-V target.
    auto& globalPasses = unifiedShader->globalShaderPasses();
    if (globalPasses.empty()) {
        return tl::make_unexpected(Error{ErrorCode::RuntimeError, "No shader passes found"});
    }

    auto* globalPass = globalPasses[0].get();
    auto targetPassId = globalPass->getTargetShaderPassId(shader::ShaderTarget_SPIRV);
    auto* targetPass = unifiedShader->targetShaderPass(targetPassId);
    if (!targetPass) {
        return tl::make_unexpected(Error{ErrorCode::RuntimeError, "No SPIRV target pass"});
    }

    auto* vertEP = unifiedShader->targetEntryPoint(targetPass->vertEntryPointId);
    auto* fragEP = unifiedShader->targetEntryPoint(targetPass->fragEntryPointId);
    if (!vertEP || !fragEP) {
        return tl::make_unexpected(Error{ErrorCode::RuntimeError, "Missing entry points"});
    }

    auto* vertBlob = unifiedShader->blob(vertEP->codeBlobId);
    auto* fragBlob = unifiedShader->blob(fragEP->codeBlobId);

    // Create shader modules
    rhi::ShaderModuleDesc vsDesc;
    vsDesc.spirvCode = reinterpret_cast<const u32*>(vertBlob->data.data());
    vsDesc.spirvSizeBytes = vertBlob->data.size();
    auto vsResult = device->createShaderModule(vsDesc);
    if (!vsResult) return tl::make_unexpected(vsResult.error());

    rhi::ShaderModuleDesc fsDesc;
    fsDesc.spirvCode = reinterpret_cast<const u32*>(fragBlob->data.data());
    fsDesc.spirvSizeBytes = fragBlob->data.size();
    auto fsResult = device->createShaderModule(fsDesc);
    if (!fsResult) return tl::make_unexpected(fsResult.error());

    // Build material BindGroupLayout from "$Material" ParameterBlock reflection
    auto* materialBlock = findParameterBlock(unifiedShader.get(), "$Material");
    if (!materialBlock) {
        return tl::make_unexpected(Error{ErrorCode::RuntimeError, "No '$Material' ParameterBlock found"});
    }

    auto materialLayoutDesc = buildBindGroupLayoutFromReflection(*materialBlock, targetPass->bindingLayout);
    int16_t materialSetIdx = materialBlock->setIndex;

    // Get material CB size from reflection
    int16_t cbSize = findConstantBufferSize(*materialBlock);
    if (cbSize <= 0) {
        return tl::make_unexpected(Error{ErrorCode::RuntimeError, "No constant buffer found in '$Material'"});
    }

    // Determine the max set index to size the PipelineLayoutDesc properly
    int16_t maxSet = std::max({materialSetIdx, viewSetIndex, sceneSetIndex, objectSetIndex});

    // Assemble PipelineLayoutDesc using reflection-based set indices
    rhi::PipelineLayoutDesc plDesc;
    plDesc.setLayouts.resize(static_cast<size_t>(maxSet) + 1);
    plDesc.setLayouts[materialSetIdx] = materialLayoutDesc;
    plDesc.setLayouts[viewSetIndex] = viewLayoutDesc;
    plDesc.setLayouts[sceneSetIndex] = sceneLayoutDesc;
    plDesc.setLayouts[objectSetIndex] = objectLayoutDesc;

    // Convert GlobalMemberInfo to MaterialMemberInfo
    std::vector<MaterialMemberInfo> members;
    members.reserve(materialBlock->members.size());
    for (const auto& gm : materialBlock->members) {
        members.push_back({gm.name, gm.offset, gm.size});
    }

    return create(
        std::move(*vsResult), std::move(*fsResult),
        vertEP->name, fragEP->name,
        std::move(plDesc),
        static_cast<u64>(cbSize),
        materialSetIdx,
        std::move(members),
        device);
}

} // namespace ln
