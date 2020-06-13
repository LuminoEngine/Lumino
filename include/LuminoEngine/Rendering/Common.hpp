#pragma once
#include <LuminoEngine/Graphics/GeometryStructs.hpp>
#include <LuminoEngine/Graphics/ColorStructs.hpp>
#include <LuminoEngine/Graphics/Common.hpp>

#define LN_BOX_ELEMENT_RENDER_FEATURE_TEST 1

namespace ln {

// Note: ShadingModel は組み込みの SurfaceShader を選択するもの。Unlit にしても陰はつく。
enum class ShadingModel : uint8_t
{
	Default,
	Unlit,
};

/** 合成方法 */
enum class BlendMode : uint8_t
{
	Normal,			/**< 通常 */
	Alpha,			/**< アルファブレンド (RGB をアルファブレンドし、A を加算合成) */
	Add,			/**< 加算合成 */
	Subtract,		/**< 減算合成 */
	Multiply,		/**< 乗算合成 */
};

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

/** ビルボードの計算方法 */
enum class BillboardType
{
	None,					/**< ビルボードの計算を行わない */
	ToCameraPoint,			/**< カメラ (ビュー行列) に対して正面を向く */
	ToScreen,				/**< スクリーン (ビュー平面) に対して正面を向く */
	RotY,					/**< Y 軸回転のみ行う */
};

/** ライトの種類 */
enum class LightType
{
	/** アンビエントライト */
	Ambient,

	/** 半球ライト */
	Hemisphere,

	/** ディレクショナルライト */
	Directional,

	/** ポイントライト */
	Point,

	/** スポットライト */
	Spot,
};

enum class RenderViewClearMode
{
	None,
	ColorAndDepth,
	Sky,
	Sky0,
};

// DrawElement の大分類。SceneRenderer に投入する DrawElement を決める。
// SceneRenderer 内の各 RenderPass は、さらに小項目を RenderDrawElementTypeFlags で識別し、描画するかどうかを決定する。
enum class RenderPhaseClass
{
	// 通常のオブジェクトの他、BackgroundSky, LightDisc もこれに含まれる。
    Geometry = 0,

	// Geometry のライティングに関係せず、前面に描画されるもの。
    Gizmo,

	// スクリーン全体にオーバーレイ描画されるもの。Zソートなど一部の不要な工程が省略される。
    ImageEffect,


    _Count,
};


//
//struct TextureRegion
//{
//public:
//    Ref<Texture> texture;
//    Rect rect;
//    Thickness border;
//};

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


struct FrameBuffer
{
	Ref<RenderTargetTexture> renderTarget[detail::MaxMultiRenderTargets];
	Ref<DepthBuffer> depthBuffer;
};


enum class RequestBatchResult
{
	Staging,	// request はステージされ、まだ Batch 化されていない
	Submitted,	// 直前までの request は submit され、List に新しい Batch が追加された。最新の request はステージされ、まだ Batch 化されていない。
				// なお、State が変わったため新しい Batch を作りたいとき、Batch の中身が 0 であるときは作ってはならない。
};

struct ClearInfo
{
    ClearFlags flags = ClearFlags::None;
    Color color;
    float depth = 1.0f;
    uint8_t stencil = 0x00;
};



// 必須情報。border の情報が含まれているが、Shadow を正しく描画するために必要となる
struct BoxElementShapeBaseStyle
{
    Matrix transform;
    Rect baseRect;
    CornerRadius cornerRadius;
    bool aligndLineAntiAlias = false;	// 軸に平行な辺にも AA を適用するか (回転させたい場合に使用)
};

struct BoxElementShapeBackgroundStyle
{
    Color color;
};

struct BoxElementShapeBorderStyle
{
    Color borderLeftColor;
    Color borderTopColor;
    Color borderRightColor;
    Color borderBottomColor;
    Thickness borderThickness;
    bool borderInset = false;
};

struct BoxElementShapeShadowStyle
{
	/** シャドウのオフセット距離です (シャドウの生成処理は、オフセットが BoxElement から大きく離れないことを前提として最適化されています。オフセットは shadowWidth を超えないようにしてください) */
    Vector2 shadowOffset;

    Color shadowColor;
    float shadowWidth = 0.0f;
    float shadowBlur = 0.0f;
    bool shadowInset = false;
};

enum class GBuffer
{
	ViewNormalMap,	// experiment
	ViewDepthMap,
	ViewMaterialMap,
};

namespace detail {
class SceneRenderer;
class SceneRenderingPipeline;

enum class RenderDrawElementTypeFlags : uint8_t
{
	None = 0,

	// 内部で自動決定
	Clear = 1 << 1,			// clear など、ポリゴンを描画しないが、レンダーターゲットを変更する
	Opaque = 1 << 2,		// 不透明
	Transparent = 1 << 3,	// 半透明

	// 外部指定
	LightDisc = 1 << 4,	// 半透明オブジェクトの後ろにあるライトもレイ引いたりレンズフレアかけたりしたいので、ElementList に含め、Zソートなどの対象として描画する
	BackgroundSky = 1 << 5,
};
LN_FLAGS_OPERATORS(RenderDrawElementTypeFlags);


enum class SpriteFlipFlags : uint8_t
{
    None = 0,
    FlipX = 1 << 1,
    FlipY = 1 << 2,
};
LN_FLAGS_OPERATORS(SpriteFlipFlags);

struct DynamicLightInfo
{
	DynamicLightInfo()
		: m_type(LightType::Ambient)
		, m_color(Color::White)
		, m_color2(Color::White)
		//, m_ambient(Color::White)
		, m_specular(Color::White)
		, m_position(0, 0, 0)
		, m_direction(0, -1, 0)
		, m_intensity(1.0f)
		, m_range(10.0f)
		, m_attenuation(1.0f)
		, m_spotAngle(Math::PI / 3)
		, m_spotPenumbra(0.1f)
		, castShadow(false)
		//, m_shadowCasterPass(nullptr)
	{}

	LightType	m_type;				// ライトの種類

	// 基本色 (Phong ではディフーズカラー)
	// [Ambient] ambient color
	// [Hemisphere] sky color
	Color		m_color;

	// [Hemisphere] ground color
	// [Directional, Point, Spot] (Phong shading でのみ使用) アンビエントカラー
	Color		m_color2;		

	//Color		m_ambient;			// (Phong shading でのみ使用) アンビエントカラー
	Color		m_specular;			// (Phong shading でのみ使用) スペキュラカラー
	Vector3		m_position;			// [Point, Spot]
	Vector3		m_direction;		// [Directional, Spot]向き
	float		m_intensity;		// [All] 光の強さ (Shader uniform にセットする前に、m_color と乗算する)
	float		m_range;			// [Point, Spot] 減衰
	float		m_attenuation;		// [Point, Spot] 減衰
	float		m_spotAngle;		// [Spot]
	float		m_spotPenumbra;		// [Spot]

	bool castShadow;	// [Directional, Point, Spot]
	//float		m_shadowZFar;

	//detail::ShadowCasterPass*	m_shadowCasterPass;
	//Matrix		transform;

	//float		tempDistance;		// 作業用変数

	//static const int MaxLights = 3;		// MMD based

	bool mainLight = false;

	static DynamicLightInfo makeAmbientLightInfo(const Color& color, float intensity)
	{
		DynamicLightInfo info;
		info.m_type = LightType::Ambient;
		info.m_color = color;
		info.m_intensity = intensity;
		return info;
	}

	static DynamicLightInfo makeHemisphereLightInfo(const Color& skyColor, const Color& groundColor, float intensity)
	{
		DynamicLightInfo info;
		info.m_type = LightType::Hemisphere;
		info.m_color = skyColor;
		info.m_color2 = groundColor;
		info.m_intensity = intensity;
		return info;
	}

	static DynamicLightInfo makeDirectionalLightInfo(const Color& color, float intensity, const Vector3& direction, bool mainLight)
	{
		DynamicLightInfo info;
		info.m_type = LightType::Directional;
		info.m_color = color;
		info.m_direction = direction;
		info.m_intensity = intensity;
		info.mainLight = mainLight;
		return info;
	}

	static DynamicLightInfo makePointLightInfo(const Color& color, float intensity, const Vector3& position, float range, float attenuation)
	{
		DynamicLightInfo info;
		info.m_type = LightType::Point;
		info.m_color = color;
		info.m_position = position;
		info.m_intensity = intensity;
		info.m_range = range;
		info.m_attenuation = attenuation;
		return info;
	}

	static DynamicLightInfo makeSpotLightInfo(const Color& color, float intensity, const Vector3& position, const Vector3& direction, float range, float attenuation, float spotAngle, float spotPenumbra)
	{
		DynamicLightInfo info;
		info.m_type = LightType::Spot;
		info.m_color = color;
		info.m_position = position;
		info.m_direction = direction;
		info.m_intensity = intensity;
		info.m_range = range;
		info.m_attenuation = attenuation;
		info.m_spotAngle = spotAngle;
		info.m_spotPenumbra = spotPenumbra;
		return info;
	}
};

struct SceneGlobalRenderParams
{
	//Color ambientColor;
	//Color ambientSkyColor;
	//Color ambientGroundColor;

	float lowerHeight;
	float upperHeight;
	float startDistance;
	Color fogColor;
	float fogDensity = 0.0f;
	float heightFogDensity = 0.0f;
};

} // namespace detail
} // namespace ln
