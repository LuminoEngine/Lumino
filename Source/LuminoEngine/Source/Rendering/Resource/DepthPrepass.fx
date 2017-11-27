
float4x4	ln_WorldView;
float4x4	ln_Projection;
float		ln_NearClip;
float		ln_FarClip;

struct VSInput
{
	float3	Pos			: POSITION;
};

struct VSOutput
{
	float4	svPos		: POSITION;
	float4	ViewPos		: TEXCOORD0;
};

struct PSInput
{
	float4	ViewPos		: TEXCOORD0;
};

VSOutput VS_WriteLinearDepth(VSInput input)
{
	VSOutput output;
	output.ViewPos = mul(float4(input.Pos, 1.0), ln_WorldView);
	output.svPos = mul(output.ViewPos, ln_Projection);
	return output;
}

float4 PS_WriteLinearDepth(PSInput input) : COLOR0
{
	return (input.ViewPos.z - ln_NearClip) / (ln_FarClip - ln_NearClip);
}

technique Main
{
	pass ShadowCaster
	{
		ZEnable = FALSE;
		ZWRITEENABLE = FALSE;
		VertexShader = compile vs_3_0 VS_WriteLinearDepth();
		PixelShader	 = compile ps_3_0 PS_WriteLinearDepth();
	}
}
