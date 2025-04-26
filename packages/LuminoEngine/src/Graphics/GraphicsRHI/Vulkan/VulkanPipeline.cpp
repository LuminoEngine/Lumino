#include <LuminoEngine/Graphics/GraphicsRHI/Vulkan/VulkanDevice.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/Vulkan/VulkanShaderPass.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/Vulkan/VulkanVertexLayout.hpp>
#include <LuminoEngine/Graphics/GraphicsRHI/Vulkan/VulkanPipeline.hpp>

namespace ln {
namespace detail {

VulkanPipeline::VulkanPipeline()
    : m_device(nullptr)
    , m_ownerRenderPass(nullptr)
    , m_pipeline(VK_NULL_HANDLE) {
}

Result_deprecated<> VulkanPipeline::init(VulkanDevice* deviceContext, const DevicePipelineStateDesc& state) {
    LN_DCHECK(deviceContext);
    m_device = deviceContext;

    if (state.renderPass) {
        return createGraphicsPipeline(state);
    }
    else {
        return createComputePipeline(state);
    }
}

void VulkanPipeline::onDestroy() {
    if (m_pipeline) {
        vkDestroyPipeline(m_device->vulkanDevice(), m_pipeline, m_device->vulkanAllocator());
        m_pipeline = VK_NULL_HANDLE;
    }

    m_ownerRenderPass = nullptr;
    m_device = nullptr;
    IPipeline::onDestroy();
}

Result_deprecated<> VulkanPipeline::createGraphicsPipeline(const DevicePipelineStateDesc& state) {
    LN_DCHECK(state.renderPass);
    m_ownerRenderPass = static_cast<VulkanRenderPass2*>(state.renderPass);

    auto* vertexDeclaration = static_cast<VulkanVertexDeclaration*>(state.vertexDeclaration);
    auto* shaderPass = static_cast<VulkanShaderPass*>(state.shaderPass);
    //m_relatedFramebuffer = m_deviceContext->framebufferCache()->findOrCreate(state.framebufferState);

    VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = shaderPass->vulkanVertShaderModule();
    vertShaderStageInfo.pName = shaderPass->vertEntryPointName().c_str();

    VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = shaderPass->vulkanFragShaderModule();
    fragShaderStageInfo.pName = shaderPass->fragEntryPointName().c_str();

    VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

    //if (shaderPass->isComputeShader()) {
    //    VkPipelineShaderStageCreateInfo csShaderStageInfo = {};
    //    csShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    //    csShaderStageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
    //    csShaderStageInfo.module = shaderPass->vulkanCompShaderModule();
    //    csShaderStageInfo.pName = shaderPass->compEntryPointName().c_str();
    //}

    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;

    std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
    if (shaderPass->m_isVer2) {
        auto result1 = vertexDeclaration->createPipelineVertexLayout(shaderPass, &attributeDescriptions);
        if (!result1) {
            return err(result1.error());
        }
    }
    else {
        const auto& attrs = shaderPass->attributes();
        for (size_t i = 0; i < attrs.size(); i++) {
            const auto& attr = attrs[i];
            if (const auto* s = vertexDeclaration->findAttributeDescriptionSource(attr.usage, attr.index)) {
                VkVertexInputAttributeDescription desc;
                desc.location = i;
                desc.binding = s->binding;
                desc.format = s->format;
                desc.offset = s->offset;
                attributeDescriptions.push_back(desc);
            }
            else {
                // Shader が必要としている情報が頂点バッファ側に足りていない。警告が出る。
            }
        }
    }


    const auto& bindingDescription = vertexDeclaration->vertexBindingDescriptions();
    vertexInputInfo.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescription.size());
    vertexInputInfo.pVertexBindingDescriptions = bindingDescription.data();
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VulkanHelper::LNPrimitiveTopologyToVkPrimitiveTopology(state.topology);
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkViewport viewport = {};
    //viewport.x = 0.0f;
    //viewport.y = 0.0f;
    //viewport.width = state.regionRects.viewportRect.width; //(float)swapChainExtent.width;
    //viewport.height = state.regionRects.viewportRect.height;//(float)swapChainExtent.height;
    //viewport.minDepth = 0.0f;
    //viewport.maxDepth = 1.0f;

    VkRect2D scissor = {};
    //scissor.offset = { 0, 0 };
    //scissor.extent.width = state.regionRects.scissorRect.width;;
    //scissor.extent.height = state.regionRects.scissorRect.height;;

    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    VkPipelineRasterizationStateCreateInfo rasterizerInfo = {};
    {
        const RasterizerStateDesc& desc = state.rasterizerState;

        rasterizerInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizerInfo.depthClampEnable = VK_FALSE;
        rasterizerInfo.rasterizerDiscardEnable = VK_FALSE;
        rasterizerInfo.polygonMode = VulkanHelper::LNFillModeToVkPolygonMode(desc.fillMode);
        rasterizerInfo.cullMode = VulkanHelper::LNCullModeToVkCullMode(desc.cullMode);
#ifdef LN_FACE_FRONT_CW
        rasterizerInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
#else
        rasterizerInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
#endif
        rasterizerInfo.depthBiasEnable = VK_FALSE;
        rasterizerInfo.depthBiasConstantFactor = 0.0f;
        rasterizerInfo.depthBiasClamp = 0.0f;
        rasterizerInfo.depthBiasSlopeFactor = 0.0f;
        rasterizerInfo.lineWidth = 1.0f;
    }
    //VkPipelineRasterizationStateCreateInfo rasterizer = {};
    //rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    //rasterizer.depthClampEnable = VK_FALSE;
    //rasterizer.rasterizerDiscardEnable = VK_FALSE;
    //rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    //rasterizer.lineWidth = 1.0f;
    //rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    //rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE; // Viewport height を反転しているので、時計回りを正面 //VK_FRONT_FACE_COUNTER_CLOCKWISE;
    //rasterizer.depthBiasEnable = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = m_ownerRenderPass->containsMultisampleTarget() ? m_device->msaaSamples() : VK_SAMPLE_COUNT_1_BIT;

    //VkPipelineDepthStencilStateCreateInfo depthStencil = {};
    //depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    //depthStencil.depthTestEnable = VK_TRUE;
    //depthStencil.depthWriteEnable = VK_TRUE;
    //depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
    //depthStencil.depthBoundsTestEnable = VK_FALSE;
    //depthStencil.stencilTestEnable = VK_FALSE;
    VkPipelineDepthStencilStateCreateInfo depthStencilStateInfo = {};
    {
        const DepthStencilStateDesc& desc = state.depthStencilState;

        depthStencilStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        depthStencilStateInfo.pNext = nullptr;
        depthStencilStateInfo.flags = 0;
        depthStencilStateInfo.depthTestEnable = (desc.depthTestFunc == ComparisonFunc::Always ? VK_FALSE : VK_TRUE);
        depthStencilStateInfo.depthWriteEnable = (desc.depthWriteEnabled ? VK_TRUE : VK_FALSE);
        depthStencilStateInfo.depthCompareOp = VulkanHelper::LNComparisonFuncToVkCompareOp(desc.depthTestFunc);
        depthStencilStateInfo.depthBoundsTestEnable = VK_FALSE;
        depthStencilStateInfo.stencilTestEnable = (desc.stencilEnabled ? VK_TRUE : VK_FALSE);

        depthStencilStateInfo.front.failOp = VulkanHelper::LNStencilOpToVkStencilOp(desc.frontFace.stencilFailOp);
        depthStencilStateInfo.front.passOp = VulkanHelper::LNStencilOpToVkStencilOp(desc.frontFace.stencilPassOp);
        depthStencilStateInfo.front.depthFailOp = VulkanHelper::LNStencilOpToVkStencilOp(desc.frontFace.stencilDepthFailOp);
        depthStencilStateInfo.front.compareOp = VulkanHelper::LNComparisonFuncToVkCompareOp(desc.frontFace.stencilFunc);
        depthStencilStateInfo.front.compareMask = 0xff;
        depthStencilStateInfo.front.writeMask = 0xff;
        depthStencilStateInfo.front.reference = desc.stencilReferenceValue;

        depthStencilStateInfo.back.failOp = VulkanHelper::LNStencilOpToVkStencilOp(desc.backFace.stencilFailOp);
        depthStencilStateInfo.back.passOp = VulkanHelper::LNStencilOpToVkStencilOp(desc.backFace.stencilPassOp);
        depthStencilStateInfo.back.depthFailOp = VulkanHelper::LNStencilOpToVkStencilOp(desc.backFace.stencilDepthFailOp);
        depthStencilStateInfo.back.compareOp = VulkanHelper::LNComparisonFuncToVkCompareOp(desc.backFace.stencilFunc);
        depthStencilStateInfo.back.compareMask = 0xff;
        depthStencilStateInfo.back.writeMask = 0xff;
        depthStencilStateInfo.back.reference = desc.stencilReferenceValue;

        depthStencilStateInfo.minDepthBounds = 0.0f;
        depthStencilStateInfo.maxDepthBounds = 0.0f;
    }

    auto& framebuffer = m_ownerRenderPass->framebuffer();
    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;
    VkPipelineColorBlendStateCreateInfo colorBlending = {};
    VkPipelineColorBlendAttachmentState colorBlendAttachments[BlendStateDesc::MaxRenderTargets] = {};
    {
        const BlendStateDesc& desc = state.blendState;
        int attachmentsCount = 0;
        for (int i = 0; i < BlendStateDesc::MaxRenderTargets; i++) {
            if (framebuffer->renderTargets()[i]) {
                const RenderTargetBlendDesc* atdesc = (desc.independentBlendEnable) ? &desc.renderTargets[i] : &desc.renderTargets[0];

                colorBlendAttachments[i].blendEnable = (atdesc->blendEnable) ? VK_TRUE : VK_FALSE;

                colorBlendAttachments[i].srcColorBlendFactor = VulkanHelper::LNBlendFactorToVkBlendFactor_Color(atdesc->sourceBlend);
                colorBlendAttachments[i].dstColorBlendFactor = VulkanHelper::LNBlendFactorToVkBlendFactor_Color(atdesc->destinationBlend);
                colorBlendAttachments[i].colorBlendOp = VulkanHelper::LNBlendOpToVkBlendOp(atdesc->blendOp);

                colorBlendAttachments[i].srcAlphaBlendFactor = VulkanHelper::LNBlendFactorToVkBlendFactor_Alpha(atdesc->sourceBlendAlpha);
                colorBlendAttachments[i].dstAlphaBlendFactor = VulkanHelper::LNBlendFactorToVkBlendFactor_Alpha(atdesc->destinationBlendAlpha);
                colorBlendAttachments[i].alphaBlendOp = VulkanHelper::LNBlendOpToVkBlendOp(atdesc->blendOpAlpha);

                colorBlendAttachments[i].colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

                attachmentsCount++;
            }
        }

        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY;
        colorBlending.attachmentCount = attachmentsCount;
        colorBlending.pAttachments = colorBlendAttachments;
        colorBlending.blendConstants[0] = 1.0f;
        colorBlending.blendConstants[1] = 1.0f;
        colorBlending.blendConstants[2] = 1.0f;
        colorBlending.blendConstants[3] = 1.0f;
    }
    //VkPipelineColorBlendStateCreateInfo colorBlending = {};
    //colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    //colorBlending.logicOpEnable = VK_FALSE;
    //colorBlending.logicOp = VK_LOGIC_OP_COPY;
    //colorBlending.attachmentCount = 1;
    //colorBlending.pAttachments = &colorBlendAttachment;
    //colorBlending.blendConstants[0] = 0.0f;
    //colorBlending.blendConstants[1] = 0.0f;
    //colorBlending.blendConstants[2] = 0.0f;
    //colorBlending.blendConstants[3] = 0.0f;

    const VkDynamicState dynamicStates[] = {
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
        //VK_DYNAMIC_STATE_BLEND_CONSTANTS,
        //VK_DYNAMIC_STATE_STENCIL_REFERENCE,
    };
    VkPipelineDynamicStateCreateInfo dynamicState;
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.pNext = nullptr;
    dynamicState.flags = 0;
    dynamicState.dynamicStateCount = LN_ARRAY_SIZE_OF(dynamicStates);
    dynamicState.pDynamicStates = dynamicStates;

    //VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    //pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    //pipelineLayoutInfo.setLayoutCount = m_shaderPass->descriptorSetLayouts().size();
    //pipelineLayoutInfo.pSetLayouts = m_shaderPass->descriptorSetLayouts().data();

    //if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
    //    throw std::runtime_error("failed to create pipeline layout!");
    //}

    //renderPass = framebuffer->vulkanRenderPass();//

    VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
    pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineCreateInfo.stageCount = 2;
    pipelineCreateInfo.pStages = shaderStages;
    pipelineCreateInfo.pVertexInputState = &vertexInputInfo;
    pipelineCreateInfo.pInputAssemblyState = &inputAssembly;
    pipelineCreateInfo.pViewportState = &viewportState;
    pipelineCreateInfo.pRasterizationState = &rasterizerInfo;
    pipelineCreateInfo.pMultisampleState = &multisampling;
    pipelineCreateInfo.pDepthStencilState = &depthStencilStateInfo;
    pipelineCreateInfo.pColorBlendState = &colorBlending;
    pipelineCreateInfo.layout = shaderPass->vulkanPipelineLayout(); // 省略不可 https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/VkGraphicsPipelineCreateInfo.html
    pipelineCreateInfo.pDynamicState = &dynamicState;
    pipelineCreateInfo.renderPass = m_ownerRenderPass->nativeRenderPass();
    pipelineCreateInfo.subpass = 0;
    pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;

    // FIXME:
    // - validation layer: vkCreateGraphicsPipelines(): pCreateInfos[0].pVertexInputState Vertex attribute at location 1 not consumed by vertex shader.
    // 2025/4/13 時点 の slang では、ある VertexAttribute が実際に使われているかどうかを判断する方法が無いようだ。
    // 使われていないものは pVertexAttributeDescriptions に入れないようにするべきなのだが、今はそれができない。
    // どうしてもやるなら、 spirv-dis 等を使って調べる必要がある。
    LN_VK_CHECK(vkCreateGraphicsPipelines(m_device->vulkanDevice(), VK_NULL_HANDLE, 1, &pipelineCreateInfo, m_device->vulkanAllocator(), &m_pipeline));

    return ok();
}

Result_deprecated<> VulkanPipeline::createComputePipeline(const DevicePipelineStateDesc& state) {
    auto* shaderPass = static_cast<VulkanShaderPass*>(state.shaderPass);

    VkPipelineShaderStageCreateInfo shaderStage = {};
    shaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStage.flags = 0;
    shaderStage.stage = VK_SHADER_STAGE_COMPUTE_BIT;
    shaderStage.module = shaderPass->vulkanCompShaderModule();
    shaderStage.pName = shaderPass->compEntryPointName().c_str();
    shaderStage.pSpecializationInfo = nullptr;

    // Create pipeline
    VkComputePipelineCreateInfo pipelineCreateInfo; // = vks::initializers::computePipelineCreateInfo(compute.pipelineLayout, 0);
    pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pipelineCreateInfo.pNext = nullptr;
    pipelineCreateInfo.flags = 0;
    pipelineCreateInfo.stage = shaderStage;
    pipelineCreateInfo.layout = shaderPass->vulkanPipelineLayout();
    pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineCreateInfo.basePipelineIndex = 0;

    //VkGraphicsPipelineCreateInfo pipelineCreateInfo = {};
    //pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    //pipelineInfo.stageCount = 2;
    //pipelineInfo.pStages = shaderStages;
    //pipelineInfo.pVertexInputState = &vertexInputInfo;
    //pipelineInfo.pInputAssemblyState = &inputAssembly;
    //pipelineInfo.pViewportState = &viewportState;
    //pipelineInfo.pRasterizationState = &rasterizerInfo;
    //pipelineInfo.pMultisampleState = &multisampling;
    //pipelineInfo.pDepthStencilState = &depthStencilStateInfo;
    //pipelineInfo.pColorBlendState = &colorBlending;
    //pipelineInfo.layout = shaderPass->vulkanPipelineLayout();	// 省略不可 https://www.khronos.org/registry/vulkan/specs/1.1-extensions/man/html/VkGraphicsPipelineCreateInfo.html
    //pipelineInfo.pDynamicState = &dynamicState;
    //pipelineInfo.renderPass = m_ownerRenderPass->nativeRenderPass();
    //pipelineInfo.subpass = 0;
    //pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    LN_VK_CHECK(vkCreateComputePipelines(m_device->vulkanDevice(), VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &m_pipeline));

    return ok();
}

} // namespace detail
} // namespace ln
