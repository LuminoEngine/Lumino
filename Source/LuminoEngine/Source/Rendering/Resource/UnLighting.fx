
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
	return (tex2D(TextureSampler, input.UV)) * input.Color;
}

technique Main
{
	pass Geometry
	{
		VertexShader = compile vs_3_0 VS_ShadowCaster();
		PixelShader	 = compile ps_3_0 PS_ShadowCaster();
	}
}
