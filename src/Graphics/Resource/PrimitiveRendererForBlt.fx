//=============================================================================
// PrimitiveRendererForBlt (HLSL:DX9)
//=============================================================================
#ifdef LN_HLSL_DX9

float2		g_pixelStep;
texture		g_texture;
sampler2D	g_texSampler = sampler_state
{
    texture = <g_texture>;
    MINFILTER = NONE;
    MAGFILTER = NONE;
};

struct VS_OUTPUT
{
    float4 Pos      : POSITION;
    float4 Color    : COLOR0;
    float2 TexUV    : TEXCOORD0;
};

//-------------------------------------------------------------------------
// vertex shader
//-------------------------------------------------------------------------
VS_OUTPUT vsBasic(
	float3 inPos	: POSITION,
	float4 inColor	: COLOR0,
	float2 inUV		: TEXCOORD0)
{
	VS_OUTPUT o;
	o.Pos   = float4(inPos, 1.0f);
	o.Color = inColor;
	o.TexUV = inUV;
	o.Pos.x -= g_pixelStep.x;
	o.Pos.y += g_pixelStep.y;
	return o;
}

//-------------------------------------------------------------------------
// pixel shader
//-------------------------------------------------------------------------
float4 psBasic(
	float4 inColor	: COLOR0,
	float2 inUV		: TEXCOORD0) : COLOR0
{
    return tex2D(g_texSampler, inUV) * inColor;
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
// PrimitiveRendererForBlt (GLSL)
//=============================================================================
#ifdef LN_GLSL_VERTEX

attribute vec3	ln_Vertex0;
attribute vec4	ln_Color0;
attribute vec2	ln_MultiTexCoord0;

varying vec4	v_Color;
varying vec2	v_TexUV;

void main()
{
	gl_Position = vec4(ln_Vertex0, 1.0);
	v_Color = ln_Color0;
	v_TexUV = ln_MultiTexCoord0;
}
#endif

#ifdef LN_GLSL_FRAGMENT

uniform sampler2D	g_texture;
varying vec4		v_Color;
varying vec2		v_TexUV;

void main()
{
	vec4 color = texture2D(g_texture, v_TexUV);
	gl_FragColor = color * v_Color;
}

#endif

