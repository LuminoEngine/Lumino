#include <algorithm>
#include <cassert>
#include <cstring>
#include <iostream>
#include <numeric>
#include <unordered_set>

#include <LuminoBase/SmallVector.hpp>
#include "VulkanHelpers.hpp"
#include "VulkanBuffer.hpp"
#include "VulkanTextureView.hpp"
#include "VulkanBackend.hpp"
#include "VulkanDevice.hpp"

namespace ln::rhi::vulkan {

// ------ VulkanSampler --------------------------------------------------------------------------------------------------------------

static VkFilter toVkFilter(FilterMode f) {
    return f == FilterMode::Nearest ? VK_FILTER_NEAREST : VK_FILTER_LINEAR;
}
static VkSamplerAddressMode toVkAddressMode(AddressMode a) {
    switch (a) {
        case AddressMode::Repeat:         return VK_SAMPLER_ADDRESS_MODE_REPEAT;
        case AddressMode::MirroredRepeat: return VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
        case AddressMode::ClampToEdge:    return VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    }
    return VK_SAMPLER_ADDRESS_MODE_REPEAT;
}

VulkanSampler::VulkanSampler() = default;

VoidResult VulkanSampler::init(VulkanDevice* device, const SamplerDesc& desc) {
    m_device = device;

    VkSamplerCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    info.magFilter = toVkFilter(desc.magFilter);
    info.minFilter = toVkFilter(desc.minFilter);
    info.addressModeU = toVkAddressMode(desc.addressU);
    info.addressModeV = toVkAddressMode(desc.addressV);
    info.addressModeW = toVkAddressMode(desc.addressW);
    info.maxAnisotropy = static_cast<float>(desc.maxAnisotropy);
    info.anisotropyEnable = desc.maxAnisotropy > 1 ? VK_TRUE : VK_FALSE;
    info.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    info.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    if (vkCreateSampler(m_device->vkDevice(), &info, nullptr, &m_sampler) != VK_SUCCESS) {
        return LN_MAKE_ERROR("vkCreateSampler failed.");
    }
    return LN_MAKE_SUCCESS();
}

void VulkanSampler::finalize() {
    VkDevice dev = m_device->vkDevice();
    VkSampler s = m_sampler;
    m_device->frameResources().queueDelete(m_device->currentFrameIndex(), [dev, s]() {
        if (s) vkDestroySampler(dev, s, nullptr);
    });
    Sampler::finalize();
}

// ------ VulkanShaderModule ----------------------------------------------------------------------------------------------------

VulkanShaderModule::VulkanShaderModule() = default;

VoidResult VulkanShaderModule::init(VulkanDevice* device, const ShaderModuleDesc& desc) {
    m_device = device;

    if (desc.format != ShaderCodeFormat::SPIRV) {
        return LN_MAKE_ERROR("VulkanShaderModule only accepts SPIRV format.");
    }

    const uint32_t* spirvCode = static_cast<const uint32_t*>(desc.code);
    m_spirv.assign(spirvCode, spirvCode + desc.codeSizeBytes / sizeof(uint32_t));

    VkShaderModuleCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    info.codeSize = desc.codeSizeBytes;
    info.pCode = spirvCode;
    if (vkCreateShaderModule(m_device->vkDevice(), &info, nullptr, &m_module) != VK_SUCCESS) {
        return LN_MAKE_ERROR("vkCreateShaderModule failed.");
    }
    return LN_MAKE_SUCCESS();
}

void VulkanShaderModule::finalize() {
    if (m_module) vkDestroyShaderModule(m_device->vkDevice(), m_module, nullptr);
    ShaderModule::finalize();
}

// ------ VulkanBindGroupLayout ----------------------------------------------------------------------------------------------

static VkDescriptorType toVkDescriptorType(BindingType t) {
    switch (t) {
        case BindingType::UniformBuffer:  return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        case BindingType::StorageBuffer:  return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        case BindingType::SampledTexture: return VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
        case BindingType::StorageTexture: return VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
        case BindingType::Sampler:        return VK_DESCRIPTOR_TYPE_SAMPLER;
    }
    return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
}

static VkShaderStageFlags toVkShaderStage(ShaderStage s) {
    VkShaderStageFlags flags = 0;
    if (static_cast<uint32_t>(s) & static_cast<uint32_t>(ShaderStage::Vertex))   flags |= VK_SHADER_STAGE_VERTEX_BIT;
    if (static_cast<uint32_t>(s) & static_cast<uint32_t>(ShaderStage::Fragment)) flags |= VK_SHADER_STAGE_FRAGMENT_BIT;
    if (static_cast<uint32_t>(s) & static_cast<uint32_t>(ShaderStage::Compute))  flags |= VK_SHADER_STAGE_COMPUTE_BIT;
    return flags;
}

VulkanBindGroupLayout::VulkanBindGroupLayout() = default;

VoidResult VulkanBindGroupLayout::init(VulkanDevice* device, const BindGroupLayoutDesc& desc) {
    m_device = device;

    std::vector<VkDescriptorSetLayoutBinding> bindings;
    bindings.reserve(desc.entries.size());
    m_dynamicFlags.reserve(desc.entries.size());
    for (auto& e : desc.entries) {
        VkDescriptorSetLayoutBinding b{};
        b.binding = e.binding;
        b.descriptorCount = 1;
        b.stageFlags = toVkShaderStage(e.visibility);

        // Use dynamic descriptor type when hasDynamicOffset is set.
        bool isDynamic = e.hasDynamicOffset &&
            (e.type == BindingType::UniformBuffer || e.type == BindingType::StorageBuffer);
        if (isDynamic) {
            b.descriptorType = (e.type == BindingType::UniformBuffer)
                ? VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC
                : VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
        } else {
            b.descriptorType = toVkDescriptorType(e.type);
        }

        bindings.push_back(b);
        m_dynamicFlags.push_back(isDynamic);
    }

    VkDescriptorSetLayoutCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    info.bindingCount = static_cast<uint32_t>(bindings.size());
    info.pBindings = bindings.data();
    if (vkCreateDescriptorSetLayout(m_device->vkDevice(), &info, nullptr, &m_layout) != VK_SUCCESS) {
        return LN_MAKE_ERROR("vkCreateDescriptorSetLayout failed.");
    }
    return LN_MAKE_SUCCESS();
}

void VulkanBindGroupLayout::finalize() {
    if (m_layout) vkDestroyDescriptorSetLayout(m_device->vkDevice(), m_layout, nullptr);
    BindGroupLayout::finalize();
}

// ------ VulkanBindGroup ----------------------------------------------------------------------------------------------------------

VulkanBindGroup::VulkanBindGroup() = default;

VoidResult VulkanBindGroup::init(VulkanDevice* device, DescriptorPoolManager& poolManager,
                                  VulkanBindGroupLayout* layout, const BindGroupDesc& desc) {
    m_device = device;

    auto [pool, set] = poolManager.allocate(layout->handle());
    m_pool = pool;
    m_set  = set;

    const auto& dynFlags = layout->dynamicFlags();

    SmallVector<VkWriteDescriptorSet, kMaxBindGroupEntries> writes;
    VkDescriptorBufferInfo bufInfos[kMaxBindGroupEntries] = {};
    VkDescriptorImageInfo imgInfos[kMaxBindGroupEntries] = {};
    assert(desc.entries.size() <= kMaxBindGroupEntries);

    for (size_t i = 0; i < desc.entries.size(); ++i) {
        auto& e = desc.entries[i];
        VkWriteDescriptorSet w{};
        w.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        w.dstSet = m_set;
        w.dstBinding = e.binding;
        w.descriptorCount = 1;

        if (e.buffer) {
            auto* vb = static_cast<VulkanBuffer*>(e.buffer);
            bufInfos[i].buffer = vb->handle();
            bufInfos[i].offset = e.offset;
            bufInfos[i].range = e.size > 0 ? e.size : VK_WHOLE_SIZE;

            // Use dynamic descriptor type if the layout binding has hasDynamicOffset.
            bool isDynamic = (e.binding < dynFlags.size()) && dynFlags[e.binding];
            if (isDynamic) {
                w.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
            } else {
                w.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            }
            w.pBufferInfo = &bufInfos[i];
        } else if (e.textureView) {
            auto* tv = static_cast<VulkanTextureView*>(e.textureView);
            imgInfos[i].imageView = tv->handle();
            imgInfos[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            w.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
            w.pImageInfo = &imgInfos[i];
        } else if (e.sampler) {
            auto* vs = static_cast<VulkanSampler*>(e.sampler);
            imgInfos[i].sampler = vs->handle();
            w.descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
            w.pImageInfo = &imgInfos[i];
        }
        writes.push_back(w);
    }

    if (!writes.empty()) {
        vkUpdateDescriptorSets(m_device->vkDevice(), static_cast<uint32_t>(writes.size()), writes.data(), 0, nullptr);
    }
    return LN_MAKE_SUCCESS();
}

void VulkanBindGroup::finalize() {
    VkDevice dev = m_device->vkDevice();
    VkDescriptorPool pool = m_pool;
    VkDescriptorSet set = m_set;
    m_device->frameResources().queueDelete(m_device->currentFrameIndex(), [dev, pool, set]() {
        if (set != VK_NULL_HANDLE && pool != VK_NULL_HANDLE)
            vkFreeDescriptorSets(dev, pool, 1, &set);
    });
    BindGroup::finalize();
}

// ------ VulkanPipelineLayout ------------------------------------------------------------------------------------------------

VulkanPipelineLayout::VulkanPipelineLayout() = default;

VoidResult VulkanPipelineLayout::init(VulkanDevice* vulkanDevice, const PipelineLayoutDesc& desc) {
    m_device = vulkanDevice;

    // Create internal BindGroupLayouts from the descriptor
    m_bindGroupLayouts.reserve(desc.setLayouts.size());
    std::vector<VkDescriptorSetLayout> vkLayouts;
    vkLayouts.reserve(desc.setLayouts.size());

    for (const auto& setLayout : desc.setLayouts) {
        auto bgl = Ref<VulkanBindGroupLayout>::adopt(new VulkanBindGroupLayout());
        if (!bgl->init(m_device, setLayout)) {
            return LN_MAKE_ERROR("Failed to create internal bind group layout.");
        }
        vkLayouts.push_back(bgl->handle());
        m_bindGroupLayouts.push_back(std::move(bgl));
    }

    VkPipelineLayoutCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    info.setLayoutCount = static_cast<uint32_t>(vkLayouts.size());
    info.pSetLayouts = vkLayouts.data();
    if (vkCreatePipelineLayout(m_device->vkDevice(), &info, nullptr, &m_layout) != VK_SUCCESS) {
        return LN_MAKE_ERROR("vkCreatePipelineLayout failed.");
    }
    return LN_MAKE_SUCCESS();
}

Result<Ref<BindGroup>> VulkanPipelineLayout::createBindGroup(
    uint32_t setIndex, const std::vector<BindGroupEntry>& entries) {
    if (setIndex >= m_bindGroupLayouts.size()) {
        return LN_MAKE_ERROR("setIndex out of range.");
    }

    BindGroupDesc bgDesc;
    bgDesc.layout = m_bindGroupLayouts[setIndex].get();
    bgDesc.entries = entries;

    auto bg = Ref<VulkanBindGroup>::adopt(new VulkanBindGroup());
    if (!bg->init(m_device, m_device->descriptorPoolManager(),
                  m_bindGroupLayouts[setIndex].get(), bgDesc)) {
        return LN_MAKE_ERROR("Failed to create bind group.");
    }
    return Ref<BindGroup>(bg);
}

void VulkanPipelineLayout::finalize() {
    if (m_layout) vkDestroyPipelineLayout(m_device->vkDevice(), m_layout, nullptr);
    PipelineLayout::finalize();
}

// ------ VulkanRenderPipeline ------------------------------------------------------------------------------------------------

// Minimal SPIR-V reflection: returns the set of vertex input locations consumed by the shader.
// Parses OpVariable (storage class Input) and OpDecorate (Location decoration) instructions.
static std::unordered_set<uint32_t> reflectVertexInputLocations(const uint32_t* spirv, size_t sizeBytes) {
    const size_t wordCount = sizeBytes / 4;
    if (wordCount < 5) return {};

    std::unordered_set<uint32_t> inputVarIds;
    std::unordered_map<uint32_t, uint32_t> locationByVarId;

    size_t i = 5; // skip header
    while (i < wordCount) {
        uint32_t word = spirv[i];
        uint32_t opcode = word & 0xFFFF;
        uint32_t wc    = (word >> 16) & 0xFFFF;
        if (wc == 0 || i + wc > wordCount) break;

        if (opcode == 59 && wc >= 4) {          // OpVariable: result_type, result_id, storage_class
            uint32_t resultId     = spirv[i + 2];
            uint32_t storageClass = spirv[i + 3];
            if (storageClass == 1)               // Input
                inputVarIds.insert(resultId);
        } else if (opcode == 71 && wc >= 4) {   // OpDecorate: target_id, decoration, value...
            uint32_t targetId   = spirv[i + 1];
            uint32_t decoration = spirv[i + 2];
            if (decoration == 30)                // Location
                locationByVarId[targetId] = spirv[i + 3];
        }
        i += wc;
    }

    std::unordered_set<uint32_t> usedLocations;
    for (uint32_t id : inputVarIds) {
        auto it = locationByVarId.find(id);
        if (it != locationByVarId.end())
            usedLocations.insert(it->second);
    }
    return usedLocations;
}

VulkanRenderPipeline::VulkanRenderPipeline() = default;

VoidResult VulkanRenderPipeline::init(VulkanDevice* device, VkRenderPass renderPass, const RenderPipelineDesc& desc) {
    m_device = device;
    // Shader stages
    VkPipelineShaderStageCreateInfo stages[2]{};
    stages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
    stages[0].module = static_cast<VulkanShaderModule*>(desc.vertexShader)->handle();
    stages[0].pName = desc.vertexEntry.c_str();
    stages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    stages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    stages[1].module = static_cast<VulkanShaderModule*>(desc.fragmentShader)->handle();
    stages[1].pName = desc.fragmentEntry.c_str();

    // Vertex input
    std::vector<VkVertexInputBindingDescription> bindingDescs;
    std::vector<VkVertexInputAttributeDescription> attrDescs;
    for (uint32_t i = 0; i < desc.vertexBuffers.size(); ++i) {
        auto& vb = desc.vertexBuffers[i];
        VkVertexInputBindingDescription bd{};
        bd.binding = i;
        bd.stride = vb.stride;
        bd.inputRate = vb.perInstance ? VK_VERTEX_INPUT_RATE_INSTANCE : VK_VERTEX_INPUT_RATE_VERTEX;
        bindingDescs.push_back(bd);

        for (auto& a : vb.attributes) {
            VkVertexInputAttributeDescription ad{};
            ad.location = a.location;
            ad.binding = i;
            ad.format = VulkanHelpers::toVkVertexFormat(a.format);
            ad.offset = a.offset;
            attrDescs.push_back(ad);
        }
    }

    // Filter vertex attributes to only those actually consumed by the vertex shader.
    // This avoids Vulkan Validation WARNING-Shader-OutputNotConsumed when the standard
    // layout declares attributes (e.g. normal, tangent) that the shader doesn't use.
    if (desc.vertexShader) {
        auto* vsModule = static_cast<VulkanShaderModule*>(desc.vertexShader);
        auto usedLocations = reflectVertexInputLocations(vsModule->spirvData(), vsModule->spirvSizeBytes());
        if (!usedLocations.empty()) {
            attrDescs.erase(
                std::remove_if(attrDescs.begin(), attrDescs.end(),
                    [&](const VkVertexInputAttributeDescription& a) {
                        return usedLocations.find(a.location) == usedLocations.end();
                    }),
                attrDescs.end());
        }
    }

    VkPipelineVertexInputStateCreateInfo vertexInput{};
    vertexInput.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInput.vertexBindingDescriptionCount = static_cast<uint32_t>(bindingDescs.size());
    vertexInput.pVertexBindingDescriptions = bindingDescs.data();
    vertexInput.vertexAttributeDescriptionCount = static_cast<uint32_t>(attrDescs.size());
    vertexInput.pVertexAttributeDescriptions = attrDescs.data();

    // Input assembly
    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    switch (desc.topology) {
        case PrimitiveTopology::TriangleList:  inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST; break;
        case PrimitiveTopology::TriangleStrip: inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP; break;
        case PrimitiveTopology::LineList:      inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_LINE_LIST; break;
        case PrimitiveTopology::LineStrip:     inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP; break;
        case PrimitiveTopology::PointList:     inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_POINT_LIST; break;
    }

    // Dynamic viewport/scissor
    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    VkDynamicState dynStates[] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR, VK_DYNAMIC_STATE_STENCIL_REFERENCE};
    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = 3;
    dynamicState.pDynamicStates = dynStates;

    // Rasterization
    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    rasterizer.lineWidth = 1.0f;
    switch (desc.cullMode) {
        case CullMode::None:  rasterizer.cullMode = VK_CULL_MODE_NONE; break;
        case CullMode::Front: rasterizer.cullMode = VK_CULL_MODE_FRONT_BIT; break;
        case CullMode::Back:  rasterizer.cullMode = VK_CULL_MODE_BACK_BIT; break;
    }
    rasterizer.frontFace = desc.frontFace == FrontFace::CCW ? VK_FRONT_FACE_COUNTER_CLOCKWISE : VK_FRONT_FACE_CLOCKWISE;

    // Multisampling
    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    // Color blend
    // NOTE: independentBlend is not required, so all attachments must be identical.
    // For attachments beyond what the material specifies, fall back to blendStates[0].
    std::vector<VkPipelineColorBlendAttachmentState> blendAttachments;
    const auto& colorFormats = desc.renderPass->layoutDesc().colorFormats;
    const bool hasBs = !desc.blendStates.empty();
    for (size_t i = 0; i < colorFormats.size(); ++i) {
        // Use per-attachment state if available, otherwise replicate the first entry.
        const size_t bsIdx = (i < desc.blendStates.size()) ? i : 0;
        VkPipelineColorBlendAttachmentState att{};
        bool colorWrite = hasBs ? desc.blendStates[bsIdx].colorWriteEnabled : true;
        att.colorWriteMask = colorWrite
            ? (VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
               VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT)
            : 0;
        if (hasBs && desc.blendStates[bsIdx].enabled) {
            auto& bs = desc.blendStates[bsIdx];
            att.blendEnable = VK_TRUE;
            att.srcColorBlendFactor = VulkanHelpers::toVkBlendFactor(bs.srcColor);
            att.dstColorBlendFactor = VulkanHelpers::toVkBlendFactor(bs.dstColor);
            att.colorBlendOp = VulkanHelpers::toVkBlendOp(bs.colorOp);
            att.srcAlphaBlendFactor = VulkanHelpers::toVkBlendFactor(bs.srcAlpha);
            att.dstAlphaBlendFactor = VulkanHelpers::toVkBlendFactor(bs.dstAlpha);
            att.alphaBlendOp = VulkanHelpers::toVkBlendOp(bs.alphaOp);
        }
        blendAttachments.push_back(att);
    }
    if (blendAttachments.empty()) {
        VkPipelineColorBlendAttachmentState att{};
        att.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                             VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        blendAttachments.push_back(att);
    }

    VkPipelineColorBlendStateCreateInfo colorBlend{};
    colorBlend.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlend.attachmentCount = static_cast<uint32_t>(blendAttachments.size());
    colorBlend.pAttachments = blendAttachments.data();

    // Depth stencil
    VkPipelineDepthStencilStateCreateInfo depthStencil{};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = desc.depthStencil.depthTestEnable ? VK_TRUE : VK_FALSE;
    depthStencil.depthWriteEnable = desc.depthStencil.depthWriteEnable ? VK_TRUE : VK_FALSE;
    depthStencil.depthCompareOp = VulkanHelpers::toVkCompareOp(desc.depthStencil.depthCompare);
    depthStencil.stencilTestEnable = desc.depthStencil.stencilTestEnable ? VK_TRUE : VK_FALSE;
    if (desc.depthStencil.stencilTestEnable) {
        depthStencil.front = VulkanHelpers::toVkStencilOpState(
            desc.depthStencil.stencilFront,
            desc.depthStencil.stencilReadMask,
            desc.depthStencil.stencilWriteMask);
        depthStencil.back = VulkanHelpers::toVkStencilOpState(
            desc.depthStencil.stencilBack,
            desc.depthStencil.stencilReadMask,
            desc.depthStencil.stencilWriteMask);
    }

    // Pipeline layout
    auto* pLayout = static_cast<VulkanPipelineLayout*>(desc.layout);
    VkPipelineLayout vkLayout = pLayout ? pLayout->handle() : VK_NULL_HANDLE;

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = stages;
    pipelineInfo.pVertexInputState = &vertexInput;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pColorBlendState = &colorBlend;
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = vkLayout;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;

    if (vkCreateGraphicsPipelines(m_device->vkDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_pipeline) != VK_SUCCESS) {
        return LN_MAKE_ERROR("vkCreateGraphicsPipelines failed.");
    }
    m_layout = vkLayout;
    return LN_MAKE_SUCCESS();
}

void VulkanRenderPipeline::finalize() {
    VkDevice dev = m_device->vkDevice();
    VkPipeline p = m_pipeline;
    m_device->frameResources().queueDelete(m_device->currentFrameIndex(), [dev, p]() {
        if (p) vkDestroyPipeline(dev, p, nullptr);
    });
    RenderPipeline::finalize();
}

// ------ VulkanRenderPass encoding methods ---------------------------------------------------------------------------------

void VulkanRenderPass::beginEncoding(
    VkCommandBuffer cmd, VkFramebuffer framebuffer,
    VkExtent2D extent, const RenderPassDesc& desc) {
    m_cmd = cmd;
    m_currentPipelineLayout = VK_NULL_HANDLE;

    SmallVector<VkClearValue, kMaxMultiRenderTargets + 1> clearValues;
    for (auto& ca : desc.colorAttachments) {
        VkClearValue cv{};
        cv.color = {{ca.clearColor.r, ca.clearColor.g, ca.clearColor.b, ca.clearColor.a}};
        clearValues.push_back(cv);
    }
    if (desc.depthStencilAttachment) {
        VkClearValue cv{};
        cv.depthStencil = {desc.depthStencilAttachment->clearDepth, desc.depthStencilAttachment->clearStencil};
        clearValues.push_back(cv);
    }

    VkRenderPassBeginInfo rpBegin{};
    rpBegin.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    rpBegin.renderPass = m_vkRenderPass;
    rpBegin.framebuffer = framebuffer;
    rpBegin.renderArea.extent = extent;
    rpBegin.clearValueCount = static_cast<uint32_t>(clearValues.size());
    rpBegin.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(m_cmd, &rpBegin, VK_SUBPASS_CONTENTS_INLINE);

    // Set default viewport and scissor.
    // Use negative height (y = height, h = -height) to flip Vulkan's Y axis so that
    // +Y points upward, matching standard math / OpenGL NDC convention.
    // (VK_KHR_maintenance1)
    VkViewport vp{0, static_cast<float>(extent.height), static_cast<float>(extent.width), -static_cast<float>(extent.height), 0, 1};
    vkCmdSetViewport(m_cmd, 0, 1, &vp);
    VkRect2D scissor{{0, 0}, extent};
    vkCmdSetScissor(m_cmd, 0, 1, &scissor);
}

void VulkanRenderPass::setPipeline(RenderPipeline* pipeline) {
    auto* vp = static_cast<VulkanRenderPipeline*>(pipeline);
    vkCmdBindPipeline(m_cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, vp->handle());
    m_currentPipelineLayout = vp->layoutHandle();
}

void VulkanRenderPass::setVertexBuffer(uint32_t slot, Buffer* buffer, uint64_t offset) {
    auto* vb = static_cast<VulkanBuffer*>(buffer);
    VkBuffer buf = vb->handle();
    VkDeviceSize off = offset;
    vkCmdBindVertexBuffers(m_cmd, slot, 1, &buf, &off);
}

void VulkanRenderPass::setIndexBuffer(Buffer* buffer, IndexFormat format, uint64_t offset) {
    auto* vb = static_cast<VulkanBuffer*>(buffer);
    VkIndexType type = format == IndexFormat::Uint16 ? VK_INDEX_TYPE_UINT16 : VK_INDEX_TYPE_UINT32;
    vkCmdBindIndexBuffer(m_cmd, vb->handle(), offset, type);
}

void VulkanRenderPass::setBindGroup(uint32_t index, BindGroup* group) {
    auto* vg = static_cast<VulkanBindGroup*>(group);
    VkDescriptorSet set = vg->handle();
    vkCmdBindDescriptorSets(m_cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, m_currentPipelineLayout, index, 1, &set, 0, nullptr);
}

void VulkanRenderPass::setBindGroup(uint32_t index, BindGroup* group,
                                     const uint32_t* dynamicOffsets, uint32_t dynamicOffsetCount) {
    auto* vg = static_cast<VulkanBindGroup*>(group);
    VkDescriptorSet set = vg->handle();
    vkCmdBindDescriptorSets(m_cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, m_currentPipelineLayout,
                            index, 1, &set, dynamicOffsetCount, dynamicOffsets);
}

void VulkanRenderPass::setViewport(float x, float y, float w, float h, float minDepth, float maxDepth) {
    VkViewport vp{x, y, w, h, minDepth, maxDepth};
    vkCmdSetViewport(m_cmd, 0, 1, &vp);
}

void VulkanRenderPass::setScissorRect(uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
    VkRect2D sc{{static_cast<int32_t>(x), static_cast<int32_t>(y)}, {w, h}};
    vkCmdSetScissor(m_cmd, 0, 1, &sc);
}

void VulkanRenderPass::setStencilReference(uint32_t reference) {
    vkCmdSetStencilReference(m_cmd, VK_STENCIL_FACE_FRONT_AND_BACK, reference);
}

void VulkanRenderPass::draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) {
    vkCmdDraw(m_cmd, vertexCount, instanceCount, firstVertex, firstInstance);
}

void VulkanRenderPass::drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t baseVertex, uint32_t firstInstance) {
    vkCmdDrawIndexed(m_cmd, indexCount, instanceCount, firstIndex, baseVertex, firstInstance);
}

void VulkanRenderPass::end() {
    vkCmdEndRenderPass(m_cmd);
    m_cmd = VK_NULL_HANDLE;
}

// ------ RenderPass Cache helpers ----------------------------------------------------------------------------------------

bool RenderPassKey::ColorAttachment::operator==(const ColorAttachment& other) const {
    return format == other.format && loadOp == other.loadOp
        && isSwapchainBackbuffer == other.isSwapchainBackbuffer;
}

bool RenderPassKey::operator==(const RenderPassKey& other) const {
    return colorAttachments == other.colorAttachments && depthFormat == other.depthFormat &&
        depthLoadOp == other.depthLoadOp && stencilLoadOp == other.stencilLoadOp;
}

size_t RenderPassKeyHash::operator()(const RenderPassKey& key) const {
    size_t h = 0;
    for (const auto& a : key.colorAttachments) {
        h ^= std::hash<int>()(a.format) + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= std::hash<int>()(a.loadOp) + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= std::hash<bool>()(a.isSwapchainBackbuffer) + 0x9e3779b9 + (h << 6) + (h >> 2);
    }
    h ^= std::hash<int>()(key.depthFormat) + 0x9e3779b9 + (h << 6) + (h >> 2);
    h ^= std::hash<int>()(key.depthLoadOp) + 0x9e3779b9 + (h << 6) + (h >> 2);
    h ^= std::hash<int>()(key.stencilLoadOp) + 0x9e3779b9 + (h << 6) + (h >> 2);
    return h;
}

bool FramebufferKey::operator==(const FramebufferKey& o) const {
    return renderPass == o.renderPass && attachments == o.attachments && width == o.width && height == o.height;
}

size_t FramebufferKeyHash::operator()(const FramebufferKey& key) const {
    size_t h = std::hash<void*>()(key.renderPass);
    for (auto v : key.attachments) h ^= std::hash<void*>()(v) + 0x9e3779b9 + (h << 6) + (h >> 2);
    h ^= std::hash<uint32_t>()(key.width) + 0x9e3779b9 + (h << 6) + (h >> 2);
    h ^= std::hash<uint32_t>()(key.height) + 0x9e3779b9 + (h << 6) + (h >> 2);
    return h;
}

} // namespace ln::rhi::vulkan
