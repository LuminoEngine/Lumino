#include <Lumino.fxh>
//sampler2D ln_MaterialTexture;

//==============================================================================
// Vertex shader

struct VS_Output
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD0;
};

VS_Output VS_Main(LN_VSInput input)
{
    VS_Output output;
    output.Pos = float4(input.Pos, 1.0);
    output.UV = input.UV;
    return output;
}

//==============================================================================
// Pixel shader

struct PS_Input
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD0;
};

float4 PS_Main(PS_Input input) : SV_TARGET
{
    return tex2D(ln_MaterialTexture, input.UV);
}

//==============================================================================
// Technique

technique Default
{
    ShadingModel = Unlit;
    pass Pass0
    {
        VertexShader = VS_Main;
        PixelShader = PS_Main;
    }
}
