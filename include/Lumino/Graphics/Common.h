
#pragma once

#include <string>
#include <Lumino/Base/Typedef.h>
#include <Lumino/Base/Array.h>
#include <Lumino/Base/RefObject.h>
#include <Lumino/Base/Rect.h>
#include <Lumino/Base/String.h>
#include <Lumino/Base/EnumExtension.h>
#include <Lumino/Platform/Window.h>
#include <LuminoMath.h>

namespace Lumino
{
namespace Graphics
{
namespace Device
{
class IGraphicsDevice;
class ISwapChain;
class IRenderer;
class IVertexBuffer;
class IIndexBuffer;
class ITexture;
class IShader;
class IShaderVariable;
class IShaderTechnique;
class IShaderPass;

} // namespace Device

class GraphicsManager;
class Texture;

/// グラフィックス API
LN_ENUM(GraphicsAPI)
{
	DirectX9 = 0,	///< DirectX9
	//DIRECTX11,	///< DirectX11
	OpenGL,			///< OpenGL
};
LN_ENUM_REFLECTION(GraphicsAPI, DirectX9, OpenGL);
LN_ENUM_DECLARE(GraphicsAPI);

/// 頂点宣言の要素の型
enum VertexElementType
{
	VertexElementType_Unknown = 0,
	VertexElementType_Float1,		///< float
	VertexElementType_Float2,		///< float[2] (Vector2)
	VertexElementType_Float3,		///< float[3] (Vector3)
	VertexElementType_Float4,		///< float[4] (Vector4)
	VertexElementType_Ubyte4,		///< uint8_t[4]
	VertexElementType_Color4,		///< 32ビット色コード (使用非推奨。DirectX と OpenGL ではバイトオーダが異なる。DXは0xAARRGGBB、GLは0xAABBGGRRで、GLES ではオーダーの変更ができない)
	VertexElementType_Short2,		///< short[2]
	VertexElementType_Short4,		///< short[4]

	VertexElementType_Max,
};

/// 頂点宣言の要素の用途
enum VertexElementUsage
{
	VertexElementUsage_Unknown = 0,
	VertexElementUsage_Position,
	VertexElementUsage_Normal,
	VertexElementUsage_Color,
	VertexElementUsage_TexCoord,
	VertexElementUsage_PointSize,	///< MME との互換性のために残している。
	VertexElementUsage_BlendIndices,
	VertexElementUsage_BlendWeight,

	VertexElementUsage_Max,
};

/// 頂点宣言の1要素
struct VertexElement
{
	uint32_t			StreamIndex;    ///< ストリーム番号 (現在使用していない)
	VertexElementType	Type;			///< 要素の型
	VertexElementUsage	Usage;			///< 要素の用途
	uint8_t				UsageIndex;     ///< (OpenGL で許可しているのは TEXCOORD0～8、PSIZE15 のみ。それ以外は 0)
};

/// デバイスリソースの使用方法
enum DeviceResourceUsage
{
	DeviceResourceUsage_Static = 0,		///< 頻繁に更新を行わないリソース
	DeviceResourceUsage_Dynamic,		///< 頻繁に更新を行うリソース
};

/// インデックスバッファのフォーマット
enum IndexBufferFormat
{
	IndexBufferFormat_UInt16 = 0,
	IndexBufferFormat_UInt32,
};

/// テクスチャのピクセルフォーマット
enum TextureFormat
{
	TextureFormat_Unknown = 0,

	TextureFormat_R8G8B8A8,				///< 32 ビットのアルファ付きフォーマット (GPUネイティブフォーマット。D3D_FMT_A8B8G8R8, DXGI_FORMAT_R8G8B8A8_UNORM)
	TextureFormat_R8G8B8X8,				///< 32 ビットのアルファ無しフォーマット

	TextureFormat_B8G8R8A8,				///< 32 ビットのアルファ付きフォーマット (GDI互換フォーマット。MME 互換のために定義している)
	TextureFormat_B8G8R8X8,				///< 32 ビットのアルファ無しフォーマット
	
	TextureFormat_R16G16B16A16_Float,	///< 64 ビットの浮動小数点フォーマット
	TextureFormat_R32G32B32A32_Float,	///< 128 ビットの浮動小数点フォーマット
	TextureFormat_D24S8,				///< S8 32 ビットの Z バッファフォーマット
	TextureFormat_R16_Float,
	TextureFormat_R32_Float,

	TextureFormat_Max,					///< (terminator)

	/*
		↑の定数のRGBA の並びは、実際のメモリ上のバイトシーケンス。エンディアンは関係ない。

		DX11とOpenGLはGPUネイティブフォーマット (RGBA)
		DX9は、バックバッファはGDI互換フォーマット (BGRA) でなければならない。
		TIFFやPNGは (RGBA)

		DX9は RGBA フォーマットを扱えない。(D3DFMT_ に定義されていない)

	*/
};


/// シェーダプログラムのコンパイル結果の概要
enum ShaderCompileResultLevel
{
	ShaderCompileResultLevel_Success = 0,	///< 成功
	ShaderCompileResultLevel_Warning,		///< 警告が発生している (実行は可能)
	ShaderCompileResultLevel_Error,			///< エラーが発生している
};

/// シェーダプログラムのコンパイル結果
struct ShaderCompileResult
{
	ShaderCompileResultLevel	Level;		///< 結果の概要
	StringA						Message;	///< メッセージ (警告・エラーメッセージが格納される)
};

/// シェーダ変数の型
enum ShaderVariableType
{
	ShaderVariableType_Unknown = 0,
	ShaderVariableType_Bool,
	ShaderVariableType_Int,
	ShaderVariableType_Float,
	ShaderVariableType_Vector,				///< Vector2～4
	ShaderVariableType_VectorArray,
	ShaderVariableType_Matrix,
	ShaderVariableType_MatrixArray,
	ShaderVariableType_Texture,
	ShaderVariableType_String,

	/*
	*	ShaderVariableType_Texture は、DirectX9HLSL では texture 型を表し、GLSL では sampler 型を表す。
	*	GLSL では sampler しか無いが、DirectX9HLSL では texture と sampler の2種類が存在する。
	*/

	ShaderVariableType_Max,				///< (terminator)
};

/// プリミティブの種類
enum PrimitiveType
{
	PrimitiveType_TriangleList = 1,
	PrimitiveType_TriangleStrip,
	PrimitiveType_LineList,
	PrimitiveType_LineStrip,
	PrimitiveType_PointList,

	PrimitiveType_Max,				///< (terminator)
};

/// デバイスリソースのロック方法
enum LockMode
{
	LockMode_Write = 0,		///< 書き込み
	LockMode_Read,			///< 読み込み
};

/// 描画方式
enum RenderingType
{
	RenderingType_Immediate	= 0,	///< 
	RenderingType_Deferred,
};


/// 3D 空間での基準方向を表す値
enum AxisDirection
{
	AxisDirection_X = 0,   ///< X+ 方向 (右向き)
	AxisDirection_Y,       ///< Y+ 方向 (上向き)
	AxisDirection_Z,       ///< Z+ 方向 (奥向き)
	AxisDirection_RX,      ///< X- 方向 (左向き)
	AxisDirection_RY,      ///< Y- 方向 (下向き)
	AxisDirection_RZ,      ///< Z- 方向 (手前向き)

	AxisDirection_MAX,
};

/// オブジェクトのソートの基準
enum SortingDistanceBasis
{
	SortingDistanceBasis_RawZ = 0,		///< オブジェクトの Z 値
	SortingDistanceBasis_ViewPont,		///< オブジェクトの位置と視点との距離
};

/// スプライトのソート方法
enum SpriteSortMode
{
	SpriteSortMode_None = 0x00,					///< ソートしない
	SpriteSortMode_DepthBackToFront = 0x01,		///< Z値が大きいものが先に描画されるようにソートする (アルファブレンド使用時の推奨)
	SpriteSortMode_DepthFrontToBack = 0x02,		///< Z値が小さいものが先に描画されるようにソートする (SpriteSortMode_DepthBackToFront と同時には使えない)
	SpriteSortMode_Texture = 0x04,				///< テクスチャを優先してソートする (同じテクスチャを持つスプライトが多数あるとき用。ただし、アルファブレンドが有効な場合は描画が不自然になることがある)
};

/// テキストの配置方法
LN_ENUM(TextAlignment)
{
	Left = 0,				///< 左揃え
	Center,					///< 中央揃え
	Right,					///< 右揃え
	Justify,				///< 両端揃え
};
LN_ENUM_DECLARE(TextAlignment)

/// 描画領域にテキストが収まりきらない場合に、テキストを切り取る方法
LN_ENUM(TextTrimming)
{
	None = 0,				///< 切り取りを行わない。
	CharacterEllipsis,		///< 略記号(...) を描画する。
};
LN_ENUM_DECLARE(TextTrimming)

/// テキストの描画方向
LN_ENUM(FlowDirection)
{
	LeftToRight = 0,		///< テキストを左から右に描画する。
	RightToLeft,			///< テキストを左から右に描画する。
};
LN_ENUM_DECLARE(FlowDirection)


} // namespace Graphics
} // namespace Lumino

