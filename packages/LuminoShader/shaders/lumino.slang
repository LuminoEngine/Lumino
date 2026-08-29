// lumino.slang - Lumino 標準シェーダライブラリ
//
// Lumino のシェーダ間で共有する共通の構造体。
// 使い方: import lumino;

struct ViewParams {
    float4x4 viewProj;
    float4   cameraPos;    // xyz = 位置
    float4x4 view;         // ビュー行列 (ワールド -> ビュー)
    float4x4 proj;         // 射影行列 (ビュー -> クリップ)
    float4x4 invViewProj;  // viewProj の逆行列 (クリップ -> ワールド)
    float4x4 invProj;      // proj の逆行列 (クリップ -> ビュー)
    float4   screenSize;   // (width, height, 1/width, 1/height)
};

struct SceneParams {
    float4 lightDir;       // xyz = 方向 (ライトへ向かう向き)
    float4 lightColor;     // xyz = 色, w = 強度
    float4 ambientColor;   // xyz = 環境光の色
};

struct ObjectParams {
    float4x4 world;
    float4x4 normalMatrix;
};

struct VSInput {
    float3 position : POSITION;
    float3 normal   : NORMAL;
    float2 uv       : TEXCOORD;
    float4 color    : COLOR;
    float4 tangent  : TANGENT;
};

//------------------------------------------------------------------------------
// システム ParameterBlock (Set 1-3)
//------------------------------------------------------------------------------
ParameterBlock<ViewParams>     viewData;
ParameterBlock<SceneParams>    sceneData;
ParameterBlock<ObjectParams>   objectData;

//------------------------------------------------------------------------------
// 便利エイリアス (旧版互換の ln_ プレフィックス)
//------------------------------------------------------------------------------

// View

/** View 行列 */
#define ln_View              viewData.view

/** Projection 行列 */
#define ln_Projection        viewData.proj

/** View-Projection 行列 */
#define ln_ViewProjection    viewData.viewProj

/** Projection の逆行列 */
#define ln_ProjectionI       viewData.invProj

/** View-Projection の逆行列 */
#define ln_ViewProjectionI   viewData.invViewProj

/** ワールド空間上のカメラ位置 */
#define ln_CameraPosition    viewData.cameraPos.xyz

/** スクリーン解像度 (width, height, 1/width, 1/height) */
#define ln_Resolution        viewData.screenSize

// Scene
// #define ln_MainLightDir      sceneData.lightDir.xyz
// #define ln_MainLightColor    (sceneData.lightColor.xyz * sceneData.lightColor.w)
// #define ln_AmbientColor      sceneData.ambientColor.xyz

// Object
#define ln_World             objectData.world
#define ln_NormalMatrix      objectData.normalMatrix

//------------------------------------------------------------------------------
// GBuffer layout (Deferred / SSR 共通)
//------------------------------------------------------------------------------
// GBufferA : [Albedo.rgb, _]                    (RGBA8_UNORM)
// GBufferB : [WorldNormal.xyz, ViewDepth]       (RGBA32_FLOAT)
//            ViewDepth = -viewPos.z (カメラ前方 +, 線形距離)
// GBufferC : [Metallic, Roughness, AO, _]       (RGBA8_UNORM, glTF Metal-Roughness)
//
// ※ Depth を線形にしている理由については examples/assets/GBuffer.slang のコメントを参照。
struct LNGBufferOutput {
    float4 gbufferA : SV_TARGET0;
    float4 gbufferB : SV_TARGET1;
    float4 gbufferC : SV_TARGET2;
};

/** GBuffer 出力を共通レイアウトでパックします。
 *  @param albedo       sRGB 空間の基底色 (0..1)
 *  @param worldNormal  ワールド空間の正規化済み法線
 *  @param viewDepth    -viewPos.z (線形, カメラ前方 +)
 *  @param metallic     0..1 (glTF Metal-Roughness)
 *  @param roughness    0..1
 *  @param ao           0..1
 */
LNGBufferOutput lnPackGBuffer(
    float3 albedo, float3 worldNormal, float viewDepth,
    float metallic, float roughness, float ao) {
    LNGBufferOutput o;
    o.gbufferA = float4(albedo, 1.0);
    o.gbufferB = float4(worldNormal, viewDepth);
    o.gbufferC = float4(metallic, roughness, ao, 1.0);
    return o;
}
