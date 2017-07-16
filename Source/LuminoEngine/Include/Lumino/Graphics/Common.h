
#pragma once

#include <string>
#include <Lumino/Base/Typedef.h>
#include <Lumino/Base/List.h>
#include <Lumino/Base/RefObject.h>
#include <Lumino/Base/String.h>
#include <Lumino/Base/EnumExtension.h>
#include <Lumino/Reflection/ReflectionObject.h>
#include <Lumino/Reflection/Property.h>
#include <LuminoMath.h>
#include "../Common.h"
#include "../Base/GeometryStructs.h"

#define LN_NAMESPACE_GRAPHICS_BEGIN		//namespace Graphics {
#define LN_NAMESPACE_GRAPHICS_END		//}

#define LN_BEGIN_INTERNAL_NAMESPACE(module)	LN_NAMESPACE_BEGIN /*namespace module {*/ namespace Internal {
#define LN_END_INTERNAL_NAMESPACE	} /*}*/ LN_NAMESPACE_END

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN



/** テクスチャのピクセルフォーマット */
LN_ENUM_()
enum class TextureFormat
{
	/** Unknown */
	Unknown = 0,

	/** 32 ビットのアルファ付きフォーマット (GPUネイティブフォーマット。D3D_FMT_A8B8G8R8, DXGI_FORMAT_R8G8B8A8_UNORM) */
	R8G8B8A8,

	/** 32 ビットのアルファ無しフォーマット */
	R8G8B8X8,

	/** 32 ビットのアルファ付きフォーマット (GDI互換フォーマット。MME 互換のために定義している) */
	B8G8R8A8,

	/** 32 ビットのアルファ無しフォーマット */
	B8G8R8X8,

	/** 64 ビットの浮動小数点フォーマット */
	R16G16B16A16_Float,

	/** 128 ビットの浮動小数点フォーマット */
	R32G32B32A32_Float,

	/** 16 ビットの浮動小数点フォーマット */
	R16_Float,

	/** 32 ビットの浮動小数点フォーマット */
	R32_Float,

	/** S8 32 ビットの深度バッファフォーマット */
	D24S8,

	//_Count,					///< (terminator)

};


namespace detail { class GraphicsManager; }

class RenderState;
class DepthStencilState;
class SamplerState;
class ShaderValue;

struct Color32;
struct Color;

class GraphicsResourceObject;
class VertexDeclaration;
class VertexBuffer;
class IndexBuffer;
class Texture;
class Texture2D;
typedef Ref<Texture2D>		Texture2DPtr;
class RenderTargetTexture;
typedef Ref<RenderTargetTexture>	RenderTargetTexturePtr;
class DepthBuffer;
class Shader;
class ShaderVariable;
class ShaderTechnique;
class ShaderPass;

class GlyphRun;

class Helper;

class Brush;
typedef Ref<Brush>				BrushPtr;

class ScreenMotionBlurImageEffect;
typedef Ref<ScreenMotionBlurImageEffect>	ScreenMotionBlurImageEffectPtr;
class ToneImageEffect;
typedef Ref<ToneImageEffect>	ToneImageEffectPtr;

class ImageEffect;
typedef tr::ReflectionObjectList<ImageEffect*>	ImageEffectList;

struct TextLayoutResult;


namespace detail
{

class CoreGraphicsRenderFeature;
class RenderingCommandList;
class RenderingThread;

} // namespace detail

/// グラフィックス API
LN_ENUM(GraphicsAPI)
{
	DirectX9 = 0,	///< DirectX9
	//DIRECTX11,	///< DirectX11
	OpenGL,			///< OpenGL
};
LN_ENUM_REFLECTION(GraphicsAPI, DirectX9, OpenGL);
LN_ENUM_DECLARE(GraphicsAPI);

LN_ENUM_FLAGS(ClearFlags)
{
	None	= 0x0000,
	Color	= 0x0001,					///< カラーバッファをクリアします。
	Depth	= 0x0002,					///< 深度バッファをクリアします。
	Stencil = 0x0004,					///< ステンシルバッファをクリアします。
	All		= Color | Depth | Stencil,	///< 全てのバッファをクリアします。
};
LN_ENUM_FLAGS_DECLARE(ClearFlags);

/** グラフィックスリソースの管理方法 */
enum class GraphicsResourcePool
{
	None = 0,		/**< デバイス変更時に内容を復元しません。*/
	Managed,		/**< デバイス変更時に内容を復元します。*/
};


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

/**< リソースの使用方法 */
enum class ResourceUsage : uint8_t
{
	Static = 0,			/**< 頻繁に更新を行わないリソース */
	Dynamic,			/**< 頻繁に更新を行うリソース */
};

/// インデックスバッファのフォーマット
enum IndexBufferFormat
{
	IndexBufferFormat_UInt16 = 0,	// TODO: Index16
	IndexBufferFormat_UInt32,
};

/*
	↑の定数のRGBA の並びは、実際のメモリ上のバイトシーケンス。エンディアンは関係ない。

	DX11とOpenGLはGPUネイティブフォーマット (RGBA)
	DX9は、バックバッファはGDI互換フォーマット (BGRA) でなければならない。
	TIFFやPNGは (RGBA)

	DX9は RGBA フォーマットを扱えない。(D3DFMT_ に定義されていない)
*/


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
	ShaderVariableType_BoolArray,
	ShaderVariableType_Int,
	ShaderVariableType_Float,
	ShaderVariableType_FloatArray,
	ShaderVariableType_Vector,				///< Vector2～4
	ShaderVariableType_VectorArray,
	ShaderVariableType_Matrix,
	ShaderVariableType_MatrixArray,
	ShaderVariableType_DeviceTexture,
	ShaderVariableType_ManagedTexture,
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
	PrimitiveType_TriangleFan,
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

/** 描画方式 */
LN_ENUM(GraphicsRenderingType)
{
	Immediate,			/**< 即時描画 */
	Threaded,			/**< 遅延描画 */
};
LN_ENUM_REFLECTION(GraphicsRenderingType, Immediate, Threaded);
LN_ENUM_DECLARE(GraphicsRenderingType)


/** 3D 空間での基準方向を表す値 */
enum class SpriteBaseDirection
{
	XPlus,			/**< X+ 方向 (右向き) */
	YPlus,			/**< Y+ 方向 (上向き) */
	ZPlus,			/**< Z+ 方向 (奥向き) */
	XMinus,			/**< X- 方向 (左向き) */
	YMinus,			/**< Y- 方向 (下向き) */
	ZMinus,			/**< Z- 方向 (手前向き) */
	Basic2D,		/**< Z+ 方向、左上原点 */
};

/// オブジェクトのソートの基準
enum SortingDistanceBasis
{
	SortingDistanceBasis_RawZ = 0,		///< オブジェクトの Z 値
	SortingDistanceBasis_ViewPont,		///< オブジェクトの位置と視点との距離
};

/** スプライトのソート方法 */
LN_ENUM_FLAGS(SpriteSortMode)
{
	None				= 0x00,			/**< ソートしない */
	DepthBackToFront	= 0x01,			/**< Z値が大きいものが先に描画されるようにソートする (アルファブレンド使用時の推奨) */
	DepthFrontToBack	= 0x02,			/**< Z値が小さいものが先に描画されるようにソートする (SpriteSortMode_DepthBackToFront と同時には使えない) */
	Texture				= 0x04,			/**< テクスチャを優先してソートする (同じテクスチャを持つスプライトが多数あるとき用。ただし、アルファブレンドが有効な場合は描画が不自然になることがある) */
};
LN_ENUM_FLAGS_DECLARE(SpriteSortMode);

/** ビルボードの計算方法 */
enum class BillboardType
{
	None,					/**< ビルボードの計算を行わない */
	ToCameraPoint,			/**< カメラ (ビュー行列) に対して正面を向く */
	ToScreen,				/**<  */
	RotY,					/**< Y 軸回転のみ行う */
};

enum class TextLayoutOptions
{
	None				= 0x0000,
	Baselining			= 0x0001,
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
	TopToBottom,			///< テキストを上から下に描画する。 (試験実装)
};
LN_ENUM_DECLARE(FlowDirection)

LN_ENUM_FLAGS(StringFormatFlags)	// TODO: LayoutFlags のほうがいいかな？
{
	LeftAlignment = 0x0001,
	CenterAlignment = 0x0002,
	RightAlignment = 0x0004,

	Default = LeftAlignment,
};
LN_ENUM_FLAGS_DECLARE(StringFormatFlags);

// ライトの種類
enum LightType
{
	LightType_Directional = 0,	// ディレクショナルライト
	LightType_Point,			// ポイントライト
	LightType_Spot,				// スポットライト
};

enum class BuiltinShader
{
	Sprite,
	LegacyDiffuse,		// Lambert Shading
};

enum class BorderDirection
{
	Inside,
	Outside,
};

enum class ShadowDirection
{
	Inside,
	Outside,
};

namespace Driver
{
	class IGraphicsDevice;
	class ISwapChain;
	class IRenderer;
	class IVertexDeclaration;
	class IVertexBuffer;
	class IIndexBuffer;
	class ITexture;
	class IShader;
	class IShaderVariable;
	class IShaderTechnique;
	class IShaderPass;

} // namespace Driver

namespace detail {

class FontData
{
public:

	String	Family;
	int		Size;
	//int		EdgeSize;
	bool	isBold;
	bool	isItalic;
	bool	isAntiAlias;

	FontData();
	bool operator < (const FontData& right);
	uint64_t calcHash() const;
};


template<typename T>
class CacheBuffer
{
public:
	CacheBuffer()
		: m_buffer()
		, m_capacity(0)
		, m_count(0)
	{
	}

	void reserve(int count)
	{
		m_buffer.resize(sizeof(T) * count, false);
		m_capacity = count;
	}

	void add(const T& value)
	{
		tryGlow(m_count + 1);
		memcpy(&m_buffer[sizeof(T) * m_count], &value, sizeof(T));
		m_count++;
	}

	T* request(int count)
	{
		tryGlow(m_count + count);
		size_t begin = getBufferUsedByteCount();
		m_count += count;
		return reinterpret_cast<T*>(m_buffer.getData() + begin);
	}

	void clear()
	{
		m_count = 0;
	}

	T& getAt(int index) { return ((T*)(m_buffer.getData()))[index]; }
	T& getLast() { return getAt(m_count - 1); }

	int getCount() const { return m_count; }
	byte_t* getBuffer() { return m_buffer.getData(); }
	size_t getBufferUsedByteCount() { return m_count * sizeof(T); }

private:
	void tryGlow(int requestCount)
	{
		if (/*m_count + */requestCount > m_capacity)
		{
			reserve(m_capacity * 2);
		}
	}

	ByteBuffer	m_buffer;
	int			m_capacity;
	int			m_count;
};

class CommandDataCache
	: public RefObject
{
public:
	using DataHandle = size_t;

	CommandDataCache();
	virtual ~CommandDataCache();

	void reserve(size_t dataCount, size_t byteCount);
	void clear();
	DataHandle allocData(size_t byteCount, const void* data = nullptr);
	byte_t* getData(DataHandle handle);

	int getDataCount() const { return m_dataList.getCount(); }
	byte_t* getDataByIndex(int index) { return getData(m_dataList[index]); }

private:
	List<DataHandle>		m_dataList;
	ByteBuffer				m_dataBuffer;
	size_t					m_dataBufferUsed;
};

} // namespace detail
LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END

