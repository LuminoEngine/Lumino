
#include <Lumino.fxh>
#include "ShaderLib/Common.fxh"
#include "ShaderLib/BloomComposite.fxh"
#include "ShaderLib/SSRComposite.fxh"
#include "ShaderLib/Tonemap.part.fxh"
#include "ShaderLib/Vignette.part.fxh"

#if 0
#define FXAA_GRAY_AS_LUMA 1
//#define FXAA_GATHER4_ALPHA 0
#define FXAA_PC 1
#define FXAA_HLSL_5 1
#define FxaaTexGreen4(t, p) tex2D(t, p).g
#define FxaaTexOffGreen4(t, p, o) tex2D(t, p + float2(ln_Resolution.z * ((float)o.x), ln_Resolution.w * ((float)o.y))).g
//#define FxaaTexTop(t, p) tex2Dlod(t, float4(p, 0.0, 0.0))
//#define FxaaTexOff(t, p, o, r) tex2Dlod(t, float4(p + float2(ln_Resolution.z * ((float)o.x), ln_Resolution.w * ((float)o.y), 0.0, 0.0)))

#define FxaaTexTop(t, p) tex2Dlod(t, float4(p, 0.0, 0.0))
#define FxaaTexOff(t, p, o, r) tex2Dlod(t, float4(p + (o * r), 0, 0))
#endif


#define FXAA_HLSL_5 0
#define FXAA_HLSL_3 1
#define FXAA_GATHER4_ALPHA 0
#include "ShaderLib/FXAA.hlsli"


//==============================================================================

sampler2D _occlusionMap;

cbuffer EffectSettings
{
    float4 _vignetteColor;
    float4 _vignettePosition;
    float4 _vignetteSettings;
    float4 _blendColor;
    float4 _colorTone;
    int _antialiasEnabled;
    int _ssrEnabled;
    int _ssaoEnabled;
    int _bloomEnabled;
    int _dofEnabled;
    int _tonemapEnabled;
    int _vignetteEnabled;
    int _gammaEnabled;
    float _focusedLinearDepth;// = 10.0 / 100.0;
};



sampler2D _depthTexture;
sampler2D _dofTexture0;
sampler2D _dofTexture1;
sampler2D _dofTexture2;
sampler2D _dofTexture3;
static const int MaxDOFMips = 4;

struct VSInput
{
    float3 pos : POSITION0;
    float2 uv  : TEXCOORD0;
};

struct PSInput
{
    float4 pos : SV_POSITION;
    float4 uv  : TEXCOORD0;
};

//==============================================================================
// Vertex shader

PSInput VSMain(VSInput input)
{
    PSInput output;
    output.pos = float4(input.pos, 1.0);
    output.uv = float4(
        input.uv,
        input.uv.xy - (ln_Resolution.zw * (0.5 + FxaaSubpixShift)));
    return output;
}

//==============================================================================
// Pixel shader

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


float4 GaussianFilteredColor5x5(sampler2D tex, float2 invTextureSize, float2 uv) {
    float dx = invTextureSize.x;
    float dy = invTextureSize.y;
    float4 center = tex2D(tex, uv);
    float4 ret =
        tex2D(tex, uv + float2(-dx * 2, -dy * 2)) +
        tex2D(tex, uv + float2(-dx * 1, -dy * 2)) * 4 +
        tex2D(tex, uv + float2(-dx * 0, -dy * 2)) * 6 +
        tex2D(tex, uv + float2(dx * 1, -dy * 2)) * 4 +
        tex2D(tex, uv + float2(dx * 2, -dy * 2)) +

        tex2D(tex, uv + float2(-dx * 2, -dy * 1)) * 4 +
        tex2D(tex, uv + float2(-dx * 1, -dy * 1)) * 16 +
        tex2D(tex, uv + float2(-dx * 0, -dy * 1)) * 24 +
        tex2D(tex, uv + float2(dx * 1, -dy * 1)) * 16 +
        tex2D(tex, uv + float2(dx * 2, -dy * 1)) * 4 +

        tex2D(tex, uv + float2(-dx * 2, dy * 0)) * 6 +
        tex2D(tex, uv + float2(-dx * 1, dy * 0)) * 24 +
        center * 36 +
        tex2D(tex, uv + float2(dx * 1, dy * 0)) * 24 +
        tex2D(tex, uv + float2(dx * 2, dy * 0)) * 6 +

        tex2D(tex, uv + float2(-dx * 2, dy * 1)) * 4 +
        tex2D(tex, uv + float2(-dx * 1, dy * 1)) * 16 +
        tex2D(tex, uv + float2(-dx * 0, dy * 1)) * 24 +
        tex2D(tex, uv + float2(dx * 1, dy * 1)) * 16 +
        tex2D(tex, uv + float2(dx * 2, dy * 1)) * 4 +

        tex2D(tex, uv + float2(-dx * 2, dy * 2)) +
        tex2D(tex, uv + float2(-dx * 1, dy * 2)) * 4 +
        tex2D(tex, uv + float2(-dx * 0, dy * 2)) * 6 +
        tex2D(tex, uv + float2(dx * 1, dy * 2)) * 4 +
        tex2D(tex, uv + float2(dx * 2, dy * 2));
    return float4((ret.rgb / 256.0f), ret.a);

}



float4 PSMain(PSInput input) : SV_TARGET0
{
    float4 result = float4(0, 0, 0, 1);

    result = tex2D(ln_MaterialTexture, input.uv.xy);

    //--------------------
    // SSR
    if (_ssrEnabled) {
        result = CompositeSSR(result, input.uv.xy);
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
    // FXAA
    /*
    if (_antialiasEnabled) {
        result = FxaaPixelShader(ln_MaterialTexture, input.uv, ln_Resolution.zw, ln_Resolution.zw);
    }
    else {
    }
    */
    if (_antialiasEnabled) {
#if 0
        // TODO: 今のところ src と dst のサイズが同じなのでこれでいいが、本当は ln_MaterialTexture のサイズを取った方が良い
        const float dx = ln_Resolution.z; // 1.0 / ln_Resolution.x
        const float dy = ln_Resolution.w; // 1.0 / ln_Resolution.z
        
        FxaaTex InputFXAATex = ln_MaterialTexture;//{ smp, tex };
        float3 aa = FxaaPixelShader(
            input.uv.xy,                            // FxaaFloat2 pos,
            FxaaFloat4(0.0f, 0.0f, 0.0f, 0.0f),        // FxaaFloat4 fxaaConsolePosPos,
            InputFXAATex,                            // FxaaTex tex,
            InputFXAATex,                            // FxaaTex fxaaConsole360TexExpBiasNegOne,
            InputFXAATex,                            // FxaaTex fxaaConsole360TexExpBiasNegTwo,
            float2(dx, dy),                            // FxaaFloat2 fxaaQualityRcpFrame,
            FxaaFloat4(0.0f, 0.0f, 0.0f, 0.0f),        // FxaaFloat4 fxaaConsoleRcpFrameOpt,
            FxaaFloat4(0.0f, 0.0f, 0.0f, 0.0f),        // FxaaFloat4 fxaaConsoleRcpFrameOpt2,
            FxaaFloat4(0.0f, 0.0f, 0.0f, 0.0f),        // FxaaFloat4 fxaaConsole360RcpFrameOpt2,
            0.75f,                                    // FxaaFloat fxaaQualitySubpix,  [0..1], default 0.75
            0.166f,                                    // FxaaFloat fxaaQualityEdgeThreshold,   [0.125..0.33], default 0.166
            0.0833f,                                // FxaaFloat fxaaQualityEdgeThresholdMin,    //0.0625; // ?
            0.0f,                                    // FxaaFloat fxaaConsoleEdgeSharpness,
            0.0f,                                    // FxaaFloat fxaaConsoleEdgeThreshold,
            0.0f,                                    // FxaaFloat fxaaConsoleEdgeThresholdMin,
            FxaaFloat4(0.0f, 0.0f, 0.0f, 0.0f)        // FxaaFloat fxaaConsole360ConstDir,
        ).rgb;
        result.rgb = aa;
        //result = float4(1, 0, 0, 1);
#endif
    }

    //--------------------
    // Bloom
    if (_bloomEnabled){
        result.rgb += Bloom(input.uv.xy);
    }

    //--------------------
    // DOF
    if (_dofEnabled) {
        float2 uv = input.uv.xy;

        //sampler2D dofTextures[MaxDOFMips] = { _dofTexture0, _dofTexture1, _dofTexture2, _dofTexture3 };


        float linearDepth = tex2D(_depthTexture, uv).g;
        float d = distance(_focusedLinearDepth, linearDepth);

        float t = pow(d, 1.05);//1.25);

        //return float4(t, 0, 0, 1);

        //t *= 0.5;

        t *= (float)MaxDOFMips;
        float alpha, mipNo;
        alpha = modf(t, mipNo);

        int dofIndex = (int)mipNo;
        float3 colA, colB;
        // TODO: TextureArray でうけとりたい。
        if (dofIndex == 0) {
            colA = result.rgb;
            colB = GaussianFilteredColor5x5(_dofTexture0, ln_Resolution.zw, uv);
        }
        else if (dofIndex == 1) {
            colA = GaussianFilteredColor5x5(_dofTexture0, ln_Resolution.zw, uv);
            colB = GaussianFilteredColor5x5(_dofTexture1, ln_Resolution.zw, uv);
        }
        else if (dofIndex == 2) {
            colA = GaussianFilteredColor5x5(_dofTexture1, ln_Resolution.zw, uv);
            colB = GaussianFilteredColor5x5(_dofTexture2, ln_Resolution.zw, uv);
        }
        else {
            colA = GaussianFilteredColor5x5(_dofTexture2, ln_Resolution.zw, uv);
            colB = GaussianFilteredColor5x5(_dofTexture3, ln_Resolution.zw, uv);
        }
        result.rgb = lerp(colA, colB, alpha);
    }

    //--------------------
    // Tonemap
    if (_tonemapEnabled) {
        result.rgb = Tonemap(result.rgb);
    }

    //--------------------
    // Vignette
    if (_vignetteEnabled) {
        result.rgb = Vignette(result.rgb, input.uv.xy, _vignetteColor, _vignettePosition.xy, _vignetteSettings);
    }
    
    //--------------------
    // Color Effect

    // apply blend color.
    result.rgb = lerp(result.rgb, _blendColor.rgb, _blendColor.a);

    // apply tone. (NTSC Coef method)
    result.rgb = LN_CalculateToneColor(float4(result.rgb, 1.0), _colorTone).rgb;

    //--------------------
    // Gamma
    if (_gammaEnabled) {
        result.rgb = pow(result.rgb, 1.0 / 2.2);
    }

    //float c2 = Blur(input.uv, BlurParams.xy);
    //float4 result = c * c2* c2;
    //return c1 * c2;
    //return tex2D(_occlusionMap, input.uv);


    return result;
}

//==============================================================================
// Technique

technique Default
{
    ShadingModel = Unlit;
    pass Pass1
    {
        VertexShader = VSMain;
        PixelShader = PSMain;
    }
}

