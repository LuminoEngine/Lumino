//==============================================================================
// ImageFilter_Tone
//-----------------------------------------------------------------------------
//=============================================================================

//=============================================================================
#ifdef LN_HLSL_DX9

float2          gViewportSize   : VIEWPORTPIXELSIZE;
static float2   gViewportOffset = ( float2( 0.5, 0.5 ) / gViewportSize );

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
	o.TexUV	= texUV + gViewportOffset;
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

uniform vec4 Tone;
uniform sampler2D ScreenTexture;

varying vec2		v_TexUV;

void main()
{
	vec4 outColor = texture2D(ScreenTexture, v_TexUV);
	
	float y = ( 0.208012 * outColor.r + 0.586611 * outColor.g + 0.114478 * outColor.b ) * Tone.w;
    outColor.rgb = (outColor.rgb * ( 1.0 - Tone.w )) + y + Tone.rgb;
    
	gl_FragColor = outColor;
}

#endif
