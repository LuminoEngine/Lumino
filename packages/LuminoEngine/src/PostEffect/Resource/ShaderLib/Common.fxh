

//==============================================================================
// Blurs

/**
 * 周辺 5*5 ピクセルの平均で計算するブラー関数です。
 */
float4 LN_AverageBlur5x5(sampler2D s, float2 invSize, float2 uv)
{
    float2 texelSize = invSize;
    float4 result = float4(0.0, 0.0, 0.0, 0.0);

    for (int i=-2; i<=2; i++) {
        for (int j=-2; j<=2; j++) {
            float2 offset = ((float2(float(i),float(j)))*texelSize);
            result += tex2D(s, uv + offset);
        }
    }

    return result / (5.0*5.0);
}

/**
 * 5 点サンプリングのシンプルなガウスブラー関数です。
 *
 * uv で指定した中心点の 1 点。
 * direction(px 単位) 方向と、その逆方向に 2 点ずつ。
 */
float4 LN_GaussianBlur5(sampler2D s, float2 invSize, float2 uv, float2 direction) {
    float4 color = float4(0, 0, 0, 0);
    float2 off1 = float2(1.3846153846, 1.3846153846) * direction;
    float2 off2 = float2(3.2307692308, 3.2307692308) * direction;
    color += tex2D(s, uv) * 0.2270270270;
    color += tex2D(s, uv + (off1 * invSize)) * 0.3162162162;
    color += tex2D(s, uv - (off1 * invSize)) * 0.3162162162;
    color += tex2D(s, uv + (off2 * invSize)) * 0.0702702703;
    color += tex2D(s, uv - (off2 * invSize)) * 0.0702702703;
    return color;
}


//==============================================================================
// FXAA

static const float FxaaSubpixShift = 1.0/4.0;  // Use from VS

float4 FxaaTexOff(sampler2D tex, float2 invTexSize, float2 pos, float2 offset)
{
    return tex2D(tex, pos + offset * invTexSize + (invTexSize * 0.5));
}

float FxaaLuma(float3 rgb)
{
    const float3 luma = float3(0.299, 0.587, 0.114);
    return dot(rgb, luma);
}

float4 FxaaTexLod(sampler2D tex, float2 invTexSize, float2 pos)
{
    return tex2D(tex, pos + (invTexSize * 0.5));
}

float4 FxaaPixelShader(sampler2D tex, float4 posPos, float2 rcpFrame, float2 invTexSize)
{
    // NOTE:
    //     NW N NE
    //     W  M  E
    //     SW S SE

    float3 rgbNW = FxaaTexOff(tex, invTexSize, posPos.zw, float2(0, 0)).xyz;
    float3 rgbNE = FxaaTexOff(tex, invTexSize, posPos.zw, float2(1, 0)).xyz;
    float3 rgbSW = FxaaTexOff(tex, invTexSize, posPos.zw, float2(0, 1)).xyz;
    float3 rgbSE = FxaaTexOff(tex, invTexSize, posPos.zw, float2(1, 1)).xyz;
    float3 rgbM  = FxaaTexLod(tex, invTexSize, posPos.xy).xyz;

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

    float3 rgbN1 = FxaaTexLod(tex, invTexSize, posPos.xy + dir * (1.0/3.0 - 0.5)).xyz;
    float3 rgbP1 = FxaaTexLod(tex, invTexSize, posPos.xy + dir * (2.0/3.0 - 0.5)).xyz;

    float3 rgbN2 = FxaaTexLod(tex, invTexSize, posPos.xy + dir * (0.0/3.0 - 0.5)).xyz;
    float3 rgbP2 = FxaaTexLod(tex, invTexSize, posPos.xy + dir * (3.0/3.0 - 0.5)).xyz;

    float3 rgbA = (rgbN1 + rgbP1);
    float3 rgbB = ((rgbN2 + rgbP2) * 0.25) + (rgbA * 0.25);

    float lumaB = FxaaLuma(rgbB);
    if ((lumaB < lumaMin) || (lumaB > lumaMax)) {
        rgbB.xyz = rgbA.xyz * 0.5;
    }

    return float4(rgbB, 1.0);
}
