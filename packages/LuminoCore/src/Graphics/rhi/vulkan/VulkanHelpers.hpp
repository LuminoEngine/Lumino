#pragma once
#include "VulkanCommon.hpp"

#define LN_MAKE_VULKAN_ERROR(result, func) LN_MAKE_ERROR("Failed: " func "(%d)", result)

namespace ln::rhi::vulkan {

// ------ VulkanBuffer ----------------------------------------------------------------------------------------------------------------

class VulkanHelpers final {
public:
    static void logDeviceProperties(VkPhysicalDeviceProperties deviceProperties);

    static VkFormat toVkFormat(TextureFormat fmt) {
        switch (fmt) {
            case TextureFormat::Undefined:
                return VK_FORMAT_UNDEFINED;
            case TextureFormat::BGRA8Unorm:
                return VK_FORMAT_B8G8R8A8_UNORM;
            case TextureFormat::BGRA8UnormSrgb:
                return VK_FORMAT_B8G8R8A8_SRGB;
            case TextureFormat::RGBA8Unorm:
                return VK_FORMAT_R8G8B8A8_UNORM;
            case TextureFormat::RGBA8UnormSrgb:
                return VK_FORMAT_R8G8B8A8_SRGB;
            case TextureFormat::Depth24Stencil8:
                return VK_FORMAT_D24_UNORM_S8_UINT;
            case TextureFormat::Depth32Float:
                return VK_FORMAT_D32_SFLOAT;
            case TextureFormat::R8Unorm:
                return VK_FORMAT_R8_UNORM;
            case TextureFormat::RG8Unorm:
                return VK_FORMAT_R8G8_UNORM;
            case TextureFormat::RGBA16Float:
                return VK_FORMAT_R16G16B16A16_SFLOAT;
            case TextureFormat::RGBA32Float:
                return VK_FORMAT_R32G32B32A32_SFLOAT;
        }
        return VK_FORMAT_R8G8B8A8_UNORM;
    }

    static TextureFormat fromVkFormat(VkFormat fmt) {
        switch (fmt) {
            case VK_FORMAT_UNDEFINED:
                return TextureFormat::Undefined;
            case VK_FORMAT_B8G8R8A8_UNORM:
                return TextureFormat::BGRA8Unorm;
            case VK_FORMAT_B8G8R8A8_SRGB:
                return TextureFormat::BGRA8UnormSrgb;
            case VK_FORMAT_R8G8B8A8_UNORM:
                return TextureFormat::RGBA8Unorm;
            case VK_FORMAT_R8G8B8A8_SRGB:
                return TextureFormat::RGBA8UnormSrgb;
            case VK_FORMAT_D24_UNORM_S8_UINT:
                return TextureFormat::Depth24Stencil8;
            case VK_FORMAT_D32_SFLOAT:
                return TextureFormat::Depth32Float;
            case VK_FORMAT_R8_UNORM:
                return TextureFormat::R8Unorm;
            case VK_FORMAT_R8G8_UNORM:
                return TextureFormat::RG8Unorm;
            case VK_FORMAT_R16G16B16A16_SFLOAT:
                return TextureFormat::RGBA16Float;
            case VK_FORMAT_R32G32B32A32_SFLOAT:
                return TextureFormat::RGBA32Float;
            default:
                return TextureFormat::Undefined;
        }
    }

    static VkFormat toVkVertexFormat(VertexFormat fmt) {
        switch (fmt) {
            case VertexFormat::Float32x2:
                return VK_FORMAT_R32G32_SFLOAT;
            case VertexFormat::Float32x3:
                return VK_FORMAT_R32G32B32_SFLOAT;
            case VertexFormat::Float32x4:
                return VK_FORMAT_R32G32B32A32_SFLOAT;
            case VertexFormat::Uint8x4Norm:
                return VK_FORMAT_R8G8B8A8_UNORM;
            case VertexFormat::Sint16x2:
                return VK_FORMAT_R16G16_SINT;
            case VertexFormat::Sint16x4:
                return VK_FORMAT_R16G16B16A16_SINT;
        }
        return VK_FORMAT_R32G32B32_SFLOAT;
    }

    static VkBlendFactor toVkBlendFactor(BlendFactor f) {
        switch (f) {
            case BlendFactor::Zero:
                return VK_BLEND_FACTOR_ZERO;
            case BlendFactor::One:
                return VK_BLEND_FACTOR_ONE;
            case BlendFactor::SrcColor:
                return VK_BLEND_FACTOR_SRC_COLOR;
            case BlendFactor::OneMinusSrcColor:
                return VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
            case BlendFactor::SrcAlpha:
                return VK_BLEND_FACTOR_SRC_ALPHA;
            case BlendFactor::OneMinusSrcAlpha:
                return VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
            case BlendFactor::DstColor:
                return VK_BLEND_FACTOR_DST_COLOR;
            case BlendFactor::OneMinusDstColor:
                return VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
            case BlendFactor::DstAlpha:
                return VK_BLEND_FACTOR_DST_ALPHA;
            case BlendFactor::OneMinusDstAlpha:
                return VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
        }
        return VK_BLEND_FACTOR_ONE;
    }

    static VkBlendOp toVkBlendOp(BlendOp op) {
        switch (op) {
            case BlendOp::Add:
                return VK_BLEND_OP_ADD;
            case BlendOp::Subtract:
                return VK_BLEND_OP_SUBTRACT;
            case BlendOp::ReverseSubtract:
                return VK_BLEND_OP_REVERSE_SUBTRACT;
            case BlendOp::Min:
                return VK_BLEND_OP_MIN;
            case BlendOp::Max:
                return VK_BLEND_OP_MAX;
        }
        return VK_BLEND_OP_ADD;
    }

    static VkCompareOp toVkCompareOp(CompareFunction fn) {
        switch (fn) {
            case CompareFunction::Never:
                return VK_COMPARE_OP_NEVER;
            case CompareFunction::Less:
                return VK_COMPARE_OP_LESS;
            case CompareFunction::LessEqual:
                return VK_COMPARE_OP_LESS_OR_EQUAL;
            case CompareFunction::Greater:
                return VK_COMPARE_OP_GREATER;
            case CompareFunction::GreaterEqual:
                return VK_COMPARE_OP_GREATER_OR_EQUAL;
            case CompareFunction::Equal:
                return VK_COMPARE_OP_EQUAL;
            case CompareFunction::NotEqual:
                return VK_COMPARE_OP_NOT_EQUAL;
            case CompareFunction::Always:
                return VK_COMPARE_OP_ALWAYS;
        }
        return VK_COMPARE_OP_LESS;
    }

    static VkStencilOp toVkStencilOp(StencilOp op) {
        switch (op) {
            case StencilOp::Keep:
                return VK_STENCIL_OP_KEEP;
            case StencilOp::Zero:
                return VK_STENCIL_OP_ZERO;
            case StencilOp::Replace:
                return VK_STENCIL_OP_REPLACE;
            case StencilOp::IncrementClamp:
                return VK_STENCIL_OP_INCREMENT_AND_CLAMP;
            case StencilOp::DecrementClamp:
                return VK_STENCIL_OP_DECREMENT_AND_CLAMP;
            case StencilOp::Invert:
                return VK_STENCIL_OP_INVERT;
        }
        return VK_STENCIL_OP_KEEP;
    }

    static VkStencilOpState toVkStencilOpState(
        const StencilFaceState& face,
        uint32_t readMask,
        uint32_t writeMask) {
        VkStencilOpState s{};
        s.failOp = toVkStencilOp(face.failOp);
        s.passOp = toVkStencilOp(face.passOp);
        s.depthFailOp = toVkStencilOp(face.depthFailOp);
        s.compareOp = toVkCompareOp(face.compare);
        s.compareMask = readMask;
        s.writeMask = writeMask;
        s.reference = 0; // vkCmdSetStencilReference で動的に設定する
        return s;
    }

    static uint32_t findMemoryType(
        VkPhysicalDevice physicalDevice,
        uint32_t typeFilter,
        VkMemoryPropertyFlags properties) {
        VkPhysicalDeviceMemoryProperties memProps;
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProps);
        for (uint32_t i = 0; i < memProps.memoryTypeCount; ++i) {
            if ((typeFilter & (1 << i)) &&
                (memProps.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }
        return 0;
    }
};

} // namespace ln::rhi::vulkan
