/* Lumino ShadingModel combined HLSL file (RawIR:Intermediate Representation) */

//BEGIN_HLSL

#include <Lumino.fxh>
#include <LuminoPBR.fxh>
#include <LuminoShadow.fxh>



//------------------------------------------------------------------------------
// Lib (ClusteredForward)


struct LN_VSOutput_ClusteredForward
{
	float3	WorldPos	: TEXCOORD10;
	float3	VertexPos	: TEXCOORD11;
	
	float3	vViewPosition 	: TEXCOORD12;	// 頂点位置から視点位置までのベクトル
	float4	vInLightPosition 	: TEXCOORD13;
};

struct LN_PSInput_ClusteredForward
{
	float3	WorldPos	: TEXCOORD10;
	float3	VertexPos	: TEXCOORD11;
	
	float3	vViewPosition 	: TEXCOORD12;
	float4	vInLightPosition 	: TEXCOORD13;
};

int ln_GlobalLightsCount;

texture ln_GlobalLightInfoTexture;
sampler2D ln_GlobalLightInfoSampler = sampler_state
{
	Texture = <ln_GlobalLightInfoTexture>;
	MinFilter = Point; 
	MagFilter = Point;
	MipFilter = None;
	AddressU = Clamp;
	AddressV = Clamp;
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

static const int MaxLights = 16;

float		ln_nearClip;
float		ln_farClip;
float3		ln_cameraPos;
//float4x4	ln_View;

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

float4	ln_AmbientColor;
float4	ln_AmbientSkyColor;
float4	ln_AmbientGroundColor;
float4	ln_FogParams;


// Test
float4	ln_MaterialM2Color;


// ★コア部分の処理は Lib に置く。Auto Generation ではない。ユーザーが vs/ps を直書きするときに使えるようにするため。
LN_VSOutput_ClusteredForward _LN_ProcessVertex_ClusteredForward(LN_VSInput input)
{
	LN_VSOutput_ClusteredForward output;
	output.WorldPos = mul(float4(input.Pos, 1.0), ln_World).xyz;
	output.VertexPos = input.Pos;
	
	
	float4 mvPosition = mul(float4(input.Pos, 1.0), ln_WorldView);
	output.vViewPosition  = -mvPosition.xyz;
	
	
	
	float4 pos = mul(float4(input.Pos, 1.0), ln_World);
	pos = mul(pos, ln_ViewProjection_Light0);
	output.vInLightPosition = pos;
	//output.vInLightPosition = mul(mul(float4(input.Pos, 1.0), ln_World), ln_ViewProjection_Light0);
	
	return output;
}













static float2 LightInfoTextureSize = float2(4, 64);

// LightClusters の bias の逆変換
float _LN_FlustumClustereDepthBias(float z)
{
	return sqrt(z);
}
/*
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
*/
struct LightInfo
{
	float3	position;
	float	range;
	float	attenuation;
	float4	color;
	float3	direction;
	float2	spotAngles;
};

struct GlobalLightInfo
{
	float4	color;
	float4	groundColor;
	float4	directionAndType;
};

float2 _LN_FlipV(float2 uv)
{
	return float2(uv.x, 1.0 - uv.y);
}

LightInfo _LN_GetLightInfoClusterd(int index)
{
	float2 s = 1.0 / LightInfoTextureSize;
	float y = (float)index;
	y += 0.5;
	//float4 tc0 = float4((0.0 + 0.5f) * s.x, (index + 0.5f) * s.y, 0, 1);	// +0.5 は半ピクセル分
	//float4 tc1 = float4((1.0 + 0.5f) * s.x, (index + 0.5f) * s.y, 0, 1);	// +0.5 は半ピクセル分
	//tc0.y = 1.0 - tc0.y;
	//tc1.y = 1.0 - tc1.y;
	
	//float4 posAndRange = tex2D(pointLightInfoSampler, tc0.xy);
	//float4 spotDirection = tex2D(pointLightInfoSampler, tc1.xy);
	float4 posAndRange = tex2D(pointLightInfoSampler, _LN_FlipV(float2(0.0, y) * s));
	float4 spotDirection = tex2D(pointLightInfoSampler, _LN_FlipV(float2(1.0, y) * s));
	float4 spotAngle = tex2D(pointLightInfoSampler, _LN_FlipV(float2(2.0, y) * s));
	float4 color = tex2D(pointLightInfoSampler, _LN_FlipV(float2(3.0, y) * s));
	
	LightInfo light;
	light.position = posAndRange.xyz;
	light.range = posAndRange.w;
	light.attenuation = spotDirection.w;
	light.color = color;
	light.direction = spotDirection.xyz;
	light.spotAngles = spotAngle.xy;
	
	/*
	float2 uv = 1.0 / LightInfoTextureSize;
	float4 tc0 = float4((0.0 + 0.5f) * uv.x, (index + 0.5f) * uv.y, 0, 1);	// +0.5 は半ピクセル分
	float4 tc1 = float4((1.0 + 0.5f) * uv.x, (index + 0.5f) * uv.y, 0, 1);	// +0.5 は半ピクセル分
	LightInfo light;
	
	tc0.y = 1.0 - tc0.y;
	tc1.y = 1.0 - tc1.y;
	//o.pos = tex2Dlod(m_pointLightInfoSampler, tc0);
	//o.color = tex2Dlod(m_pointLightInfoSampler, tc1);
	light.position = tex2D(pointLightInfoSampler, tc0.xy);
	light.color = tex2D(pointLightInfoSampler, tc1.xy);
	
	light.range = 1;
	light.direction = float3(1, 0, 0);
	light.spotAngles = float2(0, 0);
	*/
	return light;
}


GlobalLightInfo _LN_GetGlobalLightInfo(int index)
{
	float2 s = 1.0 / LightInfoTextureSize;
	float y = (float)index;
	y += 0.5;
	GlobalLightInfo info;
	info.color = tex2D(ln_GlobalLightInfoSampler, _LN_FlipV(float2(0.0, y) * s));
	info.groundColor = tex2D(ln_GlobalLightInfoSampler, _LN_FlipV(float2(1.5, y) * s));
	info.directionAndType = tex2D(ln_GlobalLightInfoSampler, _LN_FlipV(float2(2.5, y) * s));
	return info;
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


float LN_PunctualLightIntensityToIrradianceFactor(float lightDistance, float cutoffDistance, float decayExponent) {
  if (decayExponent > 0.0) {
    return pow(saturate(-lightDistance / cutoffDistance + 1.0), decayExponent);
  }

  return 1.0;
}

float _LN_CalcFogFactor(float depth)
{
	float f = ln_FogParams.a * depth;
	return exp2(-f);
	//return exp2(-f*f);
}


	
	
float4 _LN_PS_ClusteredForward_Default(LN_PSInput_Common common, LN_PSInput_ClusteredForward extra, LN_SurfaceOutput surface)
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
	

	
	/**/
	LN_PBRGeometry geometry;
	geometry.position = -extra.vViewPosition;//-vViewPosition;
	geometry.normal = normalize(surface.Normal);//vNormal);
	geometry.viewDir = normalize(extra.vViewPosition);//vViewPosition);

	/**/
	float metallic = 0.5;	// TODO:
	float roughness = 0.3;	// TODO:
	LN_PBRMaterial material;
	material.diffuseColor = lerp(surface.Albedo.xyz, float3(0, 0, 0), metallic);
	material.specularColor = lerp(float3(0.04, 0.04, 0.04), surface.Albedo.xyz, metallic);
	material.specularRoughness = roughness;
	
	/**/
	LN_ReflectedLight reflectedLight = {float3(0,0,0),float3(0,0,0),float3(0,0,0),float3(0,0,0)};
	LN_IncidentLight directLight;
	
	
	float3 result = float3(0, 0, 0);
	for (int i = 0; i < 4; i++)
	{
		if (lightIndices[i] > 0)
		{
			//PointLight light = LN_GetPointLight((lightIndices[i] * 255) + 0.5 - 1);
			LightInfo light = _LN_GetLightInfoClusterd((lightIndices[i] * 255) + 0.5 - 1);
			
			
			
			if (light.spotAngles.x > 0.0)
			{
				LN_SpotLight spotLight;
				spotLight.position = light.position;
				spotLight.direction = light.direction;
				spotLight.color = light.color.xyz;// * light.color.a;
				spotLight.distance = light.range;
				spotLight.decay = light.attenuation;
				spotLight.coneCos = light.spotAngles.x;
				spotLight.penumbraCos = light.spotAngles.y;
				LN_GetSpotDirectLightIrradiance(spotLight, geometry, directLight);
				if (directLight.visible)
				{
					LN_RE_Direct(directLight, geometry, material, reflectedLight);
				}
			}
			else
			{
				LN_PointLight pointLight;
				pointLight.position = light.position;
				pointLight.color = light.color.xyz;// * light.color.a;
				pointLight.distance = light.range;
				pointLight.decay = light.attenuation;
				LN_GetPointDirectLightIrradiance(pointLight, geometry, directLight);
				if (directLight.visible)
				{
					LN_RE_Direct(directLight, geometry, material, reflectedLight);
	//return float4(1,1,0, 1);
				}
			}
			
			float LightRadiusMask = 1.0;
			{
				float3 ToLight = light.position.xyz - worldPos.xyz;
				float DistanceSqr = dot(ToLight, ToLight);
				float3 L = ToLight * rsqrt(DistanceSqr);
				
				
				float LightInvRadius = 1.0 / light.range;
				float LightFalloffExponent = 0;
				
				//if (DeferredLightUniforms.LightFalloffExponent == 0)
				{
					LightRadiusMask = LN_Square(saturate( 1 - LN_Square(DistanceSqr * LN_Square(LightInvRadius))));
				}
				//else
				//{
				//	LightRadiusMask = RadialAttenuation(ToLight * DeferredLightUniforms.LightInvRadius, DeferredLightUniforms.LightFalloffExponent);
				//}
			}
				
			
			if (light.spotAngles.x > 0.0)
			{
				float3 L = normalize(light.position.xyz - worldPos.xyz);
				//float3 SpotDirection = float3(1, 0, 0);
				//float2 SpotAngle = float2(cos(3.14 / 3), 1.0 / cos(3.14 / 4));
				result += _LN_CalculateSpotAttenuation(L, light.direction, light.spotAngles) * LightRadiusMask;
			}
			else
			{
				result += LightRadiusMask;
			}
			
			
		}
	}
	
#if 1
	float3 ambientIrradiance = float3(0, 0, 0);
	{
    	float3 color = float3(0, 0, 0);
		float count = LN_EPSILON;
	    for (int i = 0; i < ln_GlobalLightsCount; i++)
		{
			GlobalLightInfo light = _LN_GetGlobalLightInfo(i);

			// HemisphereLight
			if (light.directionAndType.w >= 3.0)
			{
				//float3 up = float3(0, 1, 0);
				//float hemisphere = (dot(surface.Normal, up) + 1.0) * 0.5;
				//float4 c = lerp((light.groundColor.rgb * light.groundColor.a)), (light.color.rgb * light.color.a), hemisphere);
				//ambientColor = saturate(ambientColor + c.xyz * c.a);
				LN_HemisphereLight tl;
				tl.upDirection = float3(0, 1, 0);
				tl.skyColor = (light.color.rgb * light.color.a);
				tl.groundColor = (light.groundColor.rgb * light.groundColor.a);
				ambientIrradiance += LN_GetHemisphereLightIrradiance(tl, geometry);
			}
			// AmbientLight
			else if (light.directionAndType.w >= 2.0)
			{
				//ambientColor = saturate(ambientColor + light.color.rgb * light.color.a);
				ambientIrradiance += LN_GetAmbientLightIrradiance(light.color.rgb * light.color.a);
			}
			// DirectionalLight
			else if (light.directionAndType.w >= 1.0)
			{
				color += saturate(max(0, (light.color.rgb * light.color.a) * dot(surface.Normal, -light.directionAndType.xyz)));
	            count += 1.0f;
				
				/**/
				LN_DirectionalLight tl;
				tl.direction = light.directionAndType.xyz;//mul(float4(light.directionAndType.xyz, 1.0), ln_View).xyz;//light.directionAndType.xyz;
				tl.color = (light.color.rgb * light.color.a);
				LN_GetDirectionalDirectLightIrradiance(tl, geometry, directLight);
				LN_RE_Direct(directLight, geometry, material, reflectedLight);
	        }
			//else
			//{
			//	break;
			//}
	    }
	    result.rgb += color;
	}

	RE_IndirectDiffuse_BlinnPhong(ambientIrradiance, geometry, material, reflectedLight);

#endif
	
#if 0
	//result = float3(0, 0, 0);
	if (lightIndices[0] > 0) result.r += 1;
	if (lightIndices[1] > 0) result.g += 1;
	if (lightIndices[2] > 0) result.b += 1;
#endif


	/**/
	float3 emissive = float3(0,0,0);
	float opacity = 1.0;
	float3 outgoingLight = emissive + reflectedLight.directDiffuse + reflectedLight.directSpecular + reflectedLight.indirectDiffuse + reflectedLight.indirectSpecular;
	
	// Shadow
    float4 posInLight = extra.vInLightPosition;
    outgoingLight *= LN_CalculateShadow(posInLight);
	
	return float4(outgoingLight, opacity);
	
	
	
	
	
	
	result.rgb = mc.rgb * result.rgb;
	
	// calc ambient color
	float3 ambient = float3(0, 0, 0);
	{
		// basic ambient light
		ambient = ln_AmbientColor.xyz * ln_AmbientColor.a;
		
		// hemisphere ambient light
		float hemisphere = (dot(surface.Normal, float3(0, 1, 0)) + 1.0) * 0.5;
		float4 c = lerp(ln_AmbientGroundColor, ln_AmbientSkyColor, hemisphere);
		ambient = saturate(ambient + c.xyz * c.a);
	}
	
	result.rgb += ambient;
	
	// fog
	result.rgb = lerp(ln_FogParams.rgb, result.rgb, _LN_CalcFogFactor(viewPos.z));
	
	
	return float4(result.rgb, depth);
}

//------------------------------------------------------------------------------
// User code

struct MyVFOutput
{
//	float4	WorldPos	: TEXCOORD5;
};

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

void MyVFMain(LN_VSInput input, inout MyVFOutput output)	// ★ out 引数の型をパース
{
}

// Surface Shader
void MySSMain(MySSInput input, inout LN_SurfaceOutput output)
{
	output.Albedo = (tex2D(MaterialTextureSampler, input.UV)) * input.Color;
	//output.Albedo = float4(1, 0, 0, 1);
}

//------------------------------------------------------------------------------
// (auto generation)
// ClusteredShading_Default.template.fx から生成する

struct _lngs_VSOutput
{
	LN_VSOutput_Common				common;
	LN_VSOutput_ClusteredForward	extra;
	/* VF 定義なし */ MyVFOutput						user;	// ★ MyVFMain() の最後の引数をパースして得る
};

// auto generation
_lngs_VSOutput _lngs_VS_ClusteredForward_Geometry(LN_VSInput vsi)
{
	_lngs_VSOutput o;
	o.common	= _LN_ProcessVertex_Common(vsi);
	o.extra		= _LN_ProcessVertex_ClusteredForward(vsi);
	// ★ Scene固有のコードはここに直接生成する (ピクセルシェーダと書き方を合わせたい)
	MyVFMain(vsi, o.user);	// ★ User定義呼び出し
	return o;
}


struct _lngs_PSInput
{
	LN_PSInput_Common				common;
	LN_PSInput_ClusteredForward		extra;
	MySSInput						user;	// ★ MyVFMain() の最後の引数をパースして得る
};

struct _lngs_PSOutput
{
	float4 color0 : COLOR0;
	//float4 color1 : COLOR1;
};

_lngs_PSOutput _lngs_PS_ClusteredForward_Geometry(_lngs_PSInput input)
{
	LN_SurfaceOutput surface;
	_LN_InitSurfaceOutput(input.common, surface);
	MySSMain(input.user, surface);	// ★ User定義呼び出し
	
	// ★ライティングのコードはここに直接生成する (GBuffer生成などではマルチRT書き込みするため、戻り値も変えなければならない)
	// ・・・というより、ピクセルシェーダ全体を生成する。フラグメントの結合じゃダメ。
	
	_lngs_PSOutput o;
	o.color0 = _LN_PS_ClusteredForward_Default(input.common, input.extra, surface);
	//o.color1 = float4(o.color0.a, 0, 0, 1);
	o.color0.a = surface.Albedo.a;
	return o;
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
		VertexShader = compile vs_3_0 _lngs_VS_ClusteredForward_Geometry();
		PixelShader	 = compile ps_3_0 _lngs_PS_ClusteredForward_Geometry();
	}
}

//END_HLSL

