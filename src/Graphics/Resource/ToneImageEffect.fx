//==============================================================================
// ImageFilter_Tone
//-----------------------------------------------------------------------------
//=============================================================================

//=============================================================================
#ifdef LN_HLSL_DX9

static float2   gViewportOffset = (float2( 0.5, 0.5 ) / ln_ViewportPixelSize);

// 色調
float4 _Tone;

// スクリーン
sampler ScreenSampler = sampler_state
{ 
	Texture		= <ln_MaterialTexture>;
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
VSOutput VSMain(
	float3 pos		: POSITION,
	float2 texUV	: TEXCOORD0)
{
	VSOutput o;
	o.Pos	= float4(pos, 1.0f);
	o.TexUV	= texUV + gViewportOffset;
	return o;
}

//-----------------------------------------------------------------------------
float4 PSMain(
	float2 texUV	: TEXCOORD0) : COLOR
{
	return LN_CalculateToneColor(tex2D(ScreenSampler, texUV), _Tone);
}

//-----------------------------------------------------------------------------
technique MainDraw
{
	pass P0
	{
		VertexShader	= compile vs_2_0 VSMain();
		PixelShader		= compile ps_2_0 PSMain();
	}
}
#endif


//=============================================================================
#ifdef LN_GLSL_VERTEX_Main



attribute vec3	ln_Vertex0;
attribute vec2	ln_MultiTexCoord0;

varying vec2	v_TexUV;

void main()
{
	gl_Position = vec4(ln_Vertex0, 1.0);
	v_TexUV = ln_MultiTexCoord0;
}
#endif

#ifdef LN_GLSL_FRAGMENT_Main

uniform vec4 _Tone;

varying vec2		v_TexUV;

void main()
{
	gl_FragColor = LN_CalculateToneColor(ln_MaterialTexture, _Tone);
}

#endif
