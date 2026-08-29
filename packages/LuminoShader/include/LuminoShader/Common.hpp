// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <LuminoBase/Result.hpp>
#include <LuminoBase/RefObject.hpp>
#include <LuminoBase/Types.hpp>

namespace ln {
namespace shader {

static const char* kGlobalConstantBufferName = "$Global";

enum ShaderTarget {
    ShaderTarget_UNKNOWN = 0,
    ShaderTarget_SPIRV = 1,
    ShaderTarget_DXIL = 2,
    ShaderTarget_WGSL = 3,
    ShaderTarget_METAL = 4,
};

enum DescriptorType {
    DescriptorType_UniformBuffer = 0,
    DescriptorType_Texture = 1,
    DescriptorType_SamplerState = 2,
    DescriptorType_UnorderdAccess = 3,
    DescriptorType_Count = 4,
};

enum ShaderStage2 {
    ShaderStage2_Vertex = 0,
    ShaderStage2_Fragment = 1,
    ShaderStage2_Compute = 2,
    ShaderStage2_Count = 3,
};

enum ShaderStageFlags {
    ShaderStageFlags_None = 0x00,
    ShaderStageFlags_Vertex = 0x01,
    ShaderStageFlags_Pixel = 0x02,
    ShaderStageFlags_Compute = 0x04,
};

enum ParameterBlockElementKind {
    ParameterBlockElementKind_ConstantBuffer = 0,
    ParameterBlockElementKind_Texture = 1,
    ParameterBlockElementKind_SamplerState = 2,
    ParameterBlockElementKind_StorageBuffer = 3,
};

enum AttributeUsage {
    AttributeUsage_Unknown = 0,
    AttributeUsage_Position = 1,
    AttributeUsage_BlendIndices = 2,
    AttributeUsage_BlendWeight = 3,
    AttributeUsage_Normal = 4,
    AttributeUsage_TexCoord = 5,
    AttributeUsage_Tangent = 6,
    AttributeUsage_Binormal = 7,
    AttributeUsage_Color = 8,
    AttributeUsage_InstanceID = 9,
};

struct VertexInputAttribute {
    AttributeUsage usage;
    int32_t index;
    int32_t layoutLocation;
};

using VertexInputAttributeTable = std::vector<VertexInputAttribute>;

using BlobId = int16_t;

struct Blob {
    BlobId id;
    std::vector<uint8_t> data;
};

// シェーダパッケージ用のエラーヘルパーマクロ。
#define LNSHADER_MAKE_ERROR(msg) tl::make_unexpected(::ln::Error{::ln::ErrorCode::ShaderCompilationFailed, msg})
#define LNSHADER_MAKE_ERROR_FMT(msg) tl::make_unexpected(::ln::Error{::ln::ErrorCode::ShaderCompilationFailed, msg})
#define LNSHADER_OK() ::ln::VoidResult{}

} // namespace shader
} // namespace ln
