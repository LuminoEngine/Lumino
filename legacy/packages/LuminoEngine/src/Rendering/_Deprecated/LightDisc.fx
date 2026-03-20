
#include <Lumino.fxh>

//==============================================================================
// Vertex shader

struct VS_Input
{
    float3 Pos : POSITION;
};

struct VS_Output
{
    float4 Pos : SV_POSITION;
};

VS_Output VS_Main(LN_VSInput input)
{
    VS_Output output;
    output.Pos = mul(float4(input.Pos, 1.0f), ln_WorldViewProjection);
    return output;
}

//==============================================================================
// Pixel shader

float4 PS_Main() : SV_TARGET
{
    return float4(1, 1, 1, 1);
}

//==============================================================================
// Technique

technique LightDisc
{
    pass Pass1
    {
        VertexShader = VS_Main;
        PixelShader = PS_Main;
    }
}
