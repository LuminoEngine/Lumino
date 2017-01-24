//=============================================================================
// PrimitiveRendererForBlt (HLSL:DX9)
//=============================================================================
#ifdef LN_HLSL_DX9

static float2	g_viewportOffset = (float2(0.5, 0.5) / ln_ViewportPixelSize);

float4			_BlurColor;		// ブラーの色・強さ
float4x4		_BlurMatrix;	// ブラーイメージの座標変換行列

// secondaryTexture
sampler2D		g_texSampler = sampler_state
{
	texture = <ln_MaterialTexture>;
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
    o.pos = mul(float4(inPos, 1.0f), _BlurMatrix);
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
    out_color *= _BlurColor;
	
	//out_color.a = 0.5;
    
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
uniform mat4	_BlurMatrix;

attribute vec3	ln_Vertex0;
attribute vec2	ln_MultiTexCoord0;

varying vec2	v_TexUV;

void main()
{
	gl_Position = _BlurMatrix * vec4(ln_Vertex0, 1.0);
	v_TexUV = ln_MultiTexCoord0;
}
#endif

#ifdef LN_GLSL_FRAGMENT_Main

uniform vec4		_BlurColor;	// ブラーの色

varying vec4		v_Color;
varying vec2		v_TexUV;

void main()
{
	vec4 color = texture2D(ln_MaterialTexture, v_TexUV);
    color *= _BlurColor;
	gl_FragColor = color;
}

#endif

