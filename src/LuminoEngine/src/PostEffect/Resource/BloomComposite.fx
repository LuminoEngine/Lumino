
#include <Lumino.fxh>

#define NUM_MIPS 5

//==============================================================================
// Uniforms

sampler2D _BlurTexture1;
sampler2D _BlurTexture2;
sampler2D _BlurTexture3;
sampler2D _BlurTexture4;
sampler2D _BlurTexture5;
float _BloomStrength;
float _BloomRadius;
float _BloomFactors[NUM_MIPS];
float3 _BloomTintColors[NUM_MIPS];

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

struct PS_Input
{
    float2 UV : TEXCOORD0;
};

float LerpBloomFactor(float factor)
{
    float mirrorFactor = 1.2 - factor;
    return lerp(factor, mirrorFactor, _BloomRadius);
}

float4 PS_Main(PS_Input input) : SV_TARGET
{
    float4 result = tex2D(ln_MaterialTexture, input.UV);
    float4 col = _BloomStrength * ( LerpBloomFactor(_BloomFactors[0]) * float4(_BloomTintColors[0], 1.0) * tex2D(_BlurTexture1, input.UV) +
                                    LerpBloomFactor(_BloomFactors[1]) * float4(_BloomTintColors[1], 1.0) * tex2D(_BlurTexture2, input.UV) +
                                    LerpBloomFactor(_BloomFactors[2]) * float4(_BloomTintColors[2], 1.0) * tex2D(_BlurTexture3, input.UV) +
                                    LerpBloomFactor(_BloomFactors[3]) * float4(_BloomTintColors[3], 1.0) * tex2D(_BlurTexture4, input.UV) +
                                    LerpBloomFactor(_BloomFactors[4]) * float4(_BloomTintColors[4], 1.0) * tex2D(_BlurTexture5, input.UV) );
    result.rgb += (col.rgb * col.a);
    return result;
}

//==============================================================================
// Technique

technique Forward_Geometry_UnLighting
{
    pass Pass1
    {
        VertexShader = VS_Main;
        PixelShader = PS_Main;
    }
}

