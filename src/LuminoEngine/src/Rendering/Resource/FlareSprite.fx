
#include <Lumino.fxh>

sampler2D _LensflareOcclusionMap;

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
    output.Pos = mul(float4(input.Pos, 1.0f), ln_WorldViewProjection);
    output.UV = input.UV;
    return output;
}

//==============================================================================
// Pixel shader

struct PS_Input
{
    float2 UV : TEXCOORD0;
};

float4 PS_Main(PS_Input input) : SV_TARGET
{
    //return float4(input.UV, 0, 1);
    float4 color = tex2D(_LensflareOcclusionMap, input.UV);
    return color;
}

//==============================================================================
// Technique

technique Forward_Geometry
{
    pass Pass1
    {
        VertexShader = VS_Main;
        PixelShader = PS_Main;
    }
}
