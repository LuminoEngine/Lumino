struct LN_VSInput
{
	float3	Pos		: POSITION;		// 位置
	float2	UV		: TEXCOORD0;	// テクスチャ座標
	float3	Normal	: NORMAL0;		// 法線
	float4	Color	: COLOR0;		// 頂点色
};





//------------------------------------------------------------------------------

float4x4	ln_WorldViewProjection;
texture		ln_MaterialTexture;
float2		ln_ViewportPixelSize;

static float2	ViewportOffset = (float2(0.5, 0.5) / ln_ViewportPixelSize);

sampler		MaterialTextureSampler = sampler_state
{
	texture = <ln_MaterialTexture>;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
};

struct VSOutput
{
	float4	Pos		: POSITION;
	float4	Color	: COLOR0;
	float2	UV		: TEXCOORD0;
};

struct PSInput
{
	float4	Color	: COLOR0;
	float2	UV		: TEXCOORD0;
};

//------------------------------------------------------------------------------
VSOutput VSBasic(LN_VSInput v)
{
	VSOutput o;
	o.Pos	= mul(float4(v.Pos, 1.0f), ln_WorldViewProjection);
	o.Color	= v.Color;
	o.UV	= v.UV + ViewportOffset;
	return o;
}

//------------------------------------------------------------------------------
float4 PSBasic(PSInput p) : COLOR0
{
	return tex2D(MaterialTextureSampler, p.UV) * p.Color;
}

//------------------------------------------------------------------------------
technique MainDraw
{
	pass P0
	{
		VertexShader = compile vs_3_0 VSBasic();
		PixelShader	 = compile ps_3_0 PSBasic();
	}
}
