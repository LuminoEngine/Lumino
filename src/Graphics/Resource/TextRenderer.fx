//=============================================================================
#ifdef LN_HLSL_DX9

float4x4	g_worldMatrix;
float4x4	g_viewProjMatrix;
float2		g_pixelStep;
float4		g_tone;
texture		g_texture;
//texture		g_glyphMaskTexture;

sampler2D	g_texSampler = sampler_state
{
	texture = <g_texture>;
	MINFILTER = NONE;
	MAGFILTER = NONE;
};
/*
sampler2D	g_glyphMaskSampler = sampler_state
{
	texture = <g_glyphMaskTexture>;
	MINFILTER = NONE;
	MAGFILTER = NONE;
};
*/
struct VS_OUTPUT
{
	float4 Pos			: POSITION;
	float4 Color		: COLOR0;
	float2 TexUV		: TEXCOORD0;
};

//-------------------------------------------------------------------------
// vertex shader
//-------------------------------------------------------------------------
VS_OUTPUT vsBasic(
	float3 inPos		: POSITION,
	float4 inColor		: COLOR0,
	float2 inTexUV		: TEXCOORD0)
{
	VS_OUTPUT o;
	o.Pos			= mul(float4(inPos, 1.0f), g_worldMatrix);
	o.Pos			= mul(o.Pos, g_viewProjMatrix);
	o.Color			= inColor;
	o.TexUV			= inTexUV;
	o.Pos.x -= g_pixelStep.x;
	o.Pos.y += g_pixelStep.y;
	return o;
}

//-------------------------------------------------------------------------
// pixel shader
//-------------------------------------------------------------------------
float4 psBasic(
	float4 inColor		: COLOR0,
	float2 inTexUV		: TEXCOORD0) : COLOR0
{
	float4 outColor = tex2D(g_texSampler, inTexUV)/* * tex2D(g_glyphMaskSampler, inTexUV) */ * inColor;
	
	// êFí≤ÇÃåvéZ (NTSC ånâ¡èdïΩãœñ@ÅBóŒÇæÇØÇ™Ç»ÇÒÇ∆Ç»Ç≠ñæÇÈÇ≠ä¥Ç∂ÇÈÇ∆Ç©ÇñhÇÆ)
	float y = ( 0.208012 * outColor.r + 0.586611 * outColor.g + 0.114478 * outColor.b ) * g_tone.w;
    outColor.rgb = (outColor.rgb * ( 1.0 - g_tone.w )) + y + g_tone.rgb;
	
	return outColor;
}

//-------------------------------------------------------------------------
// technique
//-------------------------------------------------------------------------
technique MainDraw
{
	pass P0
	{
		VertexShader = compile vs_2_0 vsBasic();
		PixelShader	 = compile ps_2_0 psBasic();
	}
}
#endif
//=============================================================================
#ifdef LN_GLSL_VERTEX
uniform mat4	g_worldMatrix;
uniform mat4	g_viewProjMatrix;
attribute vec3	ln_Vertex;			// Pos
attribute vec4	ln_Color0;			// Color
attribute vec2	ln_MultiTexCoord0;	// UV
varying vec4	v_Color;
varying vec2	v_TexUV;
void main()
{
	gl_Position		= g_worldMatrix * vec4(ln_Vertex, 1.0f);
	gl_Position		= g_viewProjMatrix * gl_Position;
	v_Color			= ln_Color0;
	v_TexUV			= ln_MultiTexCoord0;
}

#endif
//=============================================================================
#ifdef LN_GLSL_FRAGMENT
uniform vec4		g_tone;
uniform sampler2D	g_texture;
//uniform sampler2D	g_glyphMaskTexture;
varying vec4		v_Color;
varying vec2		v_TexUV;
void main()
{
	vec4 outColor = texture2D(g_texture, v_TexUV)/* * texture2D(g_glyphMaskTexture, v_TexUV)*/ * v_Color;
	
	// êFí≤ÇÃåvéZ
	float y = ( 0.208012 * outColor.r + 0.586611 * outColor.g + 0.114478 * outColor.b ) * g_tone.w;
    outColor.rgb = (outColor.rgb * ( 1.0 - g_tone.w )) + y + g_tone.rgb;
	
    gl_FragColor = outColor;
}
#endif
