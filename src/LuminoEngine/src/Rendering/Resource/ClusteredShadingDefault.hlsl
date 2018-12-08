/* Lumino ShadingModel combined HLSL file (RawIR:Intermediate Representation) */

//BEGIN_HLSL

#include <Lumino.fxh>
#include <LuminoForward.fxh>
#include <LuminoPBR.fxh>
#include <LuminoShadow.fxh>
#include <LuminoSkinning.fxh>



//------------------------------------------------------------------------------
// Lib (ClusteredForward)



Texture2D ln_GlobalLightInfoTexture;
SamplerState ln_GlobalLightInfoTextureSamplerState;
/*
sampler2D ln_GlobalLightInfoSampler = sampler_state
{
	Texture = <ln_GlobalLightInfoTexture>;
	MinFilter = Point; 
	MagFilter = Point;
	MipFilter = None;
	AddressU = Clamp;
	AddressV = Clamp;
};
*/

Texture2D ln_pointLightInfoTexture;
SamplerState ln_pointLightInfoTextureSamplerState;
/*
sampler2D pointLightInfoSampler = sampler_state
{
	Texture = <ln_pointLightInfoTexture>;
	MinFilter = Point; 
	MagFilter = Point;
	MipFilter = None;
	AddressU = Clamp;
	AddressV = Clamp;
};
*/

static const int MaxLights = 16;

float		ln_nearClip;
float		ln_farClip;
float3		ln_cameraPos;
//float4x4	ln_View;

Texture3D ln_clustersTexture;
SamplerState ln_clustersTextureSamplerState;
/*
sampler3D	clustersSampler = sampler_state
{
	texture = <ln_clustersTexture>;
	MinFilter = Point; 
	MagFilter = Point;
	MipFilter = None;
	AddressU = Clamp;
	AddressV = Clamp;
};
*/

float4	ln_AmbientColor;
float4	ln_AmbientSkyColor;
float4	ln_AmbientGroundColor;
float4	ln_FogParams;












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

	//float4 posAndRange = tex2D(pointLightInfoSampler, (float2(0.0, y) * s));
	//float4 spotDirection = tex2D(pointLightInfoSampler, (float2(1.0, y) * s));
	//float4 spotAngle = tex2D(pointLightInfoSampler, (float2(2.0, y) * s));
	//float4 color = tex2D(pointLightInfoSampler, (float2(3.0, y) * s));
	float4 posAndRange = ln_pointLightInfoTexture.Sample(ln_pointLightInfoTextureSamplerState, (float2(0.0, y) * s));
	float4 spotDirection = ln_pointLightInfoTexture.Sample(ln_pointLightInfoTextureSamplerState, (float2(1.0, y) * s));
	float4 spotAngle = ln_pointLightInfoTexture.Sample(ln_pointLightInfoTextureSamplerState, (float2(2.0, y) * s));
	float4 color = ln_pointLightInfoTexture.Sample(ln_pointLightInfoTextureSamplerState, (float2(3.0, y) * s));
	
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
	//info.color = tex2D(ln_GlobalLightInfoSampler, (float2(0.0, y) * s));
	//info.groundColor = tex2D(ln_GlobalLightInfoSampler, (float2(1.5, y) * s));
	//info.directionAndType = tex2D(ln_GlobalLightInfoSampler, (float2(2.5, y) * s));
	info.color = ln_GlobalLightInfoTexture.Sample(ln_GlobalLightInfoTextureSamplerState, (float2(0.0, y) * s));
	info.groundColor = ln_GlobalLightInfoTexture.Sample(ln_GlobalLightInfoTextureSamplerState, (float2(1.5, y) * s));
	info.directionAndType = ln_GlobalLightInfoTexture.Sample(ln_GlobalLightInfoTextureSamplerState, (float2(2.5, y) * s));
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



float _LN_CalcFogFactor(float depth)
{
	float f = ln_FogParams.a * depth;
	return exp2(-f);
	//return exp2(-f*f);
}


	
	
float4 _LN_PS_ClusteredForward_Default(
	//LN_PSInput_ClusteredForward extra,
	float3 worldPos,
	float3 vertexPos,

	LN_SurfaceOutput surface)
{
	float4 viewPos = mul(float4(worldPos, 1.0f), ln_View);
	float depth = (viewPos.z - ln_nearClip) / (ln_farClip - ln_nearClip);
	
	// 頂点位置から視点位置へのベクトル
	float3 vViewPosition = -viewPos.xyz;
	
	
	float4 vp = mul(float4(vertexPos, 1.0f), ln_WorldViewProjection);
	vp.xyz /= vp.w;
	
	float cx = (vp.x + 1.0) / 2.0;
	float cy = (vp.y + 1.0) / 2.0;
	float cz = _LN_FlustumClustereDepthBias(depth);
	
	//float4 cluster = tex3D(clustersSampler, float3(cx, cy, cz));
	float4 cluster = ln_clustersTexture.Sample(ln_clustersTextureSamplerState, float3(cx, cy, cz));
	float lightIndices[4] = {cluster.r, cluster.g, cluster.b, cluster.a};
	
	
	float4 mc = surface.Albedo;// * ln_ColorScale;//(tex2D(MaterialTextureSampler, p.UV) * p.Color) * ln_ColorScale;
	
	
	/**/
	LN_PBRGeometry geometry;
	geometry.position = -vViewPosition;//-vViewPosition;
	geometry.normal = normalize(surface.Normal);//vNormal);
	geometry.viewDir = normalize(vViewPosition);//vViewPosition);

	/**/
	float metallic = 0.0;//0.5;	// TODO:
	float roughness = 1.0;//0.3;	// TODO:
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
				// Spot light
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
				// Point light
				LN_PointLight pointLight;
				pointLight.position = light.position;
				pointLight.color = light.color.xyz;// * light.color.a;
				pointLight.distance = light.range;
				pointLight.decay = light.attenuation;
				LN_GetPointDirectLightIrradiance(pointLight, geometry, directLight);
				if (directLight.visible)
				{
					//　FixMe: ↓ ライト位置が 0, 0, 0 だと dotNL が 0 になってしまう・・・？
					LN_RE_Direct(directLight, geometry, material, reflectedLight);

		/*			
	// コサイン項
	//float dotNL = saturate(dot(geometry.normal, directLight.direction));
	float dotNL = 
						geometry.normal.x * directLight.direction.x+
						geometry.normal.y * directLight.direction.y+
						geometry.normal.z * directLight.direction.z;

	// 放射照度 = コサイン項 * 放射輝度
	float3 irradiance = dotNL * directLight.color;
	//return float4(directLight.direction, 1);
					//return float4(abs(dotNL) * 10000, 0, 0, 1);
					//return float4(
					//	geometry.normal.x * directLight.direction.x,
					//	geometry.normal.y * directLight.direction.y,
					//	geometry.normal.z * directLight.direction.z,
					//	1);
					*/
				}
			}
			
			float LightRadiusMask = 1.0;
			{
				float3 ToLight = light.position.xyz - worldPos.xyz;
				float DistanceSqr = dot(ToLight, ToLight);
				float3 L = ToLight * rsqrt(DistanceSqr);
				
				//return float4(rsqrt(DistanceSqr), 0, 0, 1);
				//return float4(L, 1);
				
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
	    for (int i = 0; i < LN_MAX_GLOBAL_LIGHTS; i++)
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
				//return light.color;
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
			else
			{
				break;
			}
	    }
	    result.rgb += color;
	}


	// TODO: ひとまず
	reflectedLight.directDiffuse += ambientIrradiance * material.diffuseColor;

	//RE_IndirectDiffuse_BlinnPhong(ambientIrradiance, geometry, material, reflectedLight);

#endif
	//return float4(result, 1);
	
#if 0
	//result = float3(0, 0, 0);
	if (lightIndices[0] > 0) result.r += 1;
	if (lightIndices[1] > 0) result.g += 1;
	if (lightIndices[2] > 0) result.b += 1;
#endif


	/**/
	float opacity = 1.0;
	float3 outgoingLight =
		surface.Emission +
		reflectedLight.directDiffuse +
		reflectedLight.directSpecular +
		reflectedLight.indirectDiffuse +
		reflectedLight.indirectSpecular;

	return float4(outgoingLight, opacity);
	
	// Shadow
    //float4 posInLight = extra.vInLightPosition;
    //outgoingLight *= LN_CalculateShadow(posInLight);
	
	// Fog
	result.rgb = lerp(ln_FogParams.rgb, outgoingLight, _LN_CalcFogFactor(viewPos.z));

	return float4(result.rgb, opacity);
	
	
	
	
	
	
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
	float4	dummy	: TEXCOORD6;
};

struct MySSInput
{
	float4	Color		: COLOR0;		// defined
	float2	UV			: TEXCOORD0;	// defined
	float4	WorldPos	: TEXCOORD5;
};


void MyVFMain(LN_VSInput input, inout MyVFOutput output)	// ★ out 引数の型をパース
{
	output.dummy = float4(0, 0, 0, 0);
}

// Surface Shader
void MySSMain(MySSInput input, inout LN_SurfaceOutput output)
{
	output.Albedo = ln_MaterialTexture.Sample(ln_MaterialTextureSamplerState, input.UV) * input.Color;
	//output.Albedo = float4(1, 0, 0, 1);
}

//------------------------------------------------------------------------------
// (auto generation)
// ClusteredShading_Default.template.fx から生成する

struct _lngs_VSOutput
{
	float4	svPos		: SV_POSITION;

	// common
	float3	Normal		: NORMAL0;
	float2	UV			: TEXCOORD0;
	float4	Color		: COLOR0;

	// clustered forward
	float3	WorldPos	: TEXCOORD10;
	float3	VertexPos	: TEXCOORD11;
};

// auto generation
_lngs_VSOutput _lngs_VS_ClusteredForward_Geometry(LN_VSInput vsi)
{
	LN_VSOutput_Common common = LN_ProcessVertex_Common(vsi);
	LN_VSOutput_ClusteredForward extra = LN_ProcessVertex_ClusteredForward(vsi);

	_lngs_VSOutput output;
	output.svPos = common.svPos;
	output.Normal = common.Normal;
	output.UV = common.UV;
	output.Color = common.Color;
	output.WorldPos = extra.WorldPos;
	output.VertexPos = extra.VertexPos;
	return output;
}
// auto generation
/*
_lngs_VSOutput _lngs_VS_ClusteredForward_Geometry_SkinnedMesh(LN_VSInput vsi)
{
	_lngs_VSOutput o;
	o.common	= LN_ProcessVertex_SkinnedCommon(vsi);
	o.extra		= LN_ProcessVertex_ClusteredForward(vsi);
	// ★ Scene固有のコードはここに直接生成する (ピクセルシェーダと書き方を合わせたい)
	MyVFMain(vsi, o.user);	// ★ User定義呼び出し
	return o;
}
*/


struct _lngs_PSInput
{
	// common
	float3	Normal		: NORMAL0;
	float2	UV			: TEXCOORD0;
	float4	Color		: COLOR0;
	
	// clustered forward
	float3	WorldPos	: TEXCOORD10;
	float3	VertexPos	: TEXCOORD11;
};

struct _lngs_PSOutput
{
	float4 color0 : COLOR0;
	//float4 color1 : COLOR1;
};

_lngs_PSOutput _lngs_PS_ClusteredForward_Geometry(_lngs_PSInput input)
{
	LN_SurfaceOutput surface;
	_LN_InitSurfaceOutput(input.Normal, surface);

	//surface.Normal = input.Normal;
	
	// ★ライティングのコードはここに直接生成する (GBuffer生成などではマルチRT書き込みするため、戻り値も変えなければならない)
	// ・・・というより、ピクセルシェーダ全体を生成する。フラグメントの結合じゃダメ。

	// TODO: SurfaceShader を入れるのはこのあたり
	surface.Albedo = ln_MaterialTexture.Sample(ln_MaterialTextureSamplerState, input.UV);// * input.Color;

	surface.Emission = (ln_MaterialEmissive.rgb * ln_MaterialEmissive.a);
	
	_lngs_PSOutput o;
	o.color0 = _LN_PS_ClusteredForward_Default(input.WorldPos, input.VertexPos, surface);
	o.color0.a = surface.Albedo.a;
	//o.color0 = float4(1, 0, 0, 1);
	o.color0 = LN_GetBuiltinEffectColor(o.color0);
	return o;
}

float4	ln_MaterialAmbient;	// TODO: とりあえず MMD モデル用のために用意。
// ※MMM だと、GUI で設定したライトの「色」は Ambient に入ってくるようだ。Diffuse は常に 0 みたい。

float4 _lngs_PS_UnLighting(_lngs_PSInput input) : COLOR0
{
	//float4 result = input.Color * ln_MaterialColor;

	//float3 ambient = float3(1, 1, 1) * ln_MaterialAmbient.rgb;
	//result.rgb = saturate(result.rgb + ambient);

	return float4(1, 0, 0, 1);

	//result *= (tex2D(MaterialTextureSampler, input.common.UV));
	//result *= ln_MaterialTexture.Sample(ln_MaterialTextureSamplerState, input.UV);
	float4 result = ln_MaterialTexture.Sample(ln_MaterialTextureSamplerState, input.UV);

	return result;
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

technique Forward_Geometry
{
	pass Pass1
	{
		VertexShader = _lngs_VS_ClusteredForward_Geometry;
		PixelShader	 = _lngs_PS_ClusteredForward_Geometry;
	}
}

technique Forward_Geometry_StaticMesh_UnLighting
{
	pass Pass1
	{
		VertexShader = _lngs_VS_ClusteredForward_Geometry;
		PixelShader	 = _lngs_PS_UnLighting;
	}
}

/*
technique Forward_Geometry_SkinnedMesh
{
	pass Pass1
	{
		VertexShader = _lngs_VS_ClusteredForward_Geometry_SkinnedMesh;
		PixelShader	 = _lngs_PS_ClusteredForward_Geometry;
	}
}

technique Forward_Geometry_SkinnedMesh_UnLighting
{
	pass Pass1
	{
		VertexShader = _lngs_VS_ClusteredForward_Geometry_SkinnedMesh;
		PixelShader	 = _lngs_PS_UnLighting;
	}
}
*/

//END_HLSL

