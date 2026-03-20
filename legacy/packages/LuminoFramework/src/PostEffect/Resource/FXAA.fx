
#include <Lumino.fxh>

//==============================================================================
// Vertex shader

static const float FxaaSubpixShift = 1.0/4.0;

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

#define RcpMaterialTextureSize ln_Resolution.zw
#define RcpMaterialTextureSize2 (ln_Resolution.zw * 0.5)

struct PSInput
{
    float4 uv : TEXCOORD0;
};

float4 FxaaTexOff(sampler2D tex, float2 pos, float2 offset)
{
    return tex2D(tex, pos + offset * RcpMaterialTextureSize + RcpMaterialTextureSize2);
}

float FxaaLuma(float3 rgb)
{
    const float3 luma = float3(0.299, 0.587, 0.114);
    return dot(rgb, luma);
}

float4 FxaaTexLod(sampler2D tex, float2 pos)
{
    return tex2D(tex, pos + RcpMaterialTextureSize2);
}

float4 FxaaPixelShader(sampler2D tex, float4 posPos, float2 rcpFrame)
{
    // NOTE:
    //     NW N NE
    //     W  M  E
    //     SW S SE

    float3 rgbNW = FxaaTexOff(tex, posPos.zw, float2(0, 0)).xyz;
    float3 rgbNE = FxaaTexOff(tex, posPos.zw, float2(1, 0)).xyz;
    float3 rgbSW = FxaaTexOff(tex, posPos.zw, float2(0, 1)).xyz;
    float3 rgbSE = FxaaTexOff(tex, posPos.zw, float2(1, 1)).xyz;
    float3 rgbM  = FxaaTexLod(tex, posPos.xy).xyz;

    float lumaNW = FxaaLuma(rgbNW);
    float lumaNE = FxaaLuma(rgbNE);
    float lumaSW = FxaaLuma(rgbSW);
    float lumaSE = FxaaLuma(rgbSE);
    float lumaM  = FxaaLuma(rgbM);

    float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
    float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));

    float dirSwMinusNe = lumaSW - lumaNE;
    float dirSeMinusNw = lumaSE - lumaNW;

    float2 dir = float2(dirSwMinusNe + dirSeMinusNw, dirSwMinusNe - dirSeMinusNw);

    const float FxaaReduceMin = 1.0/128.0;
    const float FxaaReduceMul = 1.0/8.0;
    const float FxaaSpanMax = 8.0;

    float dirReduce = max(
        (lumaNW + lumaNE + lumaSW + lumaSE) * (0.25 * FxaaReduceMul),
        FxaaReduceMin);

    float rcpDirMin = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);
    dir = clamp(mul(dir, rcpDirMin), float2(-FxaaSpanMax, -FxaaSpanMax), float2(FxaaSpanMax, FxaaSpanMax)) * rcpFrame.xy;

    float3 rgbN1 = FxaaTexLod(tex, posPos.xy + dir * (1.0/3.0 - 0.5)).xyz;
    float3 rgbP1 = FxaaTexLod(tex, posPos.xy + dir * (2.0/3.0 - 0.5)).xyz;

    float3 rgbN2 = FxaaTexLod(tex, posPos.xy + dir * (0.0/3.0 - 0.5)).xyz;
    float3 rgbP2 = FxaaTexLod(tex, posPos.xy + dir * (3.0/3.0 - 0.5)).xyz;

    float3 rgbA = (rgbN1 + rgbP1);
    float3 rgbB = ((rgbN2 + rgbP2) * 0.25) + (rgbA * 0.25);

    float lumaB = FxaaLuma(rgbB);
    if ((lumaB < lumaMin) || (lumaB > lumaMax)) {
        rgbB.xyz = rgbA.xyz * 0.5;
    }

    return float4(rgbB, 1.0);
}

float4 PSMain(PSInput input) : SV_TARGET0
{
    return FxaaPixelShader(ln_MaterialTexture, input.uv, ln_Resolution.zw);
}

//==============================================================================
// Technique

technique Forward_Geometry_UnLighting
{
    ShadingModel = Unlit;
    pass Pass1
    {
        VertexShader = VSMain;
        PixelShader = PSMain;
    }
}

