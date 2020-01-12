

#include <Lumino.fxh>

#define NUM_MIPS 5

sampler2D blurTexture1;
sampler2D blurTexture2;
sampler2D blurTexture3;
sampler2D blurTexture4;
sampler2D blurTexture5;
sampler2D dirtTexture;
float bloomStrength;
float bloomRadius;
float bloomFactors[NUM_MIPS];
float3 bloomTintColors[NUM_MIPS];

LN_VSOutput_Common VS_Main(LN_VSInput input)
{
	LN_VSOutput_Common output = LN_ProcessVertex_Common(input);
	output.svPos = float4(input.Pos, 1.0f);
	return output;
}

float lerpBloomFactor(float factor)
{
	float mirrorFactor = 1.2 - factor;
	return lerp(factor, mirrorFactor, bloomRadius);
}

float4 PS_Main(LN_PSInput_Common input) : SV_TARGET
{
	float4 result = tex2D( ln_MaterialTexture, input.UV );

	float4 c = bloomStrength * ( lerpBloomFactor(bloomFactors[0]) * float4(bloomTintColors[0], 1.0) * tex2D(blurTexture1, input.UV) +
													 lerpBloomFactor(bloomFactors[1]) * float4(bloomTintColors[1], 1.0) * tex2D(blurTexture2, input.UV) +
													 lerpBloomFactor(bloomFactors[2]) * float4(bloomTintColors[2], 1.0) * tex2D(blurTexture3, input.UV) +
													 lerpBloomFactor(bloomFactors[3]) * float4(bloomTintColors[3], 1.0) * tex2D(blurTexture4, input.UV) +
													 lerpBloomFactor(bloomFactors[4]) * float4(bloomTintColors[4], 1.0) * tex2D(blurTexture5, input.UV) );
	//return c;

	result.rgb += (c.rgb * c.r);
	//c.a = 0.5;
	return result;
	//return float4(c.a, 0, 0, 1);
}

technique Forward_Geometry_UnLighting
{
    pass Pass1
    {
        VertexShader = VS_Main;
        PixelShader = PS_Main;
    }
}


