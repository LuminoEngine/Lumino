#pragma once
#include <LuminoCore/Graphics/rhi/Rhi.hpp>
#include <webgpu/webgpu.h>

// 最初に Dawn を使っていた時の名残り。
// 今は WebGPU-distribution  を使っているが、少しヘッダが古いようだ。
#define LN_WEBGPU_DAWN_LATEST 0
#define WGPU_TRUE 1
#define WGPU_FALSE 0

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
    if (static_cast<uint32_t>(usage) & static_cast<uint32_t>(TextureUsage::Sampled))
        result |= WGPUTextureUsage_TextureBinding;
    if (static_cast<uint32_t>(usage) & static_cast<uint32_t>(TextureUsage::Storage))
        result |= WGPUTextureUsage_StorageBinding;
    if (static_cast<uint32_t>(usage) & static_cast<uint32_t>(TextureUsage::RenderTarget))
        result |= WGPUTextureUsage_RenderAttachment;
    if (static_cast<uint32_t>(usage) & static_cast<uint32_t>(TextureUsage::CopySrc))
        result |= WGPUTextureUsage_CopySrc;
    if (static_cast<uint32_t>(usage) & static_cast<uint32_t>(TextureUsage::CopyDst))
        result |= WGPUTextureUsage_CopyDst;
    if (static_cast<uint32_t>(usage) & static_cast<uint32_t>(TextureUsage::DepthStencil))
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

//------------------------------------------------------------------------------
inline WGPUFilterMode toWGPUFilterMode(FilterMode v) {
    switch (v) {
        case FilterMode::Nearest: return WGPUFilterMode_Nearest;
        case FilterMode::Linear:  return WGPUFilterMode_Linear;
        default:                  return WGPUFilterMode_Linear;
    }
}

//------------------------------------------------------------------------------
inline WGPUMipmapFilterMode toWGPUMipmapFilterMode(FilterMode v) {
    switch (v) {
        case FilterMode::Nearest: return WGPUMipmapFilterMode_Nearest;
        case FilterMode::Linear:  return WGPUMipmapFilterMode_Linear;
        default:                  return WGPUMipmapFilterMode_Linear;
    }
}

//------------------------------------------------------------------------------
inline WGPUAddressMode toWGPUAddressMode(AddressMode v) {
    switch (v) {
        case AddressMode::Repeat:         return WGPUAddressMode_Repeat;
        case AddressMode::MirroredRepeat: return WGPUAddressMode_MirrorRepeat;
        case AddressMode::ClampToEdge:    return WGPUAddressMode_ClampToEdge;
        default:                          return WGPUAddressMode_Repeat;
    }
}

//------------------------------------------------------------------------------
inline WGPUPrimitiveTopology toWGPUPrimitiveTopology(PrimitiveTopology v) {
    switch (v) {
        case PrimitiveTopology::TriangleList:  return WGPUPrimitiveTopology_TriangleList;
        case PrimitiveTopology::TriangleStrip: return WGPUPrimitiveTopology_TriangleStrip;
        case PrimitiveTopology::LineList:      return WGPUPrimitiveTopology_LineList;
        case PrimitiveTopology::LineStrip:     return WGPUPrimitiveTopology_LineStrip;
        case PrimitiveTopology::PointList:     return WGPUPrimitiveTopology_PointList;
        default:                               return WGPUPrimitiveTopology_TriangleList;
    }
}

//------------------------------------------------------------------------------
inline WGPUCullMode toWGPUCullMode(CullMode v) {
    switch (v) {
        case CullMode::None:  return WGPUCullMode_None;
        case CullMode::Front: return WGPUCullMode_Front;
        case CullMode::Back:  return WGPUCullMode_Back;
        default:              return WGPUCullMode_None;
    }
}

//------------------------------------------------------------------------------
inline WGPUFrontFace toWGPUFrontFace(FrontFace v) {
    switch (v) {
        case FrontFace::CCW: return WGPUFrontFace_CCW;
        case FrontFace::CW:  return WGPUFrontFace_CW;
        default:             return WGPUFrontFace_CCW;
    }
}

//------------------------------------------------------------------------------
inline WGPUBlendFactor toWGPUBlendFactor(BlendFactor v) {
    switch (v) {
        case BlendFactor::Zero:             return WGPUBlendFactor_Zero;
        case BlendFactor::One:              return WGPUBlendFactor_One;
        case BlendFactor::SrcColor:         return WGPUBlendFactor_Src;
        case BlendFactor::OneMinusSrcColor: return WGPUBlendFactor_OneMinusSrc;
        case BlendFactor::SrcAlpha:         return WGPUBlendFactor_SrcAlpha;
        case BlendFactor::OneMinusSrcAlpha: return WGPUBlendFactor_OneMinusSrcAlpha;
        case BlendFactor::DstColor:         return WGPUBlendFactor_Dst;
        case BlendFactor::OneMinusDstColor: return WGPUBlendFactor_OneMinusDst;
        case BlendFactor::DstAlpha:         return WGPUBlendFactor_DstAlpha;
        case BlendFactor::OneMinusDstAlpha: return WGPUBlendFactor_OneMinusDstAlpha;
        default:                            return WGPUBlendFactor_Zero;
    }
}

//------------------------------------------------------------------------------
inline WGPUBlendOperation toWGPUBlendOperation(BlendOp v) {
    switch (v) {
        case BlendOp::Add:             return WGPUBlendOperation_Add;
        case BlendOp::Subtract:        return WGPUBlendOperation_Subtract;
        case BlendOp::ReverseSubtract: return WGPUBlendOperation_ReverseSubtract;
        case BlendOp::Min:             return WGPUBlendOperation_Min;
        case BlendOp::Max:             return WGPUBlendOperation_Max;
        default:                       return WGPUBlendOperation_Add;
    }
}

//------------------------------------------------------------------------------
inline WGPUCompareFunction toWGPUCompareFunction(CompareFunction v) {
    switch (v) {
        case CompareFunction::Never:        return WGPUCompareFunction_Never;
        case CompareFunction::Less:         return WGPUCompareFunction_Less;
        case CompareFunction::LessEqual:    return WGPUCompareFunction_LessEqual;
        case CompareFunction::Greater:      return WGPUCompareFunction_Greater;
        case CompareFunction::GreaterEqual: return WGPUCompareFunction_GreaterEqual;
        case CompareFunction::Equal:        return WGPUCompareFunction_Equal;
        case CompareFunction::NotEqual:     return WGPUCompareFunction_NotEqual;
        case CompareFunction::Always:       return WGPUCompareFunction_Always;
        default:                            return WGPUCompareFunction_Always;
    }
}

//------------------------------------------------------------------------------
inline WGPUStencilOperation toWGPUStencilOperation(StencilOp v) {
    switch (v) {
        case StencilOp::Keep:           return WGPUStencilOperation_Keep;
        case StencilOp::Zero:           return WGPUStencilOperation_Zero;
        case StencilOp::Replace:        return WGPUStencilOperation_Replace;
        case StencilOp::IncrementClamp: return WGPUStencilOperation_IncrementClamp;
        case StencilOp::DecrementClamp: return WGPUStencilOperation_DecrementClamp;
        case StencilOp::Invert:         return WGPUStencilOperation_Invert;
        default:                        return WGPUStencilOperation_Keep;
    }
}

//------------------------------------------------------------------------------
inline WGPUVertexFormat toWGPUVertexFormat(VertexFormat v) {
    switch (v) {
        case VertexFormat::Float32x2:   return WGPUVertexFormat_Float32x2;
        case VertexFormat::Float32x3:   return WGPUVertexFormat_Float32x3;
        case VertexFormat::Float32x4:   return WGPUVertexFormat_Float32x4;
        case VertexFormat::Uint8x4Norm: return WGPUVertexFormat_Unorm8x4;
        case VertexFormat::Sint16x2:    return WGPUVertexFormat_Sint16x2;
        case VertexFormat::Sint16x4:    return WGPUVertexFormat_Sint16x4;
        default:                        return WGPUVertexFormat_Float32x3;
    }
}

//------------------------------------------------------------------------------
inline WGPUIndexFormat toWGPUIndexFormat(IndexFormat v) {
    switch (v) {
        case IndexFormat::Uint16: return WGPUIndexFormat_Uint16;
        case IndexFormat::Uint32: return WGPUIndexFormat_Uint32;
        default:                  return WGPUIndexFormat_Uint32;
    }
}

} // namespace ln::rhi::webgpu
