
//==============================================================================
#ifdef LN_HLSL_DX9

struct LN_VSInput
{
	float3	Pos		: POSITION;		// 位置
	float2	UV		: TEXCOORD0;	// テクスチャ座標
	float4	Color	: COLOR0;		// 頂点色
};


static float2	ViewportOffset = (float2(0.5, 0.5) / ln_ViewportPixelSize);
static float2	ViewportOffset2 = (float2(1.0, 1.0) / ln_ViewportPixelSize);

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
	float3	ViewportPos		: TEXCOORD1;
	float	ViewportPos_z		: TEXCOORD2;
};

struct PSInput
{
	float4	Color	: COLOR0;
	float2	UV		: TEXCOORD0;
	float3	ViewportPos		: TEXCOORD1;
	float	ViewportPos_z		: TEXCOORD2;
};




static float near = 1.0;
static float far = 100.0;

float3 cam_pos;// = float3(0, 5, 0);

texture3D clustersTexture;
sampler	clustersSampler = sampler_state
{
	texture = <clustersTexture>;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
	AddressU = Clamp;
	AddressV = Clamp;
};

//------------------------------------------------------------------------------
VSOutput VSBasic(LN_VSInput v)
{
	//float4 cp = mul(float4(v.Pos, 1.0f), ln_World * ln_View);
	//cp.z /= cp.w;
	
	float4 cp = mul(float4(v.Pos, 1.0f), ln_World);	

	VSOutput o;
	o.Pos	= mul(float4(v.Pos, 1.0f), ln_WorldViewProjection);
	o.ViewportPos = o.Pos.xyz / o.Pos.w;
	
	
	//float d = distance(cam_pos, cp.xyz);
	
	//float3 viewDir = normalize(-cam_pos);
	//float d = dot(cp.xyz - cam_pos, viewDir);
	float d = distance(cp.z, cam_pos.z);
	
	
	o.ViewportPos_z = (d - near) / (far - near);
	//o.ViewportPos.z /= 10;
	//o.Pos.x -= ViewportOffset2.x;
	//o.Pos.y += ViewportOffset2.y;
	o.UV	= v.UV;// + ViewportOffset;
	o.Color	= v.Color;
	return o;
}


static float sx = 16.0;
static float sy = 16.0;
static float sz = 32.0;

static float dx = 255.0 / sx;
static float dy = 255.0 / sy;
static float dz = 255.0 / sz;

float bias(float b, float x)
{
	return pow(x, log(b) / log(0.5));
}

//------------------------------------------------------------------------------
float4 PSBasic(PSInput p) : COLOR0
{
	float i_cx = trunc((((p.ViewportPos.x + 1.0) / 2.0) * 255.0) / sx);
	float i_cy = trunc((((p.ViewportPos.y + 1.0) / 2.0) * 255.0) / sy);
	float i_cz = trunc((p.ViewportPos_z * 255.0) / sz);
	
	float4 mc = (tex2D(MaterialTextureSampler, p.UV) * p.Color) * ln_ColorScale;
	
	float3 clus = float3(i_cx / dx, i_cy / dy, bias(0.1, i_cz / dz));
	//float3 clus = float3(0, 0, p.ViewportPos_z);
	float4 c = tex3D(clustersSampler, clus);
	float4 c2 = float4(clus, 1);
	return c * c2 + mc * 0.5;

	//return float4(cx / sx, cy / sy, cz / sz, 1);
	//return float4(0, 0, p.ViewportPos.z, 1);
	//return (tex2D(MaterialTextureSampler, p.UV) * p.Color) * ln_ColorScale;
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

#endif /* LN_HLSL_DX9 */

//=============================================================================
#ifdef LN_GLSL_VERTEX_Main
attribute vec3	ln_Vertex;			// Pos
attribute vec2	ln_MultiTexCoord0;	// UV
attribute vec4	ln_Color0;			// Color

varying vec2	v_TexUV;
varying vec4	v_Color;

void main()
{
	gl_Position		= ln_WorldViewProjection * vec4(ln_Vertex, 1.0);
	v_TexUV			= LN_FlipTexCoord(ln_MultiTexCoord0);
	v_Color			= ln_Color0;
}
#endif /* LN_GLSL_VERTEX_Main */

//=============================================================================
#ifdef LN_GLSL_FRAGMENT_Main
varying vec4	v_Color;
varying vec2	v_TexUV;

void main()
{
    gl_FragColor = texture2D(ln_MaterialTexture, v_TexUV) * v_Color;
}
#endif /* LN_GLSL_FRAGMENT_Main */



