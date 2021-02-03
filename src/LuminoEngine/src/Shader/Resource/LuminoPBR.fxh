/*
 Note:
 計算は View 空間上で行われる。
 three.js のシェーダを参考にしているためその方式に合わせた…というのが正直なところ。
 なぜ three.js がそのようにしている。

 ゲームエンジンアーキテクチャでは Screen-Space の法線で G-Buffer 作っているらしい。
 
 でもググると World-space で扱っている例も多く出てくる。
 https://enginetrouble.net/2014/10/classic-deferred-shading-2014.html
 http://asura.iaigiri.com/OpenGL/gl70.html

 https://forum.unity.com/threads/shader-convert-normals-to-view-space-normals.285262/
 > 私は屈折効果を作成しようとしています。私が見たものから、ビュー空間で法線を使用するとより良い結果が得られます。

 https://stackoverflow.com/questions/37762692/calculating-view-space-normal-instead-of-world-space
 SSAO の実装で役に立つらしい。0.4.0 時点の実装では使ってなかったが…。

 考察としては計算量が減る。
 カメラの位置が (0,0,0) である空間をベースにしているので、
 例えば、
 - ある位置とカメラの距離を知りたければ、引き算は不要で、length(pos) だけでいい。
 - カメラの方向が知りたければ normalize(-pos)

 https://stackoverflow.com/questions/17499976/why-do-you-use-camera-space-instead-of-model-space-for-normals
 ここがわかりやすいかな。
 - 鏡面反射の処理で計算を単純化できる。
 ほとんどのケースでは Model-space でやっても問題ない。
 でも、鏡面反射など多くの処理を取り入れる場合、Model-space より後段の View-space でやったほうが無難、という感じ。


  IncidentLight | 入射光
 */

#ifndef LUMINO_PBR_INCLUDED
#define LUMINO_PBR_INCLUDED

//------------------------------------------------------------------------------
// Built-in Variables



//------------------------------------------------------------------------------

#define LN_PI 3.14159265359
#define LN_PI2 6.28318530718
#define LN_RECIPROCAL_PI 0.31830988618
#define LN_RECIPROCAL_PI2 0.15915494
#define LN_LOG2 1.442695
#define LN_EPSILON 1e-6

//------------------------------------------------------------------------------
// PBR

// 入射光情報。
// DirectLightIrradiance 系関数の出力。
// ピクセルへのライトごとの影響情報
struct LN_IncidentLight
{
    float3 color;

    /** ジオメトリ位置 (ピクセル位置) → ライト中心位置 への向きベクトル */
    float3 direction;

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
    float3 position;    // in view space
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
    /** ライトの向き (光の放射方向) [View-space] */
    float3 direction;

    float3 color;
};

// ポイントライトの情報
struct LN_PointLight
{
    /** ライトの中心点 [View-space] */
    float3 position;

    float3 color;
    float distance;
    float decay;
};

// スポットライトの情報
struct LN_SpotLight
{
    /** ライトの中心点 [View-space] */
    float3 position;
    
    /** ライトの向き (光の放射方向) [View-space] */
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
    irradiance *= LN_PI;    // PHYSICALLY_CORRECT_LIGHTS
#endif

    return irradiance;
}

// ディレクショナルライトの放射輝度の計算
void LN_GetDirectionalDirectLightIrradiance(const LN_DirectionalLight directionalLight, const LN_PBRGeometry geometry, out LN_IncidentLight directLight)
{
    directLight.color = directionalLight.color;
    directLight.direction = -directionalLight.direction;
    directLight.visible = true;
}

bool LN_IsZeros(float3 v)
{
    // TODO: どちらが高速？
#if 1
    return (v.x + v.y + v.z) == 0.0;
#else
    return v.x == 0.0 && v.x == 0.0 && v.x == 0.0;
#endif
}

// ポイントライトの放射輝度の計算
void LN_GetPointDirectLightIrradiance(const in LN_PointLight pointLight, const in LN_PBRGeometry geometry, out LN_IncidentLight directLight)
{
    float3 lVector = pointLight.position - geometry.position;
    directLight.direction = normalize(lVector);

    float lightDistance = length( lVector );

    directLight.color = pointLight.color;
    directLight.color *= LN_PunctualLightIntensityToIrradianceFactor( lightDistance, pointLight.distance, pointLight.decay );
directLight.visible =  !LN_IsZeros(directLight.color); //(directLight.color != float3(0.0, 0.0, 0.0) );
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

// スポットライトの放射輝度の計算
void LN_GetSpotDirectLightIrradiance(const in LN_SpotLight spotLight, const in LN_PBRGeometry geometry, out LN_IncidentLight directLight)
{
    float3 L = spotLight.position - geometry.position;
    directLight.direction = normalize(L);

    float lightDistance = length(L);
    float angleCos = dot(directLight.direction, -spotLight.direction);

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

#define LN_RE_Direct                LN_RE_Direct_BlinnPhong
#define LN_RE_IndirectDiffuse        LN_RE_IndirectDiffuse_BlinnPhong


// lights_fragment_begin.glsl.js
float3 _LN_ComputePBRLocalLights(_LN_LocalLightContext localLightContext, const LN_PBRGeometry geometry, LN_PBRMaterial material)
{
    LN_ReflectedLight reflectedLight = {float3(0,0,0),float3(0,0,0),float3(0,0,0),float3(0,0,0)};
    LN_IncidentLight directLight;
    
    for (int i = 0; i < 4; i++)
    {
        LightInfo light;
        if (_LN_GetLocalLightInfo(localLightContext, i, light))
        {
            if (light.spotAngles.x > 0.0)
            {
                // Spot light
                LN_SpotLight spotLight;
                spotLight.position = light.position;
                spotLight.direction = light.direction;
                spotLight.color = light.color;
                spotLight.distance = light.range;
                spotLight.decay = light.attenuation;
                spotLight.coneCos = light.spotAngles.x;
                spotLight.penumbraCos = light.spotAngles.y;
                LN_GetSpotDirectLightIrradiance(spotLight, geometry, directLight);
        
                // TODO: Three.js ではここで Shadow の処理を行っていた
                //#ifdef USE_SHADOWMAP
                //directLight.color *= all( bvec2( spotLight.shadow, directLight.visible ) ) ? getShadow( spotShadowMap[ i ], spotLight.shadowMapSize, spotLight.shadowBias, spotLight.shadowRadius, vSpotShadowCoord[ i ] ) : 1.0;
                //#endif

                LN_RE_Direct(directLight, geometry, material, reflectedLight);
            }
            else
            {
                // Point light
                LN_PointLight pointLight;
                pointLight.position = light.position;
                pointLight.color = light.color;
                pointLight.distance = light.range;
                pointLight.decay = light.attenuation;
                LN_GetPointDirectLightIrradiance(pointLight, geometry, directLight);

                // TODO: Three.js ではここで Shadow の処理を行っていた
                //#ifdef USE_SHADOWMAP
                //directLight.color *= all( bvec2( pointLight.shadow, directLight.visible ) ) ? getPointShadow( pointShadowMap[ i ], pointLight.shadowMapSize, pointLight.shadowBias, pointLight.shadowRadius, vPointShadowCoord[ i ], pointLight.shadowCameraNear, pointLight.shadowCameraFar ) : 1.0;
                //#endif

                LN_RE_Direct(directLight, geometry, material, reflectedLight);
            }
        }
    }
    
    float3 irradiance = float3(0, 0, 0);

    // EnvironmentLight
    {
        // AmbientLight
        irradiance += LN_GetAmbientLightIrradiance(ln_AmbientColor.rgb);

        // HemisphereLight
        LN_HemisphereLight tl;
        tl.upDirection = float3(0, 1, 0);
        tl.skyColor = ln_AmbientSkyColor.rgb;
        tl.groundColor = ln_AmbientGroundColor.rgb;
        irradiance += LN_GetHemisphereLightIrradiance(tl, geometry);
    }
    
    {
        float count = LN_EPSILON;
        for (int i = 0; i < LN_MAX_GLOBAL_LIGHTS; i++)
        {
            GlobalLightInfo light = _LN_GetGlobalLightInfo(i);

            // HemisphereLight
            // TODO: deprecated
            if (light.directionAndType.w >= 3.0)
            {
                LN_HemisphereLight tl;
                tl.upDirection = float3(0, 1, 0);
                tl.skyColor = light.color;
                tl.groundColor = light.groundColor;
                irradiance += LN_GetHemisphereLightIrradiance(tl, geometry);
            }
            // AmbientLight
            // TODO: deprecated
            else if (light.directionAndType.w >= 2.0)
            {
                irradiance += LN_GetAmbientLightIrradiance(light.color.rgb);
            }
            // DirectionalLight
            else if (light.directionAndType.w >= 1.0)
            {
                LN_DirectionalLight tl;
                tl.direction = light.directionAndType.xyz;//mul(float4(light.directionAndType.xyz, 1.0), ln_View).xyz;//light.directionAndType.xyz;
                tl.color = light.color;
                LN_GetDirectionalDirectLightIrradiance(tl, geometry, directLight);
                
                // TODO: Three.js ではここで Shadow の処理を行っていた
                #ifdef USE_SHADOWMAP
                directLight.color *= all( bvec2( directionalLight.shadow, directLight.visible ) ) ? getShadow( directionalShadowMap[ i ], directionalLight.shadowMapSize, directionalLight.shadowBias, directionalLight.shadowRadius, vDirectionalShadowCoord[ i ] ) : 1.0;
                #endif

                LN_RE_Direct(directLight, geometry, material, reflectedLight);
            }
            else
            {
                break;
            }
        }
    }

    // TODO: ライトマップを使う場合はここで irradiance に適用する
    // see lights_fragment_maps.glsl.js

    // see lights_fragment_end.glsl.js
    LN_RE_IndirectDiffuse( irradiance, geometry, material, reflectedLight );

    float3 outgoingLight =
        reflectedLight.directDiffuse +
        reflectedLight.directSpecular +
        reflectedLight.indirectDiffuse +
        reflectedLight.indirectSpecular;

    return outgoingLight;
}

#endif // LUMINO_PBR_INCLUDED
