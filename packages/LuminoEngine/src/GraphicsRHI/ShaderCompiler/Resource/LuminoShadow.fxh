
#ifndef LUMINO_SHADOW_INCLUDED
#define LUMINO_SHADOW_INCLUDED

cbuffer LNShadowParameters
{
	/* [0]  */ float4 ln_mainLightShadowMapResolution;
	/* [16] */ float4 ln_shadowDensity;
};  /* [32(alignd:16)] */

sampler2D         ln_mainLightShadowMap;
//Texture2D         ln_mainLightShadowMap;
//SamplerState    ln_mainLightShadowMapSamplerState;
//float2          ln_mainLightShadowMapResolution;

//static float2   ln_BlurPixelStep = float2(1.0, 1.0) / ln_mainLightShadowMapResolution;
//static float2   ln_DX9ShadowMapHalfPixelStep = float2(0.5, 0.5) / ln_mainLightShadowMapResolution;

/*
sampler2D ln_mainLightShadowMap_Sampler = sampler_state
{
    Texture = <ln_mainLightShadowMap>;
    MinFilter = Point; 
    MagFilter = Point;
    MipFilter = None;
    AddressU = Clamp;
    AddressV = Clamp;
};
*/

// シャドウマップからサンプリングした値が compare より奥にあれば 1(影をつける)、そうでなければ 0
float LN_CompareShadowTexture(float2 uv, float compareZ)
{
	//return step(compareZ, (ln_mainLightShadowMap.Sample(ln_mainLightShadowMapSamplerState, uv).r));
	return step(compareZ, (tex2D(ln_mainLightShadowMap, uv).r));
}

// posInLight : Position in viewport coord of light
float LN_CalculateShadow(float4 posInLight)
{
    if (ln_shadowDensity.x <= 0.0) {
        return 1.0;
    }
    else {
        
        float2 shadowUV = 0.5 * (posInLight.xy / posInLight.w) + float2(0.5, 0.5);
        shadowUV.y = 1.0 - shadowUV.y;

    #ifdef LN_HLSL_DX9
        shadowUV += ln_DX9ShadowMapHalfPixelStep;
    #endif

    #if 1
        float2 s2 = ln_mainLightShadowMapResolution.zw;
        float depth = posInLight.z/ posInLight.w;
        float compareZ = depth - 0.0065;
        float shadow = (
            LN_CompareShadowTexture(shadowUV + float2(-s2.x, -s2.y), compareZ) +
            LN_CompareShadowTexture(shadowUV + float2(0    , -s2.y), compareZ) +
            LN_CompareShadowTexture(shadowUV + float2( s2.x, -s2.y), compareZ) +
            LN_CompareShadowTexture(shadowUV + float2(-s2.x, 0    ), compareZ) +
            LN_CompareShadowTexture(shadowUV + float2(0    , 0    ), compareZ) +
            LN_CompareShadowTexture(shadowUV + float2(+s2.x, 0    ), compareZ) +
            LN_CompareShadowTexture(shadowUV + float2(-s2.x,  s2.y), compareZ) +
            LN_CompareShadowTexture(shadowUV + float2(0    ,  s2.y), compareZ) +
            LN_CompareShadowTexture(shadowUV + float2( s2.x,  s2.y), compareZ)
        ) * ( 1.0 / 9.0 );

        return lerp(1.0 - ln_shadowDensity.x, 1.0, shadow);

    #else
        //float shadow = ln_mainLightShadowMap.Sample(ln_mainLightShadowMapSamplerState, shadowUV).r;
        float shadow = tex2D(ln_mainLightShadowMap, shadowUV).r;

        float depth = posInLight.z/ posInLight.w;

        if (depth > shadow + 0.0065)
        {
            return 0.5;
        }
        return 1.0;
    #endif
    }
}

#endif // LUMINO_SHADOW_INCLUDED
