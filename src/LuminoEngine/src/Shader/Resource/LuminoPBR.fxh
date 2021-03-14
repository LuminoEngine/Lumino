/*
 * 用語メモ
 * ----------
 * ### 放射束
 * > 光の基本単位は光子（photon）です．放射エネルギー（radiant energy）は，光子が集まったエネルギーです．このエネルギーを単位時間あたりで表したものを放射束（Flux）といいます．
 *
 * ### 放射輝度 (Radiance)
 * > 放射源の表面上の点からある方向へと放出される放射束を表す物理量である。
 * → このシェーダとしては、光源がピクセルに向かって放出する光量。例えばポイントライトであれば距離で減衰する。
 *
 * ### 放射照度 (Irradiance)
 * > 物体へ時間あたりに照射される、面積あたりの放射エネルギーを表す物理量である。
 * > 放射照度は単位面積あたりの放射束のことです．
 * → ピクセルに実際に当たった光量、というイメージ。面法線と光線の角度(コサイン項) によって、量が決定する。垂直ならmax、水平ならmin(光は当たらなかった)
 */


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
    float3 directDiffuse;       // 拡散反射成分
    float3 directSpecular;      // 鏡面反射成分
    float3 indirectDiffuse;     // 間接光による拡散反射成分
    float3 indirectSpecular;    // 間接光による鏡面反射成分
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
    float occlusion;
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
float3 LN_GetAmbientLightRadiance(const in float3 color)
{
    float3 irradiance = color;

#ifndef PHYSICALLY_CORRECT_LIGHTS
    irradiance *= LN_PI;
#endif

    return irradiance;
}

// 半球ライトの放射輝度の計算
float3 LN_GetHemisphereLightRadiance(const in LN_HemisphereLight hemiLight, const in LN_PBRGeometry geometry)
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
void LN_GetDirectionalDirectLightRadiance(const LN_DirectionalLight directionalLight, const LN_PBRGeometry geometry, out LN_IncidentLight directLight)
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

// ポイントライトの放射輝度の計算。
// あるピクセルにおける geometry.position と、光源 pointLight.position の距離から、
void LN_GetPointDirectLightRadiance(const in LN_PointLight pointLight, const in LN_PBRGeometry geometry, out LN_IncidentLight directLight)
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
void LN_GetSpotDirectLightRadiance(const in LN_SpotLight spotLight, const in LN_PBRGeometry geometry, out LN_IncidentLight directLight)
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
    // https://zenn.dev/mebiusbox/books/619c81d2fbeafd/viewer/239ee2#%F0%9F%93%8C-%E6%94%BE%E5%B0%84%E7%85%A7%E5%BA%A6%E3%81%A8%E6%94%BE%E5%B0%84%E8%BC%9D%E5%BA%A6%E3%81%AE%E9%96%A2%E4%BF%82
    // > 放射輝度に入射角の余弦（\cos\thetacosθ）を掛けることで放射照度に変換することができます.
    // https://zenn.dev/mebiusbox/books/619c81d2fbeafd/viewer/7c1069#%E3%83%AC%E3%83%B3%E3%83%80%E3%83%AA%E3%83%B3%E3%82%B0%E6%96%B9%E7%A8%8B%E5%BC%8F
    // > 放射輝度とコサイン項を掛けて放射照度に変換しています．
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

// irradiance : AmbientLight や HemisphereLight、(将来的には) グローバルイルミネーション等の間接色
void LN_RE_IndirectDiffuse_BlinnPhong(const float3 irradiance, const LN_PBRGeometry geometry, const LN_PBRMaterial material, inout LN_ReflectedLight reflectedLight)
{
    // material.diffuseColor は metaric 反映済みの色。
    // 例えば metaric=1.0 の場合 
    reflectedLight.indirectDiffuse += irradiance * BRDF_Diffuse_Lambert(material.diffuseColor);
}

#define LN_RE_Direct                LN_RE_Direct_BlinnPhong
#define LN_RE_IndirectDiffuse        LN_RE_IndirectDiffuse_BlinnPhong


// lights_fragment_begin.glsl.js
// 
// この関数の主な目的は、ピクセルに影響する光の情報を LN_ReflectedLight に集めること。
// まずライトの種類ごとに定義されている **LightIrradiance() 関数を使って、ピクセルに入ってくる放射輝度を求める。
// その後 LN_RE_Direct を使って、LN_ReflectedLight へ加算していく。
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
                LN_GetSpotDirectLightRadiance(spotLight, geometry, directLight);
        
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
                LN_GetPointDirectLightRadiance(pointLight, geometry, directLight);

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
        irradiance += LN_GetAmbientLightRadiance(ln_AmbientColor.rgb);

        // HemisphereLight
        LN_HemisphereLight tl;
        tl.upDirection = float3(0, 1, 0);
        tl.skyColor = ln_AmbientSkyColor.rgb;
        tl.groundColor = ln_AmbientGroundColor.rgb;
        irradiance += LN_GetHemisphereLightRadiance(tl, geometry);
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
                irradiance += LN_GetHemisphereLightRadiance(tl, geometry);
            }
            // AmbientLight
            // TODO: deprecated
            else if (light.directionAndType.w >= 2.0)
            {
                irradiance += LN_GetAmbientLightRadiance(light.color.rgb);
            }
            // DirectionalLight
            else if (light.directionAndType.w >= 1.0)
            {
                LN_DirectionalLight tl;
                tl.direction = light.directionAndType.xyz;//mul(float4(light.directionAndType.xyz, 1.0), ln_View).xyz;//light.directionAndType.xyz;
                tl.color = light.color;
                LN_GetDirectionalDirectLightRadiance(tl, geometry, directLight);

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

    reflectedLight.indirectDiffuse *= material.occlusion;

    float3 outgoingLight =
        reflectedLight.directDiffuse +
        reflectedLight.directSpecular +
        reflectedLight.indirectDiffuse +
        reflectedLight.indirectSpecular;

    return outgoingLight;
}

#endif // LUMINO_PBR_INCLUDED
