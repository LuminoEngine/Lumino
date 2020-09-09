
#include <Lumino.fxh>
#include <LuminoForward.fxh>
#include <LuminoPBR.fxh>
#include <LuminoShadow.fxh>
#include <LuminoSkinning.fxh>

sampler2D ln_NormalMap;

//#define LN_USE_NORMALMAP
float3 LN_UnpackNormal(float4 packednormal)
{
    return (packednormal.xyz * 2.0) - 1.0;
}


//------------------------------------------------------------------------------
// Lib (ClusteredForward)


float4 _LN_PS_ClusteredForward_Default(
    float3 worldPos,
    float3 vertexPos,
    float4 vInLightPosition,
    LN_SurfaceOutput surface)
{
    // ビュー平面からの水平距離。視点からの距離ではないので注意
    float4 viewPos = mul(float4(worldPos, 1.0f), ln_View);
    
    // 頂点位置から視点位置へのベクトル
    float3 vViewPosition = -viewPos.xyz;
    
    _LN_LocalLightContext localLightContext;
    _LN_InitLocalLightContext(localLightContext, vertexPos, viewPos);
    
    /**/
    LN_PBRGeometry geometry;
    geometry.position = -vViewPosition;//-vViewPosition;
    geometry.normal = normalize(surface.Normal);//vNormal);
    geometry.viewDir = normalize(vViewPosition);//vViewPosition);

    /**/
    float metallic = ln_MaterialMetallic;
    float roughness = ln_MaterialRoughness;
    LN_PBRMaterial material;
    material.diffuseColor = lerp(surface.Albedo.xyz, float3(0, 0, 0), metallic);
    material.specularColor = lerp(float3(0.04, 0.04, 0.04), surface.Albedo.xyz, metallic);
    material.specularRoughness = clamp( roughness, 0.04, 1.0 );//roughness;

    float3 result = float3(0, 0, 0);

    /**/
    float opacity = 1.0;
    float3 outgoingLight = _LN_ComputePBRLocalLights(localLightContext, geometry, material);


    // Shadow
    float4 posInLight = vInLightPosition;
    float shadow = LN_CalculateShadow(posInLight);
    outgoingLight *= shadow;

    result.rgb = outgoingLight;
    
    // Environment light
    //result.rgb = LN_ApplyEnvironmentLight(result.rgb, surface.Normal);
    
    // Emission
    result.rgb += surface.Emission;

    // Fog
    result.rgb = LN_ApplyFog(result.rgb, worldPos);

    return float4(result.rgb, opacity);
}

//------------------------------------------------------------------------------
// (auto generation)
// ClusteredShading_Default.template.fx から生成する

#ifdef LN_USE_NORMALMAP
#define _LN_VARYING_DECLARE_NORMAL_MAP ; \
    float3 vTangent     : TEXCOORD12; \
    float3 vBitangent   : TEXCOORD13
#else
#define _LN_VARYING_DECLARE_NORMAL_MAP
#endif

#define LN_VS_OUTPUT_DECLARE \
    float4 svPos    : SV_POSITION; \
    float3 viewspaceNormal   : NORMAL0; \
    float2 UV       : TEXCOORD0; \
    float4 Color    : COLOR0 \
    _LN_VARYING_DECLARE_NORMAL_MAP

#define LN_PS_INPUT_DECLARE \
    float3 viewspaceNormal   : NORMAL0; \
    float2 UV       : TEXCOORD0; \
    float4 Color    : COLOR0 \
    _LN_VARYING_DECLARE_NORMAL_MAP

struct VSOutput
{
    LN_VS_OUTPUT_DECLARE;

    float4    vInLightPosition     : TEXCOORD9;
    // clustered forward
    float3    WorldPos    : TEXCOORD10;
    float3    VertexPos    : TEXCOORD11;

#ifdef LN_USE_NORMALMAP
    //float3    vTangent    : TEXCOORD12;
    //float3    vBitangent    : TEXCOORD13;

    float3    Debug    : TEXCOORD14;
#endif
};

// auto generation
VSOutput _lngs_VS_ClusteredForward_Geometry(LN_VSInput vsi)
{
    LN_VSOutput_Common common = LN_ProcessVertex_Common(vsi);
    LN_VSOutput_ClusteredForward extra = LN_ProcessVertex_ClusteredForward(vsi);

    VSOutput output;
    output.svPos = common.svPos;
    output.viewspaceNormal = common.Normal;
    output.UV = common.UV;
    output.Color = common.Color;
    output.WorldPos = extra.WorldPos;
    output.VertexPos = extra.VertexPos;
    output.vInLightPosition = extra.vInLightPosition;

    
#ifdef LN_USE_NORMALMAP
    const float3 objectTangent = vsi.tangent.xyz;
    const float3 transformedTangent = ( ln_WorldView * float4( objectTangent, 0.0 ) ).xyz;
    output.vTangent = normalize( transformedTangent );
    output.vBitangent = normalize( cross( output.viewspaceNormal, output.vTangent ) * vsi.tangent.w );
    output.Debug = objectTangent;
/*
    float3 eyePosition = ln_CameraPosition;
    float3 lightPosition = float3(1, 1, -1);

    float3 invEye   = (ln_WorldI * float4(eyePosition, 0.0)).xyz;
    float3 invLight = (ln_WorldI * float4(lightPosition, 0.0)).xyz;
    float3 eye      = invEye - common.svPos.xyz;
    float3 light    = invLight - common.svPos.xyz;



    // ローカル空間上での接空間ベクトルの方向を求める
    float3 normal = common.Normal;
    float3 n = normalize(normal);
    float3 t = normalize(cross(normal, float3(0.0, 1.0, 0.0)));
    float3 b = cross(n, t);

    // 視線ベクトルとライトベクトルを接空間上に変換
    float3 vEyeDirection;
    vEyeDirection.x   = dot(t, eye);
    vEyeDirection.y   = dot(b, eye);
    vEyeDirection.z   = dot(n, eye);
    output.vEyeDirection = normalize(vEyeDirection);
    float3 vLightDirection;
    vLightDirection.x = dot(t, light);
    vLightDirection.y = dot(b, light);
    vLightDirection.z = dot(n, light);
    output.vLightDirection = normalize(vLightDirection);
    */

#endif

    return output;
}
// auto generation

VSOutput _lngs_VS_ClusteredForward_Geometry_SkinnedMesh(LN_VSInput vsi)
{
    LN_VSOutput_Common common = LN_ProcessVertex_SkinnedCommon(vsi);
    //LN_VSOutput_Common common = LN_ProcessVertex_Common(vsi);
    LN_VSOutput_ClusteredForward extra = LN_ProcessVertex_ClusteredForward(vsi);

    VSOutput output;
    output.svPos = common.svPos;
    output.viewspaceNormal = common.Normal;
    output.UV = common.UV;
    output.Color = common.Color;
    output.WorldPos = extra.WorldPos;
    output.VertexPos = extra.VertexPos;
    
    //output.Color.rgb = vsi.BlendWeight.rgb;
    //output.Color.rg = output.UV;
    
    //float x = 0.7;//2.0 / ln_BoneTextureReciprocalSize.x;
    //float x = 1.0 * ln_BoneTextureReciprocalSize.x;
    //float y = 0.0;// / ln_BoneTextureReciprocalSize.y;
    //output.Color.rgb = tex2Dlod(ln_BoneTexture, float4(x, y, 0, 1)).rgb;
    //output.Color.x = ln_NearClip + ln_FarClip + ln_CameraPosition.x + ln_CameraDirection.x;
    return output;

    //VSOutput o;
    //o.common    = LN_ProcessVertex_SkinnedCommon(vsi);
    //o.extra        = LN_ProcessVertex_ClusteredForward(vsi);
    // ★ Scene固有のコードはここに直接生成する (ピクセルシェーダと書き方を合わせたい)
    //MyVFMain(vsi, o.user);    // ★ User定義呼び出し
    //return o;
}



struct _lngs_PSInput
{
    LN_PS_INPUT_DECLARE;

    // common
    //float3    viewspaceNormal        : NORMAL0;
    //float2    UV            : TEXCOORD0;
    //float4    Color        : COLOR0;
    
    float4    vInLightPosition     : TEXCOORD9;
    // clustered forward
    float3    WorldPos    : TEXCOORD10;
    float3    VertexPos    : TEXCOORD11;
    
#ifdef LN_USE_NORMALMAP
    //float3    vTangent    : TEXCOORD12;
    //float3    vBitangent    : TEXCOORD13;
    float3    Debug    : TEXCOORD14;
#endif
};

struct _lngs_PSOutput
{
    float4 color0 : COLOR0;
    //float4 color1 : COLOR1;
};

_lngs_PSOutput _lngs_PS_ClusteredForward_Geometry(_lngs_PSInput input)
{
    
#ifdef LN_USE_NORMALMAP
    const float3 viewSpaceTangent = normalize(input.vTangent);
    const float3 viewSpaceBitangent = normalize(input.vBitangent);
    const float3 viewSpaceNormal = normalize(input.viewspaceNormal);
    const float3 mapNormal = LN_UnpackNormal(tex2D(ln_NormalMap, input.UV));

    const float3x3 vTBN = float3x3( viewSpaceTangent, viewSpaceBitangent, viewSpaceNormal );
    const float3 normal = mul(mapNormal, vTBN);

    /*
    const float3 normal = normalize(float3(
        dot(viewSpaceTangent, mapNormal),
        dot(viewSpaceBitangent, mapNormal),
        dot(viewSpaceNormal, mapNormal)));
    const float3 normal = normalize(float3(
        dot(mapNormal, viewSpaceTangent),
        dot(mapNormal, viewSpaceBitangent),
        dot(mapNormal, viewSpaceNormal)));
    */
#else
    const float3 normal = input.viewspaceNormal;
#endif

    LN_SurfaceOutput surface;
    _LN_InitSurfaceOutput(normal, surface);

    //surface.Normal = input.Normal;
    
    // ★ライティングのコードはここに直接生成する (GBuffer生成などではマルチRT書き込みするため、戻り値も変えなければならない)
    // ・・・というより、ピクセルシェーダ全体を生成する。フラグメントの結合じゃダメ。
    

    // TODO: SurfaceShader を入れるのはこのあたり
    //surface.Albedo = ln_MaterialTexture.Sample(ln_MaterialTextureSamplerState, input.UV) * input.Color;
    surface.Albedo = tex2D(ln_MaterialTexture, input.UV) * ln_MaterialColor * input.Color;

    surface.Emission = (ln_MaterialEmissive.rgb * ln_MaterialEmissive.a);
    
    _lngs_PSOutput o;
    o.color0 = _LN_PS_ClusteredForward_Default(input.WorldPos, input.VertexPos, input.vInLightPosition, surface);
    o.color0.a *= surface.Albedo.a;
    o.color0 = LN_GetBuiltinEffectColor(o.color0);

    //o.color0 = float4(input.vInLightPosition.z / input.vInLightPosition.w, 0, 0, 1);
    //o.color0 = float4(input.viewspaceNormal, 1);
    //o.color0 = float4(input.Debug, 1);
    return o;
}

float4 _lngs_PS_UnLighting(_lngs_PSInput input) : COLOR0
{
    //return float4(1, 0, 0, 1);
    //return float4(input.Color.rgb, 1);
    //return float4(tex2D(ln_MaterialTexture, input.UV).rgb, 1);
    //return float4(ln_MaterialColor.rgb, 1);
    //return float4(input.UV, 0, 1);
    //return float4(tex2D(ln_BoneTexture, input.UV).rgb, 1);
    float4 c = tex2D(ln_MaterialTexture, input.UV) * ln_MaterialColor * input.Color;
    clip(c.a - 0.0001);

    //return float4(c);
    return LN_GetBuiltinEffectColor(c);
}

//------------------------------------------------------------------------------

technique Forward_Geometry
{
    Normal = Default;
    pass Pass1
    {
        VertexShader = _lngs_VS_ClusteredForward_Geometry;
        PixelShader     = _lngs_PS_ClusteredForward_Geometry;
    }
}
technique Forward_Geometry_NormalMap
{
    Normal = NormalMap;
    pass Pass1
    {
        VertexShader = _lngs_VS_ClusteredForward_Geometry;
        PixelShader     = _lngs_PS_ClusteredForward_Geometry;
    }
}

technique Forward_Geometry_StaticMesh_UnLighting
{
    Normal = Default;
    pass Pass1
    {
        VertexShader = _lngs_VS_ClusteredForward_Geometry;
        PixelShader     = _lngs_PS_UnLighting;
    }
}

technique Forward_Geometry_SkinnedMesh
{
    Normal = Default;
    pass Pass1
    {
        VertexShader = _lngs_VS_ClusteredForward_Geometry_SkinnedMesh;
        PixelShader     = _lngs_PS_ClusteredForward_Geometry;
    }
}

technique Forward_Geometry_SkinnedMesh_UnLighting
{
    Normal = Default;
    pass Pass1
    {
        VertexShader = _lngs_VS_ClusteredForward_Geometry_SkinnedMesh;
        PixelShader     = _lngs_PS_UnLighting;
    }
}

//END_HLSL


