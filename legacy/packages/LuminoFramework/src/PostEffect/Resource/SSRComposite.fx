
#include <Lumino.fxh>

//==============================================================================
// Uniforms


//==============================================================================
// Vertex shader

struct VS_Input
{
    float3 Pos : POSITION;
    float2 UV : TEXCOORD0;
};

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

sampler2D _ColorSampler;
sampler2D _SSRSampler;

struct PS_Input
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD0;
};

float4 PS_Main(PS_Input input) : SV_TARGET
{
    float4 color = tex2D(_ColorSampler, input.UV);
    float4 ssr = tex2D(_SSRSampler, input.UV);
    return float4(lerp(color.xyz, ssr.xyz, ssr.w), 1.0);
}

//==============================================================================
// Technique

technique Default
{
    ShadingModel = Unlit;
    pass Pass1
    {
        VertexShader = VS_Main;
        PixelShader = PS_Main;
    }
}

