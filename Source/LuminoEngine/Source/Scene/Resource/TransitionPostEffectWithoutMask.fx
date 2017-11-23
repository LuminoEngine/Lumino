
float4x4		ln_WorldViewProjection;
texture			ln_MaterialTexture;
#ifdef LN_HLSL_DX9
static float2	ln_DX9ViewportOffset = (float2(0.5, 0.5) / ln_ViewportPixelSize);
#endif

float			g_Factor;
texture 		g_FreezedTexture;

sampler g_MaterialSampler = sampler_state
{
	texture = <ln_MaterialTexture>;
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = NONE;
	AddressU  = CLAMP;
	AddressV  = CLAMP;
};

sampler g_FreezedSampler = sampler_state
{
    Texture = <g_FreezedTexture>;
    MinFilter = None;
    MagFilter = None;
};

struct VSInput
{
	float3	Position		: POSITION;
	float2	TexCoord		: TEXCOORD0;
};

struct VSOutput
{
	float4	Position		: POSITION;
	float2	TexCoord		: TEXCOORD0;
};

struct PSInput
{
	float2	TexCoord		: TEXCOORD0;
};

VSOutput VS_Main(VSInput input)
{
	VSOutput o;
	o.Position = mul(float4(input.Position, 1.0), ln_WorldViewProjection);
#ifdef LN_HLSL_DX9
	o.Position.x -= ln_DX9ViewportOffset.x;
	o.Position.y += ln_DX9ViewportOffset.y;
#endif
	o.TexCoord  = input.TexCoord;
	return o;
}

float4 PS_Main(PSInput input) : COLOR0
{
	float4 color = tex2D(g_MaterialSampler, input.TexCoord);
	color.rgb = lerp(
		color.rgb,
		tex2D(g_FreezedSampler, input.TexCoord).rgb,
		(1.0 - g_Factor));
	return color;
}

technique MainTechnique
{
	pass P0
	{
		VertexShader = compile vs_3_0 VS_Main();
		PixelShader	 = compile ps_3_0 PS_Main();
	}
}
