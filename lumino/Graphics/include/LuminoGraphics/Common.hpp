// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once
#include <LuminoEngine/Base/Promise.hpp>
#include <LuminoShaderCompiler/Common.hpp>
#include <LuminoBitmap/Common.hpp>

namespace ln {
class IGraphicsResource;
class GraphicsContext;
class GraphicsCommandList;
class SamplerState;
class SwapChain;
class RenderPass;
class Bitmap2D;
class Bitmap3D;
class Texture;
class Texture2D;
class RenderTargetTexture;
class DepthBuffer;
class ConstantBuffer;
class VertexLayout;
class VertexBuffer;
class IndexBuffer;
class INativeGraphicsExtension;

/**
 * Texture2DDelegate
 */
LN_DELEGATE()
using Texture2DDelegate = Delegate<void(Ref<Texture2D> value)>;

/**
 * Texture2DPromise
 */
LN_PROMISE()
using Texture2DPromise = Promise<Ref<Texture2D>>;


///** 描画方式 */
//enum class RenderingType
//{
//	/** 即時描画 */
//	Immediate,
//
//	/** 遅延描画 */
//	Threaded,
//};

/** バックエンドグラフィックス API の種類 */
LN_ENUM()
enum class GraphicsAPI
{
    /** 自動選択 */
    Default,

    /** OpenGL */
    OpenGL,

    /** Vulkan */
    Vulkan,

    DirectX12,
};

/** レンダーターゲットのクリア方法 */
enum class ClearFlags
{
    /** 指定なし */
    None = 0x0000,

    /** カラーバッファをクリアします。 */
    Color = 0x0001,

    /** 深度バッファをクリアします。 */
    Depth = 0x0002,

    /** ステンシルバッファをクリアします。 */
    Stencil = 0x0004,

    /** 全てのバッファをクリアします。 */
    All = Color | Depth | Stencil,
};
LN_FLAGS_OPERATORS(ClearFlags);

/** グラフィックスリソースの使用方法 */
enum class GraphicsResourceUsage : uint8_t
{
    /** 頻繁に更新を行わないリソース */
    Static,

    /** 頻繁に更新を行うリソース */
    Dynamic,
};

/** グラフィックスリソースの管理方法 */
enum class GraphicsResourcePool
{
    /** デバイス変更時に内容を復元しません。*/
    None,

    /** デバイス変更時に内容を復元します。*/
    Managed,
};

/** グラフィックスリソースのマップ方法 */
enum class MapMode
{
    /** 読み取りモード */
    Read,

    /** 書き込みモード */
    Write,
};

/** 頂点宣言の要素の型 */
enum class VertexElementType : uint8_t
{
    /** Unknown */
    Unknown,

    /** float */
    Float1,

    /** float[2] (Vector2) */
    Float2,

    /** float[3] (Vector3) */
    Float3,

    /** float[4] (Vector4) */
    Float4,

    /** uint8_t[4] */
    Ubyte4,

    /** (obsolete) 32ビット色コード (使用非推奨。DirectX と OpenGL ではバイトオーダが異なる。DXは0xAARRGGBB、GLは0xAABBGGRRで、GLES ではオーダーの変更ができない) */
    Color4,

    /** short[2] */
    Short2,

    /** short[4] */
    Short4,
};

/** 頂点宣言の要素の用途 */
enum class VertexElementUsage
{
    /** Unknown (指定なし) */
    Unknown,

    /** 頂点位置 */
    Position,

    /** 法線 */
    Normal,

    /** 頂点カラー */
    Color,

    /** テクスチャ座標 */
    TexCoord,

    /** (obsolete) ポイントスプライトサイズ */
    PointSize,

	Tangent,

	Binormal,

    /** ブレンディング (スキニング) のインデックス */
    BlendIndices,

    /** ブレンディング (スキニング) のウェイト */
    BlendWeight,

    InstanceID,
};

/** 頂点バッファのレート */
enum class VertexInputRate
{
    /** 頂点バッファを頂点データのストリームとして扱う。 */
    Vertex,

    /** 頂点バッファをインスタンスデータのストリームとして扱う。 */
    Instance,
};

/** 頂点宣言の1要素 */
struct VertexElement
{
    /** ストリーム番号 */
    uint32_t StreamIndex;

    /** 要素の型 */
    VertexElementType Type;

    /** 要素の用途 */
    VertexElementUsage Usage;

    /** UsageIndex */
    uint32_t UsageIndex;

    VertexInputRate rate = VertexInputRate::Vertex;
};

/** インデックスバッファのフォーマット */
enum class IndexBufferFormat
{
    /** 16 bit */
    UInt16,

    /** 32 bit */
    UInt32,
};

/** テクスチャのピクセルフォーマット */
LN_ENUM()
enum class TextureFormat : uint8_t
{
    /** Unknown */
    Unknown,

    /** RGBA オーダーの各要素 8bit フォーマット */
    RGBA8,

    /** RGB オーダーの各要素 8bit フォーマット */
    RGB8,

    /** RGBA オーダーの各要素 16bit 浮動小数点フォーマット */
    RGBA16F,

    /** RGBA オーダーの各要素 32bit 浮動小数点フォーマット */
    RGBA32F,

    /** 16bit 浮動小数点フォーマット */
    R16F,

    /** 32bit 浮動小数点フォーマット */
    R32F,

    /** 32bit の符号あり整数フォーマット */
    R32S,
};

/** 深度バッファのフォーマット */
LN_ENUM()
enum class DepthBufferFormat
{
    /** S8 32 ビット*/
    D24S8,
};

/** テクスチャフィルタ */
enum class TextureFilterMode
{
    /** 補間を行わない*/
    Point,

    /** 線形補間を行う */
    Linear,
};

/** テクスチャアドレッシング */
enum class TextureAddressMode
{
    /** 繰り返し */
    Repeat,

    /** 境界のピクセル */
    Clamp,
};

/** 描画プリミティブの種類 */
enum class PrimitiveTopology : uint8_t
{
    /** 独立した三角形のリスト */
    TriangleList,

    /** 連続した三角形のリスト */
    TriangleStrip,

    /** (obsolete) */
    TriangleFan,

    /** 独立した線のリスト */
    LineList,

    /** 連続する線のリスト */
    LineStrip,

    /** 点のリスト */
    PointList,
};

enum class ShaderCompilationResultLevel
{
    Success,
    Warning,
    Error,
};

struct ShaderCompilationDiag
{
    ShaderCompilationResultLevel level;
    std::string message;
};

enum class ShaderVariableType
{
    Unknown,
    Bool,
    BoolArray,
    Int,
    Float,
    FloatArray,
    Vector,
    VectorArray,
    Matrix,
    MatrixArray,
    Texture,
    Pointer,

    /*
     *    Texture は、DirectX9HLSL では texture 型を表し、GLSL では sampler 型を表す。
     *    GLSL では sampler しか無いが、DirectX9HLSL では texture と sampler の2種類が存在する。
     */
};

class GraphicsHelper
{
public:
    static size_t getVertexSize(const VertexElement* vertexElements, int elementsCount, int streamIndex);
    static size_t getVertexElementTypeSize(VertexElementType type);
    static PixelFormat translateToPixelFormat(TextureFormat format);
    static TextureFormat translateToTextureFormat(PixelFormat format);
    static size_t getPixelSize(TextureFormat format);
    static IndexBufferFormat selectIndexBufferFormat(int vertexCount) { return (vertexCount > 0xFFFF) ? IndexBufferFormat::UInt32 : IndexBufferFormat::UInt16; }
    static size_t getIndexStride(IndexBufferFormat format) { return (format == IndexBufferFormat::UInt16) ? 2 : 4; }



    static const Char* CandidateExts_Texture2D[5];
};

namespace detail {
class AssetManager;
class RenderingCommandList;
class GraphicsManager;
class GraphicsContextInternal;
class IGraphicsDevice;
class ICommandList;
class ISamplerState;
class ShaderSecondaryDescriptor;
class GraphicsProfiler;
class RHIResource;

static const int MaxMultiRenderTargets = 4;

// max=16 : https://docs.microsoft.com/en-us/windows/win32/api/d3d12/ns-d3d12-d3d12_input_element_desc
static const int MaxVertexStreams = 16;

static const int MaxDescriptors = 16;

struct SamplerStateData
{
    TextureFilterMode filter;
    TextureAddressMode address;
    bool anisotropy;

    static const SamplerStateData defaultState;
};

using ShaderRefrectionParameterType = ShaderVariableType;
//enum class ShaderRefrectionParameterType
//{
//    Unknown,
//    Bool,
//    BoolArray,
//    Int,
//    Float,
//    FloatArray,
//    Vector,
//    VectorArray,
//    Matrix,
//    MatrixArray,
//    Texture,
//};

// rows, columns はデータレイアウトとしての領域サイズ。
struct ShaderUniformTypeDesc
{
    ShaderUniformType type2;
    int rows;
    int columns;
    int elements; // 配列要素数。glslang から取り出せるのは 実際に使われているサイズ。float values[3] だけれどシェーダ内で value[1] しか使っていなければ elements=2 になる

    size_t offset;
    //size_t size;
    size_t arrayStride; // 1要素のバイトサイズ
    size_t matrixStride;

    //size_t aligndElementSize;   // アライメントが考慮された配列1要素分のサイズ

    static bool equals(const ShaderUniformTypeDesc& a, const ShaderUniformTypeDesc& b)
    {
        return a.type2 == b.type2 &&
               a.rows == b.rows &&
               a.columns == b.columns &&
               a.elements == b.elements;
    }

    bool isArray() const { return elements > 0; }
};

struct ConstantBufferView
{
    ConstantBuffer* buffer;
    size_t offset;
    void setData(const void* data, size_t size);
    void* writableData();
    void clear() { buffer = nullptr; offset = 0; }
};

} // namespace detail
} // namespace ln
