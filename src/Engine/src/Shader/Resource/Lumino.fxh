
#ifndef LUMINO_INCLUDED
#define LUMINO_INCLUDED

//------------------------------------------------------------------------------
// Lib

#define LN_LIGHTINGMETHOD_CLUSTERED 1

#define LN_EPSILON                1e-6
#define LN_MAX_GLOBAL_LIGHTS    4

cbuffer LNRenderViewBuffer
{
    /* [0]   */ float4x4 ln_View;
    /* [64]  */ float4x4 ln_Projection;
    /* [128] */ float4x4 ln_ProjectionI;
    /* [192] */ float4x4 _ln_MainLightMatrix;    // ViewProj
    /* [256] */ float4 ln_Resolution;
    /* [272] */ float4 ln_CameraPosition_;
    /* [288] */ float4 ln_CameraDirection_;
    /* [304] */ float4 ln_AmbientColor;
    /* [320] */ float4 ln_AmbientSkyColor;
    /* [336] */ float4 ln_AmbientGroundColor;
    /* [352] */ float4 _ln_MainLightColor;
    /* [368] */ float4 _ln_MainLightPos;    // .w=1
    /* [384] */ float4 _ln_MainLightDir;    // .w=1
};  /* [400(alignd:16)] */

#define ln_CameraPosition (ln_CameraPosition_.xyz)
#define ln_CameraDirection (ln_CameraDirection_.xyz)
#define ln_NearClip (ln_CameraPosition_.w)
#define ln_FarClip (ln_CameraDirection_.w)

cbuffer LNRenderElementBuffer
{
    /* [0]   */ float4x4 ln_World;
    /* [64]  */ float4x4 ln_WorldI;
    /* [128] */ float4x4 ln_WorldViewProjection;
    /* [192] */ float4x4 ln_WorldView;
    /* [256] */ float4x4 ln_WorldViewIT;
    /* [320] */ float4 ln_BoneTextureReciprocalSize;
    /* [336] */ float4 ln_MorphWeights;
    /* [352] */ int ln_objectId;
};  /* [368(alignd:16)] */

cbuffer LNPBRMaterialParameter
{
    /* [0]  */ float4	ln_MaterialColor;
    /* [16] */ float4	ln_MaterialEmissive;
    /* [32] */ float	ln_MaterialRoughness;
    /* [36] */ float	ln_MaterialMetallic;
};

// Builtin effect colors
cbuffer LNEffectColorBuffer
{
    /* [0]  */ float4 ln_ColorScale;
    /* [16] */ float4 ln_BlendColor;
    /* [32] */ float4 ln_ToneColor;
};

sampler2D ln_MaterialTexture;

// TODO: 以下の SamplerState は ln_MaterialTexture と併せたいので、やっぱり SamplerState は分離したい
sampler2D ln_MetallicRoughnessTexture;
sampler2D ln_OcclusionTexture;




//==============================================================================
// cbuffer accessors


inline float3 _LN_MainLightColor() { return _ln_MainLightColor.rgb * _ln_MainLightColor.w; }
inline float3 _LN_MainLightPos() { return _ln_MainLightPos.xyz; }



/** */
#define ln_MainLightColor _LN_MainLightColor()

/** */
#define ln_MainLightPos _LN_MainLightPos()

/** */
#define ln_MainLightMatrix _ln_MainLightMatrix



//==============================================================================
//


struct LN_VSInput
{
    float3    Pos                : POSITION;
    float3    Normal            : NORMAL0;
    float2    UV                : TEXCOORD0;
    float4    Color            : COLOR0;
    float4  tangent: TANGENT0;
#ifdef LN_USE_SKINNING
    float4    BlendIndices    : BLENDINDICES;
    float4    BlendWeight        : BLENDWEIGHT;
#endif
#ifdef LN_USE_INSTANCING
    float4 InstanceTransform0 : POSITION1;
    float4 InstanceTransform1 : POSITION2;
    float4 InstanceTransform2 : POSITION3;
    float4 InstanceTransform3 : POSITION4;
    float4 InstanceUVOffset: TEXCOORD2;
    float4 InstanceColorScale: COLOR1;
    uint instanceID : SV_InstanceID;
#endif
};

struct LN_VSOutput_Common
{
    float4    svPos        : SV_POSITION;

    /** 座標変換後の法線 [View-space] 左手座標系である点に注意。視点に対して、右は X+, 上はY+, 奥はZ+ */
    float3    Normal        : NORMAL0;

    float2    UV            : TEXCOORD0;

    float4    Color        : COLOR0;
};

struct LNVFInput
{
    // LNVSInput と同じ内容
    float3    Pos            : POSITION;
    float3    Normal        : NORMAL0;
    float2    UV            : TEXCOORD0;
    float4    Color        : COLOR0;
    
    // その他、Forward, Clusterd, Differd すべてで必要になるもの
    // (ターゲットごとに分けたりすると煩雑なのでまとめる。そんなに数は多くならないだろう)
    //float3    VertexPos    : TEXCOORD10;
    //float3    WorldPos    : TEXCOORD11;
};

struct LN_PSInput_Common
{
    // POSITION 以外は LNVSInput と同じ
    float3    Normal        : NORMAL0;
    float2    UV            : TEXCOORD0;
    float4    Color        : COLOR0;
};

struct LN_Surface
{
    float4    Albedo;        // diffuse color
    float3    Normal;        // tangent space normal, if written
    float3    Emission;
    float    Specular;    // specular power in 0..1 range
    
    float Metallic;      // 0=非メタル, 1=メタル
    float Roughness;    //  1=滑らか, 0=粗い (逆は Smoothness(Unity))
    float Occlusion;     // オクルージョン (デフォルト 1)
};

void _LN_InitSurfaceOutput(float2 uv, float4 color, float3 normal, inout LN_Surface surface)
{
    //surface.Albedo = float4(0, 0, 0, 1);
    surface.Normal = normal;
    //surface.Emission = float3(0, 0, 0);
    surface.Specular = 0;
    
    surface.Albedo = tex2D(ln_MaterialTexture, uv) * ln_MaterialColor * color;
    clip(surface.Albedo.a - 0.0001);
    surface.Emission = (ln_MaterialEmissive.rgb * ln_MaterialEmissive.a);
    
    // metallic 値は .b からサンプリングする。
    // roughness 値は .g からサンプリングする。
    // それぞれマテリアルの metallic, roughness 値と乗算しなければならない。
    // https://github.com/KhronosGroup/glTF/tree/master/specification/2.0#pbrmetallicroughnessmetallicroughnesstexture
    // https://github.com/KhronosGroup/glTF/tree/master/specification/2.0#pbrmetallicroughnessroughnessfactor
    const float4 mr = tex2D(ln_MetallicRoughnessTexture, uv);
    surface.Metallic = mr.b * ln_MaterialMetallic;
    surface.Roughness = mr.g * ln_MaterialRoughness;
    surface.Occlusion = tex2D(ln_OcclusionTexture, uv).r;
}

// deprecated: see _LN_ProcessVertex_StaticMesh
LN_VSOutput_Common LN_ProcessVertex_Common(LN_VSInput input)
{
    //float4x4 normalMatrix  = transpose(/*inverse*/(ln_WorldView));
    
    LN_VSOutput_Common o;
    o.svPos            = mul(float4(input.Pos, 1.0f), ln_WorldViewProjection);
    o.Normal        = mul(float4(input.Normal, 1.0f), ln_WorldViewIT).xyz;
    o.UV            = input.UV;// + (float2(0.5, 0.5) / ln_Resolution.xy);
    o.Color            = input.Color;
    return o;
}

void _LN_ProcessVertex_StaticMesh(
    LN_VSInput input,
    out float4 outSVPos, out float3 outViewNormal)
{
#ifdef LN_USE_INSTANCING
    const float4x4 worldMatrix = float4x4(
        input.InstanceTransform0.x, input.InstanceTransform0.y, input.InstanceTransform0.z, input.InstanceTransform0.w,
        input.InstanceTransform1.x, input.InstanceTransform1.y, input.InstanceTransform1.z, input.InstanceTransform1.w,
        input.InstanceTransform2.x, input.InstanceTransform2.y, input.InstanceTransform2.z, input.InstanceTransform2.w,
        input.InstanceTransform3.x, input.InstanceTransform3.y, input.InstanceTransform3.z, input.InstanceTransform3.w);
    float4 pos = mul(float4(input.Pos, 1.0f), worldMatrix);
    outSVPos = mul(pos, ln_WorldViewProjection);
#else
    outSVPos = mul(float4(input.Pos, 1.0f), ln_WorldViewProjection);
#endif

    outViewNormal = mul(float4(input.Normal, 1.0f), ln_WorldViewIT).xyz;
}

float LN_Square(float x)
{
    return x * x;
}

float2 LN_Square(float2 x)
{
    return x * x;
}

float3 LN_Square(float3 x)
{
    return x * x;
}

float4 LN_Square(float4 x)
{
    return x * x;
}

float4 LN_CalculateToneColor(float4 inColor, float4 inToneColor)
{
    float4 outColor = inColor;
    float y = (0.208012 * outColor.r + 0.586611 * outColor.g + 0.114478 * outColor.b) * inToneColor.w;
    outColor.rgb = (outColor.rgb * (1.0 - inToneColor.w)) + y + inToneColor.rgb;
    return outColor;
}

float4 LN_GetBuiltinEffectColor(float4 inColor)
{
    // apply color scale.
    float4 outColor = inColor * ln_ColorScale;
    
    // apply blend color.
    outColor.rgb = lerp(outColor.rgb, ln_BlendColor.rgb, ln_BlendColor.a);

    // apply tone. (NTSC Coef method)
    return LN_CalculateToneColor(outColor, ln_ToneColor);
}

/**
 * 正規化された法線を、整数型の RGB テクスチャなどに書き込ムために 0.0 ~ 1.0 にパックします。
 */
float3 LN_PackNormal(float3 normal)
{
    return (normal.xyz + 1.0) / 2.0;
}

/**
 * 整数型の RGB テクスチャなどに書き込まれている 0.0 ~ 1.0 にパックされた法線を -1.0 ~ 1.0 に展開します。
 */
float3 LN_UnpackNormal(float3 packednormal)
{
    return (packednormal * 2.0) - 1.0;
}

/**
 * Clip-space 上の xy 座標 (-1.0 ~ 1.0) を、UV 座標 (0.0 ~ 1.0) へ変換します。
 */
float2 LN_ClipSpacePositionToUV(float2 pos)
{
    return (float2(1.0, -1.0) * pos) * 0.5 + 0.5;
}

/**
 * UV 座標 (0.0 ~ 1.0) を、Clip-space 上の xy 座標 (-1.0 ~ 1.0) へ変換します。
 */
float2 LN_UVToClipSpacePosition(float2 uv)
{
    return ((uv - 0.5) * 2.0) * float2(1.0, -1.0);
}

float3 LN_ApplyEnvironmentLight(float3 color, float3 viewNormal)
{
    // basic ambient light
    const float3 ambient = ln_AmbientColor.xyz;
    
    // hemisphere ambient light
    float hemisphere = (dot(viewNormal, float3(0, 1, 0)) + 1.0) * 0.5;
    float4 c = lerp(ln_AmbientGroundColor, ln_AmbientSkyColor, hemisphere);

    float3 factors = saturate(ambient + c.xyz);

    return color * factors;
}



//==============================================================================
// Part includes

#ifdef LN_LIGHTINGMETHOD_CLUSTERED
#include <LuminoForward.fxh>
#endif

#ifdef LN_SHADINGMODEL_DEFAULT
#include <LuminoPBR.fxh>
#include <LuminoShadow.fxh>
#endif

#ifdef LN_USE_SKINNING
#include <LuminoSkinning.fxh>
#endif

#ifdef LN_USE_NORMALMAP
#include <LuminoNormalMap.fxh>
#endif

//==============================================================================
// Part combination

/*
//-------------------------------------
// Mesh Processing
#ifdef LN_USE_SKINNING
    #define _LN_VS_PROCESS_PART_MESHPROCESSING(input, output) \
        _LN_ProcessVertex_SkinnedMesh(input, output.svPos, output.viewNormal, output.UV, output.Color)
#else
    #define _LN_VS_PROCESS_PART_MESHPROCESSING(input, output) \
        _LN_ProcessVertex_StaticMesh(input, output.svPos, output.viewNormal, output.UV, output.Color)
#endif

*/
//-------------------------------------
// Normal Map
#ifdef LN_USE_NORMALMAP
    #define LN_GetPixelNormal(input) LN_GetPixelNormalFromNormalMap(input.UV, input.vTangent, input.vBitangent, input.viewNormal)
#else
    #define LN_GetPixelNormal(input) input.viewNormal
#endif



void _LN_ProcessVertex_Common(
    LN_VSInput input,
    out float4 outSVPos,
    out float3 outViewNormal,
    out float2 outUV,
    out float4 outColor,
    out float3 outTangent,
    out float3 outBitangent)
{
#ifdef LN_USE_SKINNING
    _LN_ProcessVertex_SkinnedMesh(
        input.Pos, input.Normal, input.BlendIndices, input.BlendWeight,
        outSVPos, outViewNormal);
#else
    _LN_ProcessVertex_StaticMesh(input, outSVPos, outViewNormal);
#endif

#ifdef LN_USE_INSTANCING
    outUV = (input.UV * input.InstanceUVOffset.zw) + input.InstanceUVOffset.xy;
    outColor = input.Color * input.InstanceColorScale;
#else
    outUV = input.UV;
    outColor = input.Color;
#endif

    const float3 viewNormal = outViewNormal;

#ifdef LN_USE_NORMALMAP
    _LN_ProcessVertex_NormalMap(input, outViewNormal, outTangent, outBitangent);
#else
    outTangent = input.tangent;
    outBitangent = normalize(cross(viewNormal, input.tangent) * input.tangent.w);
#endif
}

#define _LN_VS_PROCESS_PART_COMMON(input, output) \
    _LN_ProcessVertex_Common(input, output.svPos, output.viewNormal, output.UV, output.Color, output.vTangent, output.vBitangent);

//-------------------------------------
// Clustered Lighting
#ifdef LN_LIGHTINGMETHOD_CLUSTERED
    // Varying fileds.
    // vertexPos : 元の頂点データの位置情報
    // worldPos  : World 空間上の位置 (ln_World による変換結果)
    // viewPos   : View 空間上の位置 (ln_WorldView による変換結果)
    #define _LN_VARYING_DECLARE_LIGHTINGMETHOD ; \
        float3 vertexPos : POSITION10; \
        float3 worldPos  : POSITION11; \
        float3 viewPos   : POSITION12; \
        float4 vInLightPosition : POSITION13

    #define _LN_VS_PROCESS_PART_LIGHTING(input, output) \
        _LN_ProcessVertex_ClusteredForward(input, output.vertexPos, output.worldPos, output.viewPos, output.vInLightPosition)
#else
    #define _LN_VARYING_DECLARE_LIGHTINGMETHOD
    #error "Invalid LIGHTINGMETHOD."
#endif


//-------------------------------------
// PS

#if defined(LN_SHADINGMODEL_DEFAULT)
float4 _LN_PS_ClusteredLighting_PBRShading(
    float3 worldPos,
    float3 vertexPos,
    float4 positionInLightSpace,
    LN_Surface surface)
{
    // ビュー平面からの水平距離。視点からの距離ではないので注意
    float4 viewPos = mul(float4(worldPos, 1.0f), ln_View);
    
    // 頂点位置から視点位置へのベクトル
    float3 vViewPosition = -viewPos.xyz;
    
    // Create Geometry infomation.
    LN_PBRGeometry geometry;
    geometry.position = -vViewPosition;
    geometry.normal = normalize(surface.Normal);
    geometry.viewDir = normalize(vViewPosition);

    // Create Material infomation.
    LN_PBRMaterial material;
    material.diffuseColor = lerp(surface.Albedo.xyz, float3(0, 0, 0), surface.Metallic); // 鏡面反射率が 1.0 の時、入射光 (Incident Ray) は物体内部で散乱しないため、物体の色が影響することは無い
    material.specularColor = lerp(float3(0.04, 0.04, 0.04), surface.Albedo.xyz, surface.Metallic);
    material.specularRoughness = clamp(surface.Roughness, 0.04, 1.0);
    material.occlusion = surface.Occlusion;

    // Shading
    _LN_LocalLightContext localLightContext;
    _LN_InitLocalLightContext(localLightContext, vertexPos, viewPos);
    float3 outgoingLight = _LN_ComputePBRLocalLights(localLightContext, geometry, material);

    // Shadow
    float4 posInLight = positionInLightSpace;
    float shadow = LN_CalculateShadow(posInLight);
    outgoingLight *= shadow;

    float3 result = outgoingLight;

    // Emission
    result += surface.Emission;

    // Fog
    result = LN_ApplyFog(result, worldPos);

    return float4(result, surface.Albedo.a);
}
#endif

float4 _LN_ProcessPixel(float3 worldPos, float3 vertexPos, float4 positionInLightSpace, LN_Surface surface)
{
    float4 color = surface.Albedo;

#if defined(LN_SHADINGMODEL_DEFAULT)
    color = _LN_PS_ClusteredLighting_PBRShading(worldPos, vertexPos, positionInLightSpace, surface);
#endif

    color = LN_GetBuiltinEffectColor(color);
    return color;
}

//==============================================================================
// Core publics

#define LN_VS_PS_INPUT_OUTPUT_DECLARE \
    float4 svPos        : SV_POSITION; \
    float3 viewNormal   : NORMAL10; \
    float2 UV           : TEXCOORD10; \
    float4 Color        : COLOR10; \
    float3 vTangent     : TEXCOORD12; \
    float3 vBitangent   : TEXCOORD13 \
    _LN_VARYING_DECLARE_LIGHTINGMETHOD

// Standard VS Output members.
#define LN_VS_OUTPUT_DECLARE LN_VS_PS_INPUT_OUTPUT_DECLARE

// Standard PS Input members.
#define LN_PS_INPUT_DECLARE LN_VS_PS_INPUT_OUTPUT_DECLARE

// LN_VSOutput
struct LN_VSOutput
{
    LN_VS_OUTPUT_DECLARE;
};

// LN_PSInput
struct LN_PSInput
{
    LN_PS_INPUT_DECLARE;
};

// LN_ProcessVertex
#define LN_ProcessVertex(input, output) \
    _LN_VS_PROCESS_PART_COMMON(input, output); \
    _LN_VS_PROCESS_PART_LIGHTING(input, output);

// LN_ProcessSurface
#define LN_ProcessSurface(input, surface) _LN_InitSurfaceOutput(input.UV, input.Color, LN_GetPixelNormal(input), surface);

// LN_ProcessPixel
#define LN_ProcessPixel(input, surface) _LN_ProcessPixel(input.worldPos, input.vertexPos, input.vInLightPosition, surface)

//==============================================================================
// Default main functions

LN_VSOutput LN_VSMain(LN_VSInput input)
{
    LN_VSOutput output;
    LN_ProcessVertex(input, output);
    return output;
}

float4 LN_PSMain(LN_PSInput input) : SV_TARGET0
{
    LN_Surface surface;
    LN_ProcessSurface(input, surface);
    return LN_ProcessPixel(input, surface);
}

#endif // LUMINO_INCLUDED
