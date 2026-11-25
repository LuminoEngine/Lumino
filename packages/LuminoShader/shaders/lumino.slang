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

// drawSprite の頂点はワールド座標でまとめて描くため world / normalMatrix は単位行列で、
// 頂点の法線は transform の逆転置で変換済みのワールド座標の法線として届く。
// そのためメッシュと同じく normalize(mul(normalMatrix, float4(input.normal, 0)).xyz) で扱える。
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
// ln_ ビルトイン変数 (旧版互換の ln_ プレフィックス)
//
// #define は import 先のモジュールへ公開されないため、グローバル property で定義する。
//------------------------------------------------------------------------------

// View

/** View 行列 */
property float4x4 ln_View            { get { return viewData.view; } }

/** Projection 行列 */
property float4x4 ln_Projection      { get { return viewData.proj; } }

/** View-Projection 行列 */
property float4x4 ln_ViewProjection  { get { return viewData.viewProj; } }

/** Projection の逆行列 */
property float4x4 ln_ProjectionI     { get { return viewData.invProj; } }

/** View-Projection の逆行列 */
property float4x4 ln_ViewProjectionI { get { return viewData.invViewProj; } }

/** ワールド空間上のカメラ位置 */
property float3   ln_CameraPosition  { get { return viewData.cameraPos.xyz; } }

/** スクリーン解像度 (width, height, 1/width, 1/height) */
property float4   ln_Resolution      { get { return viewData.screenSize; } }

// Scene
// property float3 ln_MainLightDir   { get { return sceneData.lightDir.xyz; } }
// property float3 ln_MainLightColor { get { return sceneData.lightColor.xyz * sceneData.lightColor.w; } }
// property float3 ln_AmbientColor   { get { return sceneData.ambientColor.xyz; } }

// Object
property float4x4 ln_World           { get { return objectData.world; } }
property float4x4 ln_NormalMatrix    { get { return objectData.normalMatrix; } }

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
