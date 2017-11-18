/* Lumino ShadingModel combined HLSL file (RawIR:Intermediate Representation) */

//BEGIN_HLSL

//------------------------------------------------------------------------------

//float4x4	ln_WorldViewProjection;
float4x4	ln_World;
float4x4	ln_ViewProjection_Light0;

struct VSInput
{
	float3	Pos			: POSITION;
};

struct VSOutput
{
	float4	svPos		: POSITION;
	float4	Pos			: TEXCOORD0;
};

struct PSInput
{
	float4	Pos			: TEXCOORD0;
};

VSOutput VS_ShadowCaster(VSInput input)
{
	float4 pos = mul(float4(input.Pos, 1.0), ln_World);
	pos = mul(pos, ln_ViewProjection_Light0);
	
	VSOutput output;
	output.svPos = pos;
	
	
	pos = mul(float4(input.Pos, 1.0), ln_World);
	//pos.y *=-1;
	pos = mul(pos, ln_ViewProjection_Light0);
	output.Pos = pos;
	return output;
}



struct PSO
{
	float4	c0			: COLOR0;
	float4	c1			: COLOR1;
};

PSO PS_ShadowCaster(PSInput input)
{
	PSO po;
	//po.c0 = float4(input.Pos.z, 0, 0, 1);
	//po.c1 = float4(input.Pos.z, 0, 0, 1);
	po.c0 = float4(input.Pos.z / input.Pos.w, 0, 0, 1);
	po.c1 = float4(input.Pos.z / input.Pos.w, 0, 0, 1);
	
	return po ;
}

//float4 PS_ShadowCaster(PSInput input) : COLOR0
//{
//	return float4(input.Pos.z / input.Pos.w, 0, 0, 0);
//}

technique Main
{
	pass ShadowCaster
	{
		ZEnable = FALSE;
		ZWRITEENABLE = FALSE;
		VertexShader = compile vs_3_0 VS_ShadowCaster();
		PixelShader	 = compile ps_3_0 PS_ShadowCaster();
	}
}

//END_HLSL

