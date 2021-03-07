// Copyright (c) 2019 lriki. Distributed under the MIT license.
#pragma once

//#define LN_SHADER_UBO_TRANSPORSE_MATRIX

namespace ln {
class Shader;
class ShaderDefaultDescriptor;
class ShaderTechnique;
class ShaderPass;

namespace detail {
class ShaderSecondaryDescriptor;
class ShaderDescriptorPool;
//
//// LigitingModel
//enum class ShaderTechniqueClass_Ligiting : uint8_t
//{
//};

enum class ShaderTechniqueClass_Phase : uint8_t
{
    Forward, // default
    // TODO: Differd
    LightDisc,
    ShadowCaster,
    ForwardGBufferPrepass
};

// VertexFactory
enum class ShaderTechniqueClass_MeshProcess : uint8_t
{
    StaticMesh, // default
    SkinnedMesh,    // define LN_USE_SKINNING
};

// PixelShader
enum class ShaderTechniqueClass_ShadingModel : uint8_t
{
    Default,
    Unlit,
};

enum class ShaderTechniqueClass_DrawMode : uint8_t
{
    Primitive, // default
    Instancing,
};

enum class ShaderTechniqueClass_Normal : uint8_t
{
    Default,
    NormalMap,
};

enum class ShaderTechniqueClass_Roughness : uint8_t
{
    Default,
    RoughnessMap,
};


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
	uint32_t layoutLocation;	// GLSL "layout(location=...)"
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
	ShaderUniformType_SamplerState = 12,	// HLSL の SamplerState または samplerXX

    // SamplerState か samplerXX であるかは glslang では区別ができない。

    // https://docs.microsoft.com/en-us/windows/desktop/direct3dhlsl/dx-graphics-hlsl-sampler
    // http://www.kanazawa-net.ne.jp/~pmansato/wpf/wpf_graph_effectsample.htm


	// texture 型 uniform は "COMBINED_IMAGE_SAMPLER" と呼ばれている。
	// Lumino の OpenGL ドライバは UnifiedShader としては CombinedImageSampler は扱わない。
	// OpenGL ドライバ側で、名前一致により検索を行う。
};

struct ShaderUniformInfo
{
    std::string name;
    uint16_t type;		// ShaderUniformType
    //uint16_t ownerBufferIndex;

    uint16_t offset;		// UniformBuffer 先頭からのオフセットバイト数
    //uint16_t size;		// Uniform 1つ分の全体サイズ (配列、行列の分も含む)

    uint16_t vectorElements;
    uint16_t arrayElements;	// 配列要素数。配列で廃場合は 0
    uint16_t matrixRows;
    uint16_t matrixColumns;
    //bool rowMajor;

    //uint16_t arrayStride;		// 配列の場合、要素のバイトサイズ (float2=8, float3=12)
    //uint16_t matrixStride;	// 行列の "横" 方向のバイト数。実際のメモリ上に値を並べる時の行間を表す。(float4x4=16)

    //// 1要素のバイトサイズ
    //size_t elementSize() const
    //{
    //    switch (type)
    //    {
    //    case ln::detail::ShaderUniformType_Unknown:
    //        return 0;
    //    case ln::detail::ShaderUniformType_Bool:
    //        return 1;
    //    case ln::detail::ShaderUniformType_Int:
    //        return 4;
    //    case ln::detail::ShaderUniformType_Float:
    //        return 4;
    //    case ln::detail::ShaderUniformType_Vector:
    //        return vectorElements * 4;
    //    case ln::detail::ShaderUniformType_Matrix:
    //        return matrixRows * matrixColumns * 4;
    //    case ln::detail::ShaderUniformType_Texture:
    //        LN_NOTIMPLEMENTED();
    //        return 0;
    //    default:
    //        LN_UNREACHABLE();
    //        return 0;
    //    }
    //}
};

// TODO: 不要
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

// TODO: name: DescriptorRegisterType の方がいいと思う。
// TextureRegister の中には Texture または StorageBuffer が入る。
enum DescriptorType
{
    DescriptorType_UniformBuffer = 0,
    DescriptorType_Texture = 1, // Texture, 兼 CombinedSampler
    DescriptorType_SamplerState = 2,
    DescriptorType_UnorderdAccess = 3,

    LN_LAST_ELEMENT_MARKER(DescriptorType_Count) = 4,
};

enum ShaderStage2
{
	ShaderStage2_Vertex = 0,
	ShaderStage2_Fragment = 1,
    ShaderStage2_Compute = 2,
    LN_LAST_ELEMENT_MARKER(ShaderStage2_Count) = 3,
};

enum ShaderStageFlags
{
    ShaderStageFlags_None = 0x00,
    ShaderStageFlags_Vertex = 0x01,
    ShaderStageFlags_Pixel = 0x02,
    ShaderStageFlags_Compute = 0x04,
};

struct DescriptorLayoutItem
{
    std::string name;
    uint8_t stageFlags; // ShaderStageFlags どのステージで使われるか
    uint8_t binding;

    // 以下、UniformBuffer でのみ使用
    uint32_t size = 0;		// UniformBuffer 全体のサイズ
    std::vector<ShaderUniformInfo> members;
};

struct DescriptorLayout
{
    // これらの index が binding と一致するわけではないため注意
    std::vector<DescriptorLayoutItem> uniformBufferRegister;// ConstantBuffer
    std::vector<DescriptorLayoutItem> unorderdRegister;     // RWStructuredBuffer
    std::vector<DescriptorLayoutItem> textureRegister;      // Texture or StructuredBuffer
    std::vector<DescriptorLayoutItem> samplerRegister;

    void clear();
	std::vector<DescriptorLayoutItem>& getLayoutItems(DescriptorType registerType);
    const std::vector<DescriptorLayoutItem>& getLayoutItems(DescriptorType registerType) const;
    bool isReferenceFromVertexStage(DescriptorType registerType) const;
    bool isReferenceFromPixelStage(DescriptorType registerType) const;
    bool isReferenceFromComputeStage(DescriptorType registerType) const;
    int findUniformBufferRegisterIndex(const std::string& name) const;
    int findUnorderdRegisterIndex(const std::string& name) const;
    int findTextureRegisterIndex(const std::string& name) const;
    int findSamplerRegisterIndex(const std::string& name) const;
    int findUniformBufferMemberOffset(const std::string& name) const;

    void mergeFrom(const DescriptorLayout& other);
};


} // namespace detail
} // namespace ln
