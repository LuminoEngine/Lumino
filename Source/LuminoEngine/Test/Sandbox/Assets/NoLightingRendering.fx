
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
	
	float3	Pos2	: TEXCOORD3;
};

struct PSInput
{
	float4	Color	: COLOR0;
	float2	UV		: TEXCOORD0;
	float3	ViewportPos		: TEXCOORD1;
	float	ViewportPos_z		: TEXCOORD2;
	
	float3	Pos2	: TEXCOORD3;
};



texture m_pointLightInfoTexture;
sampler2D m_pointLightInfoSampler = sampler_state
{
	Texture = <m_pointLightInfoTexture>;
	MinFilter = Point; 
	MagFilter = Point;
	MipFilter = None;
	AddressU = Clamp;
	AddressV = Clamp;
};


float near;
float far;

float3 cam_pos;

float4x4 view;

texture3D clustersTexture;
sampler	clustersSampler = sampler_state
{
	texture = <clustersTexture>;
	MinFilter = Point; 
	MagFilter = Point;
	MipFilter = None;
	AddressU = Clamp;
	AddressV = Clamp;
};

//------------------------------------------------------------------------------
VSOutput VSBasic(LN_VSInput v)
{
	//float4 cp = mul(float4(v.Pos, 1.0f), ln_World * ln_View);
	//cp.z /= cp.w;
	
	float4 cp = mul(float4(v.Pos, 1.0f), ln_World * ln_View);

	VSOutput o;
	//o.Pos	= mul(float4(v.Pos, 1.0f), ln_World * ln_View * ln_Projection);
	o.Pos	= mul(float4(v.Pos, 1.0f), ln_WorldViewProjection);
	o.ViewportPos = o.Pos.xyz / o.Pos.w;
	
	
	//float d = distance(cam_pos, cp.xyz);
	
	//float3 viewDir = normalize(-cam_pos);
	//float d = dot(cp.xyz - cam_pos, viewDir);
	//float d = distance(cp.z, cam_pos.z);
	o.Pos2 = v.Pos.xyz;
	
	o.ViewportPos_z = (cp.z - near) / (far - near);
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



struct PointLight
{
	float4	pos;
	float4	color;
};

PointLight LN_GetPointLight(int index)
{
	float2 uv = 1.0 / float2(2, 64);//lnBoneTextureReciprocalSize;
	float4 tc0 = float4((0.0 + 0.5f) * uv.x, (index + 0.5f) * uv.y, 0, 1);	// +0.5 は半ピクセル分
	float4 tc1 = float4((1.0 + 0.5f) * uv.x, (index + 0.5f) * uv.y, 0, 1);	// +0.5 は半ピクセル分
	PointLight o;
	
	tc0.y = 1.0 - tc0.y;
	tc1.y = 1.0 - tc1.y;
	//o.pos = tex2Dlod(m_pointLightInfoSampler, tc0);
	//o.color = tex2Dlod(m_pointLightInfoSampler, tc1);
	o.pos = tex2D(m_pointLightInfoSampler, tc0.xy);
	o.color = tex2D(m_pointLightInfoSampler, tc1.xy);
	return o;
}

//------------------------------------------------------------------------------
float4 PSBasic(PSInput p) : COLOR0
{
	
	//float4 cp4 = mul(float4(5, 0, -10, 1), view);
	//return float4(0, 0, cp4.z / 10.0, 1);
	
	//return float4(p.Pos2 / 10.0, 1.0f);
	
	
	float4 worldPos = mul(float4(p.Pos2, 1.0f), ln_World);

	
	// View base
	float4 cp = mul(float4(p.Pos2, 1.0f), view);//ln_World * ln_View);
	float cz = cp.z / far; //(cp.z - near) / (far - near);
	//return float4(0, 0, cz, 1);
	
	float4 vp = mul(float4(p.Pos2, 1.0f), ln_WorldViewProjection);
	vp.xyz /= vp.w;
	
	float i_cx = trunc((((vp.x + 1.0) / 2.0) * 255.0) / sx);
	float i_cy = trunc((((vp.y + 1.0) / 2.0) * 255.0) / sy);
	float i_cz = trunc(bias(0.9, cz) * sz);//trunc((cz * 255.0) / sz);
	
	float4 mc = (tex2D(MaterialTextureSampler, p.UV) * p.Color) * ln_ColorScale;
	
	float3 clus = float3(i_cx / dx, i_cy / dy, i_cz / sz);
	//clus.z = bias(0.9, clus.z);
	
	//clus.x = 0; clus.y = 0;
	//clus.z *= 2;
	//clus.z = cz;
	
	//float3 clus = float3(0, 0, p.ViewportPos_z);
	float4 c = tex3D(clustersSampler, clus);	// TODO: 0.5 オフセット調整が必要かも
	
	
	float4 c2 = float4(clus, 1);
	
	
	if (c.r > 0)
	{
		PointLight light = LN_GetPointLight((c.r * 255) + 0.5 - 1);
		
		//点光源までの距離
    	float3 dir = light.pos.xyz - worldPos.xyz;
    	float r = length(dir);
		float a = 1.0 - (r / light.pos.w);
		
		if (distance(light.pos.xyz, worldPos.xyz) <= light.pos.w)
		//if (r <= light.pos.w)
		//if (length(light.pos.xyz) < 2)
			return mc * 0.5 + light.color * a; 
		else
			return mc * 0.5;
	}
	else
	{
		// no affect lights
		return mc * 0.5;
	}
	
	
	
	
	
	
	//return //c * c2 + mc * 0.5;

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



