#pragma once
#include <LuminoEngine/Graphics/GeometryStructs.hpp>
#include <LuminoEngine/Graphics/ColorStructs.hpp>
#include <LuminoEngine/Graphics/Common.hpp>

namespace ln {

enum class ShadingModel : uint8_t
{
	Default,
	UnLighting,
};

/** 合成方法 */
enum class BlendMode : uint8_t
{
	Normal,			/**< 通常 */
	Alpha,			/**< アルファブレンド */
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
};

// 
// https://docs.unity3d.com/ja/current/Manual/GraphicsCommandBuffers.html
// の各〇に相当する。
// ただし、Default は特殊扱い。DrawElementList が投入された SceneRenderer の中でフィルタリングがかかる。
// 例えば(今後実装するかもしれない Deferred Shading では)、
//  - Default の半透明オブジェクトは ForwardSceneRenderer でのみ描画される。
//  - Default の不透明オブジェクトは DeferredSceneRenderer でのみ描画される。
//  - Default のオブジェクトは AfterImageEffects などでは描画されない。
// などなど。
// 一方 Default 以外のものはブレンド有無などの RenderState にかかわらず必ずそのタイミングで描画される。そのへんはユーザー責任で考える。
enum class RendringPhase
{
    Default = 0,

    // 不透明オブジェクト描画後・半透明オブジェクト描画前
    BeforeTransparencies,

    ImageEffect,

    // https://docs.unity3d.com/ja/2017.4/ScriptReference/Rendering.CameraEvent.html

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

namespace detail {


enum class RenderDrawElementType
{
	Geometry,	// Material を用いてポリゴンを描画する
	Clear,		// clear など、ポリゴンを描画しないが、レンダーターゲットを変更する
};


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

	static DynamicLightInfo makeDirectionalLightInfo(const Color& color, float intensity, const Vector3& direction)
	{
		DynamicLightInfo info;
		info.m_type = LightType::Directional;
		info.m_color = color;
		info.m_direction = direction;
		info.m_intensity = intensity;
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

} // namespace detail
} // namespace ln
