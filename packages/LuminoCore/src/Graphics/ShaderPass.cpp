#include <LuminoCore/Graphics/ShaderPass.hpp>
#include <LuminoShader/UnifiedShader2.hpp>
#include <LuminoShader/UnifiedShaderSerializer2.hpp>
#include "ShaderUtils.hpp"
#include <algorithm>

namespace ln {

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
Result<Ref<ShaderPass>> ShaderPass::createFromCompiledShader(
    const void* data, size_t size,
    rhi::Device* device) {

    // Deserialize the unified shader from the binary blob.
    auto loadResult = shader::UnifiedShaderSerializer2::loadFromData(data, size);
    if (!loadResult) {
        return LN_FORWARD_ERROR(loadResult);
    }
    auto unifiedShader = std::move(*loadResult);

    // Find the first pass for the backend's shader target.
    auto& globalPasses = unifiedShader->globalShaderPasses();
    if (globalPasses.empty()) {
        return LN_MAKE_ERROR("No shader passes found");
    }

    auto shaderTarget = backendToShaderTarget(device->backend());
    auto codeFormat = backendToCodeFormat(device->backend());

    auto* globalPass = globalPasses[0].get();
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

    // Build material BindGroupLayout from "$Material" ParameterBlock reflection
    auto* materialBlock = detail::findParameterBlock(unifiedShader.get(), "$Material");
    if (!materialBlock) {
        return LN_MAKE_ERROR("No '$Material' ParameterBlock found");
    }

    auto materialLayoutDesc = detail::buildBindGroupLayoutFromReflection(*materialBlock, targetPass->bindingLayout);
    int16_t materialSetIdx = materialBlock->setIndex;

    // Get material CB size from reflection
    int16_t cbSize = detail::findConstantBufferSize(*materialBlock);
    if (cbSize <= 0) {
        return LN_MAKE_ERROR("No constant buffer found in '$Material'");
    }

    // Discover view/scene/object blocks from reflection
    auto* viewBlock = detail::findParameterBlock(unifiedShader.get(), "viewData");
    auto* sceneBlock = detail::findParameterBlock(unifiedShader.get(), "sceneData");
    auto* objectBlock = detail::findParameterBlock(unifiedShader.get(), "objectData");
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

    // Create Instance
    {
        auto plResult = device->createPipelineLayout(plDesc);
        if (!plResult) {
            return LN_FORWARD_ERROR(plResult);
        }

        auto sp = Ref<ShaderPass>::adopt(new ShaderPass());
        sp->m_vertShader = std::move(*vsResult);
        sp->m_fragShader = std::move(*fsResult);
        sp->m_vertEntry = std::move(vertEP->name);
        sp->m_fragEntry = std::move(fragEP->name);
        sp->m_pipelineLayout = std::move(*plResult);
        sp->m_materialParamBufferSize = static_cast<u64>(cbSize);
        sp->m_materialSetIndex = materialSetIdx;
        sp->m_materialMembers = std::move(members);
        sp->m_materialLayoutDesc = materialLayoutDesc;
        sp->m_materialBindingNames = std::move(materialBindingNames);
        sp->m_viewSetIndex = viewSetIndex;
        sp->m_sceneSetIndex = sceneSetIndex;
        sp->m_objectSetIndex = objectSetIndex;
        sp->m_viewLayoutDesc = std::move(viewLayoutDesc);
        sp->m_sceneLayoutDesc = std::move(sceneLayoutDesc);
        sp->m_objectLayoutDesc = std::move(objectLayoutDesc);
        sp->m_objectUBOSize = static_cast<u64>(objectCBSize);

        return sp;
    }
}

} // namespace ln
