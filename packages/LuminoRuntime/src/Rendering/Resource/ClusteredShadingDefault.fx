
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

//    MultiCompile = LN_USE_INSTANCING;
//    MultiCompile = LN_USE_ROUGHNESS_MAP;
technique Default
{
    MultiCompile = _,LN_USE_SKINNING;
    MultiCompile = _,LN_USE_NORMALMAP;
    MultiCompile = LN_SHADINGMODEL_DEFAULT,LN_SHADINGMODEL_UNLIT;
    pass Pass1
    {
        VertexShader = VSMain;
        PixelShader  = PSMain;
    }
}

