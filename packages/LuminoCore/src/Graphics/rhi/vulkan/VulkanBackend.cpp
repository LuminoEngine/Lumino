

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <algorithm>
#include <cassert>
#include <cstring>
#include <iostream>
#include <numeric>
#include <unordered_set>

#include "VulkanBackend.hpp"
#include "VulkanHelpers.hpp"
#include "VulkanDevice.hpp"

namespace ln::rhi::vulkan {

// ------ Format conversion ----------------------------------------------------------------------------------------------------

VkFormat toVkFormat(TextureFormat fmt) {
    switch (fmt) {
        case TextureFormat::Undefined:        return VK_FORMAT_UNDEFINED;
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

static VkStencilOp toVkStencilOp(StencilOp op) {
    switch (op) {
        case StencilOp::Keep:           return VK_STENCIL_OP_KEEP;
        case StencilOp::Zero:           return VK_STENCIL_OP_ZERO;
        case StencilOp::Replace:        return VK_STENCIL_OP_REPLACE;
        case StencilOp::IncrementClamp: return VK_STENCIL_OP_INCREMENT_AND_CLAMP;
        case StencilOp::DecrementClamp: return VK_STENCIL_OP_DECREMENT_AND_CLAMP;
        case StencilOp::Invert:         return VK_STENCIL_OP_INVERT;
    }
    return VK_STENCIL_OP_KEEP;
}

static VkStencilOpState toVkStencilOpState(const StencilFaceState& face, u32 readMask, u32 writeMask) {
    VkStencilOpState s{};
    s.failOp      = toVkStencilOp(face.failOp);
    s.passOp      = toVkStencilOp(face.passOp);
    s.depthFailOp = toVkStencilOp(face.depthFailOp);
    s.compareOp   = toVkCompareOp(face.compare);
    s.compareMask = readMask;
    s.writeMask   = writeMask;
    s.reference   = 0; // set dynamically via vkCmdSetStencilReference
    return s;
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

// ------ VulkanBuffer ----------------------------------------------------------------------------------------------------------------

VulkanBuffer::VulkanBuffer() = default;

VoidResult VulkanBuffer::init(VulkanDevice* device, VkPhysicalDevice physicalDevice, const BufferDesc& desc,
                               bool deviceLocal) {
    m_device = device;
    m_size = desc.size;
    m_deviceLocal = deviceLocal;

    VkDevice dev = m_device->vkDevice();
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
    if (vkCreateBuffer(dev, &bufInfo, nullptr, &m_buffer) != VK_SUCCESS) {
        return LN_MAKE_ERROR("vkCreateBuffer failed.");
    }

    VkMemoryRequirements memReqs;
    vkGetBufferMemoryRequirements(dev, m_buffer, &memReqs);

    VkMemoryPropertyFlags memFlags = deviceLocal
        ? VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        : (VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memReqs.size;
    allocInfo.memoryTypeIndex = findMemoryType(physicalDevice, memReqs.memoryTypeBits, memFlags);
    if (vkAllocateMemory(dev, &allocInfo, nullptr, &m_memory) != VK_SUCCESS) {
        return LN_MAKE_ERROR("vkAllocateMemory failed.");
    }
    vkBindBufferMemory(dev, m_buffer, m_memory, 0);

    // Device-local buffers: initialData is uploaded by the caller (via StagingBufferPool).
    // Host-visible buffers: copy directly.
    if (!deviceLocal && desc.initialData) {
        void* mapped = map();
        if (mapped) {
            std::memcpy(mapped, desc.initialData, desc.size);
            unmap();
        }
    }
    return LN_MAKE_SUCCESS();
}

void VulkanBuffer::finalize() {
    VkDevice dev = m_device->vkDevice();
    VkBuffer buf = m_buffer;
    VkDeviceMemory mem = m_memory;
    m_device->frameResources().queueDelete(m_device->currentFrameIndex(), [dev, buf, mem]() {
        if (buf) vkDestroyBuffer(dev, buf, nullptr);
        if (mem) vkFreeMemory(dev, mem, nullptr);
    });
    Buffer::finalize();
}

void* VulkanBuffer::map() {
    if (m_deviceLocal) return nullptr;  // device-local memory cannot be CPU-mapped
    if (!m_mapped) vkMapMemory(m_device->vkDevice(), m_memory, 0, m_size, 0, &m_mapped);
    return m_mapped;
}

void VulkanBuffer::unmap() {
    if (m_mapped) {
        vkUnmapMemory(m_device->vkDevice(), m_memory);
        m_mapped = nullptr;
    }
}

// ------ VulkanTexture --------------------------------------------------------------------------------------------------------------

VulkanTexture::VulkanTexture() = default;

static bool hasStencilComponent(VkFormat format) {
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}

VoidResult VulkanTexture::init(VulkanDevice* device, VkPhysicalDevice physicalDevice, const TextureDesc& desc) {
    m_device = device;
    m_format = desc.format;
    m_width = desc.width;
    m_height = desc.height;
    m_ownsImage = true;

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

    // Vulkan VK_IMAGE_LAYOUT_UNDEFINED または VK_IMAGE_LAYOUT_PREINITIALIZED でなければならない。
    // (VUID-VkImageCreateInfo-initialLayout-00993)
    imgInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    auto usage = desc.usage;
    if (static_cast<u32>(usage) & static_cast<u32>(TextureUsage::Sampled))
        imgInfo.usage |= VK_IMAGE_USAGE_SAMPLED_BIT;
    if (static_cast<u32>(usage) & static_cast<u32>(TextureUsage::RenderTarget))
        imgInfo.usage |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    if (static_cast<u32>(usage) & static_cast<u32>(TextureUsage::DepthStencil)) {
        imgInfo.usage |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
    }
    if (static_cast<u32>(usage) & static_cast<u32>(TextureUsage::CopyDst))
        imgInfo.usage |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    if (static_cast<u32>(usage) & static_cast<u32>(TextureUsage::CopySrc))
        imgInfo.usage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

    VkDevice vkDevice = m_device->vkDevice();
    if (vkCreateImage(vkDevice, &imgInfo, nullptr, &m_image) != VK_SUCCESS) {
        return LN_MAKE_ERROR("vkCreateImage failed.");
    }

    VkMemoryRequirements memReqs;
    vkGetImageMemoryRequirements(vkDevice, m_image, &memReqs);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memReqs.size;
    allocInfo.memoryTypeIndex = findMemoryType(
        physicalDevice, memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    if (vkAllocateMemory(vkDevice, &allocInfo, nullptr, &m_memory) != VK_SUCCESS) {
        return LN_MAKE_ERROR("vkAllocateMemory failed.");
    }
    vkBindImageMemory(vkDevice, m_image, m_memory, 0);


    // VkImageCreateInfo::initialLayout でレイアウトを指定できそうなものだけど、
    // ↑のコメントの通り実際にはできないので、ここで明示的にレイアウト遷移しておく。
    if (static_cast<u32>(usage) & static_cast<u32>(TextureUsage::DepthStencil)) {
        {
            auto result = m_device->beginSingleTimeCommands();
            if (!result) {
                return LN_BOX_ERROR(result);
            }
            VkCommandBuffer commandBuffer = *result;

            VkImageMemoryBarrier barrier = {};
            barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            barrier.newLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.image = m_image;
            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
            barrier.subresourceRange.baseMipLevel = 0;
            barrier.subresourceRange.levelCount = 1;
            barrier.subresourceRange.baseArrayLayer = 0;
            barrier.subresourceRange.layerCount = 1;

            VkPipelineStageFlags sourceStage;
            VkPipelineStageFlags destinationStage;
            {
                barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
                if (hasStencilComponent(imgInfo.format)) {
                    barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
                }
                sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
                destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            }

            vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 0, 0, nullptr, 0, nullptr, 1, &barrier);

            m_device->endSingleTimeCommands(commandBuffer);
        }
    }
    return LN_MAKE_SUCCESS();
}

VoidResult VulkanTexture::initFromExternalImage(VulkanDevice* device, VkImage image, TextureFormat format, u32 width, u32 height) {
    m_device = device;
    m_image = image;
    m_format = format;
    m_width = width;
    m_height = height;
    m_ownsImage = false;
    return LN_MAKE_SUCCESS();
}

void VulkanTexture::finalize() {
    if (m_ownsImage) {
        VkDevice vkDevice = m_device->vkDevice();
        if (m_image) {
            vkDestroyImage(vkDevice, m_image, nullptr);
            m_image = VK_NULL_HANDLE;
        }
        if (m_memory) {
            vkFreeMemory(vkDevice, m_memory, nullptr);
            m_memory = VK_NULL_HANDLE;
        }
    }
    Texture::finalize();
}

// ------ VulkanTextureView ------------------------------------------------------------------------------------------------------

VulkanTextureView::VulkanTextureView() = default;

VoidResult VulkanTextureView::init(VkDevice device, VkImage image, VkFormat format, VkImageAspectFlags aspect, u32 width, u32 height) {
    m_device = device;
    m_image = image;
    m_format = format;
    m_width = width;
    m_height = height;

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
    if (vkCreateImageView(m_device, &info, nullptr, &m_view) != VK_SUCCESS) {
        return LN_MAKE_ERROR("vkCreateImageView failed.");
    }
    return LN_MAKE_SUCCESS();
}

void VulkanTextureView::finalize() {
    if (m_view) vkDestroyImageView(m_device, m_view, nullptr);
    TextureView::finalize();
}

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

VoidResult VulkanShaderModule::init(VkDevice device, const ShaderModuleDesc& desc) {
    m_device = device;

    m_spirv.assign(desc.spirvCode, desc.spirvCode + desc.spirvSizeBytes / sizeof(u32));

    VkShaderModuleCreateInfo info{};
    info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    info.codeSize = desc.spirvSizeBytes;
    info.pCode = desc.spirvCode;
    if (vkCreateShaderModule(m_device, &info, nullptr, &m_module) != VK_SUCCESS) {
        return LN_MAKE_ERROR("vkCreateShaderModule failed.");
    }
    return LN_MAKE_SUCCESS();
}

void VulkanShaderModule::finalize() {
    if (m_module) vkDestroyShaderModule(m_device, m_module, nullptr);
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
    if (static_cast<u32>(s) & static_cast<u32>(ShaderStage::Vertex))   flags |= VK_SHADER_STAGE_VERTEX_BIT;
    if (static_cast<u32>(s) & static_cast<u32>(ShaderStage::Fragment)) flags |= VK_SHADER_STAGE_FRAGMENT_BIT;
    if (static_cast<u32>(s) & static_cast<u32>(ShaderStage::Compute))  flags |= VK_SHADER_STAGE_COMPUTE_BIT;
    return flags;
}

VulkanBindGroupLayout::VulkanBindGroupLayout() = default;

VoidResult VulkanBindGroupLayout::init(VkDevice device, const BindGroupLayoutDesc& desc) {
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
    if (vkCreateDescriptorSetLayout(m_device, &info, nullptr, &m_layout) != VK_SUCCESS) {
        return LN_MAKE_ERROR("vkCreateDescriptorSetLayout failed.");
    }
    return LN_MAKE_SUCCESS();
}

void VulkanBindGroupLayout::finalize() {
    if (m_layout) vkDestroyDescriptorSetLayout(m_device, m_layout, nullptr);
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

    std::vector<VkWriteDescriptorSet> writes;
    std::vector<VkDescriptorBufferInfo> bufInfos(desc.entries.size());
    std::vector<VkDescriptorImageInfo> imgInfos(desc.entries.size());

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
    m_vulkanDevice = vulkanDevice;
    m_device = vulkanDevice->vkDevice();

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
    if (vkCreatePipelineLayout(m_device, &info, nullptr, &m_layout) != VK_SUCCESS) {
        return LN_MAKE_ERROR("vkCreatePipelineLayout failed.");
    }
    return LN_MAKE_SUCCESS();
}

Result<Ref<BindGroup>> VulkanPipelineLayout::createBindGroup(
    u32 setIndex, const std::vector<BindGroupEntry>& entries) {
    if (setIndex >= m_bindGroupLayouts.size()) {
        return LN_MAKE_ERROR("setIndex out of range.");
    }

    BindGroupDesc bgDesc;
    bgDesc.layout = m_bindGroupLayouts[setIndex].get();
    bgDesc.entries = entries;

    auto bg = Ref<VulkanBindGroup>::adopt(new VulkanBindGroup());
    if (!bg->init(m_vulkanDevice, m_vulkanDevice->descriptorPoolManager(),
                  m_bindGroupLayouts[setIndex].get(), bgDesc)) {
        return LN_MAKE_ERROR("Failed to create bind group.");
    }
    return Ref<BindGroup>(bg);
}

void VulkanPipelineLayout::finalize() {
    if (m_layout) vkDestroyPipelineLayout(m_device, m_layout, nullptr);
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
    std::vector<VkPipelineColorBlendAttachmentState> blendAttachments;
    const auto& colorFormats = desc.renderPass->layoutDesc().colorFormats;
    for (size_t i = 0; i < colorFormats.size(); ++i) {
        VkPipelineColorBlendAttachmentState att{};
        bool colorWrite = (i < desc.blendStates.size()) ? desc.blendStates[i].colorWriteEnabled : true;
        att.colorWriteMask = colorWrite
            ? (VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
               VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT)
            : 0;
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
    depthStencil.stencilTestEnable = desc.depthStencil.stencilTestEnable ? VK_TRUE : VK_FALSE;
    if (desc.depthStencil.stencilTestEnable) {
        depthStencil.front = toVkStencilOpState(
            desc.depthStencil.stencilFront,
            desc.depthStencil.stencilReadMask,
            desc.depthStencil.stencilWriteMask);
        depthStencil.back = toVkStencilOpState(
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

// ------ VulkanRenderPassEncoder ------------------------------------------------------------------------------------------

VulkanRenderPassEncoder::VulkanRenderPassEncoder(
    VkCommandBuffer cmd, VkRenderPass renderPass,
    VkFramebuffer framebuffer, VkExtent2D extent,
    const RenderPassDesc& desc)
    : m_cmd(cmd) {
    std::vector<VkClearValue> clearValues;
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
    rpBegin.renderPass = renderPass;
    rpBegin.framebuffer = framebuffer;
    rpBegin.renderArea.extent = extent;
    rpBegin.clearValueCount = static_cast<uint32_t>(clearValues.size());
    rpBegin.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(m_cmd, &rpBegin, VK_SUBPASS_CONTENTS_INLINE);

    // Set default viewport and scissor.
    // Use negative height (y = height, h = -height) to flip Vulkan's Y axis so that
    // +Y points upward, matching standard math / OpenGL NDC convention.
    VkViewport vp{0, static_cast<float>(extent.height), static_cast<float>(extent.width), -static_cast<float>(extent.height), 0, 1};
    vkCmdSetViewport(m_cmd, 0, 1, &vp);
    VkRect2D scissor{{0, 0}, extent};
    vkCmdSetScissor(m_cmd, 0, 1, &scissor);
}

void VulkanRenderPassEncoder::setPipeline(RenderPipeline* pipeline) {
    auto* vp = static_cast<VulkanRenderPipeline*>(pipeline);
    vkCmdBindPipeline(m_cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, vp->handle());
    m_currentPipelineLayout = vp->layoutHandle();
}

void VulkanRenderPassEncoder::setVertexBuffer(u32 slot, Buffer* buffer, u64 offset) {
    auto* vb = static_cast<VulkanBuffer*>(buffer);
    VkBuffer buf = vb->handle();
    VkDeviceSize off = offset;
    vkCmdBindVertexBuffers(m_cmd, slot, 1, &buf, &off);
}

void VulkanRenderPassEncoder::setIndexBuffer(Buffer* buffer, IndexFormat format, u64 offset) {
    auto* vb = static_cast<VulkanBuffer*>(buffer);
    VkIndexType type = format == IndexFormat::Uint16 ? VK_INDEX_TYPE_UINT16 : VK_INDEX_TYPE_UINT32;
    vkCmdBindIndexBuffer(m_cmd, vb->handle(), offset, type);
}

void VulkanRenderPassEncoder::setBindGroup(u32 index, BindGroup* group) {
    auto* vg = static_cast<VulkanBindGroup*>(group);
    VkDescriptorSet set = vg->handle();
    vkCmdBindDescriptorSets(m_cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, m_currentPipelineLayout, index, 1, &set, 0, nullptr);
}

void VulkanRenderPassEncoder::setBindGroup(u32 index, BindGroup* group,
                                            const u32* dynamicOffsets, u32 dynamicOffsetCount) {
    auto* vg = static_cast<VulkanBindGroup*>(group);
    VkDescriptorSet set = vg->handle();
    vkCmdBindDescriptorSets(m_cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, m_currentPipelineLayout,
                            index, 1, &set, dynamicOffsetCount, dynamicOffsets);
}

void VulkanRenderPassEncoder::setViewport(f32 x, f32 y, f32 w, f32 h, f32 minDepth, f32 maxDepth) {
    VkViewport vp{x, y, w, h, minDepth, maxDepth};
    vkCmdSetViewport(m_cmd, 0, 1, &vp);
}

void VulkanRenderPassEncoder::setScissorRect(u32 x, u32 y, u32 w, u32 h) {
    VkRect2D sc{{static_cast<int32_t>(x), static_cast<int32_t>(y)}, {w, h}};
    vkCmdSetScissor(m_cmd, 0, 1, &sc);
}

void VulkanRenderPassEncoder::setStencilReference(u32 reference) {
    vkCmdSetStencilReference(m_cmd, VK_STENCIL_FACE_FRONT_AND_BACK, reference);
}

void VulkanRenderPassEncoder::draw(u32 vertexCount, u32 instanceCount, u32 firstVertex, u32 firstInstance) {
    vkCmdDraw(m_cmd, vertexCount, instanceCount, firstVertex, firstInstance);
}

void VulkanRenderPassEncoder::drawIndexed(u32 indexCount, u32 instanceCount, u32 firstIndex, i32 baseVertex, u32 firstInstance) {
    vkCmdDrawIndexed(m_cmd, indexCount, instanceCount, firstIndex, baseVertex, firstInstance);
}

void VulkanRenderPassEncoder::end() {
    vkCmdEndRenderPass(m_cmd);
}

// ------ RenderPass Cache helpers ----------------------------------------------------------------------------------------

bool RenderPassKey::ColorAttachment::operator==(const ColorAttachment& other) const {
    return format == other.format && loadOp == other.loadOp;
}

bool RenderPassKey::operator==(const RenderPassKey& other) const {
    return colorAttachments == other.colorAttachments && depthFormat == other.depthFormat &&
        stencilLoadOp == other.stencilLoadOp;
}

size_t RenderPassKeyHash::operator()(const RenderPassKey& key) const {
    size_t h = 0;
    for (const auto& a : key.colorAttachments) {
        h ^= std::hash<int>()(a.format) + 0x9e3779b9 + (h << 6) + (h >> 2);
        h ^= std::hash<int>()(a.loadOp) + 0x9e3779b9 + (h << 6) + (h >> 2);
    }
    h ^= std::hash<int>()(key.depthFormat) + 0x9e3779b9 + (h << 6) + (h >> 2);
    h ^= std::hash<int>()(key.stencilLoadOp) + 0x9e3779b9 + (h << 6) + (h >> 2);
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

// ------ VulkanSwapChain ----------------------------------------------------------------------------------------------------------

VulkanSwapChain::VulkanSwapChain()
    : m_device(nullptr)
    //, m_format(TextureFormat::BGRA8Unorm)
{
}

VoidResult VulkanSwapChain::init(VulkanDevice* device, const SwapChainDesc& desc) {
    m_device = device;
    // Create surface
    glfwCreateWindowSurface(m_device->instance(), static_cast<GLFWwindow*>(desc.nativeWindowHandle), nullptr, &m_surface);

    // Query surface capabilities
    VkSurfaceCapabilitiesKHR caps{};
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_device->physicalDevice(), m_surface, &caps);

    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(m_device->physicalDevice(), m_surface);

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    //m_format = toTextureFormat(surfaceFormat.format);

    m_extent = caps.currentExtent;
    if (m_extent.width == UINT32_MAX) {
        m_extent.width = desc.width;
        m_extent.height = desc.height;
    }

    uint32_t imageCount = caps.minImageCount + 1;
    if (caps.maxImageCount > 0 && imageCount > caps.maxImageCount) imageCount = caps.maxImageCount;

    VkSwapchainCreateInfoKHR swapInfo{};
    swapInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapInfo.surface = m_surface;
    swapInfo.minImageCount = imageCount;
    swapInfo.imageFormat = surfaceFormat.format;
    swapInfo.imageColorSpace = surfaceFormat.colorSpace;
    swapInfo.imageExtent = m_extent;
    swapInfo.imageArrayLayers = 1;
    swapInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT; // readData できるようにするため、VK_IMAGE_USAGE_TRANSFER_SRC_BIT も指定しておく
    swapInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapInfo.preTransform = caps.currentTransform;
    swapInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapInfo.presentMode = desc.vsync ? VK_PRESENT_MODE_FIFO_KHR : VK_PRESENT_MODE_MAILBOX_KHR;
    swapInfo.clipped = VK_TRUE;
    vkCreateSwapchainKHR(m_device->vkDevice(), &swapInfo, nullptr, &m_swapchain);
    
    // Get swap chain images
    vkGetSwapchainImagesKHR(m_device->vkDevice(), m_swapchain, &imageCount, nullptr);
    m_images.resize(imageCount);
    m_maxFrames = imageCount;
    vkGetSwapchainImagesKHR(m_device->vkDevice(), m_swapchain, &imageCount, m_images.data());

    // バックバッファのデフォルトレイアウトがは VK_IMAGE_LAYOUT_UNDEFINED なので、
    // VK_IMAGE_LAYOUT_PRESENT_SRC_KHR に確定しておく。
    // こうしないと RenderPass 側で、初回の描画なのかどうかで initialLayout の指定を変える必要があり、
    // キャッシュしている RenderPass のヒット率を下げてしまうことになる。
    {
        auto result = m_device->beginSingleTimeCommands();
        if (!result) {
            return LN_BOX_ERROR(result);
        }
        VkCommandBuffer commandBuffer = *result;
        for (size_t i = 0; i < m_images.size(); ++i) {
            VkImageMemoryBarrier barrier = {};
            barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED; 
            barrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
            barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            barrier.image = m_images[i];

            barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            barrier.subresourceRange.baseMipLevel = 0;
            barrier.subresourceRange.levelCount = 1;
            barrier.subresourceRange.baseArrayLayer = 0;
            barrier.subresourceRange.layerCount = 1;
            VkPipelineStageFlags sourceStage;
            VkPipelineStageFlags destinationStage;
            barrier.srcAccessMask = 0;
            barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

            sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
            destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            vkCmdPipelineBarrier(
                commandBuffer,
                sourceStage,
                destinationStage,
                0,
                0,
                nullptr,
                0,
                nullptr,
                1,
                &barrier);
        }
        m_device->endSingleTimeCommands(commandBuffer);
    }

    // Create image views
    VkFormat vkFmt = swapInfo.imageFormat;
    m_views.resize(imageCount);
    for (u32 i = 0; i < imageCount; ++i) {
        auto view = Ref<VulkanTextureView>::adopt(new VulkanTextureView());
        if (!view->init(
                m_device->vkDevice(),
                m_images[i],
                vkFmt,
                VK_IMAGE_ASPECT_COLOR_BIT,
                m_extent.width,
                m_extent.height)) {
            return LN_MAKE_ERROR("Failed to create swap chain image view.");
        }
        m_views[i] = view;
    }

    // Create CommandBuffers for rendering.
    for (u32 i = 0; i < m_maxFrames; ++i) {
        auto commandBuffer = m_device->createCommandBuffer();
        if (!commandBuffer) {
            // TODO: Error handling
            throw std::runtime_error("Failed to create command buffer for swap chain.");
        }
        m_commandBuffers.push_back(*commandBuffer);
    }

    // Create sync objects
    VkSemaphoreCreateInfo semInfo{VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO};

    m_imageAvailableSemaphores.resize(m_maxFrames);
    m_renderFinished.resize(m_maxFrames);
    for (u32 i = 0; i < m_maxFrames; ++i) {
        vkCreateSemaphore(m_device->vkDevice(), &semInfo, nullptr, &m_imageAvailableSemaphores[i]);
        vkCreateSemaphore(m_device->vkDevice(), &semInfo, nullptr, &m_renderFinished[i]);
    }
    return LN_MAKE_SUCCESS();
}

void VulkanSwapChain::finalize() {
    auto dev = m_device->vkDevice();
    vkDeviceWaitIdle(dev);

    for (u32 i = 0; i < m_maxFrames; ++i) {
        if (m_imageAvailableSemaphores[i]) vkDestroySemaphore(dev, m_imageAvailableSemaphores[i], nullptr);
        if (m_renderFinished[i]) vkDestroySemaphore(dev, m_renderFinished[i], nullptr);
    }
    m_views.clear();
    if (m_swapchain) vkDestroySwapchainKHR(dev, m_swapchain, nullptr);
    if (m_surface) vkDestroySurfaceKHR(m_device->instance(), m_surface, nullptr);
}

VulkanSwapChain::SwapChainSupportDetails VulkanSwapChain::querySwapChainSupport(
    VkPhysicalDevice device,
    VkSurfaceKHR surface) {
    SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

    if (formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
    }

    if (details.capabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
        details.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    }
    else {
        details.preTransform = details.capabilities.currentTransform;
    }

    // Find a supported composite alpha mode
    details.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    VkCompositeAlphaFlagBitsKHR compositeAlphaFlags[4] = {
        VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
        VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
        VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
    };
    for (uint32_t i = 0; i < sizeof(compositeAlphaFlags) / sizeof(compositeAlphaFlags[0]); i++) {
        if (details.capabilities.supportedCompositeAlpha & compositeAlphaFlags[i]) {
            details.compositeAlpha = compositeAlphaFlags[i];
            break;
        }
    }

    return details;
}

VkSurfaceFormatKHR VulkanSwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
    if (availableFormats.size() == 1 && availableFormats[0].format == VK_FORMAT_UNDEFINED) {
        return {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
    }

    for (const auto& availableFormat : availableFormats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_UNORM &&
            availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormat;
        }
    }

    return availableFormats[0];
}

TextureView* VulkanSwapChain::acquireNextTexture() {
    VkDevice vkDevice = m_device->vkDevice();
    if (!m_commandBuffers[m_currentFrame]->begin()) {
        return nullptr;
    }

    // Run deferred cleanups for this frame index now that the GPU is done with it.
    m_device->beginFrame(m_currentFrame);

    // https://vulkan-tutorial.com/Drawing_a_triangle/Drawing/Rendering_and_presentation
    vkAcquireNextImageKHR(
        vkDevice,
        m_swapchain,
        UINT64_MAX,
        m_imageAvailableSemaphores[m_currentFrame],
        VK_NULL_HANDLE,
        &m_imageIndex);
    m_device->setActiveSwapChain(this);
    return m_views[m_imageIndex].get();
}

void VulkanSwapChain::present() {
    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &m_renderFinished[m_currentFrame]; // このセマフォの通知を待ってから実際に present する
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &m_swapchain;
    presentInfo.pImageIndices = &m_imageIndex;
    vkQueuePresentKHR(m_device->graphicsQueue(), &presentInfo);

    m_currentFrame = (m_currentFrame + 1) % m_maxFrames;
}

VkSemaphore VulkanSwapChain::imageAvailableSemaphore() const { return m_imageAvailableSemaphores[m_currentFrame]; }
VkSemaphore VulkanSwapChain::renderFinishedSemaphore() const { return m_renderFinished[m_currentFrame]; }

CommandBuffer* VulkanSwapChain::getCurrentCommandBuffer() {
    return m_commandBuffers[m_currentFrame].get();
}

// ------ VulkanCommandBuffer --------------------------------------------------------------------------------------------------

VulkanCommandBuffer::VulkanCommandBuffer() = default;

VoidResult VulkanCommandBuffer::init(VulkanDevice* device, VkCommandBuffer cmd) {
    m_device = device;
    m_cmd = cmd;

    VkFenceCreateInfo fenceInfo = {};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    if (vkCreateFence(m_device->vkDevice(), &fenceInfo, m_device->vulkanAllocator(), &m_inFlightFences) != VK_SUCCESS) {
        return LN_MAKE_ERROR("vkCreateFence failed.");
    }
    return LN_MAKE_SUCCESS();
}

void VulkanCommandBuffer::finalize() {
    dispose();
    CommandBuffer::finalize();
}

void VulkanCommandBuffer::dispose() {
    if (m_inFlightFences) {
        // Wait for execution to complete as it may be pending.
        vkWaitForFences(m_device->vkDevice(), 1, &m_inFlightFences, VK_TRUE, UINT64_MAX);
    }

    // CommandBuffer must be released before vkResetDescriptorPool.
    if (m_cmd) {
        VkCommandPool pool = m_device->commandPool();
        vkFreeCommandBuffers(m_device->vkDevice(), pool, 1, &m_cmd);
        m_cmd = VK_NULL_HANDLE;
    }
    // Queue vkFreeCommandBuffers to run once the GPU finishes this frame.
    // If the command buffer was never submitted, free immediately.
    //VkDevice dev = m_device->vkDevice();
    //VkCommandPool pool = m_device->commandPool();
    //VkCommandBuffer cmd = m_cmd;

    //if (m_submitted) {
    //    m_device->frameResources().queueDelete(m_submittedFrame, [dev, pool, cmd]() {
    //        vkFreeCommandBuffers(dev, pool, 1, &cmd);
    //    });
    //}
    //else {
    //    vkFreeCommandBuffers(dev, pool, 1, &cmd);
    //}

    if (m_inFlightFences) {
        vkDestroyFence(m_device->vkDevice(), m_inFlightFences, m_device->vulkanAllocator());
        m_inFlightFences = VK_NULL_HANDLE;
    }
}

VoidResult VulkanCommandBuffer::begin() {
    // もし前回 vkQueueSubmit したコマンドバッファが完了していなければ待つ
    //vkWaitForFences(m_device->vulkanDevice(), 1, &m_inFlightFence, VK_TRUE, std::numeric_limits<uint64_t>::max());
    VkDevice vkDevice = m_device->vkDevice();
    vkWaitForFences(vkDevice, 1, &m_inFlightFences, VK_TRUE, UINT64_MAX);
    vkResetFences(vkDevice, 1, &m_inFlightFences);

    vkResetCommandBuffer(m_cmd, 0);
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    if (VkResult r = vkBeginCommandBuffer(m_cmd, &beginInfo); r != VK_SUCCESS) {
        return LN_MAKE_VULKAN_ERROR(r, "vkBeginCommandBuffer");
    }
    return LN_MAKE_SUCCESS();
}

RenderPassEncoder* VulkanCommandBuffer::beginRenderPass(const RenderPassDesc& desc) {
    // Build RenderPassKey
    RenderPassKey rpKey;
    u32 fbWidth = 0, fbHeight = 0;
    for (size_t i = 0; i < desc.colorAttachments.size(); ++i) {
    //}
    //for (auto& ca : desc.colorAttachments) {
        auto& ca = desc.colorAttachments[i];
        auto* view = static_cast<VulkanTextureView*>(ca.view);

        
        RenderPassKey::ColorAttachment attachment = {};
        attachment.format = view->vkFormat();
        attachment.loadOp = ca.loadOp == LoadOp::Clear ? VK_ATTACHMENT_LOAD_OP_CLEAR
            : ca.loadOp == LoadOp::Load                ? VK_ATTACHMENT_LOAD_OP_LOAD
                                                       : VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        rpKey.colorAttachments.push_back(attachment);

        if (fbWidth == 0) {
            fbWidth = view->width();
            fbHeight = view->height();
        }
    }
    if (desc.depthStencilAttachment) {
        rpKey.depthFormat = static_cast<VulkanTextureView*>(desc.depthStencilAttachment->view)->vkFormat();
        rpKey.stencilLoadOp =
            desc.depthStencilAttachment->stencilLoadOp == LoadOp::Clear ? VK_ATTACHMENT_LOAD_OP_CLEAR :
            desc.depthStencilAttachment->stencilLoadOp == LoadOp::Load  ? VK_ATTACHMENT_LOAD_OP_LOAD :
                                                                          VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    }

    VkRenderPass renderPass = m_device->getOrCreateRenderPass(rpKey);

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

    VkFramebuffer framebuffer = m_device->getOrCreateFramebuffer(fbKey);
    VkExtent2D extent{fbKey.width, fbKey.height};

    m_encoder = new VulkanRenderPassEncoder(m_cmd, renderPass, framebuffer, extent, desc);
    return m_encoder;
}

void VulkanCommandBuffer::transitionToShaderRead(TextureView* colorTarget) {
    auto* vkView = static_cast<VulkanTextureView*>(colorTarget);

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = vkView->image();
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

    vkCmdPipelineBarrier(
        m_cmd,
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
        0,
        0,
        nullptr,
        0,
        nullptr,
        1,
        &barrier);
}

void VulkanCommandBuffer::submit() {
    vkEndCommandBuffer(m_cmd);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &m_cmd;

    VkSemaphore waitSemaphore = VK_NULL_HANDLE;
    VkSemaphore signalSemaphore = VK_NULL_HANDLE;
    VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    VkFence fence = VK_NULL_HANDLE;

    auto* sc = m_device->activeSwapChain();
    if (sc) {
        m_submittedFrame = sc->currentFrame();
        m_submitted = true;
        waitSemaphore = sc->imageAvailableSemaphore();
        signalSemaphore = sc->renderFinishedSemaphore();
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = &waitSemaphore;// 実行を開始する前に待機するセマフォ
        submitInfo.pWaitDstStageMask = &waitStage;
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = &signalSemaphore;  // 実行を完了したときに通知されるセマフォ
    }

    vkQueueSubmit(m_device->graphicsQueue(), 1, &submitInfo, m_inFlightFences);

    delete m_encoder;
    m_encoder = nullptr;
}

} // namespace ln::rhi::vulkan
