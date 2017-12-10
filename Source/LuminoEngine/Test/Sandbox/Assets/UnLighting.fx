
float4x4	ln_WorldViewProjection;
texture2D	ln_MaterialTexture;

sampler2D TextureSampler = sampler_state
{
	texture = <ln_MaterialTexture>;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
};

struct VSInput
{
	float3	Pos			: POSITION;
	float2	UV			: TEXCOORD0;
	float4	Color		: COLOR0;
	float3	Normal		: NORMAL0;
};

struct VSOutput
{
	float4	svPos		: POSITION;
	float2	UV			: TEXCOORD0;
	float4	Color		: COLOR0;
};

struct PSInput
{
	float2	UV			: TEXCOORD0;
	float4	Color		: COLOR0;
};

VSOutput VS_ShadowCaster(VSInput input)
{
	VSOutput output;
	output.svPos	= mul(float4(input.Pos, 1.0), ln_WorldViewProjection);
	output.UV		= input.UV;
	output.Color	= input.Color;
	return output;
}

float4 PS_ShadowCaster(PSInput input) : COLOR0
{
	return float4(1, 0, 0, 1);
	//return (tex2D(TextureSampler, input.UV)) * input.Color;
}

VSOutput VS_Outline(VSInput input)
{
	VSOutput output;
	output.svPos	= mul(float4(input.Pos + float3(0,1,0), 1.0), ln_WorldViewProjection);
	output.UV		= input.UV;
	output.Color	= input.Color;
	return output;
}

float4 PS_Outline(PSInput input) : COLOR0
{
	return float4(0, 0, 0, 1);
}

technique Forward_Geometry
{
	pass Pass1
	{
		VertexShader = compile vs_3_0 VS_Outline();
		PixelShader	 = compile ps_3_0 PS_Outline();
	}
	pass Pass2
	{
		VertexShader = compile vs_3_0 VS_ShadowCaster();
		PixelShader	 = compile ps_3_0 PS_ShadowCaster();
	}
}
