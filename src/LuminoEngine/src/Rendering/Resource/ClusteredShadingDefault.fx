
#include <Lumino.fxh>

//==============================================================================
// Vertex shader

struct VSOutput
{
    LN_VS_OUTPUT_DECLARE;
};

VSOutput VSMain(LN_VSInput vsi)
{
    VSOutput output;
    LN_ProcessVertex(vsi, output);
    return output;
}

//==============================================================================
// Pixel shader

struct PSInput
{
    LN_PS_INPUT_DECLARE;
};

float4 PSMain(PSInput input) : SV_TARGET0
{
    LN_Surface surface;
    LN_ProcessSurface(input, surface);
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
