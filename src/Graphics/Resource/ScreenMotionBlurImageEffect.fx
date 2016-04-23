//=============================================================================
// PrimitiveRendererForBlt (HLSL:DX9)
//=============================================================================
#ifdef LN_HLSL_DX9

float2			g_viewportSize   : VIEWPORTPIXELSIZE;
static float2	g_viewportOffset = (float2(0.5, 0.5) / g_viewportSize);

float			g_blurPower;	// ブラーの強さ
float4			g_blurColor;	// ブラーの色
float4x4		g_blurMatrix;	// ブラーイメージの座標変換行列

texture			g_secondaryTexture;
sampler2D		g_texSampler = sampler_state
{
	texture = <g_secondaryTexture>;
    MinFilter = POINT;
    MagFilter = POINT;
    MipFilter = NONE;
    AddressU  = CLAMP;
    AddressV  = CLAMP;
};

struct VS_OUTPUT
{
	float4 pos		: POSITION;
	float2 texUV	: TEXCOORD0;
};

//-------------------------------------------------------------------------
// vertex shader
//-------------------------------------------------------------------------
VS_OUTPUT vsBasic(
	float3 inPos	: POSITION,
	float2 inUV		: TEXCOORD0)
{
    VS_OUTPUT o;
    o.pos = mul(float4(inPos, 1.0f), g_blurMatrix);
    o.texUV = inUV;// + g_viewportOffset;
    return o;
}

//-------------------------------------------------------------------------
// pixel shader
//-------------------------------------------------------------------------
float4 psBasic(
	float2 inUV		: TEXCOORD0) : COLOR0
{
    float4 out_color = tex2D(g_texSampler, inUV);
    out_color.a *= g_blurPower;
    out_color *= g_blurColor;
    
    // ブラーを青っぽくする
    //out_color.rgb *= float3(0, 0, 1);
    
    // 外側ほど透明になるようにする
    //float len = length( in_uv_ - 0.5 );
    //color.a *= 1.0 - len;
    
    return out_color;
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
#ifdef LN_GLSL_VERTEX_Main
uniform mat4	g_blurMatrix;

attribute vec3	ln_Vertex0;
attribute vec2	ln_MultiTexCoord0;

varying vec2	v_TexUV;

void main()
{
	gl_Position = g_blurMatrix * vec4(ln_Vertex0, 1.0);
	v_TexUV = ln_MultiTexCoord0;
}
#endif

#ifdef LN_GLSL_FRAGMENT_Main

uniform float		g_blurPower;	// ブラーの強さ
uniform vec4		g_blurColor;	// ブラーの色
uniform sampler2D	g_secondaryTexture;

varying vec4		v_Color;
varying vec2		v_TexUV;

void main()
{
	vec4 color = texture2D(g_secondaryTexture, v_TexUV);
    color.a *= g_blurPower;
    color *= g_blurColor;
	gl_FragColor = color;
}

#endif

