

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
    /* [0] */   float4 _BloomTintColorsAndFactors[8];   // .a を factor として使用する。実際は NUM_MIPS までしか使わないが alignment の計算がつらいのでキリのいいところにしておく。
    /* [128] */ float _BloomStrength;
    /* [132] */ float _BloomRadius;
};  /* size:136 */

//==============================================================================
// Pixel shader

float LerpBloomFactor(float factor)
{
    float mirrorFactor = 1.2 - factor;
    return lerp(factor, mirrorFactor, _BloomRadius);
}

float3 Bloom(float2 uv)
{
    float4 col = _BloomStrength * ( LerpBloomFactor(_BloomTintColorsAndFactors[0].a) * float4(_BloomTintColorsAndFactors[0].rgb, 1.0) * tex2D(_BlurTexture1, uv) +
                                    LerpBloomFactor(_BloomTintColorsAndFactors[1].a) * float4(_BloomTintColorsAndFactors[1].rgb, 1.0) * tex2D(_BlurTexture2, uv) +
                                    LerpBloomFactor(_BloomTintColorsAndFactors[2].a) * float4(_BloomTintColorsAndFactors[2].rgb, 1.0) * tex2D(_BlurTexture3, uv) +
                                    LerpBloomFactor(_BloomTintColorsAndFactors[3].a) * float4(_BloomTintColorsAndFactors[3].rgb, 1.0) * tex2D(_BlurTexture4, uv) +
                                    LerpBloomFactor(_BloomTintColorsAndFactors[4].a) * float4(_BloomTintColorsAndFactors[4].rgb, 1.0) * tex2D(_BlurTexture5, uv) );
    return col.rgb * col.a;
}
