
#include <Lumino.fxh>
#include "lib/Common.fxh"
#include "lib/BloomComposite.fxh"

//==============================================================================

sampler2D _occlusionMap;

cbuffer EffectSettings
{
    int _antialiasEnabled;
    int _ssaoEnabled;
    int _bloomEnabled;
    int _dofEnabled;
};

//==============================================================================
// Vertex shader

struct VSInput
{
    float3 pos : POSITION0;
    float2 uv  : TEXCOORD0;
};

struct VSOutput
{
    float4 pos : SV_POSITION;
    float4 uv  : TEXCOORD0;
};

VSOutput VSMain(VSInput input)
{
    VSOutput output;
    output.pos = float4(input.pos, 1.0);
    output.uv = float4(
        input.uv,
        input.uv.xy - (ln_Resolution.zw * (0.5 + FxaaSubpixShift)));
    return output;
}

//==============================================================================
// Pixel shader

struct PSInput
{
    float4 uv : TEXCOORD0;
};

#if 0   // バイラテラルフィルタ。TODO: 2パス必要なので後で。
#define KERNEL_RADIUS 15
uniform vec4 BlurParams;    // https://github.com/mebiusbox/ssao/blob/master/hbao.html

float CrossBilateralWeight(float r, float ddiff, inout float weightTotal) {
    float w = exp(-r*r*BlurParams.z) * (ddiff < BlurParams.w ? 1.0 : 0.0);
    weightTotal += w;
    return w;
}

// Perform a gaussian blur in one direction
float2 Blur(float2 uv, float2 texScale) {
    float2 centerCoord = uv;
    float weightTotal = 1.0;
    float2 aoDepth = tex2D(_occlusionMap, centerCoord).xy;
    float totalAO = aoDepth.x;
    float centerZ = aoDepth.y;
    // [unroll]
    for (int i=-KERNEL_RADIUS; i<KERNEL_RADIUS; i++) {
        float2 texCoord = centerCoord + (float(i)*texScale);
        float2 sampleAOZ = tex2D(_occlusionMap, texCoord).xy;
        float diff = abs(sampleAOZ.y - centerZ);
        float weight = CrossBilateralWeight(float(i), diff, weightTotal);
        totalAO += sampleAOZ.x * weight;
    }

    return float2(totalAO / weightTotal, centerZ);
}
#endif


float4 PSMain(PSInput input) : SV_TARGET0
{
    float4 result = float4(0, 0, 0, 1);

    //--------------------
    // FXAA
    if (_antialiasEnabled) {
        result = FxaaPixelShader(ln_MaterialTexture, input.uv, ln_Resolution.zw, ln_Resolution.zw);
    }
    else {
        result = tex2D(ln_MaterialTexture, input.uv.xy);
    }

    //--------------------
    // SSAO
    if (_ssaoEnabled) {
#if 1
        float4 c1 = LN_GaussianBlur5(_occlusionMap, ln_Resolution.zw, input.uv, float2(0.75, 0.0));
        float4 c2 = LN_GaussianBlur5(_occlusionMap, ln_Resolution.zw, input.uv, float2(0.0, 0.95));
        float4 ao = c1 * c2;
#else
        // 単純な平均ブラー
        float4 ao = LN_AverageBlur5x5(_occlusionMap, ln_Resolution.zw, input.uv.xy);
#endif
        result.rgb *= ao.rgb;
    }

    //--------------------
    // Bloom
    if (_bloomEnabled){
        result.rgb += Bloom(input.uv.xy);
    }

    //float c2 = Blur(input.uv, BlurParams.xy);
	//float4 result = c * c2* c2;
	//return c1 * c2;
	//return tex2D(_occlusionMap, input.uv);


    return result;
}

//==============================================================================
// Technique

technique Forward_Geometry_UnLighting
{
    pass Pass1
    {
        VertexShader = VSMain;
        PixelShader = PSMain;
    }
}

