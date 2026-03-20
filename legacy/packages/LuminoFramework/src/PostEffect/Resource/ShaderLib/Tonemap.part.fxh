
cbuffer TonemapPostEffectParams
{
    float    paramA;  // shoulderStrength
    float    paramB;  // linearStrength
    float    paramCB;    // param.linearStrength * param.linearAngle
    float    paramDE;    // param.toeStrength * param.toeNumerator
    float    paramDF;    // param.toeStrength * param.toeDenominator
    float    paramEperF;  // param.toeNumerator / param.toeDenominator
    float    paramF_White;//

    float Exposure;
    float4 _Tone;
};

float3 CalcUncharted2FilmicPreParam( float3 rgb,
    float paramA, float paramB, float paramCB,
    float paramDE, float paramDF, float paramEperF, float paramF_White )
{
    float3    ret = ((rgb * (paramA * rgb + paramCB) + paramDE)
        / (rgb * (paramA * rgb + paramB) + paramDF))
        - paramEperF;
    return ret / paramF_White;
}

float3 Tonemap(float3 color)
{
    float expBias = exp2( Exposure );
    float3 rgb = color.rgb * expBias;

    rgb = CalcUncharted2FilmicPreParam(rgb,
        paramA, paramB, paramCB, paramDE, paramDF, paramEperF, paramF_White);
    
    return rgb;
}

