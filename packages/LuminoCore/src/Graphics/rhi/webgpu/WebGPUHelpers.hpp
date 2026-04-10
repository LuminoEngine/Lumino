#pragma once
#include <LuminoCore/Graphics/rhi/Rhi.hpp>
#include <webgpu/webgpu.h>

namespace ln::rhi::webgpu {

//------------------------------------------------------------------------------
inline WGPUTextureFormat toWGPUTextureFormat(TextureFormat fmt) {
    switch (fmt) {
        case TextureFormat::BGRA8Unorm:        return WGPUTextureFormat_BGRA8Unorm;
        case TextureFormat::BGRA8UnormSrgb:    return WGPUTextureFormat_BGRA8UnormSrgb;
        case TextureFormat::RGBA8Unorm:        return WGPUTextureFormat_RGBA8Unorm;
        case TextureFormat::RGBA8UnormSrgb:    return WGPUTextureFormat_RGBA8UnormSrgb;
        case TextureFormat::Depth24Stencil8:   return WGPUTextureFormat_Depth24PlusStencil8;
        case TextureFormat::Depth32Float:      return WGPUTextureFormat_Depth32Float;
        case TextureFormat::R8Unorm:           return WGPUTextureFormat_R8Unorm;
        case TextureFormat::RG8Unorm:          return WGPUTextureFormat_RG8Unorm;
        case TextureFormat::RGBA16Float:       return WGPUTextureFormat_RGBA16Float;
        case TextureFormat::RGBA32Float:       return WGPUTextureFormat_RGBA32Float;
        default:                               return WGPUTextureFormat_Undefined;
    }
}

//------------------------------------------------------------------------------
inline TextureFormat fromWGPUTextureFormat(WGPUTextureFormat fmt) {
    switch (fmt) {
        case WGPUTextureFormat_BGRA8Unorm:          return TextureFormat::BGRA8Unorm;
        case WGPUTextureFormat_BGRA8UnormSrgb:      return TextureFormat::BGRA8UnormSrgb;
        case WGPUTextureFormat_RGBA8Unorm:           return TextureFormat::RGBA8Unorm;
        case WGPUTextureFormat_RGBA8UnormSrgb:       return TextureFormat::RGBA8UnormSrgb;
        case WGPUTextureFormat_Depth24PlusStencil8:  return TextureFormat::Depth24Stencil8;
        case WGPUTextureFormat_Depth32Float:         return TextureFormat::Depth32Float;
        case WGPUTextureFormat_R8Unorm:              return TextureFormat::R8Unorm;
        case WGPUTextureFormat_RG8Unorm:             return TextureFormat::RG8Unorm;
        case WGPUTextureFormat_RGBA16Float:          return TextureFormat::RGBA16Float;
        case WGPUTextureFormat_RGBA32Float:          return TextureFormat::RGBA32Float;
        default:                                     return TextureFormat::Undefined;
    }
}

//------------------------------------------------------------------------------
inline WGPUTextureUsage toWGPUTextureUsage(TextureUsage usage) {
    WGPUTextureUsage result = WGPUTextureUsage_None;
    if (static_cast<u32>(usage) & static_cast<u32>(TextureUsage::Sampled))
        result |= WGPUTextureUsage_TextureBinding;
    if (static_cast<u32>(usage) & static_cast<u32>(TextureUsage::Storage))
        result |= WGPUTextureUsage_StorageBinding;
    if (static_cast<u32>(usage) & static_cast<u32>(TextureUsage::RenderTarget))
        result |= WGPUTextureUsage_RenderAttachment;
    if (static_cast<u32>(usage) & static_cast<u32>(TextureUsage::CopySrc))
        result |= WGPUTextureUsage_CopySrc;
    if (static_cast<u32>(usage) & static_cast<u32>(TextureUsage::CopyDst))
        result |= WGPUTextureUsage_CopyDst;
    if (static_cast<u32>(usage) & static_cast<u32>(TextureUsage::DepthStencil))
        result |= WGPUTextureUsage_RenderAttachment;
    return result;
}

//------------------------------------------------------------------------------
inline WGPULoadOp toWGPULoadOp(LoadOp op) {
    switch (op) {
        case LoadOp::Load:    return WGPULoadOp_Load;
        case LoadOp::Clear:   return WGPULoadOp_Clear;
        case LoadOp::DontCare: return WGPULoadOp_Clear; // WebGPU has no DontCare for load
        default:              return WGPULoadOp_Clear;
    }
}

//------------------------------------------------------------------------------
inline WGPUStoreOp toWGPUStoreOp(StoreOp op) {
    switch (op) {
        case StoreOp::Store:    return WGPUStoreOp_Store;
        case StoreOp::DontCare: return WGPUStoreOp_Discard;
        default:                return WGPUStoreOp_Store;
    }
}

} // namespace ln::rhi::webgpu
