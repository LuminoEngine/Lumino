
#include <Lumino.fxh>

struct VSOutput
{
    LN_VS_OUTPUT_DECLARE;
};

//==============================================================================
// Vertex shader

VSOutput VSMain(LN_VSInput input)
{
    VSOutput output = (VSOutput)0;
    LN_ProcessVertex(input, output);
    return output;
}

//==============================================================================
// Pixel shader

float4 PSMain(VSOutput input): SV_TARGET
{
    float4 color = tex2D(ln_MaterialTexture, input.UV) * input.Color;
    clip(color.a - 0.0001); // Alpha test

    return LN_GetBuiltinEffectColor(color);
}

//==============================================================================
// Techniques

technique Forward_Geometry_UnLighting
{
    pass Pass0
    {
        VertexShader = VSMain;
        PixelShader  = PSMain;
    }
}

technique Forward_Geometry_UnLighting_Instancing
{
    Instancing = true;
    pass Pass0
    {
        VertexShader = VSMain;
        PixelShader  = PSMain;
    }
}
