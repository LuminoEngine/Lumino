/* Lumino ShadingModel combined HLSL file (RawIR:Intermediate Representation) */

//BEGIN_HLSL

//------------------------------------------------------------------------------
// Lib


float4x4	ln_World;
float4x4	ln_WorldViewProjection;
float2		ln_ViewportPixelSize;
texture		ln_MaterialTexture;


struct LN_VSInput
{
	float3	Pos			: POSITION;
	float3	Normal		: NORMAL0;
	float4	Color		: COLOR0;
	float2	UV			: TEXCOORD0;
};

struct LN_VSOutput_Common
{
	float4	svPos		: POSITION;
	float3	Normal		: NORMAL0;
	float4	Color		: COLOR0;
	float2	UV			: TEXCOORD0;
};

struct LNVFInput
{
	// LNVSInput と同じ内容
	float3	Pos			: POSITION;
	float3	Normal		: NORMAL0;
	float4	Color		: COLOR0;
	float2	UV			: TEXCOORD0;
	
	// その他、Forward, Clusterd, Differd すべてで必要になるもの
	// (ターゲットごとに分けたりすると煩雑なのでまとめる。そんなに数は多くならないだろう)
	//float3	VertexPos	: TEXCOORD10;
	//float3	WorldPos	: TEXCOORD11;
};

struct LN_PSInput_Common
{
	// POSITION 以外は LNVSInput と同じ
	float3	Normal		: NORMAL0;
	float4	Color		: COLOR0;
	float2	UV			: TEXCOORD0;
};

struct LN_SurfaceOutput
{
	float4	Albedo;		// diffuse color
	float3	Normal;		// tangent space normal, if written
	float3	Emission;
	float	Specular;	// specular power in 0..1 range
	float3	Gloss;		// specular intensity
};

void _LN_InitSurfaceOutput(LN_PSInput_Common common, inout LN_SurfaceOutput surface)
{
	surface.Albedo = float4(0, 0, 0, 1);
	surface.Normal = common.Normal;
	surface.Emission = float3(0, 0, 0);
	surface.Specular = 0;
	surface.Gloss = float3(0, 0, 0);
}

LN_VSOutput_Common _LN_ProcessVertex_Common(LN_VSInput input)
{
	LN_VSOutput_Common o;
	o.svPos			= mul(float4(input.Pos, 1.0f), ln_WorldViewProjection);
	o.Normal		= mul(float4(input.Normal, 1.0f), ln_World).xyz;
	o.UV			= input.UV + (float2(0.5, 0.5) / ln_ViewportPixelSize);
	o.Color			= input.Color;
	return o;
}










//------------------------------------------------------------------------------
// Lib (ClusteredForward)


struct LN_VSOutput_ClusteredForward
{
	float3	WorldPos	: TEXCOORD10;
	float3	VertexPos	: TEXCOORD11;
};

struct LN_PSInput_ClusteredForward
{
	float3	WorldPos	: TEXCOORD10;
	float3	VertexPos	: TEXCOORD11;
};


texture ln_pointLightInfoTexture;
sampler2D pointLightInfoSampler = sampler_state
{
	Texture = <ln_pointLightInfoTexture>;
	MinFilter = Point; 
	MagFilter = Point;
	MipFilter = None;
	AddressU = Clamp;
	AddressV = Clamp;
};


float		ln_nearClip;
float		ln_farClip;
float3		ln_cameraPos;
float4x4	ln_View;

texture3D ln_clustersTexture;
sampler	clustersSampler = sampler_state
{
	texture = <ln_clustersTexture>;
	MinFilter = Point; 
	MagFilter = Point;
	MipFilter = None;
	AddressU = Clamp;
	AddressV = Clamp;
};





// ★コア部分の処理は Lib に置く。Auto Generation ではない。ユーザーが vs/ps を直書きするときに使えるようにするため。
LN_VSOutput_ClusteredForward _LN_ProcessVertex_ClusteredForward(LN_VSInput input)
{
	LN_VSOutput_ClusteredForward output;
	output.WorldPos = mul(float4(input.Pos, 1.0), ln_World).xyz;
	output.VertexPos = input.Pos;
	return output;
}













static float2 LightInfoTextureSize = float2(4, 64);

// LightClusters の bias の逆変換
float _LN_FlustumClustereDepthBias(float z)
{
	return sqrt(z);
}

struct PointLight
{
	float4	pos;
	float4	color;
};

PointLight LN_GetPointLight(int index)
{
	float2 uv = 1.0 / LightInfoTextureSize;
	float4 tc0 = float4((0.0 + 0.5f) * uv.x, (index + 0.5f) * uv.y, 0, 1);	// +0.5 は半ピクセル分
	float4 tc1 = float4((1.0 + 0.5f) * uv.x, (index + 0.5f) * uv.y, 0, 1);	// +0.5 は半ピクセル分
	PointLight o;
	
	tc0.y = 1.0 - tc0.y;
	tc1.y = 1.0 - tc1.y;
	//o.pos = tex2Dlod(m_pointLightInfoSampler, tc0);
	//o.color = tex2Dlod(m_pointLightInfoSampler, tc1);
	o.pos = tex2D(pointLightInfoSampler, tc0.xy);
	o.color = tex2D(pointLightInfoSampler, tc1.xy);
	return o;
}

/** 
 * Calculates attenuation for a spot light.
 * toLight			: normalize vector to light. 
 * spotDirection	: the direction of the spot light.
 * spotAngles		: x=cos(outerRadius), y=1.0/cos(innerRadius)
 */
float _LN_CalculateSpotAttenuation(float3 toLight, float3 spotDirection, float2 spotAngles)
{
	float t = saturate((dot(toLight, -spotDirection) - spotAngles.x) * spotAngles.y);
	return t * t;
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

float4 _LN_ProcessPixel_ClusteredForward(LN_PSInput_Common common, LN_PSInput_ClusteredForward extra, LN_SurfaceOutput surface)
{
	float4 worldPos = float4(extra.WorldPos, 1.0f);
	float4 viewPos = mul(worldPos, ln_View);
	float depth = (viewPos.z - ln_nearClip) / (ln_farClip - ln_nearClip);
	
	float4 vp = mul(float4(extra.VertexPos, 1.0f), ln_WorldViewProjection);
	vp.xyz /= vp.w;
	
	float cx = (vp.x + 1.0) / 2.0;
	float cy = (vp.y + 1.0) / 2.0;
	float cz = _LN_FlustumClustereDepthBias(depth);
	
	float4 cluster = tex3D(clustersSampler, float3(cx, cy, cz));
	float lightIndices[4] = {cluster.r, cluster.g, cluster.b, cluster.a};
	
	
	float4 mc = surface.Albedo;// * ln_ColorScale;//(tex2D(MaterialTextureSampler, p.UV) * p.Color) * ln_ColorScale;
	
	
	
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
				
			
			if (0)
			{
				float3 L = normalize(light.pos.xyz - worldPos.xyz);
				float3 SpotDirection = float3(1, 0, 0);
				float2 SpotAngle = float2(cos(3.14 / 3), 1.0 / cos(3.14 / 4));
				result += _LN_CalculateSpotAttenuation(L, SpotDirection, SpotAngle) * LightRadiusMask;
			}
			else
			{
				result += LightRadiusMask;
			}
			
			
		}
	}
	
#if 0
	//result = float3(0, 0, 0);
	if (lightIndices[0] > 0) result.r += 1;
	if (lightIndices[1] > 0) result.g += 1;
	if (lightIndices[2] > 0) result.b += 1;
#endif
	
	// 環境色
	result += 0.5;
	
	return float4(mc.xyz * result, mc.a);
	
	
	//return float4(1, 0, 0, 1);
}

//------------------------------------------------------------------------------
// User code

//struct MyVFOutput
//{
//	float4	WorldPos	: TEXCOORD5;
//};

struct MySSInput
{
	float4	Color		: COLOR0;		// defined
	float2	UV			: TEXCOORD0;	// defined
	float4	WorldPos	: TEXCOORD5;
};

sampler		MaterialTextureSampler = sampler_state
{
	texture = <ln_MaterialTexture>;
	MINFILTER = LINEAR;
	MAGFILTER = LINEAR;
};

//void MyVFMain(LNVFInput input, inout MyVFOutput output)	// ★ out 引数の型をパース
//{
//}

// Surface Shader
void MySSMain(MySSInput input, inout LN_SurfaceOutput output)
{
	output.Albedo = (tex2D(MaterialTextureSampler, input.UV)) * input.Color;
	//output.Albedo = float4(1, 0, 0, 1);
}

//------------------------------------------------------------------------------
// ★ Vertex Shader (auto generation)

struct _VSOutput
{
	LN_VSOutput_Common				Common;
	LN_VSOutput_ClusteredForward	Extra;
	/* VF 定義なし */ //MyVFOutput						User;	// ★ MyVFMain() の最後の引数をパースして得る
};

// auto generation
_VSOutput _VS_ClusteredForward_Geometry(LN_VSInput vsi)
{
	_VSOutput o;
	o.Common	= _LN_ProcessVertex_Common(vsi);
	o.Extra		= _LN_ProcessVertex_ClusteredForward(vsi);
	// ★ Scene固有のコードはここに直接生成する (ピクセルシェーダと書き方を合わせたい)
	/* VF 定義なし */ //o.uservsout = MyVFMain(vsi);	// ★ User定義呼び出し
	return o;
}


// ★ Pixel Shader

struct _PS_Input
{
	LN_PSInput_Common				Common;
	LN_PSInput_ClusteredForward		Extra;
	MySSInput						User;	// ★ MyVFMain() の最後の引数をパースして得る
};

// auto generation
float4 _PS_ClusteredForward_Geometry(_PS_Input input) : COLOR0
{
	LN_SurfaceOutput surface;
	_LN_InitSurfaceOutput(input.Common, surface);
	MySSMain(input.User, surface);	// ★ User定義呼び出し
	
	// ★ライティングのコードはここに直接生成する (GBuffer生成などではマルチRT書き込みするため、戻り値も変えなければならない)
	// ・・・というより、ピクセルシェーダ全体を生成する。フラグメントの結合じゃダメ。
	return _LN_ProcessPixel_ClusteredForward(input.Common, input.Extra, surface);
}

//------------------------------------------------------------------------------
/*
technique ClusteredForward
{
	pass ShadowCaster
	{
		VertexFactory	= MyVFMain;
		SurfaceShader	= MySSMain;
	}
	pass Geometry
	{
		VertexFactory	= MyVFMain;
		SurfaceShader	= MySSMain;
	}
}
*/

technique ClusteredForward
{
	//pass ShadowCaster
	//{
	//	VertexShader = compile vs_3_0 _VS_ClusteredForward_ShadowCaster();
	//	PixelShader	 = compile ps_3_0 _PS_ClusteredForward_ShadowCaster();
	//}
	pass Geometry
	{
		VertexShader = compile vs_3_0 _VS_ClusteredForward_Geometry();
		PixelShader	 = compile ps_3_0 _PS_ClusteredForward_Geometry();
	}
}

//END_HLSL

