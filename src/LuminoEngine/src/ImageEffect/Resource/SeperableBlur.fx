

#include <Lumino.fxh>

//#define KERNEL_RADIUS 4
//#define SIGMA 4.0//0.35//0.125, 0.215, 0.35, 0.446, 0.526, 0.582

sampler2D colorTexture;
float2 texSize;
float2 direction;

int KERNEL_RADIUS;
float SIGMA;

LN_VSOutput_Common VS_Main(LN_VSInput input)
{
	LN_VSOutput_Common output = LN_ProcessVertex_Common(input);
	output.svPos = float4(input.Pos, 1.0f);
	return output;
}

float gaussian(float x, float sigma)
{
	return 0.39894 * exp( -0.5 * x * x/( sigma * sigma))/sigma;
}

float4 PS_Main(LN_PSInput_Common input) : SV_TARGET
{
	float2 invSize = 1.0 / texSize;
	float fSigma = float(SIGMA);
	float weightSum = gaussian(0.0, fSigma);
	float3 diffuseSum = tex2D(colorTexture, input.UV).rgb * weightSum;
	for( int i = 1; i < KERNEL_RADIUS; i ++ ) {
		float x = float(i);
		float w = gaussian(x, fSigma);
		float2 uvOffset = direction * invSize * x;
		float3 sample1 = tex2D(colorTexture, input.UV + uvOffset).rgb;
		float3 sample2 = tex2D(colorTexture, input.UV - uvOffset).rgb;
		diffuseSum += (sample1 + sample2) * w;
		weightSum += 2.0 * w;
	}
	return float4(diffuseSum/weightSum, 1.0);
}

technique Forward_Geometry_UnLighting
{
    pass Pass1
    {
        VertexShader = VS_Main;
        PixelShader = PS_Main;
    }
}


