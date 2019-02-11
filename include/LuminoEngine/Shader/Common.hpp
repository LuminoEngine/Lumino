// Copyright (c) 2019 lriki. Distributed under the MIT license.
#pragma once

namespace ln {
namespace detail {

struct UnifiedShaderTriple
{
    std::string target;
    uint32_t version;
    std::string option;
};

#define LN_CIS_PREFIX "lnCIS"
#define LN_TO_PREFIX "lnTO"
#define LN_SO_PREFIX "lnSO"
#define LN_IS_RT_POSTFIX "lnIsRT"
#define LN_CIS_PREFIX_LEN 5
#define LN_TO_PREFIX_LEN 4
#define LN_SO_PREFIX_LEN 4
#define LN_IS_RT_POSTFIX_LEN 6

enum AttributeUsage
{
    AttributeUsage_Unknown = 0,
    AttributeUsage_Position = 1,
    AttributeUsage_BlendIndices = 2,
    AttributeUsage_BlendWeight = 3,
    AttributeUsage_Normal = 4,
    AttributeUsage_TexCoord = 5,
    AttributeUsage_Tangent = 6,
    AttributeUsage_Binormal = 7,
	AttributeUsage_Color = 8,

    // based https://docs.microsoft.com/ja-jp/windows/desktop/direct3d9/d3ddeclusage
};

struct VertexInputAttribute
{
	AttributeUsage usage;
	uint32_t index;
	uint32_t layoutLocation;
};

using VertexInputAttributeTable = std::vector<VertexInputAttribute>;


enum ShaderUniformType
{
    ShaderUniformType_Unknown = 0,
    ShaderUniformType_Bool = 1,
    //ShaderUniformType_BoolArray = 2,
    ShaderUniformType_Int = 3,
    //ShaderUniformType_IntArray = 4,
    ShaderUniformType_Float = 5,
    //ShaderUniformType_FloatArray = 6,
    ShaderUniformType_Vector = 7,
    //ShaderUniformType_VectorArray = 8,
    ShaderUniformType_Matrix = 9,
    //ShaderUniformType_MatrixArray = 10,
    ShaderUniformType_Texture = 11,
};

struct ShaderUniformInfo
{
    std::string name;
    uint16_t type;		// ShaderUniformType
    //uint16_t ownerBufferIndex;

    uint16_t offset;		// UniformBuffer 先頭からのオフセットバイト数
    //uint16_t size;		// Uniform 1つ分の全体サイズ (配列、行列の分も含む)

    uint16_t vectorElements;
    uint16_t arrayElements;	// 配列要素数
    uint16_t matrixRows;
    uint16_t matrixColumns;

    //uint16_t arrayStride;		// 配列の場合、要素のバイトサイズ (float2=8, float3=12)
    //uint16_t matrixStride;	// 行列の "横" 方向のバイト数。実際のメモリ上に値を並べる時の行間を表す。(float4x4=16)

    // 1要素のバイトサイズ
    size_t elementSize() const
    {
        switch (type)
        {
        case ln::detail::ShaderUniformType_Unknown:
            return 0;
        case ln::detail::ShaderUniformType_Bool:
            return 1;
        case ln::detail::ShaderUniformType_Int:
            return 4;
        case ln::detail::ShaderUniformType_Float:
            return 4;
        case ln::detail::ShaderUniformType_Vector:
            return vectorElements * 4;
        case ln::detail::ShaderUniformType_Matrix:
            return matrixRows * matrixColumns * 4;
        case ln::detail::ShaderUniformType_Texture:
            LN_NOTIMPLEMENTED();
            return 0;
        default:
            LN_UNREACHABLE();
            return 0;
        }
    }
};

struct ShaderUniformBufferInfo
{
    std::string name;
    uint32_t size;		// UniformBuffer 全体のサイズ
    std::vector<ShaderUniformInfo> members;

    void mergeFrom(const ShaderUniformBufferInfo& other);

    static void mergeBuffers(
        const std::vector<ShaderUniformBufferInfo>& a,
        const std::vector<ShaderUniformBufferInfo>& b,
        std::vector<ShaderUniformBufferInfo>* out);
};

class UnifiedShaderRefrectionInfo
    : public RefObject
{
public:
    std::vector<ShaderUniformBufferInfo> buffers;
};

} // namespace detail
} // namespace ln
