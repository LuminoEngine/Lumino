
#include <Lumino.fxh>

#define NUM_MIPS 5

//==============================================================================
// Uniforms

sampler2D _BlurTexture1;
sampler2D _BlurTexture2;
sampler2D _BlurTexture3;
sampler2D _BlurTexture4;
sampler2D _BlurTexture5;

cbuffer BloomCompositeParams
{
    /* [0] */   float4 _BloomTintColorsAndFactors[8/*NUM_MIPS*/];   // .a を factor として使用する
    /* [128] */ float _BloomStrength;
    /* [132] */ float _BloomRadius;
};  /* size:136 */

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
    float4 col = _BloomStrength * ( LerpBloomFactor(_BloomTintColorsAndFactors[0].a) * float4(_BloomTintColorsAndFactors[0].rgb, 1.0) * tex2D(_BlurTexture1, input.UV) +
                                    LerpBloomFactor(_BloomTintColorsAndFactors[1].a) * float4(_BloomTintColorsAndFactors[1].rgb, 1.0) * tex2D(_BlurTexture2, input.UV) +
                                    LerpBloomFactor(_BloomTintColorsAndFactors[2].a) * float4(_BloomTintColorsAndFactors[2].rgb, 1.0) * tex2D(_BlurTexture3, input.UV) +
                                    LerpBloomFactor(_BloomTintColorsAndFactors[3].a) * float4(_BloomTintColorsAndFactors[3].rgb, 1.0) * tex2D(_BlurTexture4, input.UV) +
                                    LerpBloomFactor(_BloomTintColorsAndFactors[4].a) * float4(_BloomTintColorsAndFactors[4].rgb, 1.0) * tex2D(_BlurTexture5, input.UV) );
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

