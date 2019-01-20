/*
 Note:
   計算は View 空間上で行われる。
 */

#ifndef LUMINO_PBR_INCLUDED
#define LUMINO_PBR_INCLUDED

//------------------------------------------------------------------------------
// Built-in Variables

float4	ln_MaterialColor;
float	ln_MaterialRoughness;
float	ln_MaterialMetallic;
float	ln_MaterialSpecular;
float4	ln_MaterialEmissive;

//------------------------------------------------------------------------------

#define LN_PI 3.14159265359
#define LN_PI2 6.28318530718
#define LN_RECIPROCAL_PI 0.31830988618
#define LN_RECIPROCAL_PI2 0.15915494
#define LN_LOG2 1.442695
#define LN_EPSILON 1e-6

//------------------------------------------------------------------------------
// PBR

// DirectLightIrradiance 系関数の出力。
// ピクセルへのライトごとの影響情報
struct LN_IncidentLight
{
	float3 color;
	float3 direction;	// ピクセル位置 → ライト中心位置 への向きベクトル
	bool visible;
};

// ピクセルへのライトすべての影響情報
struct LN_ReflectedLight
{
	float3 directDiffuse;
	float3 directSpecular;
	float3 indirectDiffuse;
	float3 indirectSpecular;
};

// PBR 用 形状情報
struct LN_PBRGeometry
{
	float3 position;	// in view space
	float3 normal;
	float3 viewDir;
};

// PBR 用 マテリアル情報
struct LN_PBRMaterial
{
	float3 diffuseColor;
	float specularRoughness;
	float3 specularColor;
};

//------------------------------------------------------------------------------
// PBR Light

// 半球ライトの情報
struct LN_HemisphereLight
{
	float3 upDirection;
	float3 skyColor;
	float3 groundColor;
};

// ディレクショナルライトの情報
struct LN_DirectionalLight
{
	float3 direction;
	float3 color;
};

// ポイントライトの情報
struct LN_PointLight
{
	float3 position;	// View 空間上の、ライトの中心点
	float3 color;
	float distance;
	float decay;
};

// スポットライトの情報
struct LN_SpotLight
{
	float3 position;
	float3 direction;
	float3 color;
	float distance;
	float decay;
	float coneCos;
	float penumbraCos;
};

// 光源からの光が届くかどうか
bool LN_TestLightInRange(const in float lightDistance, const in float cutoffDistance)
{
	return cutoffDistance == 0.0 || lightDistance < cutoffDistance;
}

// 光源からの減衰率
float LN_PunctualLightIntensityToIrradianceFactor(const in float lightDistance, const in float cutoffDistance, const in float decayExponent)
{
	if( cutoffDistance > 0.0 && decayExponent > 0.0 ) {
		return pow( saturate( -lightDistance / cutoffDistance + 1.0 ), decayExponent );
	}
	return 1.0;
}

// アンビエントライトの放射輝度の計算
float3 LN_GetAmbientLightIrradiance(const in float3 color)
{
	float3 irradiance = color;

#ifndef PHYSICALLY_CORRECT_LIGHTS
	irradiance *= LN_PI;
#endif

	return irradiance;
}

// 半球ライトの放射輝度の計算
float3 LN_GetHemisphereLightIrradiance(const in LN_HemisphereLight hemiLight, const in LN_PBRGeometry geometry)
{
	float dotNL = dot(geometry.normal, hemiLight.upDirection);
	float hemiDiffuseWeight = 0.5 * dotNL + 0.5;

	float3 irradiance = lerp(hemiLight.groundColor, hemiLight.skyColor, hemiDiffuseWeight);

#ifndef PHYSICALLY_CORRECT_LIGHTS
	irradiance *= LN_PI;	// PHYSICALLY_CORRECT_LIGHTS
#endif

	return irradiance;
}

// ディレクショナルライトの放射輝度の計算
void LN_GetDirectionalDirectLightIrradiance(const LN_DirectionalLight directionalLight, const LN_PBRGeometry geometry, out LN_IncidentLight directLight)
{
	directLight.color = directionalLight.color;
	directLight.direction = directionalLight.direction;
	directLight.visible = true;
}

// ポイントライトの放射輝度の計算
void LN_GetPointDirectLightIrradiance(const in LN_PointLight pointLight, const in LN_PBRGeometry geometry, out LN_IncidentLight directLight)
{
	float3 lVector = pointLight.position - geometry.position;
	directLight.direction = normalize(lVector);

	float lightDistance = length( lVector );

	directLight.color = pointLight.color;
	directLight.color *= LN_PunctualLightIntensityToIrradianceFactor( lightDistance, pointLight.distance, pointLight.decay );
	directLight.visible = ( directLight.color != float3(0.0, 0.0, 0.0) );
	/*
	float3 L = pointLight.position - geometry.position;
	directLight.direction = normalize(L);

	float lightDistance = length(L);
	if (LN_TestLightInRange(lightDistance, pointLight.distance))
	{
		directLight.color = pointLight.color;
		directLight.color *= LN_PunctualLightIntensityToIrradianceFactor(lightDistance, pointLight.distance, pointLight.decay);
		directLight.visible = true;
	}
	else
	{
		directLight.color = float3(0,0,0);
		directLight.visible = false;
	}
	*/
}

// ポイントライトの放射輝度の計算
void LN_GetSpotDirectLightIrradiance(const in LN_SpotLight spotLight, const in LN_PBRGeometry geometry, out LN_IncidentLight directLight)
{
	float3 L = spotLight.position - geometry.position;
	directLight.direction = normalize(L);

	float lightDistance = length(L);
	float angleCos = dot(directLight.direction, spotLight.direction);

	if (angleCos > spotLight.coneCos && LN_TestLightInRange(lightDistance, spotLight.distance))
	{
		float spotEffect = smoothstep(spotLight.coneCos, spotLight.penumbraCos, angleCos);
		directLight.color = spotLight.color;
		directLight.color *= spotEffect * LN_PunctualLightIntensityToIrradianceFactor(lightDistance, spotLight.distance, spotLight.decay);
		directLight.visible = true;
	}
	else
	{
		directLight.color = float3(0,0,0);
		directLight.visible = false;
	}
}

//------------------------------------------------------------------------------
// BRDF

// Normalized Lambert
float3 LN_DiffuseBRDF(float3 diffuseColor)
{
	return diffuseColor / LN_PI;
}

float3 LN_F_Schlick(float3 specularColor, float3 H, float3 V)
{
	return (specularColor + (1.0 - specularColor) * pow(1.0 - saturate(dot(V,H)), 5.0));
}

float LN_D_GGX(float a, float dotNH)
{
	float a2 = a*a;
	float dotNH2 = dotNH*dotNH;
	float d = dotNH2 * (a2 - 1.0) + 1.0;
	return a2 / (LN_PI * d * d);
}

float LN_G_Smith_Schlick_GGX(float a, float dotNV, float dotNL)
{
	float k = a*a*0.5 + LN_EPSILON;
	float gl = dotNL / (dotNL * (1.0 - k) + k);
	float gv = dotNV / (dotNV * (1.0 - k) + k);
	return gl*gv;
}

// Cook-Torrance
float3 LN_SpecularBRDF(const LN_IncidentLight directLight, const LN_PBRGeometry geometry, float3 specularColor, float roughnessFactor)
{
	float3 N = geometry.normal;
	float3 V = geometry.viewDir;
	float3 L = directLight.direction;

	float dotNL = saturate(dot(N,L));
	float dotNV = saturate(dot(N,V));
	float3 H = normalize(L+V);
	float dotNH = saturate(dot(N,H));
	float dotVH = saturate(dot(V,H));
	float dotLV = saturate(dot(L,V));
	float a = roughnessFactor * roughnessFactor;

	float D = LN_D_GGX(a, dotNH);
	float G = LN_G_Smith_Schlick_GGX(a, dotNV, dotNL);
	float3 F = LN_F_Schlick(specularColor, V, H);
	return (F*(G*D))/(4.0*dotNL*dotNV+LN_EPSILON);
}

// RenderEquations(RE)
void LN_RE_Direct_BlinnPhong(const in LN_IncidentLight directLight, const LN_PBRGeometry geometry, const LN_PBRMaterial material, inout LN_ReflectedLight reflectedLight)
{
	// コサイン項
	float dotNL = saturate(dot(geometry.normal, directLight.direction));

	// 放射照度 = コサイン項 * 放射輝度
	float3 irradiance = dotNL * directLight.color;

	// punctual light
	irradiance *= LN_PI;

	// 拡散反射成分
	reflectedLight.directDiffuse += irradiance * LN_DiffuseBRDF(material.diffuseColor);

	// 鏡面反射成分
	reflectedLight.directSpecular += irradiance * LN_SpecularBRDF(directLight, geometry, material.specularColor, material.specularRoughness);
}
// RE_Direct_BlinnPhong
// RE_Direct_Physical

float3 BRDF_Diffuse_Lambert(const float3 diffuseColor)
{
	return LN_RECIPROCAL_PI * diffuseColor;
}

void LN_RE_IndirectDiffuse_BlinnPhong(const float3 irradiance, const LN_PBRGeometry geometry, const LN_PBRMaterial material, inout LN_ReflectedLight reflectedLight)
{
	reflectedLight.indirectDiffuse += irradiance * BRDF_Diffuse_Lambert( material.diffuseColor );
}

#define LN_RE_Direct				LN_RE_Direct_BlinnPhong
#define LN_RE_IndirectDiffuse		LN_RE_IndirectDiffuse_BlinnPhong




#endif // LUMINO_PBR_INCLUDED
