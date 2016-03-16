//==============================================================================
// ImageFilter_Tone
//-----------------------------------------------------------------------------
//=============================================================================

// 色調
float4 Tone;

// スクリーン
texture ScreenTexture;
sampler ScreenSampler = sampler_state
{ 
	Texture		= <ScreenTexture>;
	MinFilter	= POINT;
	MagFilter	= POINT;
	MipFilter	= NONE;
	AddressU	= CLAMP;
	AddressV	= CLAMP;
};

struct VSOutput
{ 
	float4 Pos		: POSITION;
	float2 TexUV	: TEXCOORD0;
};

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
VSOutput VSMain(
	float3 pos		: POSITION,
	float2 texUV	: TEXCOORD0)
{
	VSOutput o;
	o.Pos	= float4(pos, 1.0f);
	o.TexUV	= texUV;
	return o;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
float4 PSMain(
	float2 texUV	: TEXCOORD0) : COLOR
{
	float4 outColor = tex2D(ScreenSampler, texUV);
	float y = (0.208012 * outColor.r + 0.586611 * outColor.g + 0.114478 * outColor.b) * Tone.w;
	outColor.rgb = (outColor.rgb * (1.0 - Tone.w)) + y + Tone.rgb;
	return outColor;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
technique MainDraw
{
	pass P0
	{
		VertexShader	= compile vs_2_0 VSMain();
		PixelShader		= compile ps_2_0 PSMain();
	}
}
