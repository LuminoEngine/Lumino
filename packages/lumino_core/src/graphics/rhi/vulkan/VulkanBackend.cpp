#include "VulkanBackend.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <algorithm>
#include <cassert>
#include <cstring>
#include <numeric>

namespace ln::rhi::vulkan {

// ─── Format conversion ──────────────────────────────────────────────────

VkFormat toVkFormat(TextureFormat fmt) {
    switch (fmt) {
        case TextureFormat::BGRA8Unorm:       return VK_FORMAT_B8G8R8A8_UNORM;
        case TextureFormat::BGRA8UnormSrgb:   return VK_FORMAT_B8G8R8A8_SRGB;
        case TextureFormat::RGBA8Unorm:       return VK_FORMAT_R8G8B8A8_UNORM;
        case TextureFormat::RGBA8UnormSrgb:   return VK_FORMAT_R8G8B8A8_SRGB;
        case TextureFormat::Depth24Stencil8:  return VK_FORMAT_D24_UNORM_S8_UINT;
        case TextureFormat::Depth32Float:     return VK_FORMAT_D32_SFLOAT;
        case TextureFormat::R8Unorm:          return VK_FORMAT_R8_UNORM;
        case TextureFormat::RG8Unorm:         return VK_FORMAT_R8G8_UNORM;
        case TextureFormat::RGBA16Float:      return VK_FORMAT_R16G16B16A16_SFLOAT;
        case TextureFormat::RGBA32Float:      return VK_FORMAT_R32G32B32A32_SFLOAT;
    }
    return VK_FORMAT_R8G8B8A8_UNORM;
}

VkFormat toVkVertexFormat(VertexFormat fmt) {
    switch (fmt) {
        case VertexFormat::Float32x2:  return VK_FORMAT_R32G32_SFLOAT;
        case VertexFormat::Float32x3:  return VK_FORMAT_R32G32B32_SFLOAT;
        case VertexFormat::Float32x4:  return VK_FORMAT_R32G32B32A32_SFLOAT;
        case VertexFormat::Uint8x4Norm:return VK_FORMAT_R8G8B8A8_UNORM;
        case VertexFormat::Sint16x2:   return VK_FORMAT_R16G16_SINT;
        case VertexFormat::Sint16x4:   return VK_FORMAT_R16G16B16A16_SINT;
    }
    return VK_FORMAT_R32G32B32_SFLOAT;
}

static VkBlendFactor toVkBlendFactor(BlendFactor f) {
    switch (f) {
        case BlendFactor::Zero:             return VK_BLEND_FACTOR_ZERO;
        case BlendFactor::One:              return VK_BLEND_FACTOR_ONE;
        case BlendFactor::SrcAlpha:         return VK_BLEND_FACTOR_SRC_ALPHA;
        case BlendFactor::OneMinusSrcAlpha: return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        case BlendFactor::DstAlpha:         return VK_BLEND_FACTOR_DST_ALPHA;
        case BlendFactor::OneMinusDstAlpha: return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
    }
    return VK_BLEND_FACTOR_ONE;
}

static VkBlendOp toVkBlendOp(BlendOp op) {
    switch (op) {
        case BlendOp::Add:             return VK_BLEND_OP_ADD;
        case BlendOp::Subtract:        return VK_BLEND_OP_SUBTRACT;
        case BlendOp::ReverseSubtract: return VK_BLEND_OP_REVERSE_SUBTRACT;
        case BlendOp::Min:             return VK_BLEND_OP_MIN;
        case BlendOp::Max:             return VK_BLEND_OP_MAX;
    }
    return VK_BLEND_OP_ADD;
}

static VkCompareOp toVkCompareOp(CompareFunction fn) {
    switch (fn) {
        case CompareFunction::Never:        return VK_COMPARE_OP_NEVER;
        case CompareFunction::Less:         return VK_COMPARE_OP_LESS;
        case CompareFunction::LessEqual:    return VK_COMPARE_OP_LESS_OR_EQUAL;
        case CompareFunction::Greater:      return VK_COMPARE_OP_GREATER;
        case CompareFunction::GreaterEqual: return VK_COMPARE_OP_GREATER_OR_EQUAL;
        case CompareFunction::Equal:        return VK_COMPARE_OP_EQUAL;
        case CompareFunction::NotEqual:     return VK_COMPARE_OP_NOT_EQUAL;
        case CompareFunction::Always:       return VK_COMPARE_OP_ALWAYS;
    }
    return VK_COMPARE_OP_LESS;
}

static uint32_t findMemoryType(VkPhysicalDevice physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProps;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProps);
    for (uint32_t i = 0; i < memProps.memoryTypeCount; ++i) {
        if ((typeFilter & (1 << i)) && (memProps.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }
    return 0;
}

// ─── VulkanBuffer ────────────────────────────────────────────────────────

VulkanBuffer::VulkanBuffer(VulkanDevice* device, VkPhysicalDevice physicalDevice, const BufferDesc& desc,
                           bool deviceLocal)
    : device_(device), size_(desc.size), deviceLocal_(deviceLocal) {
    VkDevice dev = device_->vkDevice();
    VkBufferCreateInfo bufInfo{};
    bufInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufInfo.size = desc.size;

    if (desc.usage & BufferUsage::Vertex)  bufInfo.usage |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    if (desc.usage & BufferUsage::Index)   bufInfo.usage |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
    if (desc.usage & BufferUsage::Uniform) bufInfo.usage |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    if (desc.usage & BufferUsage::Storage) bufInfo.usage |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
    if (desc.usage & BufferUsage::CopySrc) bufInfo.usage |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    if (desc.usage & BufferUsage::CopyDst) bufInfo.usage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    // Device-local buffers need TRANSFER_DST so staging can copy into them.
    if (deviceLocal) bufInfo.usage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;

    bufInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    vkCreateBuffer(dev, &bufInfo, nullptr, &buffer_);

    VkMemoryRequirements memReqs;
    vkGetBufferMemoryRequirements(dev, buffer_, &memReqs);

    VkMemoryPropertyFlags memFlags = deviceLocal
        ? VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        : (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memReqs.size;
    allocInfo.memoryTypeIndex = findMemoryType(physicalDevice, memReqs.memoryTypeBits, memFlags);
    vkAllocateMemory(dev, &allocInfo, nullptr, &memory_);
    vkBindBufferMemory(dev, buffer_, memory_, 0);

    // Device-local buffers: initialData is uploaded by the caller (via StagingBufferPool).
    // Host-visible buffers: copy directly.
    if (!deviceLocal && desc.initialData) {
        void* mapped = map();
        if (mapped) {
            std::memcpy(mapped, desc.initialData, desc.size);
            unmap();
        }
    }
}

VulkanBuffer::~VulkanBuffer() {
    VkDevice dev = device_->vkDevice();
    VkBuffer buf = buffer_;
    VkDeviceMemory mem = memory_;
    device_->frameResources().queueDelete(device_->currentFrameIndex(), [dev, buf, mem]() {
        if (buf) vkDestroyBuffer(dev, buf, nullptr);
        if (mem) vkFreeMemory(dev, mem, nullptr);
    });
}

void* VulkanBuffer::map() {
    if (deviceLocal_) return nullptr;  // device-local memory cannot be CPU-mapped
    if (!mapped_) vkMapMemory(device_->vkDevice(), memory_, 0, size_, 0, &mapped_);
    return mapped_;
}

void VulkanBuffer::unmap() {
    if (mapped_) {
        vkUnmapMemory(device_->vkDevice(), memory_);
        mapped_ = nullptr;
    }
}

// ─── VulkanTexture ───────────────────────────────────────────────────────

VulkanTexture::VulkanTexture(VkDevice device, VkPhysicalDevice physicalDevice, const TextureDesc& desc)
    : device_(device), format_(desc.format), width_(desc.width), height_(desc.height), ownsImage_(true) {
    VkImageCreateInfo imgInfo{};
    imgInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imgInfo.imageType = VK_IMAGE_TYPE_2D;
    imgInfo.format = toVkFormat(desc.format);
    imgInfo.extent = {desc.width, desc.height, 1};
    imgInfo.mipLevels = desc.mipLevels;
    imgInfo.arrayLayers = desc.depthOrArrayLayers;
    imgInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imgInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imgInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imgInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    auto usage = desc.usage;
    if (static_cast<u32>(usage) & static_cast<u32>(TextureUsage::Sampled))
        imgInfo.usage |= VK_IMAGE_USAGE_SAMPLED_BIT;
    if (static_cast<u32>(usage) & static_cast<u32>(TextureUsage::RenderTarget))
        imgInfo.usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    if (static_cast<u32>(usage) & static_cast<u32>(TextureUsage::DepthStencil))
        imgInfo.usage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    if (static_cast<u32>(usage) & static_cast<u32>(TextureUsage::CopyDst))
        imgInfo.usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    if (static_cast<u32>(usage) & static_cast<u32>(TextureUsage::CopySrc))
        imgInfo.usage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

    vkCreateImage(device_, &imgInfo, nullptr, &image_);

    VkMemoryRequirements memReqs;
    vkGetImageMemoryRequirements(device_, image_, &memReqs);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memReqs.size;
    allocInfo.memoryTypeIndex = findMemoryType(
        physicalDevice, memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    vkAllocateMemory(device_, &allocInfo, nullptr, &memory_);
    vkBindImageMemory(device_, image_, memory_, 0);
}

VulkanTexture::VulkanTexture(VkDevice device, VkImage image, TextureFormat format, u32 width, u32 height)
    : device_(device), image_(image), format_(format), width_(width), height_(height), ownsImage_(false) {}

VulkanTexture::~VulkanTexture() {
    if (ownsImage_) {
        if (image_) vkDestroyImage(device_, image_, nullptr);
        if (memory_) vkFreeMemory(device_, memory_, nullptr);
    }
}

// ─── VulkanTextureView ───────────────────────────────────────────────────

VulkanTextureView::VulkanTextureView(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspect, u32 width, u32 height)
    : device_(device), format_(format), width_(width), height_(height) {
    VkImageViewCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    info.image = image;
    info.viewType = VK_IMAGE_VIEW_TYPE_2D;
    info.format = format;
    info.subresourceRange.aspectMask = aspect;
    info.subresourceRange.baseMipLevel = 0;
    info.subresourceRange.levelCount = 1;
    info.subresourceRange.baseArrayLayer = 0;
    info.subresourceRange.layerCount = 1;
    vkCreateImageView(device_, &info, nullptr, &view_);
}

VulkanTextureView::~VulkanTextureView() {
    if (view_) vkDestroyImageView(device_, view_, nullptr);
}

// ─── VulkanSampler ───────────────────────────────────────────────────────

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

VulkanSampler::VulkanSampler(VulkanDevice* device, const SamplerDesc& desc) : device_(device) {
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
    vkCreateSampler(device_->vkDevice(), &info, nullptr, &sampler_);
}

VulkanSampler::~VulkanSampler() {
    VkDevice dev = device_->vkDevice();
    VkSampler s = sampler_;
    device_->frameResources().queueDelete(device_->currentFrameIndex(), [dev, s]() {
        if (s) vkDestroySampler(dev, s, nullptr);
    });
}

// ─── VulkanShaderModule ──────────────────────────────────────────────────

VulkanShaderModule::VulkanShaderModule(VkDevice device, const ShaderModuleDesc& desc) : device_(device) {
    VkShaderModuleCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    info.codeSize = desc.spirvSizeBytes;
    info.pCode = desc.spirvCode;
    vkCreateShaderModule(device_, &info, nullptr, &module_);
}

VulkanShaderModule::~VulkanShaderModule() {
    if (module_) vkDestroyShaderModule(device_, module_, nullptr);
}

// ─── VulkanBindGroupLayout ───────────────────────────────────────────────

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
    if (static_cast<u32>(s) & static_cast<u32>(ShaderStage::Vertex))   flags |= VK_SHADER_STAGE_VERTEX_BIT;
    if (static_cast<u32>(s) & static_cast<u32>(ShaderStage::Fragment)) flags |= VK_SHADER_STAGE_FRAGMENT_BIT;
    if (static_cast<u32>(s) & static_cast<u32>(ShaderStage::Compute))  flags |= VK_SHADER_STAGE_COMPUTE_BIT;
    return flags;
}

VulkanBindGroupLayout::VulkanBindGroupLayout(VkDevice device, const BindGroupLayoutDesc& desc) : device_(device) {
    std::vector<VkDescriptorSetLayoutBinding> bindings;
    bindings.reserve(desc.entries.size());
    for (auto& e : desc.entries) {
        VkDescriptorSetLayoutBinding b{};
        b.binding = e.binding;
        b.descriptorType = toVkDescriptorType(e.type);
        b.descriptorCount = 1;
        b.stageFlags = toVkShaderStage(e.visibility);
        bindings.push_back(b);
    }

    VkDescriptorSetLayoutCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    info.bindingCount = static_cast<uint32_t>(bindings.size());
    info.pBindings = bindings.data();
    vkCreateDescriptorSetLayout(device_, &info, nullptr, &layout_);
}

VulkanBindGroupLayout::~VulkanBindGroupLayout() {
    if (layout_) vkDestroyDescriptorSetLayout(device_, layout_, nullptr);
}

// ─── VulkanBindGroup ─────────────────────────────────────────────────────

VulkanBindGroup::VulkanBindGroup(
    VulkanDevice* device, DescriptorPoolManager& poolManager, VkDescriptorSetLayout layout,
    const BindGroupDesc& desc)
    : device_(device) {
    auto [pool, set] = poolManager.allocate(layout);
    pool_ = pool;
    set_  = set;

    std::vector<VkWriteDescriptorSet> writes;
    std::vector<VkDescriptorBufferInfo> bufInfos(desc.entries.size());
    std::vector<VkDescriptorImageInfo> imgInfos(desc.entries.size());

    for (size_t i = 0; i < desc.entries.size(); ++i) {
        auto& e = desc.entries[i];
        VkWriteDescriptorSet w{};
        w.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        w.dstSet = set_;
        w.dstBinding = e.binding;
        w.descriptorCount = 1;

        if (e.buffer) {
            auto* vb = static_cast<VulkanBuffer*>(e.buffer);
            bufInfos[i].buffer = vb->handle();
            bufInfos[i].offset = e.offset;
            bufInfos[i].range = e.size > 0 ? e.size : VK_WHOLE_SIZE;
            w.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
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
        vkUpdateDescriptorSets(device_->vkDevice(), static_cast<uint32_t>(writes.size()), writes.data(), 0, nullptr);
    }
}

VulkanBindGroup::~VulkanBindGroup() {
    VkDevice dev = device_->vkDevice();
    VkDescriptorPool pool = pool_;
    VkDescriptorSet set = set_;
    device_->frameResources().queueDelete(device_->currentFrameIndex(), [dev, pool, set]() {
        if (set != VK_NULL_HANDLE && pool != VK_NULL_HANDLE)
            vkFreeDescriptorSets(dev, pool, 1, &set);
    });
}

// ─── VulkanPipelineLayout ────────────────────────────────────────────────

VulkanPipelineLayout::VulkanPipelineLayout(VkDevice device, const PipelineLayoutDesc& desc) : device_(device) {
    std::vector<VkDescriptorSetLayout> layouts;
    layouts.reserve(desc.bindGroupLayouts.size());
    for (auto* bgl : desc.bindGroupLayouts) {
        layouts.push_back(static_cast<VulkanBindGroupLayout*>(bgl)->handle());
    }

    VkPipelineLayoutCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    info.setLayoutCount = static_cast<uint32_t>(layouts.size());
    info.pSetLayouts = layouts.data();
    vkCreatePipelineLayout(device_, &info, nullptr, &layout_);
}

VulkanPipelineLayout::~VulkanPipelineLayout() {
    if (layout_) vkDestroyPipelineLayout(device_, layout_, nullptr);
}

// ─── VulkanRenderPipeline ────────────────────────────────────────────────

VulkanRenderPipeline::VulkanRenderPipeline(VulkanDevice* device, VkRenderPass renderPass, const RenderPipelineDesc& desc)
    : device_(device) {
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
    for (u32 i = 0; i < desc.vertexBuffers.size(); ++i) {
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
            ad.format = toVkVertexFormat(a.format);
            ad.offset = a.offset;
            attrDescs.push_back(ad);
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

    VkDynamicState dynStates[] = {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR};
    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = 2;
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
    std::vector<VkPipelineColorBlendAttachmentState> blendAttachments;
    for (size_t i = 0; i < desc.colorFormats.size(); ++i) {
        VkPipelineColorBlendAttachmentState att{};
        att.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                             VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        if (i < desc.blendStates.size() && desc.blendStates[i].enabled) {
            auto& bs = desc.blendStates[i];
            att.blendEnable = VK_TRUE;
            att.srcColorBlendFactor = toVkBlendFactor(bs.srcColor);
            att.dstColorBlendFactor = toVkBlendFactor(bs.dstColor);
            att.colorBlendOp = toVkBlendOp(bs.colorOp);
            att.srcAlphaBlendFactor = toVkBlendFactor(bs.srcAlpha);
            att.dstAlphaBlendFactor = toVkBlendFactor(bs.dstAlpha);
            att.alphaBlendOp = toVkBlendOp(bs.alphaOp);
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
    depthStencil.depthCompareOp = toVkCompareOp(desc.depthStencil.depthCompare);

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

    vkCreateGraphicsPipelines(device_->vkDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline_);
    layout_ = vkLayout;
}

VulkanRenderPipeline::~VulkanRenderPipeline() {
    VkDevice dev = device_->vkDevice();
    VkPipeline p = pipeline_;
    device_->frameResources().queueDelete(device_->currentFrameIndex(), [dev, p]() {
        if (p) vkDestroyPipeline(dev, p, nullptr);
    });
}

// ─── VulkanRenderPassEncoder ─────────────────────────────────────────────

VulkanRenderPassEncoder::VulkanRenderPassEncoder(
    VkCommandBuffer cmd, VkRenderPass renderPass,
    VkFramebuffer framebuffer, VkExtent2D extent,
    const RenderPassDesc& desc)
    : cmd_(cmd) {
    std::vector<VkClearValue> clearValues;
    for (auto& ca : desc.colorAttachments) {
        VkClearValue cv{};
        cv.color = {{ca.clearColor.r, ca.clearColor.g, ca.clearColor.b, ca.clearColor.a}};
        clearValues.push_back(cv);
    }
    if (desc.depthStencilAttachment) {
        VkClearValue cv{};
        cv.depthStencil = {desc.depthStencilAttachment->clearDepth, 0};
        clearValues.push_back(cv);
    }

    VkRenderPassBeginInfo rpBegin{};
    rpBegin.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    rpBegin.renderPass = renderPass;
    rpBegin.framebuffer = framebuffer;
    rpBegin.renderArea.extent = extent;
    rpBegin.clearValueCount = static_cast<uint32_t>(clearValues.size());
    rpBegin.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(cmd_, &rpBegin, VK_SUBPASS_CONTENTS_INLINE);

    // Set default viewport and scissor.
    // Use negative height (y = height, h = -height) to flip Vulkan's Y axis so that
    // +Y points upward, matching standard math / OpenGL NDC convention.
    VkViewport vp{0, static_cast<float>(extent.height), static_cast<float>(extent.width), -static_cast<float>(extent.height), 0, 1};
    vkCmdSetViewport(cmd_, 0, 1, &vp);
    VkRect2D scissor{{0, 0}, extent};
    vkCmdSetScissor(cmd_, 0, 1, &scissor);
}

void VulkanRenderPassEncoder::setPipeline(RenderPipeline* pipeline) {
    auto* vp = static_cast<VulkanRenderPipeline*>(pipeline);
    vkCmdBindPipeline(cmd_, VK_PIPELINE_BIND_POINT_GRAPHICS, vp->handle());
    currentPipelineLayout_ = vp->layoutHandle();
}

void VulkanRenderPassEncoder::setVertexBuffer(u32 slot, Buffer* buffer, u64 offset) {
    auto* vb = static_cast<VulkanBuffer*>(buffer);
    VkBuffer buf = vb->handle();
    VkDeviceSize off = offset;
    vkCmdBindVertexBuffers(cmd_, slot, 1, &buf, &off);
}

void VulkanRenderPassEncoder::setIndexBuffer(Buffer* buffer, IndexFormat format, u64 offset) {
    auto* vb = static_cast<VulkanBuffer*>(buffer);
    VkIndexType type = format == IndexFormat::Uint16 ? VK_INDEX_TYPE_UINT16 : VK_INDEX_TYPE_UINT32;
    vkCmdBindIndexBuffer(cmd_, vb->handle(), offset, type);
}

void VulkanRenderPassEncoder::setBindGroup(u32 index, BindGroup* group) {
    auto* vg = static_cast<VulkanBindGroup*>(group);
    VkDescriptorSet set = vg->handle();
    vkCmdBindDescriptorSets(cmd_, VK_PIPELINE_BIND_POINT_GRAPHICS, currentPipelineLayout_, index, 1, &set, 0, nullptr);
}

void VulkanRenderPassEncoder::setViewport(f32 x, f32 y, f32 w, f32 h, f32 minDepth, f32 maxDepth) {
    VkViewport vp{x, y, w, h, minDepth, maxDepth};
    vkCmdSetViewport(cmd_, 0, 1, &vp);
}

void VulkanRenderPassEncoder::setScissorRect(u32 x, u32 y, u32 w, u32 h) {
    VkRect2D sc{{static_cast<int32_t>(x), static_cast<int32_t>(y)}, {w, h}};
    vkCmdSetScissor(cmd_, 0, 1, &sc);
}

void VulkanRenderPassEncoder::draw(u32 vertexCount, u32 instanceCount, u32 firstVertex, u32 firstInstance) {
    vkCmdDraw(cmd_, vertexCount, instanceCount, firstVertex, firstInstance);
}

void VulkanRenderPassEncoder::drawIndexed(u32 indexCount, u32 instanceCount, u32 firstIndex, i32 baseVertex, u32 firstInstance) {
    vkCmdDrawIndexed(cmd_, indexCount, instanceCount, firstIndex, baseVertex, firstInstance);
}

void VulkanRenderPassEncoder::end() {
    vkCmdEndRenderPass(cmd_);
}

// ─── RenderPass Cache helpers ────────────────────────────────────────────

bool RenderPassKey::operator==(const RenderPassKey& o) const {
    return colorFormats == o.colorFormats && depthFormat == o.depthFormat && loadOps == o.loadOps;
}

size_t RenderPassKeyHash::operator()(const RenderPassKey& key) const {
    size_t h = 0;
    for (auto f : key.colorFormats) h ^= std::hash<int>()(f) + 0x9e3779b9 + (h << 6) + (h >> 2);
    h ^= std::hash<int>()(key.depthFormat) + 0x9e3779b9 + (h << 6) + (h >> 2);
    for (auto op : key.loadOps) h ^= std::hash<int>()(op) + 0x9e3779b9 + (h << 6) + (h >> 2);
    return h;
}

bool FramebufferKey::operator==(const FramebufferKey& o) const {
    return renderPass == o.renderPass && attachments == o.attachments && width == o.width && height == o.height;
}

size_t FramebufferKeyHash::operator()(const FramebufferKey& key) const {
    size_t h = std::hash<void*>()(key.renderPass);
    for (auto v : key.attachments) h ^= std::hash<void*>()(v) + 0x9e3779b9 + (h << 6) + (h >> 2);
    h ^= std::hash<u32>()(key.width) + 0x9e3779b9 + (h << 6) + (h >> 2);
    h ^= std::hash<u32>()(key.height) + 0x9e3779b9 + (h << 6) + (h >> 2);
    return h;
}

// ─── VulkanSwapChain ─────────────────────────────────────────────────────

VulkanSwapChain::VulkanSwapChain(VulkanDevice* device, const SwapChainDesc& desc)
    : device_(device), format_(desc.format) {
    // Create surface
    glfwCreateWindowSurface(
        device_->instance(),
        static_cast<GLFWwindow*>(desc.nativeWindowHandle),
        nullptr, &surface_);

    // Query surface capabilities
    VkSurfaceCapabilitiesKHR caps{};
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device_->physicalDevice(), surface_, &caps);

    extent_ = caps.currentExtent;
    if (extent_.width == UINT32_MAX) {
        extent_.width = desc.width;
        extent_.height = desc.height;
    }

    uint32_t imageCount = caps.minImageCount + 1;
    if (caps.maxImageCount > 0 && imageCount > caps.maxImageCount)
        imageCount = caps.maxImageCount;

    VkSwapchainCreateInfoKHR swapInfo{};
    swapInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapInfo.surface = surface_;
    swapInfo.minImageCount = imageCount;
    swapInfo.imageFormat = toVkFormat(desc.format);
    swapInfo.imageColorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;
    swapInfo.imageExtent = extent_;
    swapInfo.imageArrayLayers = 1;
    swapInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapInfo.preTransform = caps.currentTransform;
    swapInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapInfo.presentMode = desc.vsync ? VK_PRESENT_MODE_FIFO_KHR : VK_PRESENT_MODE_MAILBOX_KHR;
    swapInfo.clipped = VK_TRUE;
    vkCreateSwapchainKHR(device_->vkDevice(), &swapInfo, nullptr, &swapchain_);

    // Get swap chain images
    vkGetSwapchainImagesKHR(device_->vkDevice(), swapchain_, &imageCount, nullptr);
    images_.resize(imageCount);
    vkGetSwapchainImagesKHR(device_->vkDevice(), swapchain_, &imageCount, images_.data());

    // Create image views
    VkFormat vkFmt = toVkFormat(desc.format);
    views_.resize(imageCount);
    for (u32 i = 0; i < imageCount; ++i) {
        views_[i] = Ref<VulkanTextureView>::adopt(
            new VulkanTextureView(device_->vkDevice(), images_[i], vkFmt, VK_IMAGE_ASPECT_COLOR_BIT,
                                  extent_.width, extent_.height));
    }

    // Create sync objects
    VkSemaphoreCreateInfo semInfo{VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};
    VkFenceCreateInfo fenceInfo{VK_STRUCTURE_TYPE_FENCE_CREATE_INFO};
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (u32 i = 0; i < MAX_FRAMES; ++i) {
        vkCreateSemaphore(device_->vkDevice(), &semInfo, nullptr, &imageAvailable_[i]);
        vkCreateSemaphore(device_->vkDevice(), &semInfo, nullptr, &renderFinished_[i]);
        vkCreateFence(device_->vkDevice(), &fenceInfo, nullptr, &inFlightFences_[i]);
    }

}

VulkanSwapChain::~VulkanSwapChain() {
    cleanup();
}

void VulkanSwapChain::cleanup() {
    auto dev = device_->vkDevice();
    vkDeviceWaitIdle(dev);

    for (u32 i = 0; i < MAX_FRAMES; ++i) {
        if (imageAvailable_[i]) vkDestroySemaphore(dev, imageAvailable_[i], nullptr);
        if (renderFinished_[i]) vkDestroySemaphore(dev, renderFinished_[i], nullptr);
        if (inFlightFences_[i]) vkDestroyFence(dev, inFlightFences_[i], nullptr);
    }
    views_.clear();
    if (swapchain_) vkDestroySwapchainKHR(dev, swapchain_, nullptr);
    if (surface_) vkDestroySurfaceKHR(device_->instance(), surface_, nullptr);
}

TextureView* VulkanSwapChain::acquireNextTexture() {
    auto dev = device_->vkDevice();
    vkWaitForFences(dev, 1, &inFlightFences_[currentFrame_], VK_TRUE, UINT64_MAX);
    vkResetFences(dev, 1, &inFlightFences_[currentFrame_]);

    // Run deferred cleanups for this frame index now that the GPU is done with it.
    device_->beginFrame(currentFrame_);

    vkAcquireNextImageKHR(dev, swapchain_, UINT64_MAX, imageAvailable_[currentFrame_], VK_NULL_HANDLE, &imageIndex_);
    device_->setActiveSwapChain(this);
    return views_[imageIndex_].get();
}

void VulkanSwapChain::present() {
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &renderFinished_[currentFrame_];
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &swapchain_;
    presentInfo.pImageIndices = &imageIndex_;
    vkQueuePresentKHR(device_->graphicsQueue(), &presentInfo);

    currentFrame_ = (currentFrame_ + 1) % MAX_FRAMES;
}

VkSemaphore VulkanSwapChain::imageAvailableSemaphore() const { return imageAvailable_[currentFrame_]; }
VkSemaphore VulkanSwapChain::renderFinishedSemaphore() const { return renderFinished_[currentFrame_]; }
VkFence VulkanSwapChain::inFlightFence() const { return inFlightFences_[currentFrame_]; }

// ─── VulkanCommandBuffer ─────────────────────────────────────────────────

VulkanCommandBuffer::VulkanCommandBuffer(VulkanDevice* device, VkCommandBuffer cmd)
    : device_(device), cmd_(cmd) {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(cmd_, &beginInfo);
}

RenderPassEncoder* VulkanCommandBuffer::beginRenderPass(const RenderPassDesc& desc) {
    // Build RenderPassKey
    RenderPassKey rpKey;
    u32 fbWidth = 0, fbHeight = 0;
    for (auto& ca : desc.colorAttachments) {
        auto* view = static_cast<VulkanTextureView*>(ca.view);
        rpKey.colorFormats.push_back(view->vkFormat());
        rpKey.loadOps.push_back(
            ca.loadOp == LoadOp::Clear ? VK_ATTACHMENT_LOAD_OP_CLEAR :
            ca.loadOp == LoadOp::Load  ? VK_ATTACHMENT_LOAD_OP_LOAD :
                                         VK_ATTACHMENT_LOAD_OP_DONT_CARE);
        if (fbWidth == 0) {
            fbWidth = view->width();
            fbHeight = view->height();
        }
    }
    if (desc.depthStencilAttachment) {
        rpKey.depthFormat = static_cast<VulkanTextureView*>(desc.depthStencilAttachment->view)->vkFormat();
    }

    VkRenderPass renderPass = device_->getOrCreateRenderPass(rpKey);

    // Build FramebufferKey
    FramebufferKey fbKey;
    fbKey.renderPass = renderPass;
    fbKey.width = fbWidth;
    fbKey.height = fbHeight;
    for (auto& ca : desc.colorAttachments) {
        fbKey.attachments.push_back(static_cast<VulkanTextureView*>(ca.view)->handle());
    }
    if (desc.depthStencilAttachment) {
        fbKey.attachments.push_back(static_cast<VulkanTextureView*>(desc.depthStencilAttachment->view)->handle());
    }

    VkFramebuffer framebuffer = device_->getOrCreateFramebuffer(fbKey);
    VkExtent2D extent{fbKey.width, fbKey.height};

    encoder_ = new VulkanRenderPassEncoder(cmd_, renderPass, framebuffer, extent, desc);
    return encoder_;
}

void VulkanCommandBuffer::submit() {
    vkEndCommandBuffer(cmd_);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &cmd_;

    VkSemaphore waitSemaphore = VK_NULL_HANDLE;
    VkSemaphore signalSemaphore = VK_NULL_HANDLE;
    VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    VkFence fence = VK_NULL_HANDLE;

    auto* sc = device_->activeSwapChain();
    if (sc) {
        submittedFrame_ = sc->currentFrame();
        submitted_ = true;
        waitSemaphore = sc->imageAvailableSemaphore();
        signalSemaphore = sc->renderFinishedSemaphore();
        fence = sc->inFlightFence();
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &waitSemaphore;
        submitInfo.pWaitDstStageMask = &waitStage;
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &signalSemaphore;
    }

    vkQueueSubmit(device_->graphicsQueue(), 1, &submitInfo, fence);

    delete encoder_;
    encoder_ = nullptr;
}

VulkanCommandBuffer::~VulkanCommandBuffer() {
    // Queue vkFreeCommandBuffers to run once the GPU finishes this frame.
    // If the command buffer was never submitted, free immediately.
    VkDevice dev = device_->vkDevice();
    VkCommandPool pool = device_->commandPool();
    VkCommandBuffer cmd = cmd_;

    if (submitted_) {
        device_->frameResources().queueDelete(submittedFrame_, [dev, pool, cmd]() {
            vkFreeCommandBuffers(dev, pool, 1, &cmd);
        });
    } else {
        vkFreeCommandBuffers(dev, pool, 1, &cmd);
    }
}

// ─── VulkanDevice ────────────────────────────────────────────────────────

VulkanDevice::VulkanDevice(const DeviceDesc& desc) {
    // Create instance
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Lumino";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Lumino";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_1;

    std::vector<const char*> instanceExtensions = {
        VK_KHR_SURFACE_EXTENSION_NAME,
    };

    uint32_t glfwExtCount = 0;
    const char** glfwExts = glfwGetRequiredInstanceExtensions(&glfwExtCount);
    if (glfwExts) {
        instanceExtensions.assign(glfwExts, glfwExts + glfwExtCount);
    }

    std::vector<const char*> layers;
    if (desc.enableValidation) {
        layers.push_back("VK_LAYER_KHRONOS_validation");
    }

    VkInstanceCreateInfo instInfo{};
    instInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instInfo.pApplicationInfo = &appInfo;
    instInfo.enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size());
    instInfo.ppEnabledExtensionNames = instanceExtensions.data();
    instInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
    instInfo.ppEnabledLayerNames = layers.data();

    if (vkCreateInstance(&instInfo, nullptr, &instance_) != VK_SUCCESS) {
        return;
    }

    // Select physical device
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance_, &deviceCount, nullptr);
    if (deviceCount == 0) return;
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance_, &deviceCount, devices.data());
    physicalDevice_ = devices[0];

    // Find graphics queue family
    uint32_t qfCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice_, &qfCount, nullptr);
    std::vector<VkQueueFamilyProperties> qfProps(qfCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice_, &qfCount, qfProps.data());

    for (uint32_t i = 0; i < qfCount; ++i) {
        if (qfProps[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            graphicsFamily_ = i;
            break;
        }
    }

    // Create logical device
    float priority = 1.0f;
    VkDeviceQueueCreateInfo queueInfo{};
    queueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueInfo.queueFamilyIndex = graphicsFamily_;
    queueInfo.queueCount = 1;
    queueInfo.pQueuePriorities = &priority;

    const char* devExtensions[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    VkDeviceCreateInfo devInfo{};
    devInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    devInfo.queueCreateInfoCount = 1;
    devInfo.pQueueCreateInfos = &queueInfo;
    devInfo.enabledExtensionCount = 1;
    devInfo.ppEnabledExtensionNames = devExtensions;

    if (vkCreateDevice(physicalDevice_, &devInfo, nullptr, &device_) != VK_SUCCESS) {
        return;
    }

    vkGetDeviceQueue(device_, graphicsFamily_, 0, &graphicsQueue_);

    // Command pool
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = graphicsFamily_;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    vkCreateCommandPool(device_, &poolInfo, nullptr, &commandPool_);

    // Resource management subsystems
    descriptorPoolManager_.init(device_);
    stagingPool_.init(device_, physicalDevice_);
}

VulkanDevice::~VulkanDevice() {
    if (device_) vkDeviceWaitIdle(device_);

    // Flush any deferred cleanups (e.g., command buffers) before destroying the pool.
    frameResources_.flushAll();

    // Destroy caches
    for (auto& [key, fb] : framebufferCache_) vkDestroyFramebuffer(device_, fb, nullptr);
    for (auto& [key, rp] : renderPassCache_) vkDestroyRenderPass(device_, rp, nullptr);

    stagingPool_.destroy();
    descriptorPoolManager_.destroy();
    if (commandPool_) vkDestroyCommandPool(device_, commandPool_, nullptr);
    if (device_) vkDestroyDevice(device_, nullptr);
    if (instance_) vkDestroyInstance(instance_, nullptr);
}

VkRenderPass VulkanDevice::getOrCreateRenderPass(const RenderPassKey& key) {
    std::lock_guard lock(cacheMutex_);
    auto it = renderPassCache_.find(key);
    if (it != renderPassCache_.end()) return it->second;

    std::vector<VkAttachmentDescription> attachments;
    std::vector<VkAttachmentReference> colorRefs;

    for (size_t i = 0; i < key.colorFormats.size(); ++i) {
        VkAttachmentDescription att{};
        att.format = key.colorFormats[i];
        att.samples = VK_SAMPLE_COUNT_1_BIT;
        att.loadOp = (i < key.loadOps.size()) ? key.loadOps[i] : VK_ATTACHMENT_LOAD_OP_CLEAR;
        att.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        att.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        att.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        att.initialLayout = att.loadOp == VK_ATTACHMENT_LOAD_OP_LOAD
                                ? VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
                                : VK_IMAGE_LAYOUT_UNDEFINED;
        att.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
        attachments.push_back(att);

        VkAttachmentReference ref{};
        ref.attachment = static_cast<uint32_t>(i);
        ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
        colorRefs.push_back(ref);
    }

    VkAttachmentReference depthRef{};
    if (key.depthFormat != VK_FORMAT_UNDEFINED) {
        VkAttachmentDescription att{};
        att.format = key.depthFormat;
        att.samples = VK_SAMPLE_COUNT_1_BIT;
        att.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        att.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        att.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        att.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        att.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        att.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
        attachments.push_back(att);

        depthRef.attachment = static_cast<uint32_t>(attachments.size() - 1);
        depthRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    }

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = static_cast<uint32_t>(colorRefs.size());
    subpass.pColorAttachments = colorRefs.data();
    if (key.depthFormat != VK_FORMAT_UNDEFINED) {
        subpass.pDepthStencilAttachment = &depthRef;
    }

    VkSubpassDependency dep{};
    dep.srcSubpass = VK_SUBPASS_EXTERNAL;
    dep.dstSubpass = 0;
    dep.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dep.srcAccessMask = 0;
    dep.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dep.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    VkRenderPassCreateInfo rpInfo{};
    rpInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    rpInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    rpInfo.pAttachments = attachments.data();
    rpInfo.subpassCount = 1;
    rpInfo.pSubpasses = &subpass;
    rpInfo.dependencyCount = 1;
    rpInfo.pDependencies = &dep;

    VkRenderPass rp = VK_NULL_HANDLE;
    vkCreateRenderPass(device_, &rpInfo, nullptr, &rp);
    renderPassCache_[key] = rp;
    return rp;
}

VkFramebuffer VulkanDevice::getOrCreateFramebuffer(const FramebufferKey& key) {
    std::lock_guard lock(cacheMutex_);
    auto it = framebufferCache_.find(key);
    if (it != framebufferCache_.end()) return it->second;

    VkFramebufferCreateInfo fbInfo{};
    fbInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    fbInfo.renderPass = key.renderPass;
    fbInfo.attachmentCount = static_cast<uint32_t>(key.attachments.size());
    fbInfo.pAttachments = key.attachments.data();
    fbInfo.width = key.width;
    fbInfo.height = key.height;
    fbInfo.layers = 1;

    VkFramebuffer fb = VK_NULL_HANDLE;
    vkCreateFramebuffer(device_, &fbInfo, nullptr, &fb);
    framebufferCache_[key] = fb;
    return fb;
}

Result<Ref<SwapChain>> VulkanDevice::createSwapChain(const SwapChainDesc& desc) {
    auto sc = Ref<VulkanSwapChain>::adopt(new VulkanSwapChain(this, desc));
    return Ref<SwapChain>(sc);
}

Result<Ref<Buffer>> VulkanDevice::createBuffer(const BufferDesc& desc) {
    // Vertex and Index buffers benefit from device-local (GPU-optimal) memory.
    // Other buffer types (Uniform, Storage) remain host-visible for easy CPU updates.
    bool useDeviceLocal =
        (desc.usage & BufferUsage::Vertex) || (desc.usage & BufferUsage::Index);

    auto buf = Ref<VulkanBuffer>::adopt(new VulkanBuffer(this, physicalDevice_, desc, useDeviceLocal));

    if (useDeviceLocal && desc.initialData && desc.size > 0) {
        stagingPool_.uploadImmediate(
            graphicsQueue_, commandPool_, buf->handle(), desc.initialData, desc.size);
    }

    return Ref<Buffer>(buf);
}

Result<Ref<Texture>> VulkanDevice::createTexture(const TextureDesc& desc) {
    auto tex = Ref<VulkanTexture>::adopt(new VulkanTexture(device_, physicalDevice_, desc));

    // Upload initial data via staging buffer if provided.
    if (desc.initialData) {
        u32 bpp = 4; // Assume 4 bytes per pixel for common formats.
        if (desc.format == TextureFormat::R8Unorm) bpp = 1;
        else if (desc.format == TextureFormat::RG8Unorm) bpp = 2;
        else if (desc.format == TextureFormat::RGBA16Float) bpp = 8;
        else if (desc.format == TextureFormat::RGBA32Float) bpp = 16;
        VkDeviceSize imageSize = static_cast<VkDeviceSize>(desc.width) * desc.height * bpp;

        stagingPool_.uploadTextureImmediate(
            graphicsQueue_, commandPool_,
            tex->handle(), desc.initialData, imageSize,
            desc.width, desc.height);
    }

    return Ref<Texture>(tex);
}

Result<Ref<TextureView>> VulkanDevice::createTextureView(Texture* texture) {
    auto* vtex = static_cast<VulkanTexture*>(texture);
    VkFormat fmt = toVkFormat(vtex->format());
    VkImageAspectFlags aspect = VK_IMAGE_ASPECT_COLOR_BIT;
    if (vtex->format() == TextureFormat::Depth24Stencil8 || vtex->format() == TextureFormat::Depth32Float) {
        aspect = VK_IMAGE_ASPECT_DEPTH_BIT;
    }
    auto view = Ref<VulkanTextureView>::adopt(
        new VulkanTextureView(device_, vtex->handle(), fmt, aspect, vtex->width(), vtex->height()));
    return Ref<TextureView>(view);
}

Result<Ref<Sampler>> VulkanDevice::createSampler(const SamplerDesc& desc) {
    auto s = Ref<VulkanSampler>::adopt(new VulkanSampler(this, desc));
    return Ref<Sampler>(s);
}

Result<Ref<ShaderModule>> VulkanDevice::createShaderModule(const ShaderModuleDesc& desc) {
    auto sm = Ref<VulkanShaderModule>::adopt(new VulkanShaderModule(device_, desc));
    return Ref<ShaderModule>(sm);
}

Result<Ref<BindGroupLayout>> VulkanDevice::createBindGroupLayout(const BindGroupLayoutDesc& desc) {
    auto bgl = Ref<VulkanBindGroupLayout>::adopt(new VulkanBindGroupLayout(device_, desc));
    return Ref<BindGroupLayout>(bgl);
}

Result<Ref<BindGroup>> VulkanDevice::createBindGroup(const BindGroupDesc& desc) {
    auto* layout = static_cast<VulkanBindGroupLayout*>(desc.layout);
    auto bg = Ref<VulkanBindGroup>::adopt(
        new VulkanBindGroup(this, descriptorPoolManager_, layout->handle(), desc));
    return Ref<BindGroup>(bg);
}

Result<Ref<PipelineLayout>> VulkanDevice::createPipelineLayout(const PipelineLayoutDesc& desc) {
    auto pl = Ref<VulkanPipelineLayout>::adopt(new VulkanPipelineLayout(device_, desc));
    return Ref<PipelineLayout>(pl);
}

Result<Ref<RenderPipeline>> VulkanDevice::createRenderPipeline(const RenderPipelineDesc& desc) {
    // Need a compatible render pass for pipeline creation
    RenderPassKey rpKey;
    for (auto& fmt : desc.colorFormats) {
        rpKey.colorFormats.push_back(toVkFormat(fmt));
        rpKey.loadOps.push_back(VK_ATTACHMENT_LOAD_OP_CLEAR);
    }
    if (desc.depthStencil.depthTestEnable) {
        rpKey.depthFormat = toVkFormat(desc.depthStencilFormat);
    }
    VkRenderPass renderPass = getOrCreateRenderPass(rpKey);

    auto rp = Ref<VulkanRenderPipeline>::adopt(new VulkanRenderPipeline(this, renderPass, desc));
    return Ref<RenderPipeline>(rp);
}

CommandBuffer* VulkanDevice::createCommandBuffer() {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool_;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer cmd;
    vkAllocateCommandBuffers(device_, &allocInfo, &cmd);
    return new VulkanCommandBuffer(this, cmd);
}

void VulkanDevice::waitIdle() {
    if (device_) vkDeviceWaitIdle(device_);
}

} // namespace ln::rhi::vulkan

// ─── Device::create factory ──────────────────────────────────────────────

namespace ln::rhi {

Result<Ref<Device>> Device::create(const DeviceDesc& desc) {
    if (desc.backend == Backend::Vulkan) {
        auto dev = Ref<vulkan::VulkanDevice>::adopt(new vulkan::VulkanDevice(desc));
        if (!dev->isValid()) {
            return tl::unexpected(Error{ErrorCode::NotInitialized, "Failed to initialize Vulkan device"});
        }
        return Ref<Device>(dev);
    }
    return tl::unexpected(Error{ErrorCode::NotSupported, "Unsupported backend"});
}

} // namespace ln::rhi
