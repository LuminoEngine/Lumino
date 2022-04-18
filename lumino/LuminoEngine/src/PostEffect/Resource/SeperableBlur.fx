#include <Lumino.fxh>

//==============================================================================
// Uniforms

// TODO: Compiletime constants.
int KERNEL_RADIUS;
float SIGMA;

float2 _TexSize;
float2 _Direction;

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

VS_Output VS_Main(VS_Input input)
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
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD0;
};

float Gaussian(float x, float sigma)
{
    return 0.39894 * exp(-0.5 * x * x / (sigma * sigma)) / sigma;
}

float4 PS_Main(PS_Input input) : SV_TARGET
{
    float2 invSize = 1.0 / _TexSize;
    float fSigma = float(SIGMA);
    float weightSum = Gaussian(0.0, fSigma);
    float3 diffuseSum = tex2D(ln_MaterialTexture, input.UV).rgb * weightSum;
    for( int i = 1; i < KERNEL_RADIUS; i ++ ) {
        float x = float(i);
        float w = Gaussian(x, fSigma);
        float2 uvOffset = _Direction * invSize * x;
        float3 sample1 = tex2D(ln_MaterialTexture, input.UV + uvOffset).rgb;
        float3 sample2 = tex2D(ln_MaterialTexture, input.UV - uvOffset).rgb;
        diffuseSum += (sample1 + sample2) * w;
        weightSum += 2.0 * w;
    }
    return float4(diffuseSum / weightSum, 1.0);
}

//==============================================================================
// Technique

technique Default
{
    ShadingModel = Unlit;
    pass Pass1
    {
        VertexShader = VS_Main;
        PixelShader = PS_Main;
    }
}

