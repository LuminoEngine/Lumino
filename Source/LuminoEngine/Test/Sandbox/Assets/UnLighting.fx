
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
	float3	Pos			: TEXCOORD1;
};

struct PSInput
{
	float2	UV			: TEXCOORD0;
	float4	Color		: COLOR0;
	float3	Pos			: TEXCOORD1;
};

VSOutput VS_ShadowCaster(VSInput input)
{
	VSOutput output;
	output.svPos	= mul(float4(input.Pos, 1.0), ln_WorldViewProjection);
	output.UV		= input.UV;
	output.Color	= input.Color;
	output.Pos 		= input.Pos;
	return output;
}

float4 PS_ShadowCaster(PSInput input) : COLOR0
{
	float hr = ((input.Pos.y - 10) / 5.0);

	float4 c = (tex2D(TextureSampler, input.UV)) * input.Color;
	return c;
	
	
	//c *= hr;
	float bc = 0.6;
	c = lerp(float4(bc,bc,bc,1), c, hr);
	//return c;

	float thr = 0.52;
	if (c.r < thr && c.g < thr && c.b < thr)
	{
		return float4(0.2,0.2,0.2,1);
	}
	else
	{
		return float4(1,1,1,1);
	}

	//return (tex2D(TextureSampler, input.UV)) * input.Color;
}

VSOutput VS_Outline(VSInput input)
{
	//float3 offset = input.Normal * 0.035;
	float3 offset = input.Normal * 0.035;

	VSOutput output;
	output.svPos	= mul(float4(input.Pos + offset, 1.0), ln_WorldViewProjection);
	output.UV		= input.UV;
	output.Color	= input.Color;
	output.Pos 		= input.Pos;
	return output;
}

float4 PS_Outline(PSInput input) : COLOR0
{
	float4 c = (tex2D(TextureSampler, input.UV)) * input.Color;
	c.xyz *= 0.3;
	//c *= 0.1;
	return c;
}

technique Forward_Geometry
{
#if 0
	pass Pass1
	{
		CULLMODE= CCW;
		VertexShader = compile vs_3_0 VS_Outline();
		PixelShader	 = compile ps_3_0 PS_Outline();
	}
#endif
	pass Pass2
	{
		VertexShader = compile vs_3_0 VS_ShadowCaster();
		PixelShader	 = compile ps_3_0 PS_ShadowCaster();
	}
}
