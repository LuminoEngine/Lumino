
#include <Lumino.fxh>
//#include <LuminoForward.fxh>
#include <LuminoPBR.fxh>
#include <LuminoShadow.fxh>


//#define LN_USE_NORMALMAP
float3 LN_UnpackNormal(float4 packednormal)
{
    return (packednormal.xyz * 2.0) - 1.0;
}


//------------------------------------------------------------------------------
// Lib (Common)







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



struct VSOutput
{
    LN_VS_OUTPUT_DECLARE;
};





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
