// lumino.slang — Lumino standard shader library
//
// Common structures shared across Lumino shaders.
// Usage: import lumino;

struct ViewParams {
    float4x4 viewProj;
    float4   cameraPos;    // xyz = position
    float4x4 view;         // view matrix (world -> view)
    float4x4 proj;         // projection matrix (view -> clip)
    float4x4 invViewProj;  // inverse of viewProj (clip -> world)
    float4x4 invProj;      // inverse of proj (clip -> view)
    float4   screenSize;   // (width, height, 1/width, 1/height)
};

struct SceneParams {
    float4 lightDir;       // xyz = direction (towards light)
    float4 lightColor;     // xyz = color, w = intensity
    float4 ambientColor;   // xyz = ambient color
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
// System ParameterBlocks (Sets 1–3)
//------------------------------------------------------------------------------
ParameterBlock<ViewParams>     viewData;
ParameterBlock<SceneParams>    sceneData;
ParameterBlock<ObjectParams>   objectData;

//------------------------------------------------------------------------------
// Convenience aliases (legacy-compatible ln_ prefix)
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
