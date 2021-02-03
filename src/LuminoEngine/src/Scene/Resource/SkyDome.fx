
#include <Lumino.fxh>

const float ln_Time = 0.0;

//------------------------------------------------------------------------------
// External params

const float _GlobalTimeFloatInput = 0.0;



//------------------------------------------------------------------------------
// Internal params

const float _GlobalTimeSpeed = 1.0;

const float _TimeInput  = 1.0;  // 1.0

// CPU側で計算される、TimeOfDay による時間帯における空の色。
// 地平の色には影響しない。雲の色には、雲の濃さによって影響する。
float4 _Curve_BackGroundSkyDomeColor; // = float4(0.3, 0.8, 1.25, 1.0);     // .w ignored
const float4 _Background_SkyDome_Color = float4(1.0, 1.0, 1.0, 1.0);     // .w ignored

float4 _Curve_BackGroundHorizonColor;// = float4(1.11, 1.655734, 1.85, 1.0);     // .w ignored
const float4 _Background_Horizon_Color = float4(1.0, 1.0, 1.0, 1.0);     // .w ignored

float4 _Curve_BaseCloudColorAndIntensity;// = float4(0.760417, 0.824326, 1.0, 0.0);
const float4 _AllCloudsOverlayColor = float4(1.0, 1.0, 1.0, 1.0);
const float4 _BackgroundOverlayColor = float4(1.0, 1.0, 1.0, 1.0);

const float _Zenith_Area_Falloff_Intensity = 7.0;   // 0.0 ~ 10.0


//------------------------------------------------------------------------------
// Preset params

float _Main_Clouds_Falloff_Intensity;// = 1.5;
float _Second_Clouds_Falloff_Intensity;// = 0.8;
float _AllCloudsFalloffIntensity;// = 1.0;   // 0.9~
float _AllCloudsIntensity;// = 1.0;

/** MainCloud の強度 */
const float _MainCloudsIntensity = 0.85;
/** SecondCloud の強度 */
const float _SecondCloudsIntensity = 0.5;   // 0.0 ~ 1.0
/** ThirdCloud の強度 */
float _ThirdCloudsIntensity;// = 0.0;    // デフォルトでは ThirdCloud は使わない、ということ


const float _DetailCloudsIntensity = 0.35;  // 0.0~1.0
const float4 _DetailCloudsColor = float4(0.75, 0.75, 0.75, 1.0);

const float _SunDisappearInHorizon = 1.0; // 0.0~1.0

float4 _Curve_AllOverlayColor;// = float4(1.0, 1.0, 1.0, 0.0);
const float4 _GlobalOverlayColor = float4(1.0, 1.0, 1.0, 1.0);      // .w ignored
const float4 _GlobalOverlayColorBP = float4(1.0, 1.0, 1.0, 1.0);      // .w ignored
const float _GlobalDesaturationIntensity = 0.0;

sampler2D _thirdCloudTexture;
sampler2D _detailCloudTexture;
sampler2D _secondCloudPowerMap;

sampler2D _mainCloudsTexture;
sampler2D _secondCloudsTexture;


/*
 * Flow
 * - 空の色を作る
 * - 地平の色を作る
 * - ↑でグラデーションを作る
 * - ↑に雲を lerp で混ぜる
 * - ApplyGlobalOverlay で最終調整
 */


//==============================================================================

// https://docs.unrealengine.com/ja/Engine/Rendering/Materials/ExpressionReference/Coordinates/index.html#panner
float2 Panner(float2 coordinate, float time)
{
    return float2(coordinate.x + time, coordinate.y);
}

// https://docs.unrealengine.com/ja/Engine/Rendering/Materials/ExpressionReference/Color/index.html?utm_source=editor&utm_medium=docs&utm_campaign=rich_tooltips#desaturation
float3 Desaturation(float3 color, float fraction)
{
    const float4 luminanceFactors = float4(0.3f, 0.59f, 0.11f, 0.0f);
    const float3 grey = dot(color, luminanceFactors.rgb);
    return lerp(color, grey, fraction);
}

float GetPannerOffsetTime(float globalTime, float timeSpeed)
{
    // Change Time Speed while changing TimeFloat.
    const float time = (globalTime * 5.0) + ln_Time;

    // Global Time Speed
    const float speed = (timeSpeed * 0.55);

    return time * speed;
}

// input となる _Main_Clouds_Falloff_Intensity のデフォルトは 1.5 なので↓のようなグラフとなる。
// (1 - x)^1.5
// https://www.geogebra.org/graphing?lang=ja
float SkyDomeAlpha(float2 texUV, float input)
{
    return saturate(pow(1.0 - texUV.y, input));
}

/**
 * return:
 *   _SunDisappearInHorizon = 1.0 : 地平で 0.0、中天で 25.0 を返す
 *   _SunDisappearInHorizon = 0.0 : 常に 1.0
 */
float TopPartAlpha(float2 texUV)
{
    return lerp(1.0, saturate((1.0 - texUV.y) * 25.0), _SunDisappearInHorizon);
}

// 最終出力
float3 ApplyGlobalOverlay(float3 input)
{
    const float3 allOverlay = (_Curve_AllOverlayColor.rgb * (_Curve_AllOverlayColor.a + 1.0));
    const float3 color = input * allOverlay * _GlobalOverlayColor.rgb;
    return Desaturation(color, _GlobalDesaturationIntensity) * _GlobalOverlayColorBP.rgb;
}

/**
 * texUV: y は、中天が 0, 地平で 1 となる。下端は 1。地平 ~ 下端はすべて 1.
 * outAllCloudsFalloffIntensity: 減衰強度
 */
float3 CalculateBaseClouds(
    float2 texUV,
    float globalTime,
    out float outAllCloudsIntensity,
    out float outAllCloudsFalloffIntensity)
{
    outAllCloudsIntensity = _AllCloudsIntensity;
    outAllCloudsFalloffIntensity = _AllCloudsFalloffIntensity;

    // Main Cloud
    const float4 cloudA = tex2D(_mainCloudsTexture, Panner(texUV, globalTime));
    const float4 cloudB = tex2D(ln_MaterialTexture, Panner(texUV, globalTime));
    const float mainAlpha = SkyDomeAlpha(texUV, _Main_Clouds_Falloff_Intensity);
    const float3 mainCloud = lerp((cloudA.rgb * cloudB.rgb) * 0.05, cloudB.rgb, mainAlpha);

    // Second Cloud
    const float4 secondCloudA = tex2D(_secondCloudsTexture, Panner(texUV, globalTime));
    const float4 secondCloudB = tex2D(_secondCloudPowerMap, Panner(texUV, globalTime));
    const float secondAlpha = SkyDomeAlpha(texUV, _Second_Clouds_Falloff_Intensity);
    const float3 secondcloud = lerp(pow(secondCloudA.rgb, 1.35) * 0.35, secondCloudA.rgb * secondCloudB.r, secondAlpha);

    // Third Cloud
    const float3 thirdCloudA = tex2D(_thirdCloudTexture, Panner(texUV, globalTime)).rgb;

    const float3 mergedClouds =
        (mainCloud * _MainCloudsIntensity) +
        (secondcloud * _SecondCloudsIntensity) +
        (thirdCloudA * _ThirdCloudsIntensity);

    const float3 baseCloudColor = _Curve_BaseCloudColorAndIntensity.rgb;
    const float intensitySource = _Curve_BaseCloudColorAndIntensity.a;
    const float3 cloudsRGBOutput =
        (pow(mergedClouds, _AllCloudsFalloffIntensity) * baseCloudColor) *
        (_AllCloudsOverlayColor.rgb * lerp(1, 2.2, intensitySource));

    return cloudsRGBOutput * _AllCloudsIntensity;
}

float3 CalculateSkyDome(float2 texUV)
{
    // Global Time
    const float globalTime = GetPannerOffsetTime(_GlobalTimeFloatInput, _GlobalTimeSpeed);

    float allCloudsIntensity;
    float allCloudsFalloffIntensity;
    const float3 cloudResult = CalculateBaseClouds(texUV, globalTime, allCloudsIntensity, allCloudsFalloffIntensity);

    const float topAlpha = TopPartAlpha(texUV);

    // Base BackGround Color
    //{
        // Dome Color
        const float3 domeColor = _Curve_BackGroundSkyDomeColor.rgb * _Background_SkyDome_Color.rgb;

        // Horizon Color
        const float3 horizonColor = _Curve_BackGroundHorizonColor.rgb * _Background_Horizon_Color.rgb;

        // Alpha Blend
        const float alpha = saturate(pow(texUV.y, _Zenith_Area_Falloff_Intensity));

        // メッシュの UV の V に従って、Dome Color と Horizon Color を補間するイメージ
        const float3 skyColor = lerp(domeColor, horizonColor, alpha) * 0.4;

        // Detail cloud Texture
        const float cloudAlpha = tex2D(_detailCloudTexture, Panner(texUV, globalTime)).r;

        float detail = lerp(0.0, 2.0, cloudAlpha);
        detail *= pow((1.0 - SkyDomeAlpha(texUV, 1.0)), allCloudsFalloffIntensity);
        detail *= topAlpha;
        detail *= allCloudsIntensity;
        detail = saturate(detail);
        detail *= _DetailCloudsIntensity;

        const float3 backgroundColor = lerp(skyColor, _DetailCloudsColor * cloudAlpha, detail) * _BackgroundOverlayColor;
    //}

    return ApplyGlobalOverlay(backgroundColor.rgb + (cloudResult * topAlpha));
}

struct VS_INPUT
{
    float3 Pos : POSITION;
    float4 TexUV : TEXCOORD;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float4 TexUV : TEXCOORD;
};

VS_OUTPUT vsMain(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Pos = mul(float4(input.Pos, 1.0), ln_WorldViewProjection);
    output.TexUV = input.TexUV;
    return output;
}

struct PS_INPUT
{
    float4 TexUV : TEXCOORD;
};

cbuffer ConstBuff
{
    float time;
};

float4 psMain(PS_INPUT input) : SV_TARGET
{
    float3 result = CalculateSkyDome(input.TexUV);

    //result.rgb *= 0.5;

    // gamma
    const float g = 1.0 / 2.2;
    result = pow(result, float3(g, g, g));

    return float4(result, 1.0);
}

technique MainTech
{
    pass Pass0
    {
        VertexShader = vsMain;
        PixelShader = psMain;
    }
}
