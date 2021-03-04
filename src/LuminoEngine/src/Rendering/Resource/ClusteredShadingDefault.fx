
#include <Lumino.fxh>

//==============================================================================
// Vertex shader

struct VSOutput
{
    LN_VS_OUTPUT_DECLARE;
    float3    MorphTargetPos0    : TEXCOORD9;
};

VSOutput VSMain(LN_VSInput vsi)
{
    VSOutput output;
    LN_ProcessVertex(vsi, output);
    output.MorphTargetPos0 = vsi.MorphTargetPos0;
    
    return output;
}

//==============================================================================
// Pixel shader

struct PSInput
{
    LN_PS_INPUT_DECLARE;
    float3    MorphTargetPos0    : TEXCOORD9;
};

float4 PSMain(PSInput input) : SV_TARGET0
{
    LN_Surface surface;
    LN_ProcessSurface(input, surface);


return float4(ln_MorphWeights.xyz, 1.0);
return float4(input.MorphTargetPos0, 1.0);

    return LN_ProcessPixel(input, surface);
}

//==============================================================================
// Techniques

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
