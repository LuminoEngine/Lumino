
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

struct PSInputTest
{
	float3	Pos2	: TEXCOORD3;
};

struct PSInput
{
	float4	Color	: COLOR0;
	float2	UV		: TEXCOORD0;
	float3	ViewportPos		: TEXCOORD1;
	float	ViewportPos_z		: TEXCOORD2;
	
	float3	Pos2	: TEXCOORD3;
	
	PSInputTest	test;
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
	return x;
	//return pow(x, log(b) / log(0.5));
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

/** 
 * Calculates attenuation for a spot light.
 * L normalize vector to light. 
 * SpotDirection is the direction of the spot light.
 * SpotAngles.x is CosOuterCone, SpotAngles.y is InvCosConeDifference. 
 */
float SpotAttenuation(float3 L, float3 SpotDirection, float2 SpotAngles)
{
	float t = saturate((dot(L, -SpotDirection) - SpotAngles.x) * SpotAngles.y);
	float ConeAngleFalloff = t*t;
	return ConeAngleFalloff;
}


/** 
 * Returns a radial attenuation factor for a point light.  
 * WorldLightVector is the vector from the position being shaded to the light, divided by the radius of the light. 
 */
float RadialAttenuation(float3 WorldLightVector, float FalloffExponent)
{
	float NormalizeDistanceSquared = dot(WorldLightVector, WorldLightVector);

	// UE3 (fast, but now we not use the default of 2 which looks quite bad):
	return pow(1.0f - saturate(NormalizeDistanceSquared), FalloffExponent); 
}

float Square(float x)
{
	return x * x;
}
float3 Square(float3 x)
{
	return x * x;
}

//------------------------------------------------------------------------------
float4 PSBasic(PSInput p) : COLOR0
{
	
	//float4 cp4 = mul(float4(5, 0, -10, 1), view);
	//return float4(0, 0, cp4.z / 10.0, 1);
	
	//return float4(p.Pos2 / 10.0, 1.0f);
	
	
	float4 worldPos = mul(float4(p.test.Pos2, 1.0f), ln_World);

	
	// View base
	//float4 cp = mul(float4(p.Pos2, 1.0f), view);//ln_World * ln_View);
	float4 cp = mul(worldPos, view);
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
	
	float lightIndices[4] = {c.r, c.g, c.b, c.a};
	
	
	float3 result = float3(0, 0, 0);
	for (int i = 0; i < 4; i++)
	{
		if (lightIndices[i] > 0)
		{
			PointLight light = LN_GetPointLight((lightIndices[i] * 255) + 0.5 - 1);
			
			
			
			float LightRadiusMask = 1.0;
			{
				float3 ToLight = light.pos.xyz - worldPos.xyz;
				float DistanceSqr = dot(ToLight, ToLight);
				float3 L = ToLight * rsqrt(DistanceSqr);
				
				
				float LightInvRadius = 1.0 / light.pos.w;
				float LightFalloffExponent = 0;
				
				//if (DeferredLightUniforms.LightFalloffExponent == 0)
				{
					LightRadiusMask = Square(saturate( 1 - Square(DistanceSqr * Square(LightInvRadius))));
				}
				//else
				//{
				//	LightRadiusMask = RadialAttenuation(ToLight * DeferredLightUniforms.LightInvRadius, DeferredLightUniforms.LightFalloffExponent);
				//}
			}
				
			
			
			float3 L = normalize(light.pos.xyz - worldPos.xyz);
			float3 SpotDirection = float3(1, 0, 0);
			float2 SpotAngle = float2(cos(3.14 / 3), 1.0 / cos(3.14 / 4));
			result += SpotAttenuation(L, SpotDirection, SpotAngle) * LightRadiusMask;
#if 0
			//点光源までの距離
	    	float3 toLight = light.pos.xyz - worldPos.xyz;
			float lengthSq = dot(toLight, toLight);
			lengthSq = max(lengthSq, 0.000001); // My: 0 以下はだめだよ
			
			float quadratic_attenuation = 1.0;	// range をもとに作るべき？
        	float atten = 1.0 / (1.0 + lengthSq * quadratic_attenuation);
			
			
       	 	float diff = 1.0;//max (0, dot (viewN, toLight));
        	result += light.color.rgb * (diff * atten);
#endif
			
			#if 0
			
			//点光源までの距離
	    	float3 dir = light.pos.xyz - worldPos.xyz;
	    	float r = length(dir);
			float a = 1.0 - (r / light.pos.w);
		
			if (distance(light.pos.xyz, worldPos.xyz) <= light.pos.w)
			{
				result += light.color.rgb * a;
			}
#endif
		}
	}
	
#if 0
	//result = float3(0, 0, 0);
	if (lightIndices[0] > 0) result.r += 1;
	if (lightIndices[1] > 0) result.g += 1;
	if (lightIndices[2] > 0) result.b += 1;
#endif
	
	return float4(mc.xyz * result, mc.a);
	
#if 0
	if (lightIndices[0] > 0)
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
#endif
	
	
	
	
	
	
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



