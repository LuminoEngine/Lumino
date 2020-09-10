
#include <Lumino.fxh>
//#include <LuminoForward.fxh>
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
// Lib (Common)


// LN_GetPixelNormal
#ifdef LN_USE_NORMALMAP
#define LN_GetPixelNormal(input) LN_GetPixelNormalFromNormalMap(input.UV, input.vTangent, input.vBitangent, input.viewspaceNormal)
#else
#define LN_GetPixelNormal(input) input.viewspaceNormal
#endif






//------------------------------------------------------------------------------
// Lib (ClusteredForward)












float4 _LN_PS_ClusteredForward_Default(
    float3 worldPos,
    float3 vertexPos,
    float4 vInLightPosition,
    LN_Surface surface)
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


//==============================================================================
// Vertex shader

//-------------------------------------
// Mesh Processing
#ifdef LN_USE_SKINNING
    #define _LN_VS_PROCESS_PART_MESHPROCESSING(intput, output) \
        _LN_ProcessVertex_SkinnedMesh(intput, output.svPos, output.viewspaceNormal, output.UV, output.Color)
#else
    #define _LN_VS_PROCESS_PART_MESHPROCESSING(intput, output) \
        _LN_ProcessVertex_StaticMesh(intput, output.svPos, output.viewspaceNormal, output.UV, output.Color)
#endif

//-------------------------------------
// Normal Map
#ifdef LN_USE_NORMALMAP
    #define _LN_VARYING_DECLARE_NORMAL_MAP ; \
        float3 vTangent     : TEXCOORD12; \
        float3 vBitangent   : TEXCOORD13
    #define _LN_VS_PROCESS_PART_NORMALMAP(intput, output) \
        _LN_ProcessVertex_NormalMap(intput, output.viewspaceNormal, output.vTangent, output.vBitangent)
#else
    #define _LN_VARYING_DECLARE_NORMAL_MAP
    #define _LN_VS_PROCESS_PART_NORMALMAP
#endif

//-------------------------------------
// Clustered Lighting
#ifdef LN_LIGHTINGMETHOD_CLUSTERED
    #define _LN_VS_PROCESS_PART_LIGHTING(intput, output) \
        _LN_ProcessVertex_ClusteredForward(intput, output.worldPos, output.vertexPos, output.vInLightPosition)
#else
#endif



// vertexPos : 元の頂点データの位置情報
// worldPos  : World 空間上の位置 (ln_World による変換結果)
// viewPos   : View 空間上の位置 (ln_WorldView による変換結果)
#ifdef LN_LIGHTINGMETHOD_CLUSTERED
#define _LN_VARYING_DECLARE_LIGHTINGMETHOD ; \
    float3 vertexPos : POSITION0; \
    float3 worldPos  : POSITION1; \
    float3 viewPos   : POSITION2; \
    float4 vInLightPosition : POSITION3
#else
#define _LN_VARYING_DECLARE_LIGHTINGMETHOD
#endif

// Standard VS Output members.
#define LN_VS_OUTPUT_DECLARE \
    float4 svPos     : SV_POSITION; \
    float3 viewspaceNormal   : NORMAL0; \
    float2 UV        : TEXCOORD0; \
    float4 Color     : COLOR0 \
    _LN_VARYING_DECLARE_NORMAL_MAP \
    _LN_VARYING_DECLARE_LIGHTINGMETHOD

// Standard PS Input members.
#define LN_PS_INPUT_DECLARE \
    float3 viewspaceNormal   : NORMAL0; \
    float2 UV        : TEXCOORD0; \
    float4 Color     : COLOR0 \
    _LN_VARYING_DECLARE_NORMAL_MAP \
    _LN_VARYING_DECLARE_LIGHTINGMETHOD

struct VSOutput
{
    LN_VS_OUTPUT_DECLARE;
};

void _LN_ProcessVertex_StaticMesh(
    LN_VSInput input,
    out float4 outSVPos, out float3 outViewNormal, out float2 outUV, out float4 outColor)
{
    outSVPos = mul(float4(input.Pos, 1.0f), ln_WorldViewProjection);
    outViewNormal = mul(float4(input.Normal, 1.0f), ln_WorldViewIT).xyz;
    outUV = input.UV;
    outColor = input.Color;
}

void _LN_ProcessVertex_SkinnedMesh(
    LN_VSInput input,
    out float4 outSVPos, out float3 outViewNormal, out float2 outUV, out float4 outColor)
{
	LN_SkinningOutput local = LN_SkinningVertex(input.Pos, input.Normal, input.BlendWeight, input.BlendIndices);

	LN_VSOutput_Common o;
	outSVPos			= mul(float4(local.Position, 1.0f), ln_WorldViewProjection);
	outViewNormal		= mul(float4(local.Normal, 1.0f), ln_WorldViewIT).xyz;
	outUV			= input.UV + (float2(0.5, 0.5) / ln_Resolution.xy);
	outColor			= input.Color;
}

void _LN_ProcessVertex_ClusteredForward(
    LN_VSInput input,
    out float3 outWorldPos, out float3 outVertexPos, out float4 outvInLightPosition)
{
    outVertexPos = input.Pos;
    
    float4 pos = mul(float4(input.Pos, 1.0), ln_World);
    pos = mul(pos, ln_ViewProjection_Light0);
    outvInLightPosition = pos;
    
    outWorldPos = pos.xyz;
}

void _LN_ProcessVertex_NormalMap(
    LN_VSInput input, float3 viewNormal,
    out float3 outTangent, out float3 outBitangent)
{
    const float3 objectTangent = input.tangent.xyz;
    const float3 transformedTangent = ( ln_WorldView * float4( objectTangent, 0.0 ) ).xyz;
    const float3 tangent = normalize( transformedTangent );
    outTangent = tangent;
    outBitangent= normalize( cross( viewNormal, tangent) * input.tangent.w );
    //output.Debug = objectTangent;
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
}

#define LN_ProcessVertex(input, output) \
    _LN_VS_PROCESS_PART_MESHPROCESSING(vsi, output); \
    _LN_VS_PROCESS_PART_LIGHTING(vsi, output); \
    _LN_VS_PROCESS_PART_NORMALMAP(vsi, output);

VSOutput VSMain(LN_VSInput vsi)
{
    VSOutput output;
    LN_ProcessVertex(vsi, output);
    return output;
}

//==============================================================================
// Pixel shader

struct _lngs_PSInput
{
    LN_PS_INPUT_DECLARE;

#ifdef LN_USE_NORMALMAP
    //float3    vTangent    : TEXCOORD12;
    //float3    vBitangent    : TEXCOORD13;
    float3    Debug    : TEXCOORD14;
#endif
};

float3 LN_GetPixelNormalFromNormalMap(float2 texUV, float3 viewSpaceTangent, float3 viewSpaceBitangent, float3 viewSpaceNormal)
{
    const float3 tangent = normalize(viewSpaceTangent);
    const float3 bitangent = normalize(viewSpaceBitangent);
    const float3 normal = normalize(viewSpaceNormal);
    const float3 mapNormal = LN_UnpackNormal(tex2D(ln_NormalMap, texUV));

    const float3x3 TBN = float3x3(tangent, bitangent, normal);
    return mul(mapNormal, TBN);

    /*
    const float3 normal = normalize(float3(
        dot(tangent, mapNormal),
        dot(bitangent, mapNormal),
        dot(normal, mapNormal)));
    const float3 normal = normalize(float3(
        dot(mapNormal, tangent),
        dot(mapNormal, bitangent),
        dot(mapNormal, normal)));
    */
}

float4 _lngs_PS_ClusteredForward_Geometry(_lngs_PSInput input, LN_Surface surface)
{
    float4 color = _LN_PS_ClusteredForward_Default(input.worldPos, input.vertexPos, input.vInLightPosition, surface);
    color.a *= surface.Albedo.a;
    color = LN_GetBuiltinEffectColor(color);
    return color;
}

float4 LN_ProcessPixel(_lngs_PSInput input, LN_Surface surface) : SV_TARGET0
{
#if defined(LN_SHADINGMODEL_DEFAULT)
    return _lngs_PS_ClusteredForward_Geometry(input, surface);
#elif defined(LN_SHADINGMODEL_UNLIT)
    return LN_GetBuiltinEffectColor(surface.Albedo);
#else
#error "Invalid ShadingModel."
#endif
}

float4 PSMain(_lngs_PSInput input) : SV_TARGET0
{
    LN_Surface surface;
    LN_ProcessSurface(input, surface);

    return LN_ProcessPixel(input, surface);
}

//==============================================================================
// technique

technique Forward_Geometry
{
    Normal = Default;
    pass Pass1
    {
        VertexShader = VSMain;
        PixelShader  = PSMain;
    }
}
technique Forward_Geometry_NormalMap
{
    Normal = NormalMap;
    pass Pass1
    {
        VertexShader = VSMain;
        PixelShader  = PSMain;
    }
}

technique Forward_Geometry_StaticMesh_UnLighting
{
    ShadingModel = Unlit;
    Normal = Default;
    pass Pass1
    {
        VertexShader = VSMain;
        PixelShader  = PSMain;
    }
}

technique Forward_Geometry_SkinnedMesh
{
    VertexProcessing = Skinned;
    Normal = Default;
    pass Pass1
    {
        VertexShader = VSMain;
        PixelShader  = PSMain;
    }
}

technique Forward_Geometry_SkinnedMesh_UnLighting
{
    ShadingModel = Unlit;
    VertexProcessing = Skinned;
    Normal = Default;
    pass Pass1
    {
        VertexShader = VSMain;
        PixelShader  = PSMain;
    }
}
