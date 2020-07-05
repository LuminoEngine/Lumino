
#include <Lumino.fxh>

float4x4	ln_ViewProjection_Light0;

struct VSInput
{
	float3	Pos			: POSITION0;
};

struct VSOutput
{
	float4	svPos		: SV_POSITION;
	float4	Pos			: TEXCOORD0;
};

struct PSInput
{
	float4	Pos			: TEXCOORD0;
};

VSOutput VS_ShadowCaster(VSInput input)
{
	VSOutput output;
	//output.svPos = mul(float4(input.Pos, 1.0), ln_WorldViewProjection);
	
	float4 pos = mul(float4(input.Pos, 1.0), ln_World);
	pos = mul(pos, ln_ViewProjection_Light0);
	output.Pos = pos;
	output.svPos = pos;
	return output;
}

float4 PS_ShadowCaster(PSInput input) : COLOR0
{
	//return float4(0, 0, 1, 1);
	return float4(input.Pos.z / input.Pos.w, 0, 0, 1);
}

technique Main
{
	pass ShadowCaster
	{
		VertexShader = VS_ShadowCaster;
		PixelShader	 = PS_ShadowCaster;
	}
}

//		ZEnable = FALSE;
//		ZWRITEENABLE = FALSE;
