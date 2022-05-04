#pragma once
#include "../RHI/Common.hpp"
#include <LuminoEngine/Graphics/ColorStructs.hpp>
#include <LuminoEngine/Graphics/Common.hpp>
#include <LuminoGraphics/Common.hpp>
#include "../RHI/ShaderInterfaceFramework.hpp"

#define LN_BOX_ELEMENT_RENDER_FEATURE_TEST 1
#define LN_RLI_BATCH 1
#define LN_USE_KANATA 1

namespace ln {
class Font;
class VertexLayout;
class VertexBuffer;
class Texture;
class RenderTargetTexture;
class DepthBuffer;
class Material;
class RenderingContext;
class MeshResource;
class MeshPrimitive;
class MeshContainer;
class MeshSkeleton;
class RenderViewPoint;
class RenderView;
class PostEffect;
class InstancedMeshList;
class CommandList;
class CanvasContext;

class SpriteRenderer;
class PrimitiveMeshRenderer;

class RenderingPipeline;
class SceneRenderingPipeline;
class FlatRenderingPipeline;

static const int MaxRenderMorphTargets = 4;

/** ShadingModel */
LN_ENUM()
enum class ShadingModel : uint8_t {
    /** Default */
    Default,

    /** Unlit */
    Unlit,
};
// NOTE: Lumino の ShadingModel は UE を参考にはしているが、あまり大げさなものではない。
// 動機は、Default ⇔ Unlit などを動的に簡単に切り替えられるようにするためのもの。
// Unlit 用のシェーダを用意してそれを Material にアタッチすることでも同様の表現はできるようになる。(Unity 方式)
// DX9 の頃の ShaderFragment のイメージが近いかもしれない。

/** 合成方法 */
LN_ENUM()
enum class BlendMode : uint8_t {
    /** 通常 */
    Normal,

    /** アルファブレンド (RGB をアルファブレンドし、A を加算合成) */
    Alpha,

    /** 加算合成 */
    Add,

    /** 減算合成 */
    Subtract,

    /** 乗算合成 */
    Multiply,
};

/** 3D 空間での基準方向を表す値 */
enum class SpriteBaseDirection {
    XPlus,   /**< X+ 方向 (右向き) */
    YPlus,   /**< Y+ 方向 (上向き) */
    ZPlus,   /**< Z+ 方向 (奥向き) */
    XMinus,  /**< X- 方向 (左向き) */
    YMinus,  /**< Y- 方向 (下向き) */
    ZMinus,  /**< Z- 方向 (手前向き) */
    Basic2D, /**< Z+ 方向、左上原点 */
};

/** ビルボードの計算方法 */
enum class BillboardType {
    None,          /**< ビルボードの計算を行わない */
    ToCameraPoint, /**< カメラ (ビュー行列) に対して正面を向く */
    ToScreen,      /**< スクリーン (ビュー平面) に対して正面を向く */
    RotY,          /**< Y 軸回転のみ行う */
};

/** ライトの種類 */
enum class LightType {
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

LN_ENUM()
/** 背景のクリア方法 */
enum class SceneClearMode {
    /** クリアしません。 */
    None,

    /** 背景色と深度バッファをクリアします。 */
    ColorAndDepth,

    /** (未実装) */
    Sky,

    /** (未実装) */
    Sky0,

    /** スカイドームでクリアします。 */
    SkyDome,
};

// DrawElement の大分類。SceneRenderer に投入する DrawElement を決める。
// SceneRenderer 内の各 RenderPass は、さらに小項目を RenderDrawElementTypeFlags で識別し、描画するかどうかを決定する。
enum class RenderPart {
    // シーンのメインコンテンツ。
    // Projection が 3D か 2D(デバイス非依存ピクセル2D座標系) かは RenderView に依る。
    // 通常のオブジェクトの他、BackgroundSky, LightDisc もこれに含まれる。
    Geometry = 0,

    // シーンのサブコンテンツ。
    // Projection が 3D か 2D かは RenderView に依る。
    // Geometry のライティングに関係せず、前面に描画されるもの。
    // 主に 3D 空間内のデバッグ描画に使用する。
    Gizmo,

    // スクリーン全体にオーバーレイ描画されるもの。Zソートなど一部の不要な工程が省略される。
    // Projection は ClipSpace. つまり、座標変換を行わない。
    PostEffect,

    // デバイス非依存ピクセル2D座標系
    // 主に3Dオブジェクトにオーバーレイ表示する2Dの描画(ネームプレートなど)に使用する。
    Gizmo2D,

    _Count,
};

//
// struct TextureRegion
//{
// public:
//    Ref<Texture> texture;
//    Rect rect;
//    Thickness border;
//};

enum class BorderDirection {
    Inside,
    Outside,
};

enum class ShadowDirection {
    Inside,
    Outside,
};

struct FrameBuffer {
    Ref<RenderTargetTexture> renderTarget[detail::MaxMultiRenderTargets];
    Ref<DepthBuffer> depthBuffer;
};

enum class RequestBatchResult {
    Staging,   // request はステージされ、まだ Batch 化されていない
    Submitted, // 直前までの request は submit され、List に新しい Batch が追加された。最新の request はステージされ、まだ Batch 化されていない。
               // なお、State が変わったため新しい Batch を作りたいとき、Batch の中身が 0 であるときは作ってはならない。
};

struct ClearInfo {
    ClearFlags flags = ClearFlags::None;
    Color color;
    float depth = 1.0f;
    uint8_t stencil = 0x00;
};

// 必須情報。border の情報が含まれているが、Shadow を正しく描画するために必要となる
struct BoxElementShapeBaseStyle {
    Matrix transform;
    Rect baseRect;
    CornerRadius cornerRadius;
    bool aligndLineAntiAlias = false; // 軸に平行な辺にも AA を適用するか (回転させたい場合に使用)
};

struct BoxElementShapeBackgroundStyle {
    Color color;
};

struct BoxElementShapeBorderStyle {
    Color borderLeftColor;
    Color borderTopColor;
    Color borderRightColor;
    Color borderBottomColor;
    Thickness borderThickness;
    bool borderInset = false;
};

struct BoxElementShapeShadowStyle {
    /** シャドウのオフセット距離です (シャドウの生成処理は、オフセットが BoxElement から大きく離れないことを前提として最適化されています。オフセットは shadowWidth を超えないようにしてください) */
    Vector2 shadowOffset;

    Color shadowColor;
    float shadowWidth = 0.0f;
    float shadowBlur = 0.0f;
    bool shadowInset = false;
};

enum class BuiltinRenderTextureType {
    ViewNormalMap, // experiment
    ViewDepthMap,
    ViewMaterialMap,
    ObjectId,
    _Count,
};

// RenderingContext を使った描画の起点となった視点情報。
// 特に Partcle や Tilemap で使用する。これらは Camera が視点となるが、onRender() から Camera までの参照はかなり遠い。
// また UI でも使うことがあるため、単に Camera ではなく、もう一段抽象化したデータ構造を用意しておく。(UI では Camera という概念はイメージしづらい)
// Mirror を作るときも、わざわざ CameraObject をひとつ作るのはおおげさ。
// というより、SceneGraph のモジュールの一部である Camera というクラスに依存したくない。
//
// RenderView と 1:1 ではない点に注意。
// RenderView は RenderViewPoint を持つが、RenderViewPoint は 後段の RenderPart でも複数個のインスタンスを使う。
// そちらを CameraInfo にするのもアリだが、そうすると他言語に公開ができなくなる。
class RenderViewPoint : public Object {
public:
    Matrix worldMatrix;
    Size viewPixelSize;
    Vector3 viewPosition;
    Vector3 viewDirection;
    Matrix viewMatrix;
    Matrix projMatrix;
    Matrix viewProjMatrix;
    ViewFrustum viewFrustum;

    float fovY = 1.0f;
    float nearClip = 0;
    float farClip = 0;
    float dpiScale = 1.0;

    void resetUnproject(const Size& viewPixelSize);
    void resetPerspective(const Vector3& viewPos, const Vector3& viewDir, float fovY, const Size& size, float n, float f);
    void copyFrom(const RenderViewPoint* other);
    void makeCameraInfo(detail::CameraInfo* cameraInfo) const;
};

enum class SpriteFlipFlags : uint8_t {
    None = 0,
    FlipX = 1 << 1,
    FlipY = 1 << 2,
};
LN_FLAGS_OPERATORS(SpriteFlipFlags);

struct SpriteData {
    Matrix transform;
    Vector2 size;
    Vector2 anchorRatio;
    Rect srcUVRect;
    Color color;
    SpriteBaseDirection baseDirection;
    BillboardType billboardType;
    SpriteFlipFlags flipFlags;
};

namespace detail {
class MeshGenerater;
class SceneRenderer;
class DrawElementList;
class DrawElementListBuilder;
class FontRequester;
class FlexGlyphRun;
class RenderingManager;
class DrawElementList;
class DrawElementListBuilder;
class BuiltinEffectData;
class RenderDrawElement;
class SkeletonInstance;
class MorphInstance;
class CommandListServer;
class RLIMaterial;
class RLIBatchState;

enum class RenderDrawElementTypeFlags : uint8_t {
    None = 0,

    // 内部で自動決定
    Clear = 1 << 1,       // clear など、ポリゴンを描画しないが、レンダーターゲットを変更する
    Opaque = 1 << 2,      // 不透明
    Transparent = 1 << 3, // 半透明

    // 外部指定
    LightDisc = 1 << 4, // 半透明オブジェクトの後ろにあるライトもレイ引いたりレンズフレアかけたりしたいので、ElementList に含め、Zソートなどの対象として描画する
    BackgroundSky = 1 << 5,
};
LN_FLAGS_OPERATORS(RenderDrawElementTypeFlags);

// enum class ProjectionKind
//{
//	ViewProjection3D,	// 通常の 3D 空間に配置されたモデルに適用する View-Proj
//	ClipScreen,			// ClipSpace に配置されたモデルに適用する View-Proj。 つまり、変換無し。ポストエフェクト用の Blit で使う。
//	Physical2D,			// レンダーターゲットの px サイズと一致する 2D 空間に配置されたモデルに適用する View-Proj。
//	Independent2D,		// Device-Independent Pixel (dp) で表される 2D 空間に配置されたモデルに適用する View-Proj
// };

struct DynamicLightInfo {
    DynamicLightInfo()
        : m_type(LightType::Ambient)
        , m_color(Color::White)
        , m_color2(Color::White)
        //, m_ambient(Color::White)
        , m_color3(Color::White)
        , m_position(Vector4::Zero)
        , m_direction(0, -1, 0)
        , m_intensity(1.0f)
        , m_range(10.0f)
        , m_attenuation(1.0f)
        , m_spotAngle(Math::PI / 3)
        , m_spotPenumbra(0.1f)
        //, castShadow(false)
        //, m_shadowCasterPass(nullptr)
        , shadowCameraZFar(0.0f)
        , shadowLightZFar(0.0f) {}

    LightType m_type; // ライトの種類

    // 基本色 (Phong ではディフーズカラー)
    // [Ambient] ambient color
    // [Hemisphere] sky color
    Color m_color;

    // [Hemisphere] ground color
    // [Directional, Point, Spot] (Phong shading でのみ使用) アンビエントカラー
    Color m_color2;

    // Color		m_ambient;			// (Phong shading でのみ使用) アンビエントカラー
    Color m_color3;       // (Phong shading でのみ使用) スペキュラカラー
    Vector4 m_position;   // [Point, Spot]
    Vector3 m_direction;  // [Directional, Spot]向き
    float m_intensity;    // [All] 光の強さ (Shader uniform にセットする前に、m_color と乗算する)
    float m_range;        // [Point, Spot] 減衰
    float m_attenuation;  // [Point, Spot] 減衰
    float m_spotAngle;    // [Spot]
    float m_spotPenumbra; // [Spot]

    // bool castShadow;	// [Directional, Point, Spot]
    float shadowCameraZFar; // どのくらいまでの距離に Shadow を生成するか？
    float shadowLightZFar;  // ライト空間内の far - near

    // detail::ShadowCasterPass*	m_shadowCasterPass;
    // Matrix		transform;

    // float		tempDistance;		// 作業用変数

    // static const int MaxLights = 3;		// MMD based

    bool mainLight = false;

    bool shadowEnabled() const { return shadowCameraZFar > 0.0f && shadowLightZFar > 0.0f; }

    static DynamicLightInfo makeAmbientLightInfo(const Color& color, float intensity) {
        DynamicLightInfo info;
        info.m_type = LightType::Ambient;
        info.m_color = color;
        info.m_intensity = intensity;
        return info;
    }

    static DynamicLightInfo makeHemisphereLightInfo(const Color& skyColor, const Color& groundColor, float intensity) {
        DynamicLightInfo info;
        info.m_type = LightType::Hemisphere;
        info.m_color = skyColor;
        info.m_color2 = groundColor;
        info.m_intensity = intensity;
        return info;
    }

    static DynamicLightInfo makeEnvironmentLightInfo(const Color& color, const Color& ambientColor, const Color& skyColor, const Color& groundColor, float intensity, const Vector3& direction, bool mainLight, float shadowCameraZFar, float shadowLightZFar) {
        DynamicLightInfo info;
        info.m_type = LightType::Directional;
        info.m_color = color;
        info.m_color2 = skyColor;
        info.m_color3 = groundColor;
        info.m_position = ambientColor.toVector4();
        info.m_direction = direction;
        info.m_intensity = intensity;
        info.mainLight = mainLight;
        info.shadowCameraZFar = shadowCameraZFar;
        info.shadowLightZFar = shadowLightZFar;
        return info;
    }

    static DynamicLightInfo makeDirectionalLightInfo(const Color& color, float intensity, const Vector3& direction, bool mainLight, float shadowCameraZFar, float shadowLightZFar) {
        DynamicLightInfo info;
        info.m_type = LightType::Directional;
        info.m_color = color;
        info.m_direction = direction;
        info.m_intensity = intensity;
        info.mainLight = mainLight;
        info.shadowCameraZFar = shadowCameraZFar;
        info.shadowLightZFar = shadowLightZFar;
        return info;
    }

    static DynamicLightInfo makePointLightInfo(const Color& color, float intensity, const Vector3& position, float range, float attenuation) {
        DynamicLightInfo info;
        info.m_type = LightType::Point;
        info.m_color = color;
        info.m_position = Vector4(position, 0.0f);
        info.m_intensity = intensity;
        info.m_range = range;
        info.m_attenuation = attenuation;
        return info;
    }

    static DynamicLightInfo makeSpotLightInfo(const Color& color, float intensity, const Vector3& position, const Vector3& direction, float range, float attenuation, float spotAngle, float spotPenumbra) {
        DynamicLightInfo info;
        info.m_type = LightType::Spot;
        info.m_color = color;
        info.m_position = Vector4(position, 0.0f);
        info.m_direction = direction;
        info.m_intensity = intensity;
        info.m_range = range;
        info.m_attenuation = attenuation;
        info.m_spotAngle = spotAngle;
        info.m_spotPenumbra = spotPenumbra;
        return info;
    }
};

struct SceneGlobalRenderParams {
    // Color ambientColor;
    // Color ambientSkyColor;
    // Color ambientGroundColor;

    float lowerHeight;
    float upperHeight;
    float startDistance;
    Color fogColor;
    float fogDensity = 0.0f;
    float heightFogDensity = 0.0f;

    Color skydomeSkyColor = Color(0, 0, 0, 0);
    Color skydomeHorizonColor = Color(0, 0, 0, 0);
    Color skydomeCloudColor = Color(0, 0, 0, 0);
    Color skydomeOverlayColor = Color(0, 0, 0, 0);

    void reset() {
        lowerHeight = -100.0f;
        upperHeight = 10.0f;
        startDistance = 20.0f;
        fogColor = Color(0.686f, 0.678f, 0.666f); // Color::White;
        fogDensity = 1.0f / 50.0f;
        heightFogDensity = 1.0f / 10.0f;

        lowerHeight = -500.0f;
        upperHeight = 50.0f;
        fogDensity = 1.0f / 200.0f;
        heightFogDensity = 1.0f / 100.0f;
    }
};

enum class ZSortDistanceBase {
    NodeZ,                /**< ノードの Z 値を距離として使用する */
    CameraDistance,       /**< カメラとノードの距離を使用する */
    CameraScreenDistance, /**< カメラが映すスクリーン平面とノードの距離を使用する */
};

// DrawElementList::newFrameData() で確保するデータのインターフェイス。
// 描画終了時にデストラクタを呼び出すために使用する。
// このクラスはフレーム終了時に開放されるため、自分で参照を管理する必要はないが、寿命に注意すること。
class IDrawElementListFrameData {
protected:
    IDrawElementListFrameData();
    virtual ~IDrawElementListFrameData();

private:
    // next data of linked list.
    IDrawElementListFrameData* m_nextFrameData;

    friend class DrawElementList;
};

class BuiltinEffectData
    : public IDrawElementListFrameData {
public:
    static const BuiltinEffectData DefaultValue;

    float opacity;
    Color colorScale;
    Color blendColor;
    ColorTone tone;

    BuiltinEffectData() {
        reset();
    }

    BuiltinEffectData(const BuiltinEffectData& other)
        : opacity(other.opacity)
        , colorScale(other.colorScale)
        , blendColor(other.blendColor)
        , tone(other.tone) {
    }

    void reset() {
        // m_transfrom = Matrix();
        opacity = 1.0;
        colorScale = Color(1.0f, 1.0f, 1.0f, 1.0f);
        blendColor = Color(0.0f, 0.0f, 0.0f, 0.0f);
        tone = ColorTone(0.0f, 0.0f, 0.0f, 0.0f);
    }

    bool equals(const BuiltinEffectData* other) const {
        // TODO: hash
        return
            // m_transfrom == other->m_transfrom &&
            opacity == other->opacity &&
            colorScale == other->colorScale &&
            blendColor == other->blendColor &&
            tone == other->tone;
    }

    void inherit(const BuiltinEffectData& parent) {
        opacity *= parent.opacity;
        colorScale = Color::multiplyClamp(colorScale, parent.colorScale);
        blendColor = Color::addClamp(blendColor, parent.blendColor);
        tone.addClamp(parent.tone);
    }
};

inline bool operator==(const BuiltinEffectData& lhs, const BuiltinEffectData& rhs) {
    return lhs.equals(&rhs);
}

struct ShaderTechniqueRequestClasses {
    kokage::ShaderTechniqueClass_MeshProcess meshProcess;
    kokage::ShaderTechniqueClass_DrawMode drawMode;
    kokage::ShaderTechniqueClass_Normal normal;
    kokage::ShaderTechniqueClass_Roughness roughness;
};

} // namespace detail
} // namespace ln