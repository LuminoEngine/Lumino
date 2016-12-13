//=============================================================================
#ifdef LN_HLSL_DX9

float4x4	g_worldMatrix;
float4x4	g_viewProjMatrix;

float2 g_viewportSize = float2(0.5, 0.5);
static float2 g_pixelStep = (float2(0.5, 0.5) / g_viewportSize);

float4		g_tone;
texture		g_texture;
texture		g_glyphMaskTexture;

sampler2D	g_texSampler = sampler_state
{
	texture = <g_texture>;
	MINFILTER = NONE;
	MAGFILTER = NONE;
};

sampler2D	g_glyphMaskSampler = sampler_state
{
	texture = <g_glyphMaskTexture>;
	MINFILTER = NONE;
	MAGFILTER = NONE;
};

struct VS_OUTPUT
{
	float4 Pos			: POSITION;
	float4 Color		: COLOR0;
	float4 UVOffset		: TEXCOORD0;
	float2 UVTileUnit	: TEXCOORD1;
};

//-------------------------------------------------------------------------
// vertex shader
//-------------------------------------------------------------------------
VS_OUTPUT vsBasic(
	float3 inPos		: POSITION,
	float4 inColor		: COLOR0,
	float4 inUVOffset	: TEXCOORD0,
	float2 inUVTileUnit	: TEXCOORD1)
{
	VS_OUTPUT o;
	o.Pos			= mul(float4(inPos, 1.0f), g_worldMatrix);
	o.Pos			= mul(o.Pos, g_viewProjMatrix);
	o.Color			= inColor;
	o.UVOffset		= inUVOffset;
	o.UVTileUnit	= inUVTileUnit;
	o.Pos.x -= g_pixelStep.x;
	o.Pos.y += g_pixelStep.y;
	return o;
}

//-------------------------------------------------------------------------
// pixel shader
//-------------------------------------------------------------------------
float4 psBasic(
	float4 inColor			: COLOR0,
	float4 inUVOffset		: TEXCOORD0,
	float2 inUVTileUnit		: TEXCOORD1) : COLOR0
{
	// 転送元左上 UV
	float2 uvUpperLeft = inUVOffset.xy;
	
	// 転送元矩形の幅 UV
	float2 uvSize = inUVOffset.zw;
	
	// 1つの四角形の中のどこにいるのか (0.0～1.0)
	float2 uvRatio = fmod(inUVTileUnit, 1.0);
	
	float2 uv = uvUpperLeft + lerp(float2(0.0, 0.0), uvSize, uvRatio);
	//uv -= g_pixelStep;
	
	float4 outColor = tex2D(g_texSampler, uv) * tex2D(g_glyphMaskSampler, uv) * inColor;
	
	// 色調の計算 (NTSC 系加重平均法。緑だけがなんとなく明るく感じるとかを防ぐ)
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
#ifdef LN_GLSL_VERTEX_Main
uniform mat4	g_worldMatrix;
uniform mat4	g_viewProjMatrix;
attribute vec3	ln_Vertex;			// Pos
attribute vec4	ln_Color0;			// Color
attribute vec4	ln_MultiTexCoord0;	// UVOffset
attribute vec2	ln_MultiTexCoord1;	// UVTileUnit
varying vec4	v_Color;
varying vec4	v_UVOffset;
varying vec2	v_UVTileUnit;
void main()
{
	//ln_Vertex.xy -= 0.5;
	gl_Position		= g_worldMatrix * vec4(ln_Vertex, 1.0f);
	gl_Position		= g_viewProjMatrix * gl_Position;
	v_Color			= ln_Color0;
	v_UVOffset		= ln_MultiTexCoord0;
	v_UVTileUnit	= ln_MultiTexCoord1;
}

#endif /* LN_GLSL_VERTEX_Main */
//=============================================================================
#ifdef LN_GLSL_FRAGMENT_Main
uniform vec4		g_tone;
uniform sampler2D	g_texture;
uniform sampler2D	g_glyphMaskTexture;
varying vec4		v_Color;
varying vec4		v_UVOffset;
varying vec2		v_UVTileUnit;
void main()
{
	
	
	vec2 uvUpperLeft = v_UVOffset.xy;	// 転送元左上 UV
	vec2 uvSize = v_UVOffset.zw;		// 転送元矩形の幅 UV
	vec2 uvRatio = mod(v_UVTileUnit, 1.0);	// 1つの四角形の中のどこにいるのか (0.0～1.0)
	vec2 uv = uvUpperLeft + mix(vec2(0.0, 0.0), uvSize, uvRatio);//mix(uvUpperLeft, uvUpperLeft + uvWidth, uvRatio);
	vec4 outColor = texture2D(g_texture, uv) * texture2D(g_glyphMaskTexture, uv) * v_Color;
	
	// 色調の計算
	float y = ( 0.208012 * outColor.r + 0.586611 * outColor.g + 0.114478 * outColor.b ) * g_tone.w;
    outColor.rgb = (outColor.rgb * ( 1.0 - g_tone.w )) + y + g_tone.rgb;
	
	gl_FragColor = outColor;
}
#endif /* LN_GLSL_FRAGMENT_Main */

