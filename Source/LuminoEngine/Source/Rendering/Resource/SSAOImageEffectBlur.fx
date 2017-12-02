//==============================================================================
// ImageFilter_Tone
//-----------------------------------------------------------------------------
//=============================================================================

//=============================================================================
#ifdef LN_HLSL_DX9

static float2   gViewportOffset = (float2( 0.5, 0.5 ) / ln_ViewportPixelSize);


// スクリーン
sampler ScreenSampler = sampler_state
{ 
	Texture		= <ln_MaterialTexture>;
	//MinFilter	= LINERE;
	//MagFilter	= POINT;
	//MipFilter	= NONE;
	MinFilter	= NONE;
	MagFilter	= NONE;
	MipFilter	= NONE;
	AddressU	= CLAMP;
	AddressV	= CLAMP;
};

texture _SsaoTexture;
sampler _SsaoSampler = sampler_state
{ 
	Texture		= <_SsaoTexture>;
	//MinFilter	= LINERE;
	//MagFilter	= POINT;
	//MipFilter	= NONE;
	MinFilter	= LINEAR;
	MagFilter	= LINEAR;
	MipFilter	= LINEAR;
	AddressU	= CLAMP;
	AddressV	= CLAMP;
};


struct VSOutput
{ 
	float4 Pos		: POSITION;
	float2 TexUV	: TEXCOORD0;
};



//-----------------------------------------------------------------------------
VSOutput VSMain(
	float3 pos		: POSITION,
	float2 texUV	: TEXCOORD0)
{
	VSOutput o;
	o.Pos	= float4(pos, 1.0f);
	o.TexUV	= texUV + gViewportOffset;
	return o;
}

float4 blur9(sampler image, float2 uv, float2 resolution, float2 direction) {
  float4 color = float4(0, 0, 0, 0);
  float2 off1 = float2(1.3846153846, 1.3846153846) * direction;
  float2 off2 = float2(3.2307692308, 3.2307692308) * direction;
  color += tex2D(image, uv) * 0.2270270270;
  color += tex2D(image, uv + (off1 / resolution)) * 0.3162162162;
  color += tex2D(image, uv - (off1 / resolution)) * 0.3162162162;
  color += tex2D(image, uv + (off2 / resolution)) * 0.0702702703;
  color += tex2D(image, uv - (off2 / resolution)) * 0.0702702703;
  return color;
}
//-----------------------------------------------------------------------------
float4 PSMain(
	float2 texcoord	: TEXCOORD0) : COLOR
{
	float4 c = clamp(tex2D(ScreenSampler, texcoord), float4(0,0,0,0), float4(1,1,1,1)) + 0.4;
	//float4 c1 = blur9(_SsaoSampler, texcoord, ln_ViewportPixelSize, float2(0.75, 0.0));
	float4 c2 = blur9(_SsaoSampler, texcoord, ln_ViewportPixelSize, float2(0.0, 0.95));
	return c * c2* c2;
	//return c1 * c2;
	//return tex2D(_SsaoSampler, texcoord);
}

//-----------------------------------------------------------------------------
technique MainDraw
{
	pass P0
	{
		VertexShader	= compile vs_3_0 VSMain();
		PixelShader		= compile ps_3_0 PSMain();
	}
}
#endif


//=============================================================================
#ifdef LN_GLSL_VERTEX_Main


#endif

#ifdef LN_GLSL_FRAGMENT_Main


#endif
