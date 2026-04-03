#include <LuminoCore/Graphics/ShaderPass.hpp>
#include <LuminoShader/UnifiedShader2.hpp>
#include <LuminoShader/UnifiedShaderSerializer2.hpp>
#include "ShaderUtils.hpp"

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
    return sp;
}

//------------------------------------------------------------------------------
Result<Ref<ShaderPass>> ShaderPass::createFromCompiledShader(
    const void* data, size_t size,
    const rhi::BindGroupLayoutDesc& viewLayoutDesc,
    const rhi::BindGroupLayoutDesc& sceneLayoutDesc,
    const rhi::BindGroupLayoutDesc& objectLayoutDesc,
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

    // Build material BindGroupLayout from "materialData" ParameterBlock reflection
    auto* materialBlock = findParameterBlock(unifiedShader.get(), "materialData");
    if (!materialBlock) {
        return tl::make_unexpected(Error{ErrorCode::RuntimeError, "No 'materialData' ParameterBlock found"});
    }

    auto materialLayoutDesc = buildBindGroupLayoutFromReflection(*materialBlock, targetPass->bindingLayout);

    // Get material CB size from reflection
    int16_t cbSize = findConstantBufferSize(*materialBlock);
    if (cbSize <= 0) {
        return tl::make_unexpected(Error{ErrorCode::RuntimeError, "No constant buffer found in 'materialData'"});
    }

    // Assemble PipelineLayoutDesc: Set0=view, Set1=scene, Set2=material, Set3=object
    rhi::PipelineLayoutDesc plDesc;
    plDesc.setLayouts = {viewLayoutDesc, sceneLayoutDesc, materialLayoutDesc, objectLayoutDesc};

    return create(
        std::move(*vsResult), std::move(*fsResult),
        vertEP->name, fragEP->name,
        std::move(plDesc),
        static_cast<u64>(cbSize),
        device);
}

} // namespace ln
